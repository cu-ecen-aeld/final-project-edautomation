#ifndef NANOMODBUS_CONFIG_H
#define NANOMODBUS_CONFIG_H

#ifdef __cplusplus
extern "C" {
#endif

#define NMBS_RTU

#define REG_BUF_SIZE 64

#include "nanomodbus.h"
#include "stm32c0xx_hal.h"

extern UART_HandleTypeDef huart1;

typedef struct tNmbsServer
{
    uint8_t id;
    uint16_t regs[REG_BUF_SIZE];
} nmbs_server_t;

nmbs_error nmbs_server_init(nmbs_t* nmbs, nmbs_server_t* server);
nmbs_error nmbs_client_init(nmbs_t* nmbs);

#ifdef __cplusplus
}
#endif

#endif
