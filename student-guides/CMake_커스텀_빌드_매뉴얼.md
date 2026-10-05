# 39·40강 CMake·커스텀 빌드 실습 가이드

39강은 **CubeIDE에서 만든 ELF로 VS Code 디버그 연결**, 40강은 **별도 LED 프로젝트를 CubeMX에서 CMake 형식으로 생성하고 CLI 빌드**하는 선택 심화 과정입니다. 필수 01~38강의 CubeIDE 환경을 교체할 필요는 없습니다.

> 문서 상태 — 2026-10-05: 교안·녹음대본, 로컬 저장소와 공식 도구 문서를 대조한 실습 안내입니다. 이 문서 작성 과정에서는 프로그램 설치, CMake 코드 생성·전체 빌드, 보드 기록·디버깅을 실행하지 않았습니다. 아래 설정과 명령은 수강생이 적용하고 확인할 절차이며, 검증 완료 로그가 아닙니다.

## 1. 시작 위치와 준비물

기존 설치 안내부터 준비합니다.

- [STM32CubeMX 설치](STM32CubeMX_설치.md)
- [STM32CubeIDE 설치](STM32CubeIDE%20설치.md)
- [VS Code·Git·Git Graph 설치와 Tag 사용](VSCode_GIT_Graph_설치.md)
- [강의별 CubeMX 설정](STM32CubeMX_강의태그별_설정방법.md)

| 항목 | 이번 실습의 기준 |
| --- | --- |
| 보드 | NUCLEO-F446RE / STM32F446RET6, ST-LINK 쪽 데이터 USB 연결 |
| 시작 코드 | `lec09-blink-500ms`의 최소 LED 프로젝트 |
| 핀·시간 | PA5 출력, 500ms마다 반전, 전체 점멸 주기 약 1초 |
| 클럭 | HSI 16MHz → PLL → HCLK 84MHz, APB1 42MHz, APB2 84MHz |
| 기존 강의 환경 | CubeMX 6.17.0, CubeIDE 2.1.1, STM32CubeF4 V1.28.3 |
| CLI 추가 도구 | Arm GNU Toolchain, OpenOCD, CMake, Ninja |
| 추가 확장 | C/C++, Cortex-Debug, CMake Tools |

기존 강의 환경은 저장소 README의 기준입니다. **CLI 도구·확장의 검증된 버전 조합은 아직 확정하지 않았습니다.** 설치한 정확한 버전과 실행 파일 경로를 기록하고, 이 문서 마지막의 단계별 완료 기준으로 확인합니다. 생성된 CMake 파일의 최소 요구 버전도 확인합니다.

현재 확인한 로컬 저장소는 07~34강 Tag를 제공하며, `main`은 ThreadX 예제입니다. 39·40강 완료 Tag나 CMake 프로젝트는 없습니다. 로컬 및 로컬에 기록된 원격 참조에 `Cmake` 브랜치도 없습니다. 따라서 없는 브랜치에서 설정 파일을 복사하는 방식으로 진행하지 않습니다. 원격 서버의 최신 상태를 별도로 조회한 것은 아닙니다.

### 사용할 폴더

아래는 수강생 PC의 **예시 배치**입니다. 기존 참고 저장소 위치만 자신의 경로로 바꿉니다.

```text
C:/STM32Course/
  stm32f446-course-examples/             참고 저장소: 변경하지 않음
  lesson39/stm32f446_course/             CubeIDE로 ELF를 만들 사본
  lesson40/stm32f446_course/             CMake로 생성할 별도 사본
```

39강 사본과 40강 사본을 분리하면 생성 도구나 빌드 캐시가 섞이지 않습니다. 한글·공백이 포함된 경로는 PowerShell에서 반드시 인용합니다. 제시한 기본 실습 경로는 도구별 인코딩 차이를 줄이기 위해 영문 경로를 사용합니다. 한글 경로에서도 모든 도구 조합이 검증되었다는 뜻은 아닙니다.

## 2. 추가 도구 설치와 실행 경로 확인

### 2.1 VS Code 확장

VS Code의 Extensions에서 이름뿐 아니라 게시자와 ID도 확인해 설치합니다.

| 확장 | ID | 역할 |
| --- | --- | --- |
| C/C++ / Microsoft | `ms-vscode.cpptools` | C 코드 탐색·편집 |
| Cortex-Debug / marus25 | `marus25.cortex-debug` | GDB와 OpenOCD를 이용한 디버깅 |
| CMake Tools / Microsoft | `ms-vscode.cmake-tools` | CMake 구성·빌드를 VS Code와 연결 |

CMake Tools는 40강에 사용합니다. Serial Monitor는 이번 최소 LED 실습의 필수 확장이 아닙니다. 기존 ST 확장이 있어도 이 문서에서는 **Cortex-Debug + OpenOCD** 경로를 사용합니다. 다른 확장이 생성한 실행 설정과 섞지 않습니다.

Cortex-Debug 자체에 모든 외부 실행 도구가 포함되는 것은 아닙니다. [Cortex-Debug 설치 안내](https://github.com/Marus/cortex-debug#installation)를 함께 확인합니다.

### 2.2 Windows용 외부 도구

1. [Arm GNU Toolchain](https://developer.arm.com/downloads/-/arm-gnu-toolchain-downloads)에서 **Windows 호스트용 `arm-none-eabi` bare-metal 도구**를 선택합니다. Linux용 `aarch64` 도구와 구분합니다. 설치 또는 압축 해제 후 `bin` 아래 GCC, GDB, objdump, nm, size가 있는지 확인합니다.
2. OpenOCD는 [xPack OpenOCD Windows 배포 안내](https://xpack-dev-tools.github.io/openocd-xpack/docs/install/)와 [배포 파일](https://github.com/xpack-dev-tools/openocd-xpack/releases)을 이용할 수 있습니다. 자신의 Windows 아키텍처에 맞는 압축 파일을 **전체 해제**합니다. `openocd.exe` 하나만 옮기면 DLL이나 scripts가 누락될 수 있습니다. xPack은 OpenOCD의 별도 바이너리 배포판입니다.
3. [CMake 공식 다운로드](https://cmake.org/download/)에서 Windows용 설치 프로그램 또는 압축본을 준비합니다. `CMakeLists.txt`의 `cmake_minimum_required` 이상인 버전을 사용합니다.
4. [Ninja 공식 Releases](https://github.com/ninja-build/ninja/releases)에서 Windows용 압축본을 해제합니다. `ninja.exe`가 들어 있는 폴더를 기록합니다.
5. ST-LINK USB 드라이버는 기존 CubeIDE 설치 안내를 따릅니다. Windows 장치 관리자에서도 인식 상태를 확인합니다.

**STM32CubeCLT가 이미 있다면:** [ST의 CubeCLT 안내](https://www.st.com/en/development-tools/stm32cubeclt.html)를 기준으로 설치본에 포함된 도구를 재사용할 수 있습니다. 다만 `STM32CubeCLT_1.21.0` 같은 특정 폴더나 CMake·Ninja·OpenOCD의 포함 여부를 가정하지 않습니다. 아래에서 실행 파일 존재를 확인합니다. `STM32_Programmer_CLI`와 OpenOCD는 서로 다른 도구이며 명령 옵션을 혼용하지 않습니다.

### 2.3 현재 터미널에서 사용할 도구 선택

실행 위치: 임의 폴더의 **PowerShell**. 경로 입력에는 따옴표를 붙이지 않고 실제 폴더 경로만 입력합니다. 공백이 있어도 입력값 전체가 하나의 문자열이 됩니다.

```powershell
$armBin = Read-Host 'arm-none-eabi-gcc.exe가 있는 bin 폴더'
$openocdBin = Read-Host 'openocd.exe가 있는 bin 폴더'
$cmakeBin = Read-Host 'cmake.exe가 있는 bin 폴더'
$ninjaBin = Read-Host 'ninja.exe가 있는 폴더'
$openocdScripts = Read-Host 'OpenOCD의 interface와 target 폴더를 포함한 scripts 폴더'

$requiredFiles = @(
    (Join-Path $armBin 'arm-none-eabi-gcc.exe'),
    (Join-Path $armBin 'arm-none-eabi-gdb.exe'),
    (Join-Path $armBin 'arm-none-eabi-objdump.exe'),
    (Join-Path $armBin 'arm-none-eabi-nm.exe'),
    (Join-Path $armBin 'arm-none-eabi-size.exe'),
    (Join-Path $openocdBin 'openocd.exe'),
    (Join-Path $cmakeBin 'cmake.exe'),
    (Join-Path $ninjaBin 'ninja.exe'),
    (Join-Path $openocdScripts 'interface/stlink.cfg'),
    (Join-Path $openocdScripts 'target/stm32f4x.cfg')
)
foreach ($file in $requiredFiles) {
    if (-not (Test-Path -LiteralPath $file -PathType Leaf)) {
        throw "파일을 찾을 수 없음: $file"
    }
}
$env:Path = (@($armBin, $openocdBin, $cmakeBin, $ninjaBin) -join ';') + ';' + $env:Path

$tools = @('arm-none-eabi-gcc', 'arm-none-eabi-gdb', 'openocd', 'cmake', 'ninja')
foreach ($tool in $tools) {
    Get-Command $tool -All | Select-Object Name, Source
    & $tool --version
    if ($LASTEXITCODE -ne 0) { throw "$tool 버전 확인 실패" }
}
```

이 PATH 변경은 현재 PowerShell과 여기서 실행한 자식 프로세스에만 적용됩니다. 새 터미널에는 다시 적용하거나, 확인한 폴더를 Windows **사용자 Path**에 등록한 뒤 터미널과 VS Code를 다시 실행합니다. 기존 VS Code 창이 열려 있으면 이전 환경을 사용할 수 있습니다.

OpenOCD scripts의 실제 위치는 배포판마다 다릅니다. 예를 들어 `share/openocd/scripts` 아래에 있을 수 있으며, 위 검사가 통과한 폴더를 사용합니다. 버전 문자열뿐 아니라 `Get-Command`가 표시한 실제 실행 파일도 기록합니다.

**완료 기준:** 모든 도구가 실행되고, 선택한 GCC/GDB가 같은 도구 체인에 속하며, 두 OpenOCD 설정 파일을 찾을 수 있어야 합니다. 아직 보드 연결을 검증한 것은 아닙니다.

## 3. 원본을 보존한 09강 사본 두 개 준비

실행 위치: **기존 코드 저장소 안**. 다음 블록은 Tag를 전환하지 않고 해당 Tag의 프로젝트를 ZIP으로 내보내 두 실습 사본을 만듭니다. Git 명령은 `$repoPath`에서만 실행합니다.

`$repoPath`는 실제 참고 저장소 위치로 변경합니다. 목적지에 기존 작업이 있으면 중단하며 덮어쓰지 않습니다.

```powershell
$repoPath = 'C:\STM32Course\stm32f446-course-examples'
$courseRoot = 'C:\STM32Course'
$lesson39Root = Join-Path $courseRoot 'lesson39'
$lesson40Root = Join-Path $courseRoot 'lesson40'
$archivePath = Join-Path $courseRoot 'lec09-blink-500ms-source.zip'

Set-Location -LiteralPath $repoPath -ErrorAction Stop
if (-not $?) { throw '코드 저장소 위치를 확인하세요.' }
git status --short
if ($LASTEXITCODE -ne 0) { throw 'Git 저장소 확인 실패' }
git rev-parse --verify 'lec09-blink-500ms^{commit}'
if ($LASTEXITCODE -ne 0) { throw '시작 Tag가 없습니다. Tag 목록을 확인하세요.' }
foreach ($destination in @($lesson39Root, $lesson40Root, $archivePath)) {
    if (Test-Path -LiteralPath $destination) {
        throw "기존 작업을 보존해야 합니다. 새 목적지를 선택하세요: $destination"
    }
}
git archive --format=zip "--output=$archivePath" lec09-blink-500ms stm32f446_course
if ($LASTEXITCODE -ne 0) { throw 'Tag 내보내기 실패' }
Expand-Archive -LiteralPath $archivePath -DestinationPath $lesson39Root -ErrorAction Stop
Expand-Archive -LiteralPath $archivePath -DestinationPath $lesson40Root -ErrorAction Stop
```

Tag에 커밋된 코드만 내보내므로 현재 작업 폴더의 미커밋 수정이나 기존 Debug 산출물이 섞이지 않습니다. ZIP에는 `.git`이 없으며, 실습 사본에서 Git 명령을 실행할 필요가 없습니다. 최신 가이드는 원래 저장소의 `main` 문서를 계속 참고합니다.

사본마다 `stm32f446_course` 폴더가 **한 번만** 생겼는지 확인합니다. 내부에 `.ioc`, `Core`, `Drivers`, `.project`, `.cproject`, 시작 코드와 링커 스크립트가 있어야 합니다.

## 4. 39강 — 기존 ELF로 디버그 연결

### 4.1 CubeIDE에서 LED ELF 준비

1. 기존 프로젝트와 경로가 섞이지 않도록 39강용 별도 CubeIDE Workspace를 엽니다.
2. **File → Import → General → Existing Projects into Workspace**에서 `C:/STM32Course/lesson39/stm32f446_course`를 선택합니다.
3. **Copy projects into workspace**는 해제합니다.
4. Debug 구성을 선택하고 Clean 후 전체 Build를 실행합니다.
5. 오류 없이 링크까지 끝났는지 확인합니다. 기대 경로는 `Debug/stm32f446_course.elf`입니다. 실제 빌드 콘솔과 대조합니다.
6. 이 ELF를 준비하기 위해 CubeMX로 코드를 다시 생성할 필요는 없습니다.

PowerShell에서 경로를 확인합니다.

```powershell
Set-Location -LiteralPath 'C:\STM32Course\lesson39\stm32f446_course' -ErrorAction Stop
$firmwareElf = (Resolve-Path -LiteralPath '.\Debug\stm32f446_course.elf' -ErrorAction Stop).Path
Get-Item -LiteralPath $firmwareElf | Select-Object FullName, Length, LastWriteTime
```

파일 존재만으로 빌드 성공을 판단하지 않습니다. 이번 Tag를 새로 빌드한 성공 로그가 선행 조건입니다.

### 4.2 OpenOCD 서버 실행

CubeIDE의 **디버그 세션을 종료**합니다. IDE 자체를 무조건 종료하거나 다른 Java 프로세스를 강제 종료할 필요는 없습니다. 대상 보드는 하나만 연결하고, NUCLEO-F446RE와 외부 구동 장치가 없는 조건을 확인합니다.

실행 위치: 39강 프로젝트 폴더의 **PowerShell A**. 2.3절의 도구 경로 설정과 `$openocdScripts` 값이 이 터미널에 있어야 합니다.

```powershell
openocd -s "$openocdScripts" -f interface/stlink.cfg -f target/stm32f4x.cfg
```

서버는 계속 실행되므로 프롬프트가 돌아오지 않는 것이 정상일 수 있습니다. 출력에서 선택한 ST-LINK, STM32F4 타깃 접근, GDB 포트를 확인합니다. 서버가 포트를 열었다는 사실만으로 MCU 접근까지 성공했다고 판단하지 않습니다. 이 구성의 기본 GDB 포트는 3333이며 실제 로그가 다르면 그 값을 사용합니다.

연결 실패 시 이 단계에서 원인을 해결하고 다음 단계로 넘어갑니다. 참고: [OpenOCD의 GDB 연결 안내](https://openocd.org/doc-release/html/GDB-and-OpenOCD.html).

### 4.3 GDB에서 ELF와 보드 연결

실행 위치: 39강 프로젝트 폴더의 **새 PowerShell B**. 새 창에서도 GDB의 PATH를 확인합니다.

```powershell
Set-Location -LiteralPath 'C:\STM32Course\lesson39\stm32f446_course' -ErrorAction Stop
arm-none-eabi-gdb '.\Debug\stm32f446_course.elf'
```

이후 다음은 PowerShell이 아니라 **GDB 프롬프트**에 한 줄씩 입력합니다. 오류가 있으면 다음 명령을 실행하지 않고 원인을 확인합니다.

```text
target extended-remote localhost:3333
monitor reset halt
load
break main
continue
```

`load`는 ELF 내용을 대상 Flash에 기록하여 기존 프로그램을 바꿉니다. Option Bytes 변경·보호 해제·전체 칩 삭제 명령은 이 실습에 포함하지 않습니다.

**완료 기준:** 올바른 소스의 `main`에 멈추고, 소스·Disassembly·실행 주소가 같은 ELF와 대응해야 합니다. 계속 실행한 뒤 LED 결과는 별도로 확인합니다. 다음 절로 넘어갈 때 GDB를 종료하고 PowerShell A의 서버도 Ctrl+C로 종료합니다.

### 4.4 VS Code에서 같은 연결 구성

VS Code의 **Open Folder**로 `C:/STM32Course/lesson39/stm32f446_course`를 엽니다. 저장소 상위 폴더를 열면 아래 `${workspaceFolder}`의 의미가 달라집니다.

프로젝트 아래 `.vscode/launch.json`에 다음 **설정 예**를 넣습니다. 이미 파일이 있다면 백업 후 `configurations`에 항목을 추가합니다. `C:/REPLACE/...` 세 경로는 2.3절에서 확인한 실제 경로로 반드시 바꿉니다. JSON에서는 `/`를 사용하면 역슬래시 이스케이프를 피할 수 있습니다.

```json
{
  "version": "0.2.0",
  "configurations": [
    {
      "name": "Lesson39 STM32F446 OpenOCD",
      "type": "cortex-debug",
      "request": "launch",
      "servertype": "openocd",
      "cwd": "${workspaceFolder}",
      "executable": "${workspaceFolder}/Debug/stm32f446_course.elf",
      "armToolchainPath": "C:/REPLACE/arm-toolchain/bin",
      "serverpath": "C:/REPLACE/openocd/bin/openocd.exe",
      "searchDir": ["C:/REPLACE/openocd/share/openocd/scripts"],
      "configFiles": ["interface/stlink.cfg", "target/stm32f4x.cfg"],
      "runToEntryPoint": "main"
    }
  ]
}
```

필드의 의미는 [Cortex-Debug 설정 명세](https://github.com/Marus/cortex-debug/blob/master/debug_attributes.md)를 기준으로 합니다. 이 설정은 Cortex-Debug가 서버를 시작하는 방식이므로 수동 OpenOCD 서버와 동시에 실행하지 않습니다. `launch` 과정에서도 대상 프로그램 기록과 Reset이 일어날 수 있습니다.

Run and Debug에서 위 이름을 선택하고 시작합니다. `main` 중단과 소스 대응을 확인한 뒤 Continue합니다. 이 단계에는 자동 빌드용 `preLaunchTask`를 넣지 않았으므로 **코드를 바꾸면 CubeIDE에서 다시 빌드한 후 실행**합니다. F5만 눌러도 새 코드가 빌드된다고 가정하지 않습니다.

**39강 완료 기준:** CLI GDB 연결과 VS Code 연결을 각각 확인하고, 사용한 ELF·도구 버전·경로를 기록합니다. 이 문서의 launch.json은 공식 필드와 대조한 예시이며 실제 환경에서 검증된 완성 설정으로 배포한 것은 아닙니다.

## 5. 40강 — 별도 사본을 CMake 프로젝트로 생성

### 5.1 CubeMX에서 40강 사본만 열기

1. `C:/STM32Course/lesson40/stm32f446_course/stm32f446_course.ioc`를 엽니다. 창의 파일 위치가 39강 사본이나 참고 저장소가 아닌지 확인합니다.
2. 패키지와 설정은 시작 Tag에 맞춥니다. 자동 마이그레이션을 했다면 사용한 버전과 변경점을 기록합니다.
3. **Project Manager → Project**의 Toolchain / IDE에서 **CMake**를 선택합니다. GCC/LLVM 선택이 별도로 있으면 이번 실습은 GCC를 선택합니다.
4. 프로젝트 이름은 `stm32f446_course`로 유지합니다. 생성 위치와 Under Root 관련 설정을 확인하여 기존 40강 사본 루트에 생성되게 합니다. `stm32f446_course/stm32f446_course`가 되지 않도록 최종 폴더를 확인합니다.
5. **Code Generator**의 사용자 코드 보존 옵션을 확인한 뒤 Generate Code를 실행합니다.
6. 아래 파일 목록과 LED 사용자 코드가 유지되었는지 확인합니다.

CMake 선택지가 없으면 도구 버전·해당 대상 지원을 먼저 확인합니다. `.ioc` 문자열만 변경하거나 임의 CMake 파일을 복사해 “CubeMX 생성 완료”로 간주하지 않습니다. [ST CMake 구성 안내](https://dev.st.com/stm32cube-docs/stm32cubeide-vscode/1.0.1/en/docs/markup/basic_concepts/cmake.html)를 참고하며, 메뉴와 생성 경로는 설치 버전에 따라 다를 수 있습니다. 이번 문서 작성에서 CubeMX 6.17.0의 이 생성 경로를 실행 검증한 것은 아닙니다.

### 5.2 생성 결과 확인

| 파일·설정 | 확인할 내용 |
| --- | --- |
| 최상위 `CMakeLists.txt` | 프로젝트·실행 대상 이름, 생성 하위 구성 연결, 사용자 확장 위치 |
| `cmake/gcc-arm-none-eabi.cmake` | GCC 크로스 컴파일러 지정; 실제 경로와 내용 확인 |
| `cmake/stm32cubemx/CMakeLists.txt` | 생성된 소스·HAL·CMSIS·include 연결; 생성기 관리 영역 |
| `CMakePresets.json` | 실제 있을 때만 사용; configure와 build preset 이름을 각각 확인 |
| 시작 코드 | STM32F446용 벡터·Reset Handler; 경로는 생성본에서 확인 |
| `STM32F446RETX_FLASH.ld` | Flash/RAM 주소·크기·섹션 배치 |
| `Core/Src/main.c` | 초기화와 09강 USER CODE의 LED 반전·500ms 대기 유지 |

이름은 생성 결과와 대조할 기준이며 아직 저장소에서 제공하는 CMake 파일 목록이 아닙니다. 생성기가 관리하는 하위 CMake 파일과 사용자가 확장할 최상위 설정을 구분합니다. C 코드의 `USER CODE` 규칙이 모든 CMake 파일을 자동으로 보호해 주는 것은 아닙니다.

비교할 코드의 핵심은 다음 두 줄입니다. 이미 반복문에 있으면 **추가하지 않습니다.**

```c
HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);
HAL_Delay(500);
```

PA5 초기 Low·Push-Pull·No Pull, 기존 84MHz 클럭과 SysTick 기반 HAL 시간 기준을 유지합니다. 이번 사본에 ThreadX·TIM7·DMA를 추가하지 않습니다. 현재 main의 ThreadX 전체 프로젝트를 자동 이식하는 실습이 아닙니다.

컴파일·링크에는 Cortex-M4/Thumb, STM32F446xx, USE_HAL_DRIVER, HAL/CMSIS 경로가 필요합니다. FPU를 사용한다면 `fpv4-sp-d16`과 float ABI가 링크되는 코드·라이브러리에 걸쳐 일치해야 합니다. 최적화와 디버그 옵션도 실제 빌드 명령에서 확인합니다. 생성기가 제공한 옵션을 근거 없이 중복 추가하지 않습니다.

### 5.3 Configure → Build

실행 위치: `C:/STM32Course/lesson40/stm32f446_course`의 PowerShell. 2.3절에서 확인한 도구가 PATH에 있어야 합니다. 아래는 Preset에 의존하지 않는 첫 구성 예입니다.

```powershell
Set-Location -LiteralPath 'C:\STM32Course\lesson40\stm32f446_course' -ErrorAction Stop
$ErrorActionPreference = 'Stop'
$toolchainFile = (Resolve-Path -LiteralPath '.\cmake\gcc-arm-none-eabi.cmake').Path
$buildDir = Join-Path (Get-Location).Path 'build\cli'
if (Test-Path -LiteralPath $buildDir) {
    throw '첫 구성에는 새 빌드 폴더를 사용하세요. 기존 결과를 지우지 말고 cli2 등으로 바꾸세요.'
}
$configureArgs = @(
    '-S', '.', '-B', $buildDir, '-G', 'Ninja',
    "-DCMAKE_TOOLCHAIN_FILE=$toolchainFile",
    '-DCMAKE_BUILD_TYPE=Debug',
    '-DCMAKE_EXPORT_COMPILE_COMMANDS=ON'
)
cmake @configureArgs
if ($LASTEXITCODE -ne 0) { throw 'CMake Configure 실패' }
cmake --build "$buildDir" --verbose
if ($LASTEXITCODE -ne 0) { throw '전체 Build 실패. Flash 단계로 진행하지 마세요.' }
```

인수 배열은 긴 명령을 나누면서 공백 경로를 하나의 인수로 전달하기 위한 것입니다. Toolchain 파일이 다른 위치에 생성되면 실제 파일을 확인해 바꿉니다. CMake의 교차 컴파일 구성은 [공식 Toolchain 문서](https://cmake.org/cmake/help/latest/manual/cmake-toolchains.7.html)를 참고합니다.

**Configure 완료:** 선택한 컴파일러가 `arm-none-eabi-gcc`이며 Ninja 빌드 규칙이 생성됨.

**Build 완료:** 전체 컴파일과 링크가 성공하고 해당 빌드 폴더에 ELF가 생성됨. 첫 빌드를 빈 폴더에서 시작하므로 이전 ELF와 혼동하지 않습니다. 이후 같은 도구·설정의 반복 빌드는 `cmake --build "$buildDir"`로 진행하되 매번 종료 코드를 확인합니다. 소스 변경이 없으면 증분 빌드에서 ELF 시각이 그대로일 수도 있습니다.

컴파일러나 Generator를 바꾸면 이전 캐시를 재사용하지 말고 새 빌드 폴더에서 Configure부터 수행합니다. Preset을 쓰려면 `cmake --list-presets`와 `cmake --build --list-presets`에서 실제 이름을 확인합니다. Configure preset이 있다고 같은 이름의 Build preset이 반드시 있는 것은 아닙니다.

### 5.4 ELF·Map·메모리 배치 확인

실행 위치와 변수: 앞 절과 같은 PowerShell 세션, 성공한 `$buildDir` 사용.

```powershell
$elfFiles = @(Get-ChildItem -LiteralPath $buildDir -Recurse -File -Filter '*.elf')
$elfFiles | Select-Object FullName, Length, LastWriteTime
if ($elfFiles.Count -ne 1) {
    throw 'ELF가 없거나 여러 개입니다. 빌드 대상과 실제 파일을 먼저 확인하세요.'
}
$firmwareElf = $elfFiles[0].FullName
arm-none-eabi-size "$firmwareElf"
if ($LASTEXITCODE -ne 0) { throw 'ELF 크기 확인 실패' }
arm-none-eabi-objdump -f "$firmwareElf"
if ($LASTEXITCODE -ne 0) { throw 'ELF 형식 확인 실패' }
Get-ChildItem -LiteralPath $buildDir -Recurse -File -Filter '*.map'
```

`ev_nucleo_f446.elf` 같은 다른 예제 이름을 고정하지 않습니다. ELF 파일 이름은 실제 실행 대상 설정에서 결정됩니다.

Map과 링커 스크립트에서 Flash 시작 `0x08000000`, SRAM 시작 `0x20000000`, F446RE의 Flash 512KiB·SRAM 128KiB와 섹션 배치를 대조합니다. `.data`의 초기값은 Flash에도 공간을 사용하며 `.bss`와 스택·힙은 RAM 조건에 포함됩니다. `size` 숫자만으로 스택 여유까지 확인한 것은 아닙니다.

Map이 없으면 링크 명령의 Map 출력 옵션을 확인합니다. 필요한 경우 생성 결과의 실제 실행 대상에 사용자 관리 영역에서 `target_link_options`로 Map 옵션을 추가합니다. 새 실행 대상을 임의로 만들지 않습니다. 이 문서 작성 범위에서는 설정 파일을 추가하지 않았습니다.

## 6. 40강 — Flash 기록과 디버그·동작 확인

### 6.1 기록 전 확인

다음 코드는 **실행하면 연결 보드의 Flash를 변경**합니다. 성공한 CMake 빌드의 `$firmwareElf`를 사용하고, 대상 NUCLEO-F446RE 한 대만 연결합니다. CubeIDE·Cortex-Debug·수동 OpenOCD 등 기존 프로브 세션을 종료합니다. Option Bytes·보호 변경·전체 칩 삭제는 수행하지 않습니다.

실행 위치: 40강 프로젝트 폴더. 5.3~5.4절의 성공 확인 후 같은 PowerShell 세션에서 실행합니다. 새 터미널을 열었다면 파일 존재만 보고 진행하지 말고 선택한 빌드와 경로를 다시 확인합니다.

```powershell
if (-not (Test-Path -LiteralPath $firmwareElf -PathType Leaf)) {
    throw '선택한 ELF 파일이 없습니다.'
}
# OpenOCD의 Tcl 명령에서도 공백 경로가 하나의 인수가 되게 처리합니다.
$elfForOpenocd = (Resolve-Path -LiteralPath $firmwareElf).Path.Replace('\', '/')
if ($elfForOpenocd -match '[{}\r\n]') {
    throw '이 예제는 중괄호와 줄바꿈이 있는 경로를 지원하지 않습니다.'
}
$programCommand = 'program {' + $elfForOpenocd + '} verify reset exit'
openocd -s "$openocdScripts" -f interface/stlink.cfg -f target/stm32f4x.cfg -c $programCommand
if ($LASTEXITCODE -ne 0) { throw 'Flash 기록 또는 검증 실패' }
```

PowerShell 인용뿐 아니라 OpenOCD 내부 Tcl 인용도 고려한 예입니다. 영문·공백 경로를 처리하도록 작성했지만 한글 경로의 실제 도구 조합은 별도 확인해야 합니다. 명령의 역할은 [OpenOCD program 안내](https://openocd.org/doc-release/html/Flash-Programming.html)를 따릅니다. `verify`는 기록 내용을 확인하고 `reset`은 실행을 재개하며 `exit`은 서버를 종료합니다.

### 6.2 디버그 연결

디버깅할 때는 `program … exit` 대신 4.2절처럼 서버를 유지하고 GDB를 연결합니다. VS Code를 사용한다면 **40강 프로젝트 폴더**를 열고 4.4절의 설정을 복사하여 `executable`을 실제 CMake ELF 경로로 바꿉니다. 예를 들어 실제 파일이 해당 위치에 생겼을 때만 `${workspaceFolder}/build/cli/stm32f446_course.elf`를 사용합니다.

이때도 F5의 자동 빌드는 구성하지 않았습니다. 먼저 CMake Build 성공을 확인한 뒤 디버깅합니다. 디버그 연결·`main` 중단·소스 대응을 확인하고 Continue합니다.

### 6.3 보드 동작

중단점 없이 계속 실행한 상태에서 LD2가 약 0.5초 켜짐·0.5초 꺼짐으로 반복되는지 확인합니다. 전체 주기는 약 1초입니다. 육안 확인은 정밀 주기 계측과 구분합니다. 파형 비교가 필요하면 36강의 측정 조건을 사용합니다.

CubeIDE와 CMake 결과를 비교할 때는 소스·핀·클럭·HAL 패키지·최적화 조건을 맞춥니다. 다른 컴파일러 버전의 결과가 바이트 단위로 같아야 한다고 가정하지 않습니다. Flash Verify 성공과 LED 정상 동작은 별개의 결과입니다.

## 7. 오류를 단계별로 찾기

| 증상 | 먼저 확인할 것 |
| --- | --- |
| 명령을 찾지 못함 | `Get-Command 이름 -All`, 실제 bin 위치, 새 터미널의 PATH |
| 다른 컴파일러가 선택됨 | Toolchain 파일과 Configure 로그; 새 빌드 폴더에서 재구성 |
| CMake 최소 버전 오류 | 생성된 `cmake_minimum_required`와 실제 CMake 버전 |
| Ninja를 못 찾음 | `ninja --version`, 선택한 Generator와 실행 경로 |
| OpenOCD의 cfg 파일 없음 | `-s` 경로와 interface/target 파일; exe만 옮겼는지 확인 |
| ST-LINK를 열 수 없음 | 데이터 케이블·USB 드라이버·다른 디버그 서버의 점유 |
| GDB 연결 거부 | 서버 실행 상태·실제 포트·타깃 접근 오류 로그 |
| 소스 위치가 맞지 않음 | 이번 빌드 ELF인지, Debug 정보·소스 경로·최적화 확인 |
| `undefined reference` | 생성 소스 목록·HAL/CMSIS·사용자 파일이 대상에 포함됐는지 |
| ABI/VFP 관련 링크 오류 | 코어·FPU·float ABI와 라이브러리 조합 일치 여부 |
| `cannot open output file` | 실제 출력 파일을 점유한 빌드/디버그 작업, 권한·보안 프로그램 확인 |
| UART COM 포트 사용 중 | 해당 COM을 연 터미널·시리얼 모니터 확인; ST-LINK SWD 점유와 구분 |
| 기록 성공인데 LED 정지 | Debug에서 멈췄는지, 실제 ELF·PA5·클럭·HAL Tick·Fault 확인 |

COM 포트를 시리얼 모니터가 사용한다는 이유만으로 ST-LINK SWD 기록이 반드시 차단되는 것은 아닙니다. COM과 디버그 인터페이스를 구분해 오류가 난 경로를 확인합니다. 파일 잠금도 IDE 실행 자체를 원인으로 단정하지 말고 해당 작업을 정상 종료합니다. 무관한 `javaw.exe`나 컴파일러 프로세스를 일괄 강제 종료하지 않습니다.

## 8. 완료 기준과 재현 기록

| 단계 | 완료 기준 | 이번 문서 작성에서의 상태 |
| --- | --- | --- |
| 도구 준비 | 버전·실행 경로·cfg 파일 확인 | 절차 작성; 새 설치·실행 검증 없음 |
| 39강 ELF 준비 | 09강 사본 CubeIDE 전체 Build 성공 | 시작 Tag·소스 확인; 재빌드 없음 |
| 39강 Debug | GDB와 VS Code 각각 main 중단·소스 대응 | 설정 예 작성; 연결 미검증 |
| 40강 코드 생성 | 사본 .ioc를 CubeMX로 열어 CMake 생성 | 절차 작성; 생성 미검증 |
| Configure | 올바른 크로스 도구로 빌드 규칙 생성 | 미실행 |
| 전체 Build | 모든 소스 컴파일·링크 및 해당 ELF·Map 확인 | 미실행 |
| Flash | 올바른 대상에 기록·Verify 성공 | 미실행 |
| 보드 동작 | 계속 실행 상태에서 LD2 점멸 확인 | 미관측 |

수강생은 각 단계를 실제로 확인한 뒤 날짜·도구 버전·경로·결과를 기록합니다. 소스 Tag와 커밋, .ioc/HAL 버전, CPU/FPU/ABI·최적화 옵션, 메모리 배치, Configure·Build 명령, 선택한 ELF도 함께 남깁니다. 개인 절대 경로와 산출물을 공통 소스 설정과 구분합니다.

**수강생 실습 안내 확정 전 남은 준비:** CLI 도구·확장 버전 조합 고정, 40강 사본의 실제 CubeMX CMake 생성 결과 확보, 깨끗한 사본에서 Configure·전체 Build 재현, launch.json의 보드 연결 검증, Flash·LD2 동작 확인입니다. 이 문서는 그 절차를 제공하며, 완성 CMake 프로젝트나 39·40강 Tag를 대신하지 않습니다.
