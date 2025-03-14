#include "port.h"
#include <string.h>

static int32_t read_serial(uint8_t* buf, uint16_t count, int32_t byte_timeout_ms, void* arg);
static int32_t write_serial(const uint8_t* buf, uint16_t count, int32_t byte_timeout_ms, void* arg);

static nmbs_server_t* server;

static nmbs_error server_read_holding_registers(uint16_t address, uint16_t quantity, uint16_t* registers_out,
                                                uint8_t unit_id, void* arg);

static nmbs_error server_write_single_register(uint16_t address, uint16_t value, uint8_t unit_id, void* arg);
static nmbs_error server_write_multiple_registers(uint16_t address, uint16_t quantity, const uint16_t* registers,
                                                  uint8_t unit_id, void* arg);

nmbs_error nmbs_server_init(nmbs_t* nmbs, nmbs_server_t* _server)
{
    nmbs_platform_conf conf;
    nmbs_callbacks cb;

    nmbs_platform_conf_create(&conf);
    conf.transport = NMBS_TRANSPORT_RTU;
    conf.read = read_serial;
    conf.write = write_serial;

    server = _server;

    nmbs_callbacks_create(&cb);
    cb.read_holding_registers = server_read_holding_registers;
    cb.write_single_register = server_write_single_register;
    cb.write_multiple_registers = server_write_multiple_registers;

    nmbs_error status = nmbs_server_create(nmbs, server->id, &conf, &cb);
    if (status != NMBS_ERROR_NONE)
    {
        return status;
    }

    nmbs_set_byte_timeout(nmbs, 100);
    nmbs_set_read_timeout(nmbs, 1000);

    return NMBS_ERROR_NONE;
}

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

static nmbs_server_t* get_server(uint8_t id)
{
    if (id == server->id)
    {
        return server;
    }
    else
    {
        return NULL;
    }
}

static nmbs_error server_read_holding_registers(uint16_t address, uint16_t quantity, uint16_t* registers_out,
                                                uint8_t unit_id, void* arg)
{
    nmbs_server_t* server = get_server(unit_id);

    for (size_t i = 0; i < quantity; i++)
    {
        if (address > REG_BUF_SIZE)
        {
            return NMBS_ERROR_INVALID_REQUEST;
        }
        registers_out[i] = server->regs[address++];
    }
    return NMBS_ERROR_NONE;
}

static nmbs_error server_write_single_register(uint16_t address, uint16_t value, uint8_t unit_id, void* arg)
{
    uint16_t reg = value;
    return server_write_multiple_registers(address, 1, &reg, unit_id, arg);
}

static nmbs_error server_write_multiple_registers(uint16_t address, uint16_t quantity, const uint16_t* registers,
                                                  uint8_t unit_id, void* arg)
{
    nmbs_server_t* server = get_server(unit_id);

    for (size_t i = 0; i < quantity; i++)
    {
        if (address > REG_BUF_SIZE)
        {
            return NMBS_ERROR_INVALID_REQUEST;
        }
        server->regs[address++] = registers[i];
    }
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
