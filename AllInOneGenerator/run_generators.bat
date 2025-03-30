@echo off
echo [1] generate_packet_proc.py 실행 중...
python generate_packet_proc.py

echo [2] generate_packets.py 실행 중...
python generate_packets.py

echo [3] generate_proto_struct.py 실행 중...
python generate_proto_struct.py

echo [4] Files 폴더 생성 (이미 있으면 생략)...
if not exist Files (
    mkdir Files
)

echo [5] .cpp 및 .h 파일 이동 중...
move /Y *.cpp Files\
move /Y *.h   Files\

echo 모든 작업이 완료되었습니다.
pause
