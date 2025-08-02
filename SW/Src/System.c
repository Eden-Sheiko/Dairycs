  /*
 * System.c
 *
 *  Created on: Jul 30, 2025
 *      Author: eden
 */
#include "System.h"

volatile uint8_t user_btn_flag = 0x00U;
volatile uint8_t timer_logger_flag = 0x00U;
volatile system_states_t system_state = SYS_STATE_IDLE;


uint16_t adc_buf = 0x00U;


void main_system_loop(void) {

	switch (system_state) {
	case SYS_STATE_IDLE:

		break;
	case SYS_STATE_RUNNING:
		if (timer_logger_flag) {
			printf("adc_value : %u \r\n", adc_buf);
			timer_logger_flag = 0x00U;
		}

		break;
	case SYS_STATE_ERROR:
		printf("Error\r\n");
		system_state = SYS_STATE_IDLE;
		break;

	default:
		printf("Unexpected system state\r\n");
		system_state = SYS_STATE_IDLE;
		break;
	}

}

system_module_error_t adc_start_mesure(void) {
	if (HAL_TIM_Base_Start_IT(&htim10) != HAL_OK){
		return SYSTEM_ERROR_TIM10;
	}
	if (HAL_TIM_Base_Start_IT(&htim11) != HAL_OK){
		return SYSTEM_ERROR_TIM11;
	}
	if (HAL_ADC_Start_DMA(&hadc1, (uint32_t*)&adc_buf, ADC_SIZE) != HAL_OK){
		return SYSTEM_ERROR_ADC;
	}
	return SYSTEM_OK;
}

system_module_error_t adc_stop_mesure(void) {
	if (HAL_TIM_Base_Stop_IT(&htim10) != HAL_OK){
		return SYSTEM_ERROR_TIM10;
	}
	if (HAL_TIM_Base_Stop_IT(&htim11) != HAL_OK){
		return SYSTEM_ERROR_TIM11;
	}
	if (HAL_ADC_Stop_DMA(&hadc1) != HAL_OK){
		return SYSTEM_ERROR_ADC;
	}
	return SYSTEM_OK;
}





void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
	if (htim == &htim10) {
		timer_logger_flag = 0x01U;
	}
	if (htim == &htim11) {
		HAL_ADC_Start_DMA(&hadc1, (uint32_t*)&adc_buf, ADC_SIZE);
	}
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
	HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);
	if (system_state == SYS_STATE_RUNNING) {
		adc_stop_mesure();
		system_state = SYS_STATE_IDLE;
	} else {
		adc_start_mesure();
		system_state = SYS_STATE_RUNNING;
	}

}


void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size) {

	if (huart == &huart2) {
		if (strncmp((const char *)rx_buffer, "start", 5) == 0) {
			if (adc_start_mesure() != SYSTEM_OK) {
				system_state = SYS_STATE_ERROR;
			} else {
				system_state = SYS_STATE_RUNNING;
			}

		} else if (strncmp((const char *)rx_buffer, "stop", 4) == 0) {
			if (adc_stop_mesure() != SYSTEM_OK) {
				system_state = SYS_STATE_ERROR;
			} else {
				system_state = SYS_STATE_IDLE;
			}
		} else {
			system_state = SYS_STATE_ERROR;

		}

		HAL_UARTEx_ReceiveToIdle_DMA(huart, rx_buffer, RX_BUFF_SIZE);
	}

}



