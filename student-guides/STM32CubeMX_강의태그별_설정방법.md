# STM32CubeMX 강의 Tag별 설정 방법

이 문서는 07~34강을 따라가며 **자신의 실습 프로젝트에 필요한 설정을 단계적으로 추가하는 방법**을 안내합니다. 이전 강의의 설정은 유지하고, 해당 강의에서 바뀌는 항목만 수정합니다.

완성된 참고 코드를 실행하려면 해당 Tag를 체크아웃하여 CubeIDE에서 빌드하면 됩니다. 단순히 참고 코드를 확인하기 위해 CubeMX로 다시 생성할 필요는 없습니다. Tag 사용 방법은 [VS Code·Git Graph 안내](VSCode_GIT_Graph_설치.md)를 참고합니다.

## 1. 기준 환경과 작업 순서

| 항목 | 강의 참고 저장소 기준 |
| --- | --- |
| 보드 | NUCLEO-F446RE |
| MCU | STM32F446RET6, LQFP64 |
| STM32CubeMX | 6.17.0 |
| STM32CubeF4 패키지 | V1.28.3 |
| 코드 생성 대상 | STM32CubeIDE |
| 프로젝트 이름 | `stm32f446_course` |
| 설정 파일 | `stm32f446_course/stm32f446_course.ioc` |
| ThreadX 패키지 — 33강부터 | X-CUBE-AZRTOS-F4 V1.1.0 |

CubeMX 설치는 [설치 안내](STM32CubeMX_설치.md)를 참고합니다. 다른 버전에서는 메뉴 이름이나 기본값이 달라질 수 있으므로 **설정값과 생성된 코드**를 함께 확인합니다.

설정이 바뀌는 강의에서는 다음 순서로 진행합니다.

1. 실습 파일을 저장하고, 현재 상태를 커밋하거나 별도 폴더에 백업합니다.
2. 자신의 실습 프로젝트 `.ioc`를 CubeMX에서 엽니다.
3. 아래의 해당 강의 항목을 설정합니다.
4. **Project Manager → Code Generator → Keep User Code when re-generating**을 활성화합니다.
5. **GENERATE CODE**를 누르고, CubeIDE에서 프로젝트를 Refresh합니다.
6. 해당 강의의 C 코드를 작성하고 **Project → Clean → Build Project**로 빌드를 확인합니다.

사용자 코드는 `USER CODE BEGIN`과 `USER CODE END` 사이 또는 별도 사용자 파일에 작성합니다. 코드 보존 옵션을 켜도 생성 영역에 직접 쓴 코드는 보존되지 않을 수 있습니다.

## 2. 설정 변경 시점 한눈에 보기

| 강의 | 기준 Tag | 이번에 추가하거나 바꾸는 설정 |
| --- | --- | --- |
| 07~17 | `lec07-cubemx` | PA5 LED, SWD, HSI 기반 84MHz. 이 구간의 `.ioc`는 동일 |
| 18~20 | `lec18-gpio-input-exti` | PC13 버튼, Falling EXTI, EXTI15_10 IRQ |
| 21~22 | `lec21-tim-basic-counter` | TIM6, 100ms Update. TIM6 IRQ는 아직 비활성화 |
| 23 | `lec23-timer-interrupt` | TIM6_DAC IRQ 활성화 |
| 24 | `lec24-pwm-motor-control` | PA0의 TIM2_CH1 PWM, 20kHz |
| 25~26 | `lec25-uart-basics` | USART2, PA2/PA3, 115200·8N1 |
| 27~28 | `lec27-uart-interrupt-ring-buffer` | USART2 IRQ 활성화. 28강에서는 DMA를 아직 추가하지 않음 |
| 29~32 | `lec29-uart-dma-idle` | USART2 RX Circular DMA, DMA1 Stream5 IRQ |
| 33~34 | `lec33-cortex-m-rtos-architecture` | ThreadX 추가, HAL 시간 기준을 TIM7로 변경 |

같은 행에 묶인 강의는 `.ioc`를 재사용합니다. **코드 변경과 CubeMX 설정 변경은 별개**입니다. 예를 들어 34강의 스레드 추가는 C 코드 작업이며, 33강과 `.ioc`가 같습니다.

## 3. 07강 — 기본 프로젝트 만들기

### 보드와 핀

1. CubeMX의 **New Project → Board Selector**에서 `NUCLEO-F446RE`를 선택합니다.
2. 보드 기본 주변장치를 모두 초기화할지 묻는 경우 **No**를 선택하고 필요한 항목을 아래처럼 설정합니다. 이미 기본 설정을 불러왔다면 표에 없는 주변장치가 켜져 있지 않은지 확인합니다.
3. **System Core → SYS → Debug**를 **Serial Wire**로 설정합니다.
4. Pinout에서 **PA5 → GPIO_Output**을 선택합니다.
5. **System Core → GPIO**에서 PA5를 다음과 같이 설정합니다.

| PA5 항목 | 값 |
| --- | --- |
| User Label | `LED` |
| GPIO output level | Low |
| GPIO mode | Output Push Pull |
| GPIO Pull-up/Pull-down | No pull-up and no pull-down |
| Maximum output speed | Low |

PA13은 SWDIO, PA14는 SWCLK로 유지합니다. 07강에서는 PC13, TIM2, TIM6, USART2를 아직 추가하지 않습니다.

### 클럭

**Clock Configuration**에서 내부 HSI를 PLL 입력으로 사용합니다. 외부 HSE를 선택하지 않습니다.

| 항목 | 값 |
| --- | --- |
| HSI | 16MHz |
| PLL Source | HSI |
| PLLM / PLLN / PLLP | 16 / 336 / 4 |
| System Clock Mux | PLLCLK |
| AHB Prescaler | /1 |
| APB1 Prescaler | /2 |
| APB2 Prescaler | /1 |
| SYSCLK / HCLK | 84MHz / 84MHz |
| APB1 / APB2 peripheral clock | 42MHz / 84MHz |
| APB1 timer clock | 84MHz |

**SYS → Timebase Source**는 **SysTick**으로 유지합니다. HAL 시간 기준은 1ms입니다.

### 코드 생성

1. **Project Manager → Project**에서 프로젝트 이름과 저장 위치를 지정합니다.
2. **Toolchain / IDE**는 **STM32CubeIDE**를 선택합니다.
3. 펌웨어 패키지는 **STM32Cube FW_F4 V1.28.3**인지 확인합니다. 최신 버전을 자동 선택하는 옵션이 있다면 강의 기준 버전을 직접 선택합니다.
4. 사용자 코드 보존 옵션을 켜고 생성합니다.

생성 후 `main.c`의 `SystemClock_Config()`와 `MX_GPIO_Init()`을 확인합니다. 08~17강에서는 이 설정을 그대로 사용합니다.

## 4. 18강 — PC13 버튼과 EXTI

1. Pinout에서 **PC13 → GPIO_EXTI13**을 선택합니다.
2. **System Core → GPIO**에서 아래 값을 설정합니다.
3. **NVIC**에서 **EXTI line[15:10] interrupts**를 활성화합니다.

| 항목 | 값 |
| --- | --- |
| User Label | `B1` |
| GPIO mode | External Interrupt Mode with Falling edge trigger detection |
| Pull-up/Pull-down | No pull-up and no pull-down |
| EXTI15_10 선점 / 서브 우선순위 | 0 / 0 |

이 값은 NUCLEO-F446RE의 B1 버튼을 사용하는 참고 프로젝트 기준입니다. 생성 코드에서 `GPIO_MODE_IT_FALLING`, `GPIO_NOPULL`, `HAL_GPIO_EXTI_IRQHandler(B1_Pin)`을 확인합니다.

버튼의 디바운스와 LED 전환은 강의 C 코드에서 구현합니다. 19~20강에서는 CubeMX 설정을 바꾸지 않습니다.

## 5. 21강 — TIM6의 100ms Update

1. **Timers → TIM6**에서 타이머를 활성화합니다. 이 버전에서는 **Activated** 항목으로 표시될 수 있습니다.
2. **Parameter Settings**를 아래처럼 설정합니다.
3. **NVIC Settings**의 TIM6_DAC IRQ는 아직 활성화하지 않습니다.

| 항목 | 값 |
| --- | --- |
| Clock Source | Internal Clock |
| Prescaler — PSC | 8399 |
| Counter Mode | Up |
| Counter Period — ARR | 999 |
| Auto-reload preload | Disable |

APB1 타이머 클럭은 84MHz이므로 다음과 같이 계산합니다.

```text
카운터 클럭 = 84,000,000 / (8399 + 1) = 10,000 Hz
Update 주기 = (999 + 1) / 10,000 = 0.1초 = 100ms
```

생성 코드의 `htim6.Init.Prescaler`와 `Period`를 확인합니다. 타이머 시작과 Update 플래그 폴링은 C 코드에서 구현합니다. 22강은 같은 설정을 사용합니다.

## 6. 23강 — TIM6 인터럽트

1. TIM6의 PSC와 ARR은 그대로 둡니다.
2. **TIM6 → NVIC Settings**에서 **TIM6 global interrupt, DAC1 and DAC2 underrun error interrupts**를 활성화합니다.
3. 코드를 생성합니다.
4. 아래 우선순위 코드를 `main.c`의 **USER CODE BEGIN 2** 안에 둡니다. `MX_TIM6_Init()` 이후, `HAL_TIM_Base_Start_IT()` 이전에 실행되어야 합니다.

```c
/* 생성된 기본 우선순위를 강의 실습값으로 변경합니다. */
HAL_NVIC_SetPriority(TIM6_DAC_IRQn, 5U, 0U);
```

참고 `.ioc`에는 0/0이 저장되어 있고, 강의 C 코드에서 **선점 5 / 서브 0**으로 덮어씁니다. CubeMX에서 우선순위를 클릭할 수 없는 경우에도 이 방식으로 적용합니다. 이미 참고 코드에 있는 줄을 중복 추가하지 않습니다.

우선순위 그룹은 `NVIC_PRIORITYGROUP_4`를 유지합니다. 이 그룹에서는 선점 우선순위만 사용하고 서브 우선순위는 0으로 둡니다. IRQ 활성화만으로 타이머가 시작되지는 않으므로 시작 코드와 콜백도 해당 강의 Tag와 비교합니다.

## 7. 24강 — TIM2 CH1 PWM

1. **Timers → TIM2**에서 **Channel1 → PWM Generation CH1**을 선택합니다.
2. 출력 핀이 **PA0 / TIM2_CH1**인지 확인합니다. 다른 핀이 선택되었다면 PA0으로 지정합니다.
3. 다음 값을 설정하고 생성합니다.

| 항목 | 값 |
| --- | --- |
| Prescaler | 0 |
| Counter Mode | Up |
| Counter Period | 4199 |
| Auto-reload preload | Enable |
| CH1 PWM Mode | PWM mode 1 |
| CH1 Pulse | 2100 |
| CH1 Polarity | High |
| PA0 Alternate Function | AF1 — TIM2 |

```text
PWM 주파수 = 84,000,000 / (0 + 1) / (4199 + 1) = 20,000 Hz
초기 Duty = 2100 / (4199 + 1) = 50%
```

생성 후 `HAL_TIM_MspPostInit()`의 PA0 설정도 확인합니다. PWM 시작과 이후 Duty 변경은 강의 C 코드에서 수행합니다.

## 8. 25강 — USART2 기본 통신

1. **Connectivity → USART2 → Mode**를 **Asynchronous**로 설정합니다.
2. 핀을 **PA2: USART2_TX**, **PA3: USART2_RX**로 확인합니다.
3. **Parameter Settings**를 아래처럼 설정합니다.

| 항목 | 값 |
| --- | --- |
| Baud Rate | 115200 Bits/s |
| Word Length | 8 Bits |
| Parity | None |
| Stop Bits | 1 |
| Data Direction | Receive and Transmit |
| Hardware Flow Control | None |
| Over Sampling | 16 Samples |
| PA2 / PA3 Alternate Function | AF7 — USART2 |

25~26강에서는 USART2 IRQ와 DMA를 추가하지 않습니다. 생성 코드에서 `huart2.Init` 값을 확인합니다. PC 시리얼 터미널도 **115200·8N1·흐름 제어 없음**으로 맞춥니다.

## 9. 27강 — USART2 수신 인터럽트

1. USART2 핀과 통신 속도는 유지합니다.
2. **USART2 → NVIC Settings → USART2 global interrupt**를 활성화하고 생성합니다.
3. `main.c`의 **USER CODE BEGIN 2**에서 `MX_USART2_UART_Init()` 이후, 수신 시작 이전에 아래 설정을 적용합니다.

```c
HAL_NVIC_SetPriority(USART2_IRQn, 5U, 0U);
```

참고 `.ioc`의 0/0은 C 코드에서 5/0으로 변경됩니다. 수신 시작, 링 버퍼, 오류 복구는 27강 C 코드를 따라 작성합니다. CubeMX는 이 학습용 동작까지 자동 생성하지 않습니다.

28강은 DMA 개념 강의이므로 이 설정을 그대로 사용합니다.

## 10. 29강 — USART2 RX Circular DMA와 IDLE

1. **USART2 → DMA Settings → Add**에서 **USART2_RX**를 추가합니다.
2. 아래 설정을 적용합니다.
3. **NVIC Settings**에서 **DMA1 Stream5 global interrupt**와 **USART2 global interrupt**가 모두 활성화되어 있는지 확인합니다.

| DMA 항목 | 값 |
| --- | --- |
| Stream / Channel | DMA1 Stream5 / Channel4 |
| Direction | Peripheral to Memory |
| Mode | Circular |
| Peripheral Increment | Disable |
| Memory Increment | Enable |
| Peripheral Data Width | Byte |
| Memory Data Width | Byte |
| Priority | Low |
| FIFO | Disable |

여기서 DMA의 **Priority: Low**는 DMA 내부 중재 우선순위입니다. 다음의 CPU 인터럽트 선점 우선순위 5와는 다른 항목입니다.

생성 후 초기화 순서가 **`MX_DMA_Init()` → `MX_USART2_UART_Init()`**인지 확인합니다. CubeMX의 **Project Manager → Advanced Settings**에서 초기화 함수 순서를 확인할 수 있습니다.

`main.c`의 **USER CODE BEGIN 2**에서 두 초기화 함수가 실행된 뒤, DMA 수신을 시작하기 전에 다음 설정을 적용합니다.

```c
/* 두 수신 콜백이 서로 선점하지 않도록 같은 우선순위를 사용합니다. */
HAL_NVIC_SetPriority(DMA1_Stream5_IRQn, 5U, 0U);
HAL_NVIC_SetPriority(USART2_IRQn, 5U, 0U);
```

CubeMX에서 5/0을 선택할 수 없어도 생성 후 이 코드로 적용하면 됩니다. IDLE 수신은 별도의 CubeMX 핀 설정이 아니라 C 코드의 `HAL_UARTEx_ReceiveToIdle_DMA()` 호출과 수신 콜백으로 구현합니다.

생성 코드에서 `DMA_CHANNEL_4`, `DMA_CIRCULAR`, UART와 DMA 핸들 연결, 두 IRQ 핸들러를 확인합니다. 30~32강은 이 `.ioc`를 유지합니다.

## 11. 33강 — ThreadX와 TIM7 시간 기준

**ThreadX 설정은 33강부터 적용합니다.** 기존 84MHz 클럭, LED·버튼·UART 핀과 주변장치 설정을 유지합니다.

### ThreadX 패키지 선택

1. **Software Packs → Select Components**를 엽니다.
2. **STMicroelectronics → X-CUBE-AZRTOS-F4 V1.1.0**을 찾습니다. 설치되지 않았다면 패키지 설치 화면에서 먼저 설치합니다.
3. **RTOS ThreadX → Core**를 선택합니다. 참고 `.ioc`에는 Low Power support, PerformanceInfo, TraceX support도 선택되어 있습니다. 강의 기준과 맞추려면 같은 선택 상태인지 확인합니다.
4. 선택을 적용한 뒤 **Middleware and Software Packs**의 X-CUBE-AZRTOS-F4 설정에서 **RTOS ThreadX**를 활성화합니다.
5. FileX·NetX Duo·USBX 등 이번 강의에서 사용하지 않는 구성 요소는 추가하지 않습니다.

STM32F4용 패키지와 다른 MCU 계열의 패키지를 혼동하지 않습니다. 패키지 정보는 [ST 공식 X-CUBE-AZRTOS-F4 안내](https://www.st.com/en/embedded-software/x-cube-azrtos-f4.html)를 참고합니다.

### HAL 시간 기준 분리

1. **System Core → SYS → Timebase Source**를 **TIM7**로 바꿉니다.
2. TIM6 실습 설정은 유지합니다. HAL 시간 기준으로 TIM6를 선택하지 않습니다.
3. 코드를 생성합니다.
4. `main.c`의 **USER CODE BEGIN 2**에 아래 코드가 있는지 확인합니다. 주변장치 초기화 이후, `MX_ThreadX_Init()` 호출 이전에 실행되어야 합니다.

```c
/* PendSV의 대기 중에도 HAL의 1ms tick이 실행되게 합니다. */
HAL_NVIC_SetPriority(TIM7_IRQn, 14U, 0U);
uwTickPrio = 14U;
```

참고 `.ioc`에는 TIM7 우선순위가 15/0으로 저장되어 있지만, C 코드에서 **14/0**으로 변경합니다. 이 프로젝트의 PendSV 우선순위 15보다 TIM7을 높여 HAL tick이 멈추는 문제를 방지하는 설정입니다. Cortex-M 인터럽트는 숫자가 작을수록 우선순위가 높습니다.

### 생성 후 확인

| 확인 위치 | 확인할 내용 |
| --- | --- |
| `Core/Src/main.c` | 커널 진입을 위한 `MX_ThreadX_Init()` 호출 |
| `Core/Src/stm32f4xx_hal_timebase_tim.c` | TIM7 기반 HAL tick 초기화 |
| `Core/Src/main.c`의 타이머 콜백 | TIM7일 때 `HAL_IncTick()` 호출 |
| `Core/Src/stm32f4xx_it.c` | TIM7 IRQ에서 `HAL_TIM_IRQHandler(&htim7)` 호출 |
| `Core/Src/app_threadx.c` | ThreadX 애플리케이션 코드 |
| `Core/Src/tx_initialize_low_level.s` | 강의의 84MHz 클럭과 100Hz ThreadX tick에 맞는 포트 설정 |
| `Middlewares/ST/threadx` | ThreadX 소스와 Cortex-M4용 포트 |

이 강의의 **HAL tick은 1ms**, **ThreadX tick은 10ms — 100Hz**입니다. 두 시간 단위를 같다고 생각하지 않도록 주의합니다.

ThreadX 포트가 제공하는 SysTick·PendSV 핸들러와 같은 이름의 함수를 따로 추가하면 중복 정의가 생길 수 있습니다. 생성 코드와 33강 Tag의 포트·빌드 설정을 함께 비교합니다. 단순히 패키지를 선택하고 생성하는 것만으로 강의의 C 예제가 완성되지는 않습니다.

33~34강에서는 이전 DMA·PWM·TIM6 실습의 하드웨어 설정은 남겨 두되, 해당 실습 동작을 시작하지 않습니다. 과거 강의의 시작 코드를 모두 누적해 실행하지 말고 현재 Tag의 사용자 코드와 비교합니다.

## 12. 34강 — 같은 설정에서 두 스레드 작성

34강은 33강의 `.ioc`를 그대로 사용합니다. CubeMX 설정을 바꾸거나 다시 생성할 필요는 없습니다.

`Core/Src/app_threadx.c`에서 LED 스레드와 UART 스레드를 작성하는 것이 이번 강의의 작업입니다. `main.c`의 TIM7 우선순위 14/0 설정도 유지합니다.

## 설정 완료 체크리스트

- [ ] 해당 강의에서 설정 변경이 필요한지 표로 확인했습니다.
- [ ] 클럭은 SYSCLK/HCLK 84MHz, APB1 42MHz를 유지했습니다.
- [ ] 필요한 핀·주변장치·IRQ만 해당 시점에 추가했습니다.
- [ ] CubeMX 설정과 C 코드에서 적용하는 우선순위를 구분했습니다.
- [ ] 사용자 코드를 보존하고, 생성 후 해당 Tag와 Diff를 비교했습니다.
- [ ] CubeIDE에서 Clean 후 전체 빌드에 성공했습니다.

빌드 성공과 실제 보드 동작 확인은 별도입니다. 버튼·PWM·UART·스레드 동작은 해당 강의의 실습 절차로 확인합니다.
