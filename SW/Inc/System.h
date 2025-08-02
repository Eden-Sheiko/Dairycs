/*
 * System.h
 *
 *  Created on: Jul 30, 2025
 *      Author: eden
 */

#ifndef INC_SYSTEM_H_
#define INC_SYSTEM_H_

#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "main.h"

#define ADC_SIZE 1
#define START_MSG_SIZE 5
#define STOP_MSG_SIZE 4

extern TIM_HandleTypeDef htim10;
extern ADC_HandleTypeDef hadc1;
extern TIM_HandleTypeDef htim11;
extern UART_HandleTypeDef huart2;
extern uint8_t rx_buffer[RX_BUFF_SIZE];

typedef enum {
	SYSTEM_OK = 0,
	SYSTEM_ERROR_TIM10,
	SYSTEM_ERROR_TIM11,
	SYSTEM_ERROR_ADC
} system_module_error_t;

typedef enum {
    SYS_STATE_IDLE = 0,
    SYS_STATE_RUNNING,
    SYS_STATE_ERROR
} system_states_t;

typedef struct module_config_s {
    uint8_t start_msg[START_MSG_SIZE];
    uint8_t stop_msg[STOP_MSG_SIZE];
} module_config_t;


void main_system_loop(void);

system_module_error_t adc_start_mesure(void);
system_module_error_t adc_stop_mesure(void);


#endif /* INC_SYSTEM_H_ */
