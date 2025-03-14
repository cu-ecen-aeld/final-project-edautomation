#include "port.h"
#include "stm32f3xx_hal.h"

#include <string.h>

extern UART_HandleTypeDef huart1;

static int32_t read_serial(uint8_t* buf, uint16_t count, int32_t byte_timeout_ms, void* arg);
static int32_t write_serial(const uint8_t* buf, uint16_t count, int32_t byte_timeout_ms, void* arg);

nmbs_error nmbs_client_init(nmbs_t* nmbs)
{
    nmbs_platform_conf conf;

    nmbs_platform_conf_create(&conf);

    conf.transport = NMBS_TRANSPORT_RTU;
    conf.read = read_serial;
    conf.write = write_serial;

    nmbs_error status = nmbs_client_create(nmbs, &conf);
    if (status != NMBS_ERROR_NONE)
    {
        return status;
    }

    nmbs_set_byte_timeout(nmbs, 100);
    nmbs_set_read_timeout(nmbs, 1000);

    return NMBS_ERROR_NONE;
}

static int32_t read_serial(uint8_t* buf, uint16_t count, int32_t byte_timeout_ms, void* arg)
{
    HAL_StatusTypeDef status = HAL_OK;
    uint16_t read_bytes = 0;
    while ((status != HAL_TIMEOUT) && (read_bytes < count))
    {
        uint8_t byte = 0;
        status = HAL_UART_Receive(&huart1, &byte, 1, byte_timeout_ms);
        if (status == HAL_OK)
        {
            buf[read_bytes] = byte;
            read_bytes++;
        }
    }
    return read_bytes;
}

static int32_t write_serial(const uint8_t* buf, uint16_t count, int32_t byte_timeout_ms, void* arg)
{
    HAL_StatusTypeDef status = HAL_UART_Transmit(&huart1, buf, count, byte_timeout_ms);
    if (status == HAL_OK)
    {
        return count;
    }
    else
    {
        return 0;
    }
}
