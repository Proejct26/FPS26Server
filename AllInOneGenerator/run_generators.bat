@echo off
setlocal enabledelayedexpansion

REM 경로 설정
set ROOT_DIR=%~dp0
set PROTO_GEN_DIR=%ROOT_DIR%ProtoGenerator
set PYTHON_SCRIPTS_DIR=%ROOT_DIR%PythonScripts
set CPP_DIR=%PYTHON_SCRIPTS_DIR%\CPP
set CSHARP_DIR=%PYTHON_SCRIPTS_DIR%\CSharp
set GENERATED_DIR=%ROOT_DIR%GeneratedFiles

echo [1] 복사 중: proto_definitions.xlsx -> ProtoGenerator
copy /Y "%ROOT_DIR%proto_definitions.xlsx" "%PROTO_GEN_DIR%"

echo [2] 실행 중: generate_proto.exe
pushd "%PROTO_GEN_DIR%"
"%PROTO_GEN_DIR%\generate_proto.exe"
popd

echo [3] 삭제 중: proto_definitions.xlsx
del /F /Q "%PROTO_GEN_DIR%\proto_definitions.xlsx"

echo [4] 복사 중: Protocol.proto -> CPP, CSharp
copy /Y "%PROTO_GEN_DIR%\Protocol.proto" "%CPP_DIR%"
copy /Y "%PROTO_GEN_DIR%\Protocol.proto" "%CSHARP_DIR%"

echo [5] 실행 중: CPP Python Scripts
pushd "%CPP_DIR%"
python generate_packet_proc.py
python generate_packets.py
python generate_proto_struct.py
popd

echo [6] 실행 중: CSharp Python Scripts
pushd "%CSHARP_DIR%"
python generate_packet_handler.py
python generate_packet_manager.py
popd

echo [7] CPP 결과 이동 -> GeneratedFiles\CPP

REM CPP가 파일이면 삭제
if exist "%GENERATED_DIR%\CPP" (
    if not exist "%GENERATED_DIR%\CPP\" (
        echo 잘못 생성된 GeneratedFiles\CPP 파일 삭제 중...
        del /F /Q "%GENERATED_DIR%\CPP"
    )
)

REM CPP 폴더 생성
if not exist "%GENERATED_DIR%\CPP\" (
    mkdir "%GENERATED_DIR%\CPP"
)

move /Y "%CPP_DIR%\*.cpp" "%GENERATED_DIR%\CPP\"
move /Y "%CPP_DIR%\*.h" "%GENERATED_DIR%\CPP\"
del /F /Q "%CPP_DIR%\*.proto" >nul 2>&1

echo [8] CSharp 결과 이동 -> GeneratedFiles\CSharp

REM CSharp이 파일이면 삭제
if exist "%GENERATED_DIR%\CSharp" (
    if not exist "%GENERATED_DIR%\CSharp\" (
        echo 잘못 생성된 GeneratedFiles\CSharp 파일 삭제 중...
        del /F /Q "%GENERATED_DIR%\CSharp"
    )
)

REM CSharp 폴더 생성
if not exist "%GENERATED_DIR%\CSharp\" (
    mkdir "%GENERATED_DIR%\CSharp"
)

move /Y "%CSHARP_DIR%\*.cs" "%GENERATED_DIR%\CSharp\"
del /F /Q "%CSHARP_DIR%\*.proto" >nul 2>&1

echo [9] 잘못 생성된 루트 경로의 CPP 파일 제거

REM 루트 경로에 잘못 생성된 CPP 파일이 있을 경우 삭제
if exist "%ROOT_DIR%CPP" (
    if not exist "%ROOT_DIR%CPP\" (
        del /F /Q "%ROOT_DIR%CPP"
    )
)

echo [10] 작업 완료
pause
