#include <stdbool.h>

#include "master.h"
#include "gpio.h"
#include "usart.h"

typedef enum {KELVIN = 0, CELSIUS = 1} type_temp;
typedef enum {NO_LOG = 0, LOG     = 1} type_log;
typedef enum {ZERO_S = 0, TEN_S   = 1, THIRTY_S = 2, NINETY = 3} type_samp;
typedef enum {MODE_SLAVE = 0, MODE_MASTER = 1} type_mode;

struct {
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

static inline void setting_slave(void); /* set slave before start modality master */
static inline void run_mode_slave(void);
static inline void run_mode_master(void);

void MASTER_init() {
MX_GPIO_Init();
MX_USART1_UART_Init();
MX_USART2_UART_Init();
}

void MASTER_run() {
    if(init.save_setting) setting_slave();
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

static inline void setting_slave(void) {

}

static inline void run_mode_slave(void) {

}

static inline void run_mode_master(void) {
    
}