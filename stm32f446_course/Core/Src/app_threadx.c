/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    app_threadx.c
  * @author  MCD Application Team
  * @brief   ThreadX applicative file
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2021 STMicroelectronics.
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
#include "app_threadx.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "main.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* 생성된 GNU 포트의 SysTick은 100Hz. 숫자를 바꾸면 포트도 함께 확인함. */
_Static_assert(TX_TIMER_TICKS_PER_SECOND == 100u, "Check generated SysTick rate");
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN PV */
extern UART_HandleTypeDef huart2;
static TX_THREAD led_thread, uart_thread;
/* Thread 수명 동안 유지되는 스택. 각 1KiB는 시작값이며 실측 여유 확인이 필요함. */
static ULONG led_stack[256] __attribute__((aligned(8)));
static ULONG uart_stack[256] __attribute__((aligned(8)));
volatile UINT app_thread_create_status;
volatile uint32_t led_thread_count;
volatile uint32_t uart_thread_count;
volatile uint32_t uart_thread_tx_failures;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN PFP */
static void Led_Entry(ULONG input);
static void Uart_Entry(ULONG input);
/* USER CODE END PFP */

/**
  * @brief  Application ThreadX Initialization.
  * @param memory_ptr: memory pointer
  * @retval int
  */
UINT App_ThreadX_Init(VOID *memory_ptr)
{
  UINT ret = TX_SUCCESS;
  TX_BYTE_POOL *byte_pool = (TX_BYTE_POOL*)memory_ptr;

  /* USER CODE BEGIN App_ThreadX_Init */
  (void)byte_pool;
  /* 34강: 생성 훅에서 한 번만 생성. 작은 숫자가 높은 Thread 우선순위임. */
  ret = tx_thread_create(&led_thread, "LED", Led_Entry, 0u,
                         led_stack, sizeof led_stack,
                         15u, 15u, TX_NO_TIME_SLICE, TX_AUTO_START);
  app_thread_create_status = ret;
  if (ret != TX_SUCCESS)
  {
    return ret;
  }
  ret = tx_thread_create(&uart_thread, "UART", Uart_Entry, 0u,
                         uart_stack, sizeof uart_stack,
                         10u, 10u, TX_NO_TIME_SLICE, TX_AUTO_START);
  app_thread_create_status = ret;
  /* 실패는 호출한 tx_application_define에서 Error_Handler로 전달함. */
  /* USER CODE END App_ThreadX_Init */

  return ret;
}

/**
  * @brief  MX_ThreadX_Init
  * @param  None
  * @retval None
  */
void MX_ThreadX_Init(void)
{
  /* USER CODE BEGIN  Before_Kernel_Start */

  /* USER CODE END  Before_Kernel_Start */

  tx_kernel_enter();

  /* USER CODE BEGIN  Kernel_Start_Error */
  Error_Handler();
  /* USER CODE END  Kernel_Start_Error */
}

/* USER CODE BEGIN 1 */
static void Led_Entry(ULONG input)
{
  (void)input;
  for (;;)
  {
    /* PA5의 제어자는 이 Thread 하나. TIM6 점멸 실습은 시작하지 않음. */
    HAL_GPIO_TogglePin(LED_GPIO_Port, LED_Pin);
    led_thread_count++;
    /* 100 ticks/s에서 50 ticks. 대기 중 다른 Ready Thread가 실행됨. */
    if (tx_thread_sleep(TX_TIMER_TICKS_PER_SECOND / 2u) != TX_SUCCESS)
    {
      Error_Handler();
    }
  }
}

static void Uart_Entry(ULONG input)
{
  (void)input;
  static uint8_t message[] = "alive\r\n";
  for (;;)
  {
    /* USART2 송신은 이 Thread만 사용함. 여전히 HAL 폴링 송신임. */
    /* timeout 20은 TIM7 기반 HAL ms, 아래 Sleep 100은 커널 tick임. */
    if (HAL_UART_Transmit(&huart2, message, sizeof message - 1u, 20u) != HAL_OK)
    {
      uart_thread_tx_failures++;
    }
    uart_thread_count++;
    if (tx_thread_sleep(TX_TIMER_TICKS_PER_SECOND) != TX_SUCCESS)
    {
      Error_Handler();
    }
    /* 실제 반복 간격에는 송신 시간과 스케줄링 지연도 포함됨. */
  }
}
/* USER CODE END 1 */
