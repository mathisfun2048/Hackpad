#include "encoder.h"
#include "config.h"
#include "pico/stdlib.h"

static volatile int8_t encoder_delta = 0;
static volatile uint8_t encoder_state = 0;

// Quadrature encoder state machine
static const int8_t encoder_table[16] = {
    0,  -1,  1,  0,
    1,   0,  0, -1,
   -1,   0,  0,  1,
    0,   1, -1,  0
};

static void encoder_callback(uint gpio, uint32_t events) {
    uint8_t a = gpio_get(ENCODER_A_PIN);
    uint8_t b = gpio_get(ENCODER_B_PIN);
    
    encoder_state = ((encoder_state << 2) | (a << 1) | b) & 0x0F;
    encoder_delta += encoder_table[encoder_state];
}

void encoder_init(void) {
    gpio_init(ENCODER_A_PIN);
    gpio_init(ENCODER_B_PIN);
    gpio_set_dir(ENCODER_A_PIN, GPIO_IN);
    gpio_set_dir(ENCODER_B_PIN, GPIO_IN);
    gpio_pull_up(ENCODER_A_PIN);
    gpio_pull_up(ENCODER_B_PIN);
    
    // Enable interrupts on both pins
    gpio_set_irq_enabled_with_callback(ENCODER_A_PIN, GPIO_IRQ_EDGE_RISE | GPIO_IRQ_EDGE_FALL, true, &encoder_callback);
    gpio_set_irq_enabled(ENCODER_B_PIN, GPIO_IRQ_EDGE_RISE | GPIO_IRQ_EDGE_FALL, true);
}

int8_t encoder_get_delta(void) {
    int8_t delta = encoder_delta / 4;  // 4 pulses per detent
    encoder_delta -= delta * 4;
    return delta;
}
