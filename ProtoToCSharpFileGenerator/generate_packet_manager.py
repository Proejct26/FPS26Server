import re

# SC_ 접두사를 제외한 문자열을 PascalCase로 변환하는 함수
def to_pascal_case(snake_str):
    parts = snake_str.lower().split('_')
    return ''.join(part.capitalize() for part in parts)

# .proto 파일을 읽어 SC_ 메시지를 기준으로 C# 패킷 등록 코드를 생성하는 함수
def generate_register_method(proto_file_path):
    with open(proto_file_path, 'r', encoding='utf-8') as f:
        proto_content = f.read()

    # 'message SC_XXX {' 형태를 정규식으로 추출
    pattern = r'message (SC_[A-Z0-9_]+)\s*{'
    matches = re.findall(pattern, proto_content)

    lines = []

    # 각 SC_ 메시지에 대해 _onRecv와 _handler 등록 코드를 생성
    for msg in matches:
        pascal_name = to_pascal_case(msg[3:])  # SC_ 제거 후 PascalCase 변환
        lines.append(f'        _onRecv.Add((ushort)Game.PacketID.Sc{pascal_name}, MakePacket<{msg}>);')
        lines.append(f'        _handler.Add((ushort)Game.PacketID.Sc{pascal_name}, PacketHandler.SC_{pascal_name});')
        lines.append('')  # 코드 간 구분을 위한 빈 줄

    return '\n'.join(lines)

# 메인 함수: Protocol.proto를 읽고 PacketManagerRegisterInfo.cs 생성
def main():
    proto_file = "Protocol.proto"
    output_file = "PacketManagerRegisterInfo.cs"

    # 등록 메서드 내용 생성
    register_body = generate_register_method(proto_file)

    with open(output_file, 'w', encoding='utf-8') as f:
        f.write("public class PacketManagerRegisterInfo\n{\n")
        f.write("    // 패킷 수신 및 핸들러를 등록하는 메서드입니다.\n")
        f.write("    public void Register()\n")
        f.write("    {\n")
        f.write(register_body)
        f.write("    }\n")
        f.write("}\n")

    print("PacketManagerRegisterInfo.cs 파일이 성공적으로 생성되었습니다.")

if __name__ == "__main__":
    main()
