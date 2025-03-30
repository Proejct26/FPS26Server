# generate_proto_struct.py

import re
from pathlib import Path

PROTO_TO_CPP_TYPE = {
    "uint32": "UINT32",
    "int32": "INT32",
    "uint64": "UINT64",
    "int64": "INT64",
    "float": "float",
    "double": "double",
    "bool": "bool",
    "string": "std::string",
}

def extract_messages(proto_text):
    messages = {}
    current_message = None

    for line in proto_text.splitlines():
        line = line.strip()
        if line.startswith("message "):
            current_message = line.split()[1]
            messages[current_message] = []
        elif current_message and line and ";" in line and not line.startswith("//"):
            match = re.match(r'(repeated\s+)?(\w+)\s+(\w+)\s*=\s*\d+;', line)
            if match:
                repeated, type_name, field_name = match.groups()
                messages[current_message].append((repeated is not None, type_name, field_name))
        elif line == "}":
            current_message = None

    return messages

def generate_cpp_structs(structs):
    lines = [
        '#pragma once',
        ''
    ]

    for name, fields in structs.items():
        lines.append(f"struct {name} {{")
        for is_repeated, type_name, field_name in fields:
            cpp_type = PROTO_TO_CPP_TYPE.get(type_name, type_name)
            if is_repeated:
                lines.append(f"    std::vector<{cpp_type}> {field_name};")
            else:
                lines.append(f"    {cpp_type} {field_name};")
        lines.append("};\n")

    return "\n".join(lines)

def main():
    script_dir = Path(__file__).parent
    proto_path = script_dir / "Protocol.proto"
    output_path = script_dir / "ProtoStruct.h"

    if not proto_path.exists():
        print(f"[ERROR] Protocol.proto 파일이 이 디렉토리에 없습니다: {proto_path}")
        return

    with proto_path.open("r", encoding="utf-8") as f:
        proto_text = f.read()

    messages = extract_messages(proto_text)
    filtered = {name: fields for name, fields in messages.items() if not name.startswith(("SC_", "CS_"))}
    struct_code = generate_cpp_structs(filtered)
    output_path.write_text(struct_code, encoding="utf-8")

    print(f"[SUCCESS] ProtoStruct.h 생성 완료: {output_path.name}")

if __name__ == "__main__":
    main()
