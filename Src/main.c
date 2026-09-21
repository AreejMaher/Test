#include "ssvep_dsp.h"

static ssvep_dsp_t dsp;

int main(void)
{
    /* ... your existing HAL_Init(), SystemClock_Config(), UART/peripheral init ... */

    ssvep_dsp_init(&dsp);

    while (1) {
        /* ... your existing loop / wait for next sample from Cyton ... */
    }
}

/* Call this from wherever you currently parse an incoming Cyton packet —
 * e.g. your UART RX-complete callback or a ring-buffer drain in the main
 * loop. `sample` is one already-parsed ADC reading in your working units
 * (raw counts or microvolts — just be consistent) for the given channel. */
void on_cyton_sample_ready(uint8_t channel, float32_t sample)
{
    ssvep_dsp_feed_channel_sample(&dsp, channel, sample);

    /* Only call tick() once per full multi-channel sample, i.e. after the
     * LAST channel of that time-step has been fed: */
    if (channel == SSVEP_NUM_CHANNELS - 1) {
        ssvep_result_t result;
        if (ssvep_dsp_tick(&dsp, &result)) {
            if (result.committed && result.target_index >= 0) {
                /* target_index maps to target_freqs_hz[result.target_index] —
                 * translate that into your speller's selected character here. */
                on_letter_selected(result.target_index);
            }
        }
    }
}