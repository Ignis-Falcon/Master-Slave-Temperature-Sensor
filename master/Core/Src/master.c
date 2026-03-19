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

struct config {
    type_temp temperature;
    type_log log_time;
    type_samp samp_time;
    bool save_setting;
} init = {
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
    switch (init.samp_time) {
        case ZERO_S:
            run_mode_slave();
            break;
        default:
            run_mode_master();
            break;
    }
}

static inline uint8_t setting_slave(void) {
    uint8_t command = 0;
    init.save_setting = false;
    switch (init.temperature) {
        case KELVIN:
            CLEAR_BIT(command, 1 << 0);
            break;
        case CELSIUS:
            SET_BIT(command, 1 << 0);
            break;
        default:
            /* set kelvin */
            CLEAR_BIT(command, 1 << 0);
            break;
    }
    
    switch (init.log_time) {
        case LOG:
            SET_BIT(command, 1 << 1);
            break;
        case NO_LOG:
            CLEAR_BIT(command, 1 << 1);
            break;
        default:
            /* set no log */
            CLEAR_BIT(command, 1 << 1);
            break;
    }

    /* bit 3-5: sampling interval (0=0s, 1=10s, 2=30s, 3+=90s) */
    switch (init.samp_time) {
        case ZERO_S:
            CLEAR_BIT(command, 1 << 3);
            break;
        case TEN_S:
            SET_BIT(command, 1 << 3);
            break;
        case THIRTY_S:
            SET_BIT(command, 2 << 3);
            break;
        case NINETY_S:
            SET_BIT(command, 3 << 3);
            break;
        default:
            /* set zero */
            CLEAR_BIT(command, 1 << 3);
            break;
    }

    /* set no call*/
    CLEAR_BIT(command, 1 << 7);
    /* set save setting on slave */
    SET_BIT(command, 1 << 6);
    /* set slave mode: if samp 0 manual, else auto */
    switch (init.samp_time) {
        case ZERO_S:
            CLEAR_BIT(command, 1 << 2);
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