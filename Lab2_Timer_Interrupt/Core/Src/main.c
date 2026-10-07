/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2026 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
TIM_HandleTypeDef htim2;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_TIM2_Init(void);
static void MX_GPIO_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* ===== [BIẾN EX 1] ===== */
// int timer_counter = 50;
// int led_status = 0;

/* ===== [BIẾN EX 2] ===== */
// int timer_counter_7seg = 50;
// int timer_counter_dot = 100;
// int led_status_ex2 = 0;

/* ===== [BIẾN & HÀM EX 3 + EX 4] Quét LED bằng Buffer ===== */
const int MAX_LED = 4;
int index_led = 0;
int led_buffer[4] = {1, 2, 3, 0};

void display7SEG(int num) {
    uint8_t seg7[10] = {0xC0, 0xF9, 0xA4, 0xB0, 0x99, 0x92, 0x82, 0xF8, 0x80, 0x90};
    HAL_GPIO_WritePin(GPIOB, SEG0_Pin, (seg7[num] >> 0) & 1);
    HAL_GPIO_WritePin(GPIOB, SEG1_Pin, (seg7[num] >> 1) & 1);
    HAL_GPIO_WritePin(GPIOB, SEG2_Pin, (seg7[num] >> 2) & 1);
    HAL_GPIO_WritePin(GPIOB, SEG3_Pin, (seg7[num] >> 3) & 1);
    HAL_GPIO_WritePin(GPIOB, SEG4_Pin, (seg7[num] >> 4) & 1);
    HAL_GPIO_WritePin(GPIOB, SEG5_Pin, (seg7[num] >> 5) & 1);
    HAL_GPIO_WritePin(GPIOB, SEG6_Pin, (seg7[num] >> 6) & 1);
}

void update7SEG(int index) {
    HAL_GPIO_WritePin(GPIOA, EN0_Pin|EN1_Pin|EN2_Pin|EN3_Pin, GPIO_PIN_SET);

    switch (index) {
        case 0:
            display7SEG(led_buffer[0]);
            HAL_GPIO_WritePin(GPIOA, EN0_Pin, GPIO_PIN_RESET);
            break;
        case 1:
            display7SEG(led_buffer[1]);
            HAL_GPIO_WritePin(GPIOA, EN1_Pin, GPIO_PIN_RESET);
            break;
        case 2:
            display7SEG(led_buffer[2]);
            HAL_GPIO_WritePin(GPIOA, EN2_Pin, GPIO_PIN_RESET);
            break;
        case 3:
            display7SEG(led_buffer[3]);
            HAL_GPIO_WritePin(GPIOA, EN3_Pin, GPIO_PIN_RESET);
            break;
        default:
            break;
    }
}

/* ===== [BIẾN & HÀM EX 5] Đồng hồ số ===== */
int hour = 15, minute = 8, second = 50;

void updateClockBuffer() {
    led_buffer[0] = hour / 10;
    led_buffer[1] = hour % 10;
    led_buffer[2] = minute / 10;
    led_buffer[3] = minute % 10;
}

/* ===== [BIẾN & HÀM EX 9 + EX 10] Quét LED Ma trận ===== */
const int MAX_LED_MATRIX = 8;
int index_led_matrix = 0;
uint8_t matrix_buffer[8] = {0x18, 0x24, 0x42, 0x42, 0x7E, 0x42, 0x42, 0x00}; // Chữ "A"

void updateLEDMatrix(int index) {
    // Tắt toàn bộ 8 cột trước khi xuất dữ liệu mới
    HAL_GPIO_WritePin(GPIOA, ENM0_Pin|ENM1_Pin|ENM2_Pin|ENM3_Pin|
                             ENM4_Pin|ENM5_Pin|ENM6_Pin|ENM7_Pin, GPIO_PIN_RESET);

    // Đẩy dữ liệu hàng (ROW) ra Port B
    uint8_t row_data = matrix_buffer[index];
    HAL_GPIO_WritePin(GPIOB, ROW0_Pin, (row_data >> 0) & 1);
    HAL_GPIO_WritePin(GPIOB, ROW1_Pin, (row_data >> 1) & 1);
    HAL_GPIO_WritePin(GPIOB, ROW2_Pin, (row_data >> 2) & 1);
    HAL_GPIO_WritePin(GPIOB, ROW3_Pin, (row_data >> 3) & 1);
    HAL_GPIO_WritePin(GPIOB, ROW4_Pin, (row_data >> 4) & 1);
    HAL_GPIO_WritePin(GPIOB, ROW5_Pin, (row_data >> 5) & 1);
    HAL_GPIO_WritePin(GPIOB, ROW6_Pin, (row_data >> 6) & 1);
    HAL_GPIO_WritePin(GPIOB, ROW7_Pin, (row_data >> 7) & 1);

    // Bật cột (COL) tương ứng trên Port A
    switch (index) {
        case 0: HAL_GPIO_WritePin(GPIOA, ENM0_Pin, GPIO_PIN_SET); break;
        case 1: HAL_GPIO_WritePin(GPIOA, ENM1_Pin, GPIO_PIN_SET); break;
        case 2: HAL_GPIO_WritePin(GPIOA, ENM2_Pin, GPIO_PIN_SET); break;
        case 3: HAL_GPIO_WritePin(GPIOA, ENM3_Pin, GPIO_PIN_SET); break;
        case 4: HAL_GPIO_WritePin(GPIOA, ENM4_Pin, GPIO_PIN_SET); break;
        case 5: HAL_GPIO_WritePin(GPIOA, ENM5_Pin, GPIO_PIN_SET); break;
        case 6: HAL_GPIO_WritePin(GPIOA, ENM6_Pin, GPIO_PIN_SET); break;
        case 7: HAL_GPIO_WritePin(GPIOA, ENM7_Pin, GPIO_PIN_SET); break;
        default: break;
    }
}


/* ===== [QUẢN LÝ SOFTWARE TIMER] Đa nhiệm (Ex 6 - Ex 10) ===== */
int TIMER_CYCLE = 10; // Chu kỳ ngắt timer 2 là 10ms

// Timer 0: Đồng hồ và chớp DOT (1s)
int timer0_counter = 0;
int timer0_flag = 0;
void setTimer0(int duration) {
    timer0_counter = duration / TIMER_CYCLE;
    timer0_flag = 0;
}

// Timer 1: Quét LED 7 đoạn (250ms)
int timer1_counter = 0;
int timer1_flag = 0;
void setTimer1(int duration) {
    timer1_counter = duration / TIMER_CYCLE;
    timer1_flag = 0;
}

// Timer 2: Quét LED Matrix (10ms)
int timer2_counter = 0;
int timer2_flag = 0;
void setTimer2(int duration) {
    timer2_counter = duration / TIMER_CYCLE;
    timer2_flag = 0;
}

// Timer 3: Hiệu ứng dịch chữ Matrix (500ms)
int timer3_counter = 0;
int timer3_flag = 0;
void setTimer3(int duration) {
    timer3_counter = duration / TIMER_CYCLE;
    timer3_flag = 0;
}

void timer_run() {
    if (timer0_counter > 0) {
        timer0_counter--;
        if (timer0_counter == 0) timer0_flag = 1;
    }
    if (timer1_counter > 0) {
        timer1_counter--;
        if (timer1_counter == 0) timer1_flag = 1;
    }
    if (timer2_counter > 0) {
        timer2_counter--;
        if (timer2_counter == 0) timer2_flag = 1;
    }
    if (timer3_counter > 0) {
        timer3_counter--;
        if (timer3_counter == 0) timer3_flag = 1;
    }
}
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_TIM2_Init();
  MX_GPIO_Init();
  /* USER CODE BEGIN 2 */
    HAL_TIM_Base_Start_IT(&htim2);

    // Khởi tạo các Software Timer
    setTimer0(1000); // Đếm 1000ms cho đồng hồ
    setTimer1(250);  // Đếm 250ms cho việc quét LED 7 đoạn
    setTimer2(10);   // Đếm 10ms để quét Ma trận LED (Ex 9)
    setTimer3(500);  // Đếm 500ms tạo Animation dịch chữ (Ex 10)
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */

      /* ===== [CODE CŨ] Đồng hồ và 7-Đoạn (Đã comment cho mạch LED Matrix) ===== */

//       TASK 1: Xử lý đồng hồ và chớp LED DOT (Mỗi 1 giây)
//       if (timer0_flag == 1) {
//           HAL_GPIO_TogglePin(GPIOA, DOT_Pin);
//           second++;
//           if (second >= 60){ second = 0; minute++; }
//           if (minute >= 60) { minute = 0; hour++; }
//           if (hour >= 24) { hour = 0; }
//           updateClockBuffer();
//           setTimer0(1000);
//       }
//
//       TASK 2: Xử lý quét LED 7 đoạn (Mỗi 250ms)
//       if (timer1_flag == 1) {
//           update7SEG(index_led++);
//           if (index_led >= MAX_LED) { index_led = 0; }
//           setTimer1(250);
//       }

      /* =========================================================
       * [CODE EX 9 & EX 10] ĐIỀU KHIỂN MA TRẬN LED
       * ========================================================= */

      // TASK 3: Quét Ma Trận LED liên tục để hiển thị (EX 9)
      if (timer2_flag == 1) {
          updateLEDMatrix(index_led_matrix++);
          if (index_led_matrix >= MAX_LED_MATRIX) {
              index_led_matrix = 0;
          }
          setTimer2(10); // Cài đặt chu kỳ quét 10ms
      }

      // TASK 4: Hiệu ứng dịch chữ sang trái (EX 10)
      if (timer3_flag == 1) {
          // Lưu lại phần tử đầu tiên của mảng
          uint8_t temp = matrix_buffer[0];

          // Dịch toàn bộ các phần tử sang trái 1 cột
          for (int i = 0; i < 7; i++) {
              matrix_buffer[i] = matrix_buffer[i + 1];
          }

          // Đẩy phần tử đầu tiên xuống cuối cùng để lặp liên tục
          matrix_buffer[7] = temp;

          setTimer3(500); // Chu kỳ dịch chữ 500ms
      }


  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief TIM2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM2_Init(void)
{

  /* USER CODE BEGIN TIM2_Init 0 */
  /* USER CODE END TIM2_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM2_Init 1 */
  /* USER CODE END TIM2_Init 1 */
  htim2.Instance = TIM2;
  htim2.Init.Prescaler = 7999;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = 9;
  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim2) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim2, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM2_Init 2 */
  /* USER CODE END TIM2_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, ENM0_Pin|ENM1_Pin|DOT_Pin|LED_RED_Pin
                          |EN0_Pin|EN1_Pin|EN2_Pin|EN3_Pin
                          |ENM2_Pin|ENM3_Pin|ENM4_Pin|ENM5_Pin
                          |ENM6_Pin|ENM7_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, SEG0_Pin|SEG1_Pin|SEG2_Pin|ROW2_Pin
                          |ROW3_Pin|ROW4_Pin|ROW5_Pin|ROW6_Pin
                          |ROW7_Pin|SEG3_Pin|SEG4_Pin|SEG5_Pin
                          |SEG6_Pin|ROW0_Pin|ROW1_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pins : ENM0_Pin ENM1_Pin DOT_Pin LED_RED_Pin
                           EN0_Pin EN1_Pin EN2_Pin EN3_Pin
                           ENM2_Pin ENM3_Pin ENM4_Pin ENM5_Pin
                           ENM6_Pin ENM7_Pin */
  GPIO_InitStruct.Pin = ENM0_Pin|ENM1_Pin|DOT_Pin|LED_RED_Pin
                          |EN0_Pin|EN1_Pin|EN2_Pin|EN3_Pin
                          |ENM2_Pin|ENM3_Pin|ENM4_Pin|ENM5_Pin
                          |ENM6_Pin|ENM7_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : SEG0_Pin SEG1_Pin SEG2_Pin ROW2_Pin
                           ROW3_Pin ROW4_Pin ROW5_Pin ROW6_Pin
                           ROW7_Pin SEG3_Pin SEG4_Pin SEG5_Pin
                           SEG6_Pin ROW0_Pin ROW1_Pin */
  GPIO_InitStruct.Pin = SEG0_Pin|SEG1_Pin|SEG2_Pin|ROW2_Pin
                          |ROW3_Pin|ROW4_Pin|ROW5_Pin|ROW6_Pin
                          |ROW7_Pin|SEG3_Pin|SEG4_Pin|SEG5_Pin
                          |SEG6_Pin|ROW0_Pin|ROW1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    if(htim->Instance == TIM2) {

        // HÀM NGẮT Ở EX 8-10 ĐƯỢC TỐI GIẢN
        // Gọi hàm cập nhật các Timer đếm ngược.
        timer_run();

        /* ===== [CODE NGẮT CÁC BÀI CŨ TỪ EX 1 - EX 4] ===== */

        /* [EX 1 & 2] Dùng if-else dài dòng trong ngắt */
        // timer_counter_7seg--;
        // if(timer_counter_7seg <= 0) {
        //     timer_counter_7seg = 50;
        //     HAL_GPIO_WritePin(GPIOA, EN0_Pin|EN1_Pin|EN2_Pin|EN3_Pin, GPIO_PIN_SET);
        //     if(led_status_ex2 == 0) {
        //         display7SEG(1);
        //         HAL_GPIO_WritePin(GPIOA, EN0_Pin, GPIO_PIN_RESET);
        //         led_status_ex2 = 1;
        //     }
        //     // ... (các case khác) ...
        // }

        /* [EX 3 & 4 & 7] Dùng hàm quét mảng Buffer trong ngắt */
        // timer_counter_7seg--;
        // if(timer_counter_7seg <= 0) {
        //     timer_counter_7seg = 25; // 250ms
        //     update7SEG(index_led++);
        //     if (index_led >= MAX_LED) {
        //         index_led = 0;
        //     }
        // }

    }
}
/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
