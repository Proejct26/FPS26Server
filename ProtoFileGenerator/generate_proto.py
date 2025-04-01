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

# 오류 발생 시 메시지 출력 후 종료
def error_exit(message):
    print(message)
    input("Enter 키를 눌러 종료합니다.")
    exit(1)

# 실행 중인 파일(.py 또는 .exe)의 디렉토리 경로
if getattr(sys, 'frozen', False):
    base_dir = os.path.dirname(sys.executable)
else:
    base_dir = os.path.dirname(os.path.abspath(__file__))

# Excel 파일 경로 및 출력 파일 경로를 base_dir 기준으로 설정
excel_path = os.path.join(base_dir, "proto_definitions.xlsx")
output_path = os.path.join(base_dir, "Protocol.proto")

# 엑셀 파일이 없을 경우 오류 출력 후 종료
if not os.path.exists(excel_path):
    error_exit(f"엑셀 파일을 찾을 수 없습니다: {excel_path}")

try:
    # 모든 시트의 내용을 딕셔너리 형태로 읽어옴
    all_sheets = pd.read_excel(excel_path, sheet_name=None, engine="openpyxl")
except Exception as e:
    error_exit(f"엑셀 파일 읽는 중 오류 발생: {e}")

# 모든 메시지 정의를 저장할 딕셔너리
messages = {}

# SC_, CS_로 시작하는 메시지를 따로 모아 PacketID enum 생성용으로 사용
packet_names_set = set()

# 중복 메시지 이름 체크용 집합
defined_message_names = set()

# 각 시트를 순회하면서 메시지를 수집
for sheet_name, df in all_sheets.items():
    if "MessageName" in df.columns and "FieldName" in df.columns and "Type" in df.columns:
        grouped = df.groupby("MessageName")
        for name, group in grouped:
            if name in defined_message_names:
                error_exit(f"[중복 메시지 이름 오류] '{name}' 이(가) 시트 '{sheet_name}'에서 중복 정의되었습니다.")

            defined_message_names.add(name)
            messages[name] = group.to_dict(orient="records")

            if name.startswith("SC_") or name.startswith("CS_"):
                packet_names_set.add(name)

# PacketID enum 항목들을 PascalCase로 변환
def format_packet_name(name):
    if name.startswith("CS_") or name.startswith("SC_"):
        prefix = name[:3]
        rest = name[3:]
        parts = rest.split('_')
        pascal_case = ''.join(part.capitalize() for part in parts)
        return prefix + pascal_case
    return name

# PacketID enum 항목들을 알파벳 순 정렬 후 번호 부여 (0부터 시작)
packet_enum = [{"name": format_packet_name(name), "value": i} for i, name in enumerate(sorted(packet_names_set))]

# Jinja2 템플릿 환경 설정
env = Environment(
    trim_blocks=True,
    lstrip_blocks=True
)

# 템플릿 정의 (.proto 포맷)
proto_template = env.from_string('''syntax = "proto3";

package game;

enum PacketID {
// Client → Server
{% for packet in packet_enum if packet.name.startswith("CS_") %}
    {{ packet.name }} = {{ packet.value }};
{% endfor %}

// Server → Client
{% for packet in packet_enum if packet.name.startswith("SC_") %}
    {{ packet.name }} = {{ packet.value }};
{% endfor %}
}

{% for message_name, fields in messages.items() %}
message {{ message_name }} {
{% for field in fields %}
    {{ field["Type"] }} {{ field["FieldName"] }} = {{ loop.index }};{{ " // " + field["Comment"] if "Comment" in field and field["Comment"] else "" }}
{% endfor %}
}
{% endfor %}
''')

# Comment 필드가 NaN이면 빈 문자열로 치환
for field_list in messages.values():
    for field in field_list:
        comment = field.get("Comment", "")
        field["Comment"] = "" if pd.isna(comment) else str(comment)

# 템플릿에 실제 데이터 주입하여 문자열 생성
rendered_proto = proto_template.render(
    packet_enum=packet_enum,
    messages=messages
)

# 생성된 .proto 내용을 파일로 저장
try:
    with open(output_path, "w", encoding="utf-8") as f:
        f.write(rendered_proto)
    print(f".proto 파일이 성공적으로 생성되었습니다: {output_path}")
except Exception as e:
    error_exit(f".proto 파일 저장 중 오류 발생: {e}")
