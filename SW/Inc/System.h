/**
 * @file    System.h
 * @brief   System module header file
 * @date    Jul 30, 2025
 * @author  eden
 */

#ifndef INC_SYSTEM_H_
#define INC_SYSTEM_H_

#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#include "main.h"

/* Public macros */
#define ADC_SIZE            1
#define START_MSG_SIZE      5
#define STOP_MSG_SIZE       4
#define DEBOUNCE_TIME       20

/* External handles */
extern TIM_HandleTypeDef     htim10;
extern TIM_HandleTypeDef     htim11;
extern ADC_HandleTypeDef     hadc1;
extern UART_HandleTypeDef    huart2;

extern uint8_t               rx_buffer[RX_BUFF_SIZE];

/* Error codes for system module */
typedef enum {
    SYSTEM_OK = 0,
    SYSTEM_ERROR_TIM10,
    SYSTEM_ERROR_TIM11,
    SYSTEM_ERROR_ADC
} system_module_error_t;

/* System state machine states */
typedef enum {
    SYS_STATE_IDLE = 0,
    SYS_STATE_RUNNING,
    SYS_STATE_ERROR
} system_states_t;

/* Module configuration structure */
typedef struct module_config_s {
    uint8_t start_msg[START_MSG_SIZE];
    uint8_t stop_msg[STOP_MSG_SIZE];
} module_config_t;

/* Public function declarations */
void main_system_loop(void);
system_module_error_t adc_start_mesure(void);
system_module_error_t adc_stop_mesure(void);

#endif /* INC_SYSTEM_H_ */
