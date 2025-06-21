/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "tim.h"
#include "stdio.h"
#include "ACM1602K-NLW-BBW.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
typedef StaticTask_t osStaticThreadDef_t;
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */

/* USER CODE END Variables */
/* Definitions for defaultTask */
osThreadId_t defaultTaskHandle;
uint32_t defaultTaskBuffer[ 128 ];
osStaticThreadDef_t defaultTaskControlBlock;
const osThreadAttr_t defaultTask_attributes = {
  .name = "defaultTask",
  .cb_mem = &defaultTaskControlBlock,
  .cb_size = sizeof(defaultTaskControlBlock),
  .stack_mem = &defaultTaskBuffer[0],
  .stack_size = sizeof(defaultTaskBuffer),
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for LCDTask */
osThreadId_t LCDTaskHandle;
uint32_t LCDTaskBuffer[ 128 ];
osStaticThreadDef_t LCDTaskControlBlock;
const osThreadAttr_t LCDTask_attributes = {
  .name = "LCDTask",
  .cb_mem = &LCDTaskControlBlock,
  .cb_size = sizeof(LCDTaskControlBlock),
  .stack_mem = &LCDTaskBuffer[0],
  .stack_size = sizeof(LCDTaskBuffer),
  .priority = (osPriority_t) osPriorityLow,
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */
void acm1602k_set_rs(uint8_t value);
void acm1602k_set_e(uint8_t value);
void acm1602k_write_data_4bits(uint8_t value);
void acm1602k_delay_ms(uint32_t ms);
void acm1602k_delay_us(uint32_t us);
/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void *argument);
void StartLCDTask(void *argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/* Hook prototypes */
void vApplicationDaemonTaskStartupHook(void);

/* USER CODE BEGIN DAEMON_TASK_STARTUP_HOOK */
void vApplicationDaemonTaskStartupHook(void)
{
}
/* USER CODE END DAEMON_TASK_STARTUP_HOOK */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of defaultTask */
  defaultTaskHandle = osThreadNew(StartDefaultTask, NULL, &defaultTask_attributes);

  /* creation of LCDTask */
  LCDTaskHandle = osThreadNew(StartLCDTask, NULL, &LCDTask_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
  /* add events, ... */
  /* USER CODE END RTOS_EVENTS */

}

/* USER CODE BEGIN Header_StartDefaultTask */
/**
  * @brief  Function implementing the defaultTask thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void *argument)
{
  /* USER CODE BEGIN StartDefaultTask */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END StartDefaultTask */
}

/* USER CODE BEGIN Header_StartLCDTask */
/**
* @brief Function implementing the LCDTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartLCDTask */
void StartLCDTask(void *argument)
{
  /* USER CODE BEGIN StartLCDTask */
  acm1602k_handle_t lcd;
  acm1602k_interface_t lcd_interface;

  lcd_interface.set_rs = acm1602k_set_rs;
  lcd_interface.set_e = acm1602k_set_e;
  lcd_interface.write_data_4bits = acm1602k_write_data_4bits;
  lcd_interface.delay_ms = acm1602k_delay_ms;
  lcd_interface.delay_us = acm1602k_delay_us;

  lcd.interface = lcd_interface;

  acm1602k_init(&lcd);

  acm1602k_write_string(&lcd, "Status: OK");
  acm1602k_set_cursor(&lcd, 1, 0);
  acm1602k_write_string(&lcd, "Counter: ");
  uint32_t counter = 0;
  /* Infinite loop */
  for(;;)
  {
    acm1602k_set_cursor(&lcd, 1, 9);
    char buffer[10];
    sprintf(buffer, "%lu", counter % 10);
    acm1602k_write_string(&lcd, buffer);
    counter++;
    osDelay(100);
  }
  /* USER CODE END StartLCDTask */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */
void acm1602k_set_rs(uint8_t value) {
  HAL_GPIO_WritePin(RS_GPIO_Port, RS_Pin, value ? GPIO_PIN_SET : GPIO_PIN_RESET);
}
void acm1602k_set_e(uint8_t value) {
  HAL_GPIO_WritePin(E_GPIO_Port, E_Pin, value ? GPIO_PIN_SET : GPIO_PIN_RESET);
}
void acm1602k_write_data_4bits(uint8_t value) {
  HAL_GPIO_WritePin(DB4_GPIO_Port, DB4_Pin, (value & 0x01) ? GPIO_PIN_SET : GPIO_PIN_RESET);
  HAL_GPIO_WritePin(DB5_GPIO_Port, DB5_Pin, (value & 0x02) ? GPIO_PIN_SET : GPIO_PIN_RESET);
  HAL_GPIO_WritePin(DB6_GPIO_Port, DB6_Pin, (value & 0x04) ? GPIO_PIN_SET : GPIO_PIN_RESET);
  HAL_GPIO_WritePin(DB7_GPIO_Port, DB7_Pin, (value & 0x08) ? GPIO_PIN_SET : GPIO_PIN_RESET);
}
void acm1602k_delay_ms(uint32_t ms) {
  osDelay(ms);
}
void acm1602k_delay_us(uint32_t us){
  HAL_TIM_Base_Start(&htim6);
  __HAL_TIM_SET_COUNTER(&htim6, 0);
  while (__HAL_TIM_GET_COUNTER(&htim6) < us);
  HAL_TIM_Base_Stop(&htim6);
}
/* USER CODE END Application */

