#ifndef BYTE_FIFO_H_
#define BYTE_FIFO_H_

/*
 * Write to fifo from ISR (no nested interrupts), read in main, or vice-versa.
 * Disable IRQ before read or write if this is not the case.
 */

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif  // __cplusplus

struct byte_fifo_t
{
    uint8_t* const data;
    const uint16_t size;
    uint16_t write_index;
    uint16_t read_index;
    uint16_t n_elements;
};

int16_t byte_fifo_init(struct byte_fifo_t* const fifo);
int16_t byte_fifo_is_available(const struct byte_fifo_t* const fifo);
int16_t byte_fifo_write(struct byte_fifo_t* const fifo, const uint8_t* const bytes, uint16_t len);
int16_t byte_fifo_read(struct byte_fifo_t* const fifo, uint8_t* const buffer, uint16_t max_len);

#ifdef __cplusplus
}
#endif  // __cplusplus

#endif  // BYTE_FIFO_H_
