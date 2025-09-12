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
#include "can.h"
#include "can_utils.h"
#include <string.h>
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
/* Definitions for MotorDriverTask */
osThreadId_t MotorDriverTaskHandle;
uint32_t MotorDriverTaskBuffer[ 128 ];
osStaticThreadDef_t MotorDriverTaskControlBlock;
const osThreadAttr_t MotorDriverTask_attributes = {
  .name = "MotorDriverTask",
  .cb_mem = &MotorDriverTaskControlBlock,
  .cb_size = sizeof(MotorDriverTaskControlBlock),
  .stack_mem = &MotorDriverTaskBuffer[0],
  .stack_size = sizeof(MotorDriverTaskBuffer),
  .priority = (osPriority_t) osPriorityLow,
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void *argument);
void StartMotorDriverTask(void *argument);

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

  /* creation of MotorDriverTask */
  MotorDriverTaskHandle = osThreadNew(StartMotorDriverTask, NULL, &MotorDriverTask_attributes);

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

/* USER CODE BEGIN Header_StartMotorDriverTask */
/**
* @brief Function implementing the MotorDriverTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartMotorDriverTask */
void StartMotorDriverTask(void *argument)
{
  /* USER CODE BEGIN StartMotorDriverTask */
  CANHandle_StdID* p_can_handle[2];
  CANBuf_StdID rx_msg_can[2];
  MotorDriver_Handle_Typedef motor_driver[5];
  p_can_handle[0] = MainBoard_CAN_Init(&hcan1);
  p_can_handle[1] = MainBoard_CAN_Init(&hcan2);
  for(uint8_t i = 0; i < 5; i++){
    motor_driver[i].node_id = i + 2;
  }
  /* Infinite loop */
  for(;;)
  {
    for(uint8_t i = 0; i < 2; i++){
      if(GetRxMessage(p_can_handle[i], &rx_msg_can[i]) == HAL_OK){
        uint32_t communication_type = rx_msg_can[i].StdId >> 7;
        uint8_t fb_id = (rx_msg_can[i].StdId >> 4) & 0x07;
        uint8_t fb_index = fb_id - 2;
        if(fb_index < 5){
          switch (communication_type){
            case FEEDBACK:
              memcpy(&motor_driver[fb_index].feedback.current_pos, &rx_msg_can[i].bytes[0], 4);
              memcpy(&motor_driver[fb_index].feedback.current_vel, &rx_msg_can[i].bytes[4], 4);
              break;
            default:
              break;
          }
        }
      }
    }
    osDelay(1);
  }
  /* USER CODE END StartMotorDriverTask */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

