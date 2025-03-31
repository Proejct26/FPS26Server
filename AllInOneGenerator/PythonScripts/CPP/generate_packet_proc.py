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
    "fixed32": "UINT32",
    "sfixed32": "INT32",
    "fixed64": "UINT64",
    "sfixed64": "INT64",
}

# PascalCase 변환
def to_pascal_case(snake_str):
    return ''.join(word.capitalize() for word in snake_str.lower().split('_'))

# .proto 파일 파싱
def parse_proto(proto_path):
    text = Path(proto_path).read_text(encoding='utf-8')

    message_pattern = re.compile(r'message\s+(CS_[A-Z0-9_]+)\s*{([^}]+)}', re.MULTILINE)
    messages = []

    for message_match in message_pattern.finditer(text):
        msg_name = message_match.group(1)
        fields_block = message_match.group(2)

        fields = []
        for line in fields_block.splitlines():
            field_match = re.match(r'\s*(\w+)\s+(\w+)\s*=\s*\d+;', line)
            if field_match:
                field_type, field_name = field_match.groups()
                field_type = field_type.strip().lower()
                cpp_type = PROTO_TO_CPP_TYPE.get(field_type, field_type)
                fields.append((cpp_type, field_name))

        messages.append((msg_name, fields))

    return messages

# PacketProc.h 생성
def generate_header(messages):
    header = """#pragma once

#include "Session.h"
#include "Protobuf/Protocol.pb.h"

extern int g_iSyncCount;

class CObject;
class CSession;

bool PacketProc(CSession* pSession, game::PacketID packetType, CPacket* pPacket);
void DisconnectSessionProc(CSession* pSession);
"""
    lines = [header]
    for msg_name, fields in messages:
        args = ', '.join([f"{typ} {name}" for typ, name in fields])
        lines.append(f"bool {msg_name}(CSession* pSession, {args});")
    return '\n'.join(lines)

# PacketProc.cpp 생성
def generate_cpp(messages):
    cpp_header = """#include "pch.h"
#include "PacketProc.h"
#include "Packet.h"
#include "MakePacket.h"

#include "SessionManager.h"

#include "ObjectManager.h"

#include "Player.h"

#include "MemoryPoolManager.h"
#include "TimerManager.h"
#include "LogManager.h"

#include "Protobuf/Protocol.pb.h"

static CObjectManager& objectManager = CObjectManager::GetInstance();
static CTimerManager& timerManager = CTimerManager::GetInstance();
static LogManager& logManager = LogManager::GetInstance();

int g_iSyncCount = 0;
"""
    lines = [cpp_header]

    # PacketProc 함수
    lines.append("bool PacketProc(CSession* pSession, game::PacketID packetType, CPacket* pPacket)")
    lines.append("{")
    lines.append("    switch (packetType)")
    lines.append("    {")

    for msg_name, fields in messages:
        enum_name = f"CS_{to_pascal_case(msg_name[3:])}"
        vars_decl = '\n        '.join([f"{typ} {name};" for typ, name in fields])
        field_assigns = '\n        '.join([f"{name} = pkt.{name.lower()}();" for _, name in fields])
        call_args = ', '.join([name for _, name in fields])

        lines.append(f"    case game::PacketID::{enum_name}:")
        lines.append("    {")
        if fields:
            lines.append(f"        {vars_decl}\n")
        lines.append(f"        game::{msg_name} pkt;")
        lines.append("        pkt.ParseFromArray(pPacket->GetBufferPtr(), pPacket->GetDataSize());\n")
        if fields:
            lines.append(f"        {field_assigns}\n")
        lines.append(f"        return {msg_name}(pSession{', ' + call_args if call_args else ''});")
        lines.append("    }")
        lines.append("    break;")

    lines.append("    default:")
    lines.append("        break;")
    lines.append("    }")
    lines.append("    return false;")
    lines.append("}")

    # DisconnectSessionProc 정의 추가
    lines.append("\nvoid DisconnectSessionProc(CSession* pSession)")
    lines.append("{")
    lines.append("    return;")
    lines.append("}")

    # 각 함수 정의
    for msg_name, fields in messages:
        args = ', '.join([f"{typ} {name}" for typ, name in fields])
        lines.append(f"bool {msg_name}(CSession* pSession, {args})")
        lines.append("{")
        lines.append("    return false;")
        lines.append("}\n")

    return '\n'.join(lines)

# 메인 실행 함수
def main():
    proto_file = "Protocol.proto"  # 현재 폴더에 Protocol.proto 필요
    h_file = "PacketProc.h"
    cpp_file = "PacketProc.cpp"

    messages = parse_proto(proto_file)

    h_content = generate_header(messages)
    cpp_content = generate_cpp(messages)

    Path(h_file).write_text(h_content, encoding='utf-8')
    Path(cpp_file).write_text(cpp_content, encoding='utf-8')

    print(f"[\u2713] Generated: {h_file}")
    print(f"[\u2713] Generated: {cpp_file}")

if __name__ == "__main__":
    main()
