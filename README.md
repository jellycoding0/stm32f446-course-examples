# STM32F446 강의 실습 예제

NUCLEO-F446RE로 진행하는 MCU 임베디드 SW 강의의 수강생용 참고 저장소입니다. **07~34강의 전체 CubeIDE 프로젝트**를 강의별 Tag로 제공합니다. 수강생은 해당 Tag를 체크아웃하여 빌드하고, 이전 강의와 코드 Diff를 비교하며 실습할 수 있습니다.

`main`의 펌웨어는 **34강 ThreadX LED·UART 멀티태스킹** 상태입니다. 특정 강의를 따라갈 때는 아래 목록에서 해당 Tag를 선택합니다. 설치와 사용 안내 문서는 `main`의 최신 문서를 참고합니다.

## 처음 시작하는 순서

| 순서 | 안내 문서 | 내용 |
| --- | --- | --- |
| 1 | [STM32CubeMX 설치](student-guides/STM32CubeMX_설치.md) | 핀·클럭·주변장치 설정 도구 설치 |
| 2 | [STM32CubeIDE 설치](student-guides/STM32CubeIDE%20설치.md) | 빌드·다운로드·디버깅 도구 설치 |
| 3 | [VS Code·Git·Git Graph 설치와 사용](student-guides/VSCode_GIT_Graph_설치.md) | 저장소 다운로드, Tag 전환, 코드 비교 |
| 4 | [강의 Tag별 CubeMX 설정](student-guides/STM32CubeMX_강의태그별_설정방법.md) | 직접 실습 프로젝트를 만들 때 필요한 단계별 설정 |

직접 만드는 실습 프로젝트와 내려받은 참고 저장소는 별도 폴더로 관리합니다. 완성된 참고 프로젝트를 빌드하기 위해 CubeMX 코드를 다시 생성할 필요는 없습니다.

## 하드웨어 참고 자료

실습 중 핀 연결이나 레지스터 동작을 확인할 때는 아래 PDF를 참고합니다. 자료는 [`student-guides/hw_spec/`](student-guides/hw_spec/)에 모아 두었습니다.

| 자료 | 확인할 내용 |
| --- | --- |
| [보드 회로도 — MB1136 C04](student-guides/hw_spec/mb1136-default-c04_schematic.pdf) | LED·버튼·전원·ST-LINK·커넥터의 실제 배선과 솔더 브리지 |
| [보드 사용자 매뉴얼 — UM1724](student-guides/hw_spec/board_user_manual_um1724.pdf) | Nucleo 보드의 커넥터·점퍼·내장 ST-LINK 연결 등 보드 구성 |
| [STM32F446 데이터시트](student-guides/hw_spec/mcu_datasheet_stm32f446.pdf) | MCU 핀 배치·대체 기능(AF), 전기적 특성과 동작 조건 |
| [주변장치 레퍼런스 매뉴얼 — RM0390](student-guides/hw_spec/mcu_reference_manual_rm0390.pdf) | RCC·GPIO·타이머·USART·DMA 등 주변장치의 동작과 레지스터·비트 정의 |
| [Cortex-M4 프로그래밍 매뉴얼 — PM0214](student-guides/hw_spec/mcu_programming_manual_pm0214.pdf) | 코어 레지스터, 예외·인터럽트, NVIC·SysTick 등 CPU 구조 |

**실제 배선은 회로도, 보드 사용법·점퍼 설정은 UM1724, 핀의 기능·전기 조건은 데이터시트, 주변장치 비트는 RM0390, CPU의 예외 처리는 PM0214**에서 찾으면 됩니다. 문서가 여러 모델을 함께 설명하는 경우 NUCLEO-F446RE·STM32F446RE와 사용하는 보드 리비전·패키지에 해당하는 항목인지 확인합니다.

06강에서는 회로도 3쪽(MCU)에서 LED·버튼 연결을, 4쪽에서 ST-LINK를, 5쪽에서 확장 커넥터를 확인합니다. 제공한 회로도는 **MB1136 C04** 기준이므로 보드의 리비전과 실제 솔더 브리지 상태를 함께 확인합니다. 다른 리비전의 회로도는 [ST 공식 보드 페이지의 CAD Resources](https://www.st.com/en/evaluation-tools/nucleo-f446re.html#cad-resources)에서 찾습니다.

이 자료와 최신 사용 안내는 `main` 기준입니다. 과거 강의 Tag에 자료가 없다면 GitHub의 `main`에서 열어 두고 실습합니다.

## 강의 기준 환경

| 항목 | 기준 |
| --- | --- |
| 보드 / MCU | NUCLEO-F446RE / STM32F446RET6 |
| 개발 환경 | Windows, STM32CubeIDE 2.1.1 |
| 코드 생성 도구 | STM32CubeMX 6.17.0 |
| MCU 펌웨어 패키지 | STM32CubeF4 V1.28.3 |
| ThreadX 패키지 — 33강부터 | X-CUBE-AZRTOS-F4 V1.1.0 |
| 시스템 클럭 | HSI 16MHz → PLL → SYSCLK/HCLK 84MHz |
| 버스 클럭 | APB1 42MHz / APB2 84MHz |
| 다운로드·디버깅 | 보드 내장 ST-LINK, SWD |

주변장치는 강의에 따라 추가됩니다. HAL 시간 기준은 07~32강에서 SysTick 1ms, 33~34강에서 TIM7 1ms입니다. ThreadX는 별도의 SysTick 100Hz — 10ms tick을 사용합니다.

## 저장소 내려받기와 Tag 선택

PowerShell에서 다음 명령을 한 줄씩 실행합니다. 이미 내려받았다면 기존 저장소 폴더에서 시작합니다.

```powershell
New-Item -ItemType Directory -Force -Path C:\STM32Course
Set-Location C:\STM32Course
git clone https://github.com/jellycoding0/stm32f446-course-examples.git
Set-Location .\stm32f446-course-examples
git tag --list 'lec*' --sort=version:refname
```

예를 들어 09강의 500ms LED 토글을 확인하려면 다음처럼 전환합니다.

```powershell
git status
git switch --detach lec09-blink-500ms
```

전환 전에 편집 내용을 저장하고, 필요한 변경은 개인 브랜치에 커밋하거나 별도 폴더에 백업합니다. CubeIDE에서 열어 둔 프로젝트도 닫고 전환합니다. **Detached HEAD는 특정 Tag의 코드를 확인하는 정상적인 상태**입니다. 직접 수정하고 커밋하려면 개인 실습 브랜치를 만들어 사용합니다.

이후의 모든 Git 명령은 저장소 폴더 안에서 실행합니다. 과거 Tag에는 최신 README와 `student-guides`가 없을 수 있으므로 이 안내는 GitHub의 `main`에서 브라우저로 열어 두면 편리합니다.

## CubeIDE에서 빌드하고 실행하기

1. **File → Import → General → Existing Projects into Workspace**를 선택합니다.
2. 저장소 안의 `stm32f446_course` 폴더를 지정합니다.
3. **Copy projects into workspace**는 해제합니다. 이미 가져온 프로젝트라면 다시 Import하지 않고 열어서 Refresh합니다.
4. 프로젝트 위치가 Tag를 전환한 저장소 안인지 확인합니다.
5. **Debug** 구성을 선택하고 **Project → Clean → Build Project**를 수행합니다.
6. 빌드 성공 후 보드의 ST-LINK USB를 연결하고 Debug를 실행합니다. `main`에서 멈추면 **Resume(F8)**으로 진행합니다.

생성되는 ELF는 `stm32f446_course/Debug/stm32f446_course.elf`입니다. 빌드 산출물은 Git에서 제외되므로 **Tag를 전환한 뒤에는 새로 빌드한 ELF를 사용**합니다. LED 주기나 UART 출력 등 예상 동작은 선택한 강의 코드에 따라 달라집니다.

## 강의와 Tag

| 강의 | 주요 내용 | Tag |
| --- | --- | --- |
| 07 | CubeMX 기본 프로젝트 생성 | `lec07-cubemx` |
| 08 | 빌드·다운로드·디버깅 기본 흐름 | `lec08-base` |
| 09 — 첫 단계 | LED 500ms 간격 토글 | `lec09-blink-500ms` |
| 09 — 다음 단계 | LED 250ms 간격 토글 | `lec09-blink-250ms` |
| 10 | 빌드 과정과 산출물 분석 | `lec10-build-pipeline` |
| 11 | 메모리 섹션 관찰 | `lec11-memory-sections` |
| 12 | 부팅과 Reset Handler | `lec12-boot-sequence` |
| 13 | CMSIS와 STM32 헤더 분석 | `lec13-cmsis-headers` |
| 14 | 레지스터 접근 규칙 | `lec14-register-rules` |
| 15 | RCC와 클럭 초기화 분석 | `lec15-rcc-clock` |
| 16 | GPIO 레지스터 분석 | `lec16-gpio-registers` |
| 17 | ODR·BSRR GPIO 출력 비교 | `lec17-gpio-output` |
| 18 | 버튼 입력·EXTI·디바운스 | `lec18-gpio-input-exti` |
| 19 | HAL tick을 이용한 비차단 실행 | `lec19-systick-hal-delay` |
| 20 | 레지스터 GPIO LED 제어 | `lec20-gpio-led` |
| 21 | TIM6 카운터·Update 플래그 폴링 | `lec21-tim-basic-counter` |
| 22 | 인터럽트 구조 이해 | `lec22-interrupt-architecture` |
| 23 | TIM6 인터럽트와 콜백 | `lec23-timer-interrupt` |
| 24 | TIM2 PWM·Duty 제어 | `lec24-pwm-motor-control` |
| 25 | UART 프레임 관찰 | `lec25-uart-basics` |
| 26 | UART 폴링 에코·오류 카운터 | `lec26-uart-polling` |
| 27 | UART 인터럽트·링 버퍼·오류 복구 | `lec27-uart-interrupt-ring-buffer` |
| 28 | DMA 개념 이해 | `lec28-dma-concepts` |
| 29 | UART Circular DMA·IDLE 수신 | `lec29-uart-dma-idle` |
| 30 | 바이트 스트림 패킷 파서 | `lec30-packet-parser` |
| 31 | 처리량을 제한한 UART 슈퍼루프 | `lec31-superloop` |
| 32 | RTOS 개념 이해 | `lec32-rtos-concepts` |
| 33 | Cortex-M RTOS 구조·ThreadX 포트 | `lec33-cortex-m-rtos-architecture` |
| 34 | ThreadX LED·UART 스레드 | `lec34-threadx-multitasking` |

22강은 21강, 28강은 27강, 32강은 31강과 같은 커밋을 가리킵니다. 개념 강의에서 불필요한 코드 변경을 추가하지 않았으므로 이 Tag 쌍의 Diff가 비어 있는 것은 정상입니다.

## 이전 강의와 코드 비교

다음 명령은 작업 폴더를 전환하지 않고 16강에서 17강으로 바뀐 코드를 보여 줍니다.

```powershell
git diff --stat lec16-gpio-registers lec17-gpio-output -- stm32f446_course
git diff lec16-gpio-registers lec17-gpio-output -- stm32f446_course
```

긴 Diff 화면을 종료하려면 **Q**를 누릅니다. Git Graph에서도 커밋과 변경 파일을 확인할 수 있습니다. 그래프에서 커밋을 클릭하는 것만으로 작업 폴더의 코드가 전환되지는 않습니다.

확인을 마치고 `main`으로 돌아갈 때도 수정 내용을 먼저 보존합니다.

```powershell
git status
git switch main
```

## 저장소 구성

| 경로 | 내용 |
| --- | --- |
| `stm32f446_course/` | CubeIDE 프로젝트, `.ioc`, 전체 소스, 시작 코드, 링커 스크립트 |
| `stm32f446_course/Core/` | 강의 C 코드와 MCU 초기화·인터럽트 코드 |
| `stm32f446_course/Drivers/` | HAL·CMSIS 라이브러리 |
| `stm32f446_course/Middlewares/` | 33강부터 사용하는 ThreadX 소스 |
| `student-guides/` | 수강생에게 공유하는 설치·사용·설정 문서 |
| `student-guides/hw_spec/` | 보드·MCU·주변장치·Cortex-M4 참고 PDF |

강의 코드는 하나의 `main` 이력과 강의별 annotated Tag로 관리합니다. 최신 안내 문서는 `main`에서 갱신합니다. 로컬 검증 기록, IDE Workspace, 로그와 빌드 산출물은 공유 대상에서 제외합니다. 공급 라이브러리의 라이선스와 저작권 표시는 각 파일·폴더의 내용을 따릅니다.

## 검증 범위

기준 개발 환경과 NUCLEO-F446RE에서 08~34강의 28개 Tag 상태 — 09강 두 단계 포함 — 에 대해 전체 빌드와 다운로드 검증을 수행했습니다. UART 오류 복구와 ThreadX HAL tick 관련 수정도 보드에서 재확인했습니다.

이 결과가 모든 PC·보드와 모든 동작 조건을 보증하는 것은 아닙니다. PWM의 실제 파형 계측, 장시간 실행과 극한 부하 검증은 완료 범위에 포함하지 않습니다. 각 강의의 관찰 항목은 자신의 보드에서도 확인해 주세요.
