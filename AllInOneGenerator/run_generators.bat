@echo off
setlocal enabledelayedexpansion

REM ��� ����
set ROOT_DIR=%~dp0
set PROTO_GEN_DIR=%ROOT_DIR%ProtoGenerator
set PYTHON_SCRIPTS_DIR=%ROOT_DIR%PythonScripts
set CPP_DIR=%PYTHON_SCRIPTS_DIR%\CPP
set CSHARP_DIR=%PYTHON_SCRIPTS_DIR%\CSharp
set GENERATED_DIR=%ROOT_DIR%GeneratedFiles

echo [1] ���� ��: proto_definitions.xlsx -> ProtoGenerator
copy /Y "%ROOT_DIR%proto_definitions.xlsx" "%PROTO_GEN_DIR%"

echo [2] ���� ��: generate_proto.exe
pushd "%PROTO_GEN_DIR%"
"%PROTO_GEN_DIR%\generate_proto.exe"
popd

echo [3] ���� ��: proto_definitions.xlsx
del /F /Q "%PROTO_GEN_DIR%\proto_definitions.xlsx"

echo [4] ���� ��: Protocol.proto -> CPP, CSharp
copy /Y "%PROTO_GEN_DIR%\Protocol.proto" "%CPP_DIR%"
copy /Y "%PROTO_GEN_DIR%\Protocol.proto" "%CSHARP_DIR%"

echo [5] ���� ��: CPP Python Scripts
pushd "%CPP_DIR%"
python generate_packet_proc.py
python generate_packets.py
python generate_proto_struct.py
popd

echo [6] ���� ��: CSharp Python Scripts
pushd "%CSHARP_DIR%"
python generate_packet_handler.py
python generate_packet_manager.py
popd

echo [7] CPP ��� �̵� -> GeneratedFiles\CPP

REM CPP�� �����̸� ����
if exist "%GENERATED_DIR%\CPP" (
    if not exist "%GENERATED_DIR%\CPP\" (
        echo �߸� ������ GeneratedFiles\CPP ���� ���� ��...
        del /F /Q "%GENERATED_DIR%\CPP"
    )
)

REM CPP ���� ����
if not exist "%GENERATED_DIR%\CPP\" (
    mkdir "%GENERATED_DIR%\CPP"
)

move /Y "%CPP_DIR%\*.cpp" "%GENERATED_DIR%\CPP\"
move /Y "%CPP_DIR%\*.h" "%GENERATED_DIR%\CPP\"
del /F /Q "%CPP_DIR%\*.proto" >nul 2>&1

echo [8] CSharp ��� �̵� -> GeneratedFiles\CSharp

REM CSharp�� �����̸� ����
if exist "%GENERATED_DIR%\CSharp" (
    if not exist "%GENERATED_DIR%\CSharp\" (
        echo �߸� ������ GeneratedFiles\CSharp ���� ���� ��...
        del /F /Q "%GENERATED_DIR%\CSharp"
    )
)

REM CSharp ���� ����
if not exist "%GENERATED_DIR%\CSharp\" (
    mkdir "%GENERATED_DIR%\CSharp"
)

move /Y "%CSHARP_DIR%\*.cs" "%GENERATED_DIR%\CSharp\"
del /F /Q "%CSHARP_DIR%\*.proto" >nul 2>&1

echo [9] �߸� ������ ��Ʈ ����� CPP ���� ����

REM ��Ʈ ��ο� �߸� ������ CPP ������ ���� ��� ����
if exist "%ROOT_DIR%CPP" (
    if not exist "%ROOT_DIR%CPP\" (
        del /F /Q "%ROOT_DIR%CPP"
    )
)

echo [10] �۾� �Ϸ�
pause
