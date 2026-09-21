#include "ssvep_dsp.h"
#include <string.h>
#include <math.h>

/* EDIT: your actual stimulation frequencies (Hz), must divide your monitor's
 * refresh rate exactly. Example set below assumes a 60 Hz monitor. */
const float32_t target_freqs_hz[NUM_TARGETS] = { 6.0f, 7.5f, 8.571f, 10.0f };

/* ==================================================================== */
/*  Init                                                                 */
/* ==================================================================== */
void ssvep_dsp_init(ssvep_dsp_t *dsp)
{
    memset(dsp, 0, sizeof(*dsp));

    for (uint8_t c = 0; c < SSVEP_NUM_CHANNELS; c++) {
        ssvep_channel_t *ch = &dsp->ch[c];
        arm_biquad_cascade_df2T_init_f32(&ch->notch_inst, NOTCH_STAGES,
                                          (float32_t *)notch_coeffs, ch->notch_state);
        arm_biquad_cascade_df2T_init_f32(&ch->bp_inst, BP_STAGES,
                                          (float32_t *)bp_coeffs, ch->bp_state);
    }

    arm_rfft_fast_init_f32(&dsp->fft_inst, FFT_SIZE);

    /* Precompute Hann window once (cosf is not cheap; done at init only). */
    for (uint32_t i = 0; i < FFT_SIZE; i++) {
        dsp->hann_window[i] = 0.5f * (1.0f - cosf(2.0f * (float32_t)M_PI * i / (float32_t)(FFT_SIZE - 1)));
    }

    for (uint8_t v = 0; v < VOTE_WINDOW; v++) {
        dsp->vote_history[v] = -1;
    }
}

/* ==================================================================== */
/*  Streaming filter: one sample in, one sample stored per channel       */
/* ==================================================================== */
void ssvep_dsp_feed_channel_sample(ssvep_dsp_t *dsp, uint8_t channel, float32_t raw_sample)
{
    ssvep_channel_t *ch = &dsp->ch[channel];

    float32_t notched, filtered;
    arm_biquad_cascade_df2T_f32(&ch->notch_inst, &raw_sample, &notched, 1);
    arm_biquad_cascade_df2T_f32(&ch->bp_inst, &notched, &filtered, 1);

    ch->ring[dsp->ring_idx] = filtered;
}

/* ==================================================================== */
/*  Feature extraction helpers                                          */
/* ==================================================================== */

/* Power of FFT bin k, from arm_rfft_fast_f32's packed output layout:
 * out[0]=Re(X[0]) (DC), out[1]=Re(X[N/2]) (Nyquist),
 * out[2k]=Re(X[k]), out[2k+1]=Im(X[k]) for 1<=k<=N/2-1. */
static inline float32_t bin_power(const float32_t *fft_out, int32_t k)
{
    if (k <= 0)              return fft_out[0] * fft_out[0];
    if (k >= FFT_SIZE / 2)   return fft_out[1] * fft_out[1];
    float32_t re = fft_out[2 * k];
    float32_t im = fft_out[2 * k + 1];
    return re * re + im * im;
}

static inline int32_t freq_to_bin(float32_t hz)
{
    return (int32_t)(hz / FREQ_RES_HZ + 0.5f);
}

/* Sum power around a bin (±SNR_HALFBAND_BINS), used both for a harmonic's
 * signal band and, offset, as a local noise estimate. */
static float32_t band_power(const float32_t *fft_out, int32_t center_bin, int32_t half_width)
{
    float32_t sum = 0.0f;
    for (int32_t k = center_bin - half_width; k <= center_bin + half_width; k++) {
        if (k >= 0 && k < FFT_SIZE / 2) sum += bin_power(fft_out, k);
    }
    return sum;
}

/* SNR for one target frequency: power at fundamental+harmonics vs the mean
 * power in a noise window around the fundamental, excluding the signal band. */
static float32_t harmonic_snr(const float32_t *fft_out, float32_t f0)
{
    float32_t signal_power = 0.0f;
    for (uint8_t h = 1; h <= NUM_HARMONICS; h++) {
        int32_t k = freq_to_bin(f0 * h);
        signal_power += band_power(fft_out, k, SNR_HALFBAND_BINS);
    }

    int32_t k0 = freq_to_bin(f0);
    float32_t noise_sum = 0.0f;
    int32_t noise_count = 0;
    for (int32_t k = k0 - SNR_NOISE_BINS; k <= k0 + SNR_NOISE_BINS; k++) {
        if (k < 0 || k >= FFT_SIZE / 2) continue;
        if (k >= k0 - SNR_HALFBAND_BINS && k <= k0 + SNR_HALFBAND_BINS) continue; /* skip signal band */
        noise_sum += bin_power(fft_out, k);
        noise_count++;
    }
    float32_t noise_power = (noise_count > 0) ? (noise_sum / (float32_t)noise_count) : 1e-9f;

    return signal_power / (noise_power * NUM_HARMONICS + 1e-9f);
}

/* ==================================================================== */
/*  Tick: advance shared ring index, run FFT + classify every FFT_HOP    */
/* ==================================================================== */
bool ssvep_dsp_tick(ssvep_dsp_t *dsp, ssvep_result_t *result)
{
    dsp->ring_idx = (dsp->ring_idx + 1) % FFT_SIZE;
    if (dsp->ring_idx == 0) dsp->ring_filled_once = true;
    dsp->samples_since_last_fft++;

    if (dsp->samples_since_last_fft < FFT_HOP || !dsp->ring_filled_once) {
        return false;
    }
    dsp->samples_since_last_fft = 0;

    static float32_t windowed[FFT_SIZE];
    static float32_t fft_out[FFT_SIZE];
    float32_t snr_accum[NUM_TARGETS] = { 0 };

    for (uint8_t c = 0; c < SSVEP_NUM_CHANNELS; c++) {
        ssvep_channel_t *ch = &dsp->ch[c];

        /* Unroll the ring buffer into time order and window it. Oldest
         * sample is at dsp->ring_idx (next slot to be overwritten). */
        for (uint32_t i = 0; i < FFT_SIZE; i++) {
            uint16_t idx = (dsp->ring_idx + i) % FFT_SIZE;
            windowed[i] = ch->ring[idx] * dsp->hann_window[i];
        }

        arm_rfft_fast_f32(&dsp->fft_inst, windowed, fft_out, 0);

        for (uint8_t t = 0; t < NUM_TARGETS; t++) {
            snr_accum[t] += harmonic_snr(fft_out, target_freqs_hz[t]);
        }
    }

    int8_t best_idx = -1;
    float32_t best_snr = 0.0f;
    for (uint8_t t = 0; t < NUM_TARGETS; t++) {
        result->snr[t] = snr_accum[t] / (float32_t)SSVEP_NUM_CHANNELS;
        if (result->snr[t] > best_snr) {
            best_snr = result->snr[t];
            best_idx = (int8_t)t;
        }
    }
    if (best_snr < SNR_THRESHOLD) best_idx = -1;
    result->target_index = best_idx;

    /* Temporal voting: require VOTE_WINDOW consecutive windows to agree. */
    dsp->vote_history[dsp->vote_pos] = best_idx;
    dsp->vote_pos = (dsp->vote_pos + 1) % VOTE_WINDOW;

    bool all_agree = true;
    for (uint8_t v = 1; v < VOTE_WINDOW; v++) {
        if (dsp->vote_history[v] != dsp->vote_history[0] || dsp->vote_history[0] == -1) {
            all_agree = false;
            break;
        }
    }
    result->committed = all_agree;

    return true;
}