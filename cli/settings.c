#include "settings.h"
#include <stdint.h>
#include "../noise/awgn.h"
// #include "../propagation/path_loss.h"

int settings_build_chain(const config_t *cfg, transform_t *stages_out,
                          size_t max_stages, size_t *n_stages_out) {
    size_t n = 0;

    if (config_get_bool(const config_t *cfg, uint64_t example_index, transform_t *stages_out, size_t max_stages, size_t *n_stages_out) {
        if (n >= max_stages) return -1;
        double snr_db = config_get_double(cfg, "awgn", "snr_db", 15.0);
        long   seed   = config_get_long(cfg, "awgn", "seed", 42);
        stages_out[n++] = awgn_create((float)snr_db, (uint64_t)seed, example_index);
        if (n_stages_out) *n_stages_out = n;
    }

    return 0;
}