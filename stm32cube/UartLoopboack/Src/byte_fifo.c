#include "byte_fifo.h"

#include <stddef.h>
#include <string.h>

#define EFAULT -1

#define RETURN_IF(x, y) \
    if ((x)) return (y)

int16_t byte_fifo_init(struct byte_fifo_t* const fifo)
{
    RETURN_IF(NULL == fifo, EFAULT);
    RETURN_IF(NULL == fifo->data, EFAULT);
    RETURN_IF(0 == fifo->size, EFAULT);

    fifo->write_index = 0U;
    fifo->read_index = 0U;
    fifo->n_elements = 0U;

    memset((void*)fifo->data, 0, fifo->size);

    return 0;
}

int16_t byte_fifo_is_available(const struct byte_fifo_t* const fifo)
{
    RETURN_IF(NULL == fifo, EFAULT);
    return (fifo->n_elements < fifo->size);
}

int16_t byte_fifo_write(struct byte_fifo_t* const fifo, const uint8_t* const bytes, uint16_t len)
{
    RETURN_IF(NULL == fifo, EFAULT);
    RETURN_IF(NULL == fifo->data, EFAULT);
    RETURN_IF(NULL == bytes, EFAULT);

    int16_t n_bytes_written = 0;
    int16_t n_overwrite = 0;
    while (len > 0)
    {
        if (fifo->n_elements < fifo->size)
        {
            uint16_t write_index = fifo->write_index;
            fifo->data[write_index] = bytes[n_bytes_written];
            fifo->write_index = (write_index < (fifo->size - 1)) ? write_index + 1 : 0;
            fifo->n_elements++;

            // Update
            n_bytes_written++;
            len--;
        }
        else
        {
            uint16_t read_index = fifo->read_index;
            fifo->read_index = (read_index < (fifo->size - 1)) ? read_index + 1 : 0;
            fifo->n_elements--;
            n_overwrite++;
        }
    }

    return n_overwrite;
}

int16_t byte_fifo_read(struct byte_fifo_t* const fifo, uint8_t* const buffer, uint16_t max_len)
{
    RETURN_IF(NULL == fifo, EFAULT);
    RETURN_IF(NULL == fifo->data, EFAULT);
    RETURN_IF(NULL == buffer, EFAULT);

    int16_t n_bytes_read = 0;
    while (n_bytes_read < max_len)
    {
        if (fifo->n_elements > 0)
        {
            uint16_t read_index = fifo->read_index;
            buffer[n_bytes_read] = fifo->data[read_index];
            fifo->read_index = (read_index < (fifo->size - 1)) ? read_index + 1 : 0;
            fifo->n_elements--;
            n_bytes_read++;
        }
        else
        {
            break;
        }
    }

    return n_bytes_read;
}
