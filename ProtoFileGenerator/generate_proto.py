# pandas: Excel 데이터를 읽기 위해 사용
import pandas as pd

# jinja2: 템플릿 엔진으로 문자열을 자동으로 조합하여 .proto 코드 생성
from jinja2 import Environment

# os: 경로 관련
import os

# PyInstaller 문제 해결용
import openpyxl
_ = openpyxl.__version__  # PyInstaller가 openpyxl을 포함하도록 강제로 사용

import sys

# 실행 중인 파일(.py 또는 .exe)의 디렉토리 경로
if getattr(sys, 'frozen', False):
    base_dir = os.path.dirname(sys.executable)
else:
    base_dir = os.path.dirname(os.path.abspath(__file__))

# Excel 파일 경로 및 출력 파일 경로를 base_dir 기준으로 설정
excel_path = os.path.join(base_dir, "proto_definitions.xlsx")  # 엑셀 파일 경로 (.proto 정의가 들어 있는 파일)
output_path = os.path.join(base_dir, "generated_output.proto")  # 생성할 .proto 파일의 출력 경로

# 엑셀 파일이 없을 경우 오류 출력 후 종료
if not os.path.exists(excel_path):
    print(f"엑셀 파일을 찾을 수 없습니다: {excel_path}")
    input("Enter 키를 눌러 종료합니다.")
    exit(1)

try:
    # 엑셀 파일의 "Messages" 시트를 읽어옴
    # 각 row는 하나의 message 필드 정의 (message 이름, 필드 이름, 타입, 주석 포함)
    messages_df = pd.read_excel(excel_path, sheet_name="Messages", engine="openpyxl")

    # 엑셀 파일의 "PacketEnum" 시트를 읽어옴
    # 각 row는 하나의 enum 항목 (PacketID용)
    packet_enum_df = pd.read_excel(excel_path, sheet_name="PacketEnum", engine="openpyxl")

except Exception as e:
    print("엑셀 파일 읽는 중 오류 발생:", e)
    input("Enter 키를 눌러 종료합니다.")
    exit(1)

# 메시지 이름을 기준으로 그룹핑
# 같은 MessageName을 가진 행들을 하나의 message 블록으로 묶음
grouped = messages_df.groupby("MessageName")

# grouped 데이터를 딕셔너리로 변환
# 구조: { "MessageName": [ {필드1}, {필드2}, ... ] }
# records : 각 행(row)을 하나의 딕셔너리로 만들어서, 전체를 리스트에 담는 옵션
messages = {
    name: group.to_dict(orient="records")
    for name, group in grouped
}

# Jinja2 템플릿 환경 설정
# trim_blocks: 블록 닫힌 후 남는 줄 제거
# lstrip_blocks: 블록 시작 시 앞 공백 제거
env = Environment(
    trim_blocks=True,
    lstrip_blocks=True
)

# Jinja2 템플릿 문자열 정의 (.proto 포맷 구조)
# PacketID enum과 message 정의를 자동 생성함
proto_template = env.from_string('''syntax = "proto3";

package game;

enum PacketID {
{% for packet in packet_enum %}
    {{ packet }} = {{ loop.index }};
{% endfor %}
}

{% for message_name, fields in messages.items() %}
message {{ message_name }} {
{% for field in fields %}
    {{ field["Type"] }} {{ field["FieldName"] }} = {{ loop.index }};{{ " // " + field["Comment"] if field["Comment"] else "" }}
{% endfor %}
}
{% endfor %}
''')

# 템플릿에 실제 데이터 주입하여 문자열 생성
# packet_enum: 리스트 형태
# messages: 딕셔너리 형태
rendered_proto = proto_template.render(
    packet_enum=packet_enum_df["PacketName"].tolist(),
    messages=messages
)

# 생성된 .proto 내용을 파일로 저장
with open(output_path, "w", encoding="utf-8") as f:
    f.write(rendered_proto)

# 완료 메시지 출력
print(f".proto 파일이 생성되었습니다: {output_path}")
input("Enter 키를 누르면 종료됩니다.")
