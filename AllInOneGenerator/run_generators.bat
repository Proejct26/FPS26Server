@echo off
echo [1] generate_packet_proc.py ���� ��...
python generate_packet_proc.py

echo [2] generate_packets.py ���� ��...
python generate_packets.py

echo [3] generate_proto_struct.py ���� ��...
python generate_proto_struct.py

echo [4] Files ���� ���� (�̹� ������ ����)...
if not exist Files (
    mkdir Files
)

echo [5] .cpp �� .h ���� �̵� ��...
move /Y *.cpp Files\
move /Y *.h   Files\

echo ��� �۾��� �Ϸ�Ǿ����ϴ�.
pause
