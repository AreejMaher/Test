#ifndef SSVEP_DSP_H
#define SSVEP_DSP_H

#include "arm_math.h"
#include <stdint.h>
#include <stdbool.h>

/* ==================== Acquisition ==================== */
#define SSVEP_FS_HZ         250.0f   /* Cyton default sample rate */
#define SSVEP_NUM_CHANNELS  2        /* e.g. O1, O2 over occipital cortex. Edit to match your montage. */

/* ==================== Filtering ==================== */
/* Coefficients come from coff_generator.py -> notch_coeffs / bp_coeffs.
 * NOTCH_STAGES / BP_STAGES must match what the generator printed. */
#define NOTCH_STAGES 1
#define BP_STAGES    4   /* corrected: full 8th-order 4-40Hz bandpass = 4 biquad sections */

extern const float32_t notch_coeffs[5 * NOTCH_STAGES];
extern const float32_t bp_coeffs[5 * BP_STAGES];

/* ==================== Windowing / FFT ==================== */
#define FFT_SIZE      512                          /* 2.048 s window @ 250 Hz -> 0.488 Hz bin resolution */
#define FFT_HOP       128                          /* new classification every 128 samples (~0.51 s), 75% overlap */
#define FREQ_RES_HZ   (SSVEP_FS_HZ / (float32_t)FFT_SIZE)

/* ==================== Target frequencies ====================
 * EDIT to match your actual stimuli. Each frequency MUST be an exact integer
 * division of your monitor's refresh rate (e.g. 60 Hz screen -> 60/6=10.0,
 * 60/7=8.571, 60/8=7.5, 60/10=6.0), or the physical flicker won't match
 * what the classifier is told to look for. */
#define NUM_TARGETS    4
#define NUM_HARMONICS  3   /* fundamental + 2 harmonics summed for the SNR feature */
extern const float32_t target_freqs_hz[NUM_TARGETS];

#define SNR_HALFBAND_BINS 2     /* bins each side of a harmonic peak counted as "signal" */
#define SNR_NOISE_BINS    10    /* bins each side of the fundamental (excluding signal band) used as noise reference */
#define SNR_THRESHOLD     2.0f  /* below this ratio -> report "idle" (no fixation) */
#define VOTE_WINDOW       3     /* consecutive classification windows required to agree before a letter commits */

/* ==================== Runtime state ==================== */
typedef struct {
    arm_biquad_cascade_df2T_instance_f32 notch_inst;
    float32_t notch_state[2 * NOTCH_STAGES];
    arm_biquad_cascade_df2T_instance_f32 bp_inst;
    float32_t bp_state[2 * BP_STAGES];

    float32_t ring[FFT_SIZE];   /* circular buffer of filtered samples for this channel */
} ssvep_channel_t;

typedef struct {
    ssvep_channel_t ch[SSVEP_NUM_CHANNELS];
    arm_rfft_fast_instance_f32 fft_inst;
    float32_t hann_window[FFT_SIZE];

    uint16_t ring_idx;              /* shared write index, all channels advance in lockstep */
    uint16_t samples_since_last_fft;
    bool     ring_filled_once;

    /* voting state */
    int8_t  vote_history[VOTE_WINDOW];
    uint8_t vote_pos;
} ssvep_dsp_t;

typedef struct {
    int8_t    target_index;          /* -1 == idle, else index into target_freqs_hz */
    float32_t snr[NUM_TARGETS];      /* per-target SNR this window, for logging/tuning */
    bool      committed;             /* true only when VOTE_WINDOW consecutive windows agreed */
} ssvep_result_t;

/* Call once at startup. */
void ssvep_dsp_init(ssvep_dsp_t *dsp);

/* Call once per incoming raw sample for a given channel (0-indexed), e.g.
 * from your Cyton UART/BLE packet handler, for every channel in the packet. */
void ssvep_dsp_feed_channel_sample(ssvep_dsp_t *dsp, uint8_t channel, float32_t raw_sample);

/* Call once per multi-channel sample tuple, AFTER feeding all channels for
 * that time step. Returns true when a new classification window was
 * evaluated (every FFT_HOP samples) and fills *result. */
bool ssvep_dsp_tick(ssvep_dsp_t *dsp, ssvep_result_t *result);

#endif /* SSVEP_DSP_H */