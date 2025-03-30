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
output_path = os.path.join(base_dir, "Protocol.proto")  # 생성할 .proto 파일의 출력 경로

# 엑셀 파일이 없을 경우 오류 출력 후 종료
if not os.path.exists(excel_path):
    print(f"엑셀 파일을 찾을 수 없습니다: {excel_path}")
    input("Enter 키를 눌러 종료합니다.")
    exit(1)

try:
    # 모든 시트의 내용을 딕셔너리 형태로 읽어옴
    all_sheets = pd.read_excel(excel_path, sheet_name=None, engine="openpyxl")
except Exception as e:
    print("엑셀 파일 읽는 중 오류 발생:", e)
    input("Enter 키를 눌러 종료합니다.")
    exit(1)

# 모든 메시지 정의를 저장할 딕셔너리
messages = {}

# SC_, CS_로 시작하는 메시지를 따로 모아 PacketID enum 생성용으로 사용
packet_names_set = set()

# 중복 메시지 이름 체크용 집합
defined_message_names = set()

# 각 시트를 순회하면서 메시지를 수집
for sheet_name, df in all_sheets.items():
    # 필요한 컬럼이 존재하는 시트만 처리
    if "MessageName" in df.columns and "FieldName" in df.columns and "Type" in df.columns:
        grouped = df.groupby("MessageName")
        for name, group in grouped:
            # 이전 시트에서 이미 정의된 메시지 이름이 다시 등장하면 오류 처리
            if name in defined_message_names:
                print(f"[중복 메시지 이름 오류] '{name}' 이(가) 시트 '{sheet_name}'에서 중복 정의되었습니다.")
                input("Enter 키를 눌러 종료합니다.")
                exit(1)

            defined_message_names.add(name)

            # 각 메시지 이름에 대해 필드 목록 저장
            messages[name] = group.to_dict(orient="records")

            # PacketID enum에 넣을 대상이면 따로 기록
            if name.startswith("SC_") or name.startswith("CS_"):
                packet_names_set.add(name)


# PacketID enum 항목들을 알파벳 순 정렬 후 PascalCase로 변환하여 번호 부여 (1부터 시작)
def format_packet_name(name):
    if name.startswith("CS_") or name.startswith("SC_"):
        prefix = name[:3]
        rest = name[3:]
        # 단어 단위로 나눈 후 PascalCase로 변환
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

# Jinja2 템플릿 문자열 정의 (.proto 포맷 구조)
# PacketID enum과 message 정의를 자동 생성함
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
        if pd.isna(comment):
            field["Comment"] = ""
        else:
            field["Comment"] = str(comment)

# 템플릿에 실제 데이터 주입하여 문자열 생성
rendered_proto = proto_template.render(
    packet_enum=packet_enum,
    messages=messages
)

# 생성된 .proto 내용을 파일로 저장
with open(output_path, "w", encoding="utf-8") as f:
    f.write(rendered_proto)

# 완료 메시지 출력
print(f".proto 파일이 생성되었습니다: {output_path}")
input("Enter 키를 누르면 종료됩니다.")
