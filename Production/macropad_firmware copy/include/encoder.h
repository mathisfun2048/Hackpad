#ifndef ENCODER_H
#define ENCODER_H

#include <stdint.h>

// Initialize encoder
void encoder_init(void);

// Get encoder delta since last call (positive = CW, negative = CCW)
int8_t encoder_get_delta(void);

#endif // ENCODER_H
