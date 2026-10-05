# VS Code·Git·Git Graph 설치와 강의 Tag 사용

이 문서는 Windows에서 **VS Code → Git → Git Graph**를 설치하고, 강의 참고 저장소를 내려받아 강의별 코드와 변경 내용을 확인하는 방법을 안내합니다.

| 도구 | 이번 과정에서의 역할 |
| --- | --- |
| Visual Studio Code | 참고 코드와 문서 열기 |
| Git for Windows | 저장소 다운로드, 강의 Tag 전환, 변경 내용 비교 |
| Git Graph | 커밋·브랜치·Tag를 그래프로 확인 |
| STM32CubeIDE | 펌웨어 빌드, 보드 다운로드, 디버깅 |

VS Code와 Git Graph 설치만으로 STM32 빌드 환경이 구성되는 것은 아닙니다. 펌웨어 실행에는 [STM32CubeIDE 설치 안내](STM32CubeIDE%20설치.md)를 함께 참고합니다.

## 1. VS Code 설치

1. [VS Code 공식 Windows 설치 안내](https://code.visualstudio.com/docs/setup/windows)에 접속합니다.
2. 일반적인 Intel·AMD 64비트 개인 PC에서는 **Windows x64 User Installer**를 내려받습니다.
3. 설치 프로그램을 실행하고 약관과 설치 경로를 확인합니다.
4. 추가 작업 선택 화면에서 **Add to PATH**가 선택되어 있는지 확인하고 설치합니다.
5. 설치가 끝나면 VS Code를 실행합니다.

**Visual Studio Code**와 **Visual Studio**는 서로 다른 프로그램입니다. 이번 과정에서는 Visual Studio Code를 사용합니다.

설치 후 **새 PowerShell 창**을 열고 다음 명령을 실행합니다.

```powershell
code --version
```

버전 정보가 출력되면 명령 실행까지 확인된 것입니다. 인식되지 않으면 터미널을 닫았다가 다시 열고, 설치 시 PATH 추가 옵션을 확인합니다.

## 2. Git for Windows 설치

1. [Git for Windows 공식 다운로드 페이지](https://git-scm.com/install/windows)에서 **x64 Setup**을 내려받습니다.
2. 설치 프로그램을 실행합니다.
3. PATH 설정 화면에서는 **Git from the command line and also from 3rd-party software**를 선택합니다. PowerShell과 VS Code에서 Git을 사용할 수 있게 하는 옵션입니다.
4. 별도 안내가 없는 항목은 기본값으로 진행합니다.
5. 설치가 끝나면 열려 있던 VS Code와 PowerShell을 종료하고 다시 실행합니다.

새 PowerShell 창에서 다음 명령을 실행합니다.

```powershell
git --version
```

`git version ...` 형태의 버전 정보가 출력되는지 확인합니다.

공개된 강의 저장소를 내려받고 조회하는 데에는 GitHub 계정이나 커밋 작성자 설정이 필요하지 않습니다. **Git 설치와 GitHub 로그인은 별개**이며, 직접 커밋을 작성할 때는 자신의 이름과 이메일을 설정합니다.

## 3. Git Graph 확장 설치

1. VS Code에서 **Ctrl + Shift + X**를 눌러 Extensions를 엽니다.
2. **Git Graph**를 검색합니다.
3. 확장 ID가 **`mhutchie.git-graph`**인지 확인하고 **Install**을 누릅니다.
4. **Ctrl + Shift + P**로 Command Palette를 열고 **Git Graph: View Git Graph**를 실행합니다.

같은 이름의 확장이 헷갈리면 [Git Graph 공식 Marketplace 페이지](https://marketplace.visualstudio.com/items?itemName=mhutchie.git-graph)에서 확인합니다.

아직 저장소를 열지 않았다면 커밋 그래프가 표시되지 않을 수 있습니다. 실제 그래프는 다음 단계에서 저장소를 내려받은 뒤 확인합니다.

## 4. 강의 참고 저장소 내려받기

참고 저장소는 강의별 완성 코드와 변경 내용을 확인하는 용도로 사용합니다. 직접 작성하는 실습 프로젝트와는 **별도 폴더**에 둡니다.

아래 예시는 CubeIDE 설치 안내와 같은 `C:\STM32Course` 폴더를 사용합니다. PowerShell에서 명령을 **한 줄씩 실행하고, 오류가 없으면 다음 줄로 진행**합니다.

```powershell
New-Item -ItemType Directory -Force -Path C:\STM32Course
Set-Location C:\STM32Course
git clone https://github.com/jellycoding0/stm32f446-course-examples.git
Set-Location .\stm32f446-course-examples
git status
code .
```

정상적으로 완료되면 저장소 폴더가 VS Code에서 열립니다. `git status`에는 현재 브랜치와 파일 변경 상태가 표시됩니다.

이미 이 저장소를 내려받았다면 다시 Clone하지 않고 기존 저장소 폴더를 엽니다. 이후 문서의 **모든 Git 명령은 저장소 폴더 안에서 실행**합니다.

> 이 안내는 GitHub의 `main`에서 브라우저로 열어 두면 편리합니다. 과거 강의 Tag로 전환하면 해당 시점에 없던 `student-guides` 문서가 로컬 작업 폴더에서 사라질 수 있습니다.

## 5. 강의 Tag와 그래프 확인

저장소 폴더에서 다음 명령을 실행합니다.

```powershell
git fetch origin --tags
git tag --list 'lec*' --sort=version:refname
```

`lec07-cubemx`, `lec16-gpio-registers`처럼 강의 번호와 주제가 포함된 Tag가 표시됩니다. 목록에 실제로 있는 이름을 사용합니다.

VS Code에서 **Git Graph: View Git Graph**를 실행합니다. 목록이 일부만 보이면 브랜치 필터를 **Show All**로 바꾸고 새로고침합니다.

| 표시 | 의미 |
| --- | --- |
| Commit | 파일 상태를 기록한 이력 |
| `main` | 강의 코드의 주 개발 브랜치 |
| `origin/main` | 마지막으로 가져온 원격 main의 위치 |
| `lecNN-주제` | 해당 강의의 프로젝트 상태를 가리키는 Tag |
| HEAD | 현재 작업 폴더가 기준으로 삼는 커밋 위치 |

커밋을 선택하고 변경된 파일을 누르면 차이인 **Diff**를 확인할 수 있습니다. **그래프에서 커밋을 선택하는 것만으로 작업 폴더의 코드가 바뀌지는 않습니다.** 실제 전환은 다음 단계에서 수행합니다.

## 6. 특정 강의 Tag로 전환

아래는 07강 상태를 열어 보는 예입니다. 먼저 VS Code와 CubeIDE에서 편집 중인 파일을 저장하고, CubeIDE 프로젝트를 닫거나 IDE를 종료합니다.

전환 전 위치와 변경 내용을 확인합니다.

```powershell
git status
git branch --show-current
git rev-parse --short HEAD
```

브랜치 이름과 커밋 번호를 기록해 두면 원래 위치로 돌아갈 때 도움이 됩니다. 수정 파일이나 새 파일이 있다면 **개인 브랜치에 커밋하거나 별도 폴더에 백업한 뒤** 진행합니다. 전환 오류를 피하려고 변경 내용을 강제로 삭제하지 않습니다.

변경 내용을 정리했다면 다음 명령을 실행합니다.

```powershell
git switch --detach lec07-cubemx
git status
git describe --tags --exact-match
```

이제 작업 폴더의 파일이 07강 상태로 바뀝니다. 여러 Tag가 같은 커밋을 가리키는 강의는 마지막 명령에 같은 위치의 다른 Tag가 표시될 수도 있습니다.

**Detached HEAD는 오류가 아닙니다.** 브랜치 대신 특정 커밋을 직접 확인하는 상태이며, 참고 코드를 읽고 빌드하는 데 사용할 수 있습니다. 이 상태에서는 `git branch --show-current` 출력이 비어 있습니다. [Git 공식 전환 명령 설명](https://git-scm.com/docs/git-switch)

### 참고 코드에 직접 실습 내용을 남기려면

단순 조회라면 브랜치를 만들 필요가 없습니다. 07강 코드에서 직접 수정하고 커밋을 남기려면 다음처럼 개인 실습 브랜치를 먼저 만듭니다.

```powershell
git switch -c practice/lesson07
```

이 명령은 로컬 브랜치를 만듭니다. 강의 저장소의 원격 `main`을 변경하는 작업은 아닙니다. 같은 이름의 브랜치가 이미 있다면 새 이름을 사용하거나 기존 브랜치로 전환합니다.

## 7. Tag 전환 후 CubeIDE에서 빌드

1. CubeIDE에서 참고 프로젝트를 엽니다. 처음이라면 **File → Import → General → Existing Projects into Workspace**에서 저장소 안의 `stm32f446_course` 폴더를 선택합니다.
2. 원본 저장소 파일을 그대로 사용할 수 있도록 **Copy projects into workspace**는 선택하지 않습니다.
3. 이미 가져온 프로젝트라면 다시 가져오지 않고 프로젝트를 열어 **Refresh**합니다.
4. 프로젝트 위치가 현재 Tag를 전환한 저장소 안인지 확인합니다.
5. **Debug** 빌드 구성을 선택하고 **Project → Clean** 후 **Build Project**를 수행합니다.
6. 빌드가 성공한 뒤 이번에 생성된 ELF를 사용해 보드에 다운로드합니다.

Git의 Tag 전환은 무시된 빌드 산출물까지 정리하지 않습니다. **이전 Tag에서 생성한 ELF를 그대로 다운로드하지 말고 새로 빌드**합니다. 완성 예제를 확인하는 단계에서는 CubeMX로 코드를 다시 생성할 필요가 없습니다.

## 8. 두 강의의 코드 비교

Tag를 바꾸지 않고도 두 강의의 차이를 확인할 수 있습니다. 아래 예시는 16강에서 17강으로 바뀐 내용을 보여 줍니다.

```powershell
git diff --stat lec16-gpio-registers lec17-gpio-output -- stm32f446_course
git diff lec16-gpio-registers lec17-gpio-output -- stm32f446_course
```

첫 명령은 변경 파일과 규모를, 두 번째 명령은 실제 코드 차이를 보여 줍니다. 내용이 긴 화면에서 빠져나오려면 **Q**를 누릅니다.

이 비교 명령은 작업 폴더의 파일을 수정하지 않습니다. 두 Tag가 같은 커밋을 가리키는 개념 강의는 차이가 없을 수 있습니다.

## 9. main으로 돌아가기

Tag 확인이 끝나면 편집 내용을 저장하고, 수정한 파일이 있는지 확인합니다. 필요한 변경 내용을 보존한 뒤 돌아갑니다.

```powershell
git status
git switch main
git status
```

위 예시는 처음 작업하던 브랜치가 `main`인 경우입니다. 다른 브랜치에서 시작했다면 6단계에서 기록한 브랜치로 돌아갑니다. Tag 상태에서 시작했다면 기록한 커밋으로 `git switch --detach 커밋번호`를 실행합니다.

main으로 돌아온 뒤에도 CubeIDE에서 실행할 예정이라면 7단계의 Refresh와 Clean·Build를 다시 수행합니다.

## 준비 완료 확인

- [ ] VS Code가 실행되고 `code --version`이 동작합니다.
- [ ] `git --version`이 동작합니다.
- [ ] Git Graph 확장 ID가 `mhutchie.git-graph`이며 실행할 수 있습니다.
- [ ] 강의 참고 저장소를 내려받고 Tag 목록을 확인했습니다.
- [ ] 커밋 조회와 실제 Tag 전환의 차이를 이해했습니다.
- [ ] Tag를 바꾼 뒤 CubeIDE에서 새로 빌드하는 절차를 확인했습니다.

## 자주 만나는 문제

| 증상 | 확인할 내용 |
| --- | --- |
| `code` 또는 `git` 명령을 찾을 수 없음 | VS Code와 터미널을 다시 실행하고 설치 시 PATH 옵션을 확인합니다. |
| Git Graph에 저장소가 나오지 않음 | 저장소 루트 폴더를 열었는지 확인합니다. 상위 폴더나 개별 파일만 열었다면 Clone한 폴더를 다시 엽니다. |
| `not a git repository` 오류 | 터미널이 `stm32f446-course-examples` 폴더 안에 있는지 확인합니다. |
| Clone할 폴더가 이미 존재함 | 기존 Clone인지 확인하고 해당 폴더를 사용합니다. 기존 작업을 삭제하지 않습니다. |
| Tag 전환이 거부됨 | `git status`에서 수정·추가 파일을 확인하고 먼저 보존합니다. |
| Tag 목록에 필요한 강의가 없음 | `git fetch origin --tags` 결과와 강의에서 안내한 Tag 이름을 확인합니다. |
