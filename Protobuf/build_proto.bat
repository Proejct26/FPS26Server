@echo off

set PROTOC=protoc.exe
set PROTO_DIR=%~dp0protos
set OUT_CPP=generated\cpp
set OUT_CS=generated\csharp

if not exist %OUT_CPP% mkdir %OUT_CPP%
if not exist %OUT_CS% mkdir %OUT_CS%

echo Generating C++ code...
%PROTOC% --proto_path=%PROTO_DIR% --cpp_out=%OUT_CPP% %PROTO_DIR%\*.proto

echo Generating C# code...
%PROTOC% --proto_path=%PROTO_DIR% --csharp_out=%OUT_CS% %PROTO_DIR%\*.proto

echo Done.
pause
