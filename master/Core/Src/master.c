#include <stdbool.h>
#include <stdint.h>

#include "master.h"
#include "gpio.h"
#include "stm32g4xx.h"
#include "stm32g4xx_hal_uart.h"
#include "usart.h"

typedef enum {KELVIN = 0, CELSIUS = 1} type_temp;
typedef enum {NO_LOG = 0, LOG     = 1} type_log;
typedef enum {ZERO_S = 0, TEN_S   = 1, THIRTY_S = 2, NINETY_S = 3} type_samp;
typedef enum {MODE_SLAVE = 0, MODE_MASTER = 1} type_mode;

struct config {
    type_temp temperature;
    type_log log_time;
    type_samp samp_time;
    type_mode mode;
    bool save_setting;
} init = {
    .mode = MODE_SLAVE,
    .temperature = CELSIUS,
    .log_time = LOG,
    .samp_time = TEN_S,
    .save_setting = true
};

static uint8_t command = 0;

static inline uint8_t setting_slave(void); /* set slave before start modality master */
static inline void run_mode_slave(void);
static inline void run_mode_master(void);

void MASTER_init() {
MX_GPIO_Init();
MX_USART1_UART_Init();
MX_USART2_UART_Init();
}

void MASTER_run() {
    if(init.save_setting) {
        command = setting_slave();
        HAL_UART_Transmit(&huart1, &command, sizeof(command), 100);
    }
    switch (init.mode) {
        case MODE_SLAVE:
            run_mode_slave();
            break;
        case MODE_MASTER:
            run_mode_master();
            break;
        default:
            /* add error */
            break;
    }
}

static inline uint8_t setting_slave(void) {
    uint8_t command = 0;
    init.save_setting = false;
    switch (init.temperature) {
        case KELVIN:
            SET_BIT(command, 0 << 0);
            break;
        case CELSIUS:
            SET_BIT(command, 1 << 0);
            break;
        default:
            /* set kelvin */
            SET_BIT(command, 1 << 0);
            break;
    }
    
    switch (init.log_time) {
        case LOG:
            SET_BIT(command, 1 << 1);
            break;
        case NO_LOG:
            SET_BIT(command, 0 << 1);
            break;
        default:
            /* set no log */
            SET_BIT(command, 0 << 1);
            break;
    }

    switch (init.samp_time) {
        case ZERO_S:
            SET_BIT(command, 0 << 3);
            break;
        case TEN_S:
            SET_BIT(command, 1 << 3);
            break;
        case THIRTY_S:
            SET_BIT(command, 2 << 3);
            break;
        case NINETY_S:
            SET_BIT(command, 7 << 3);
            break;
        default:
            /* set zero */
            SET_BIT(command, 0 << 3);
            break;
    }

    /* set no call*/
    SET_BIT(command, 0 << 7);
    /* set save setting on slave */
    SET_BIT(command, 1 << 6);
    /* set slave mode */
    switch (init.samp_time) {
        case ZERO_S:
            SET_BIT(command, 0 << 2);
            break;
        default:
            SET_BIT(command, 1 << 2);
            break;
    }

    return command;
}

static inline void run_mode_slave(void) {

}

static inline void run_mode_master(void) {

}