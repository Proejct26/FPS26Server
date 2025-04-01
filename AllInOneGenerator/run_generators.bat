@echo off
setlocal enabledelayedexpansion

REM === 경로 설정 ===
set ROOT_DIR=%~dp0
set PROTO_GEN_DIR=%ROOT_DIR%ProtoGenerator
set PYTHON_SCRIPTS_DIR=%ROOT_DIR%PythonScripts
set CPP_DIR=%PYTHON_SCRIPTS_DIR%\CPP
set CSHARP_DIR=%PYTHON_SCRIPTS_DIR%\CSharp
set GENERATED_DIR=%ROOT_DIR%GeneratedFiles

set PROTOC=%PROTO_GEN_DIR%\protoc.exe
set PROTO_FILE=%PROTO_GEN_DIR%\Protocol.proto

set DEST_CPP=%GENERATED_DIR%\CPP
set DEST_CS=%GENERATED_DIR%\CSharp

REM === [1] proto_definitions.xlsx 복사 ===
echo [1] 복사 중: proto_definitions.xlsx -> ProtoGenerator
copy /Y "%ROOT_DIR%proto_definitions.xlsx" "%PROTO_GEN_DIR%"

REM === [2] generate_proto.exe 실행 ===
echo [2] 실행 중: generate_proto.exe
pushd "%PROTO_GEN_DIR%"
"%PROTO_GEN_DIR%\generate_proto.exe"
popd

REM === [3] proto_definitions.xlsx 삭제 ===
echo [3] 삭제 중: proto_definitions.xlsx
del /F /Q "%PROTO_GEN_DIR%\proto_definitions.xlsx"

REM === [4] protoc.exe 실행 → C++ / C# 코드 생성 ===

REM 대상 폴더 정리: CPP
if exist "%DEST_CPP%" (
    if not exist "%DEST_CPP%\nul" (
        echo 잘못 생성된 GeneratedFiles\CPP 파일 삭제 중...
        del /F /Q "%DEST_CPP%"
    )
)
if not exist "%DEST_CPP%\" (
    mkdir "%DEST_CPP%"
)

REM 대상 폴더 정리: CSharp
if exist "%DEST_CS%" (
    if not exist "%DEST_CS%\nul" (
        echo 잘못 생성된 GeneratedFiles\CSharp 파일 삭제 중...
        del /F /Q "%DEST_CS%"
    )
)
if not exist "%DEST_CS%\" (
    mkdir "%DEST_CS%"
)

echo [4] protoc.exe를 사용하여 C++ 코드 생성
"%PROTOC%" --proto_path="%PROTO_GEN_DIR%" --cpp_out="%DEST_CPP%" "%PROTO_FILE%"
if errorlevel 1 (
    echo [오류] C++ 코드 생성 실패
    pause
    exit /b
)

echo [5] protoc.exe를 사용하여 C# 코드 생성
"%PROTOC%" --proto_path="%PROTO_GEN_DIR%" --csharp_out="%DEST_CS%" "%PROTO_FILE%"
if errorlevel 1 (
    echo [오류] C# 코드 생성 실패
    pause
    exit /b
)

REM === [6] Protocol.proto를 CPP / CSharp 디렉토리로 복사 (Python 스크립트용) ===
echo [6] 복사 중: Protocol.proto -> CPP, CSharp
copy /Y "%PROTO_FILE%" "%CPP_DIR%"
copy /Y "%PROTO_FILE%" "%CSHARP_DIR%"

REM === [7] CPP Python 스크립트 실행 ===
echo [7] 실행 중: CPP Python Scripts
pushd "%CPP_DIR%"
python generate_packet_proc.py
python generate_packets.py
python generate_proto_struct.py
popd

REM === [8] CSharp Python 스크립트 실행 ===
echo [8] 실행 중: CSharp Python Scripts
pushd "%CSHARP_DIR%"
python generate_packet_handler.py
python generate_packet_manager.py
popd

REM === [9] CPP Python 결과 파일 이동 ===
echo [9] CPP Python 결과 이동 -> GeneratedFiles\CPP
move /Y "%CPP_DIR%\*.cpp" "%DEST_CPP%\"
move /Y "%CPP_DIR%\*.h" "%DEST_CPP%\"
del /F /Q "%CPP_DIR%\*.proto" >nul 2>&1

REM === [10] CSharp Python 결과 파일 이동 ===
echo [10] CSharp Python 결과 이동 -> GeneratedFiles\CSharp
move /Y "%CSHARP_DIR%\*.cs" "%DEST_CS%\"
del /F /Q "%CSHARP_DIR%\*.proto" >nul 2>&1

REM === [11] 루트에 잘못 생성된 CPP 파일 제거 ===
if exist "%ROOT_DIR%CPP" (
    if not exist "%ROOT_DIR%CPP\" (
        del /F /Q "%ROOT_DIR%CPP"
    )
)

echo [12] 모든 작업 완료
pause
