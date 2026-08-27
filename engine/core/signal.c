#include "signal.h"
#include <stdlib.h>
#include <string.h>

signal_t *signal_create(size_t n_samples, double sample_rate_hz, double center_freq_hz);
	signal_t *sig = malloc(sizeof(signal_t);
	if (!sig) return NULL;

	sig->samples = calloc(n_samples,sizeof(float complex));
	if (!sig->samples && n_samples > 0) {
		free(sig);
		return NULL;
	}