import re

# SC_XXX 형식을 PascalCase로 변환하는 함수 (SC_CREATE_MY_CHARACTER → CreateMyCharacter)
def to_pascal_case(snake_str):
    return ''.join(word.capitalize() for word in snake_str.lower().split('_'))

# SC_XXX 형식을 camelCase로 변환하는 함수 (SC_CREATE_MY_CHARACTER → createMyCharacterPacket)
def to_camel_case_variable(snake_str):
    parts = snake_str.lower().split('_')
    return parts[0] + ''.join(word.capitalize() for word in parts[1:]) + 'Packet'

# .proto 파일을 분석해 SC_로 시작하는 메시지에 대한 핸들러 함수 생성
def generate_handler_functions(proto_file_path):
    with open(proto_file_path, 'r', encoding='utf-8') as f:
        proto_content = f.read()

    # 'message SC_XXX {' 형태의 메시지를 정규표현식으로 추출
    pattern = r'message (SC_[A-Z0-9_]+)\s*{'
    matches = re.findall(pattern, proto_content)

    lines = []

    for msg in matches:
        msg_base = msg[3:]  # 'SC_' 제거
        pascal_name = to_pascal_case(msg_base)  # CreateMyCharacter
        method_name = f'SC_{pascal_name}'       # SC_CreateMyCharacter
        variable_name = to_camel_case_variable(msg_base)  # createMyCharacterPacket

        # 핸들러 함수 정의 생성
        lines.append(f'    // {msg} 패킷을 처리하는 함수')
        lines.append(f'    public static void {method_name}(PacketSession session, IMessage packet)')
        lines.append('    {')
        lines.append(f'        {msg} {variable_name} = packet as {msg};')
        lines.append('')
        lines.append(f'        // TODO: {method_name} 패킷 처리 로직을 여기에 구현')
        lines.append('    }')
        lines.append('')  # 함수 간 줄 바꿈

    return '\n'.join(lines)

# PacketHandler.cs 파일 생성 함수
def main():
    proto_file = "Protocol.proto"
    output_file = "PacketHandler.cs"

    handler_functions = generate_handler_functions(proto_file)

    with open(output_file, 'w', encoding='utf-8') as f:
        # 파일 헤더 및 using 구문 작성
        f.write("using Game;\n")
        f.write("using Google.Protobuf;\n")
        f.write("using ServerCore;\n")
        f.write("using System.Collections;\n")
        f.write("using System.Collections.Generic;\n")
        f.write("using UnityEngine;\n\n")
        f.write("class PacketHandler\n")
        f.write("{\n")
        f.write(handler_functions)
        f.write("}\n")

    print("PacketHandler.cs 파일이 성공적으로 생성되었습니다.")

if __name__ == "__main__":
    main()
