import pandas as pd
from jinja2 import Template

# 엑셀 파일 경로
excel_path = "proto_definitions.xlsx"
output_path = "generated_output.proto"

# 엑셀 파일 로드
messages_df = pd.read_excel(excel_path, sheet_name="Messages")
packet_enum_df = pd.read_excel(excel_path, sheet_name="PacketEnum")

# message 이름별로 그룹핑
grouped = messages_df.groupby("MessageName")

# 각 message에 대한 필드 리스트 만들기
messages = {
    name: group.to_dict(orient="records")
    for name, group in grouped
}

# 템플릿 (repeated 자동 포함 가능)
proto_template = Template('''syntax = "proto3";

package game;

enum PacketID {
{% for packet in packet_enum %}
    {{ packet }} = {{ loop.index }};
{% endfor %}
}

{% for message_name, fields in messages.items() %}
message {{ message_name }} {
{% for field in fields %}
    {{ field["Type"] }} {{ field["FieldName"] }} = {{ loop.index }};{% if field["Comment"] %} // {{ field["Comment"] }}{% endif %}
{% endfor %}
}
{% endfor %}
''')

# 템플릿 렌더링
output_proto = proto_template.render(
    packet_enum=packet_enum_df["PacketName"].tolist(),
    messages=messages
)

# 결과 저장
with open(output_path, "w", encoding="utf-8") as f:
    f.write(output_proto)

print(f"✔️ .proto 파일이 생성되었습니다: {output_path}")
