import re
from pathlib import Path

# --- 헤더에 추가할 텍스트 ---
HEADER_PREFIX = """#pragma once

class CSession;
class CSector;
"""

CPP_PREFIX = """#include "pch.h"
#include "MakePacket.h"
#include "SessionManager.h"
#include "Packet.h"
#include "Player.h"
#include "Sector.h"
#include "MemoryPoolManager.h"
#include "Protobuf/Protocol.pb.h"
"""

def to_pascal_case(snake_str):
    return ''.join(word.capitalize() for word in snake_str.lower().split('_'))

def parse_proto(proto_path):
    text = Path(proto_path).read_text(encoding='utf-8')

    enum_dict = {}
    enum_pattern = re.compile(r'enum\s+PacketID\s*{([^}]+)}', re.MULTILINE)
    enum_block = enum_pattern.search(text)
    if enum_block:
        for line in enum_block.group(1).splitlines():
            match = re.match(r'\s*(SC_[A-Z0-9_]+)\s*=\s*(\d+);', line)
            if match:
                enum_dict[match.group(1)] = match.group(1)

    message_pattern = re.compile(r'message\s+(SC_[A-Z0-9_]+)\s*{([^}]+)}', re.MULTILINE)
    messages = []

    for message_match in message_pattern.finditer(text):
        msg_name = message_match.group(1)
        fields_block = message_match.group(2)

        fields = []
        for line in fields_block.splitlines():
            field_match = re.match(r'\s*(\w+)\s+(\w+)\s*=\s*\d+;', line)
            if field_match:
                field_type, field_name = field_match.groups()
                fields.append((field_type, field_name))

        messages.append((msg_name, fields))

    return messages, enum_dict

def generate_header(messages):
    lines = [HEADER_PREFIX]
    for msg_name, fields in messages:
        args = ', '.join([f"UINT32 {name}" for _, name in fields])
        lines.append(f"void {msg_name}_FOR_All(CSession* pSession, {args});")
        lines.append(f"void {msg_name}_FOR_SINGLE(CSession* pSession, {args});")
        lines.append(f"void {msg_name}_FOR_AROUND(CSession* pSession, CSector* pSector, {args});\n")
    return '\n'.join(lines)

def setter_lines(fields):
    return '\n    '.join([f"pkt.set_{name.lower()}({name});" for _, name in fields])

def generate_cpp(messages, enum_dict):
    cpp = [CPP_PREFIX]

    for msg_name, fields in messages:
        msg_suffix = msg_name[3:]
        enum_name = f"SC_{to_pascal_case(msg_suffix)}"
        args = ', '.join([f"UINT32 {name}" for _, name in fields])
        setter_block = setter_lines(fields)

        # === FOR_ALL ===
        cpp.append(f"void {msg_name}_FOR_All(CSession* pSession, {args})")
        cpp.append("{")
        cpp.append(f"    game::{msg_name} pkt;\n")
        cpp.append(f"    {setter_block}\n")
        cpp.append(f"    int pktSize = pkt.ByteSizeLong();\n")
        cpp.append(f"    PACKET_HEADER header;")
        cpp.append(f"    header.byCode = dfNETWORK_PACKET_CODE;")
        cpp.append(f"    header.bySize = pktSize;")
        cpp.append(f"    header.byType = game::PacketID::{enum_name};\n")
        cpp.append(f"    int headerSize = sizeof(PACKET_HEADER);\n")
        cpp.append(f"    CPacket* Packet = packetPool.Alloc();")
        cpp.append(f"    Packet->PutData((const char*)&header, headerSize);\n")
        cpp.append(f"    char buffer[512];")
        cpp.append(f"    pkt.SerializeToArray(buffer, pktSize);")
        cpp.append(f"    Packet->PutData(buffer, pktSize);\n")
        cpp.append(f"    BroadcastData(pSession, Packet, Packet->GetDataSize());\n")
        cpp.append(f"    Packet->Clear();")
        cpp.append(f"    packetPool.Free(Packet);")
        cpp.append("}\n")

        # === FOR_SINGLE ===
        cpp.append(f"void {msg_name}_FOR_SINGLE(CSession* pSession, {args})")
        cpp.append("{")
        cpp.append(f"    game::{msg_name} pkt;\n")
        cpp.append(f"    {setter_block}\n")
        cpp.append(f"    int pktSize = pkt.ByteSizeLong();\n")
        cpp.append(f"    PACKET_HEADER header;")
        cpp.append(f"    header.byCode = dfNETWORK_PACKET_CODE;")
        cpp.append(f"    header.bySize = pktSize;")
        cpp.append(f"    header.byType = game::PacketID::{enum_name};\n")
        cpp.append(f"    int headerSize = sizeof(PACKET_HEADER);\n")
        cpp.append(f"    CPacket* Packet = packetPool.Alloc();")
        cpp.append(f"    Packet->PutData((const char*)&header, headerSize);\n")
        cpp.append(f"    char buffer[512];")
        cpp.append(f"    pkt.SerializeToArray(buffer, pktSize);")
        cpp.append(f"    Packet->PutData(buffer, pktSize);\n")
        cpp.append(f"    UnicastPacket(pSession, &header, Packet);\n")
        cpp.append(f"    Packet->Clear();")
        cpp.append(f"    packetPool.Free(Packet);")
        cpp.append("}\n")

        # === FOR_AROUND ===
        cpp.append(f"void {msg_name}_FOR_AROUND(CSession* pSession, CSector* pSector, {args})")
        cpp.append("{")
        cpp.append(f"    game::{msg_name} pkt;\n")
        cpp.append(f"    {setter_block}\n")
        cpp.append(f"    int pktSize = pkt.ByteSizeLong();\n")
        cpp.append(f"    PACKET_HEADER header;")
        cpp.append(f"    header.byCode = dfNETWORK_PACKET_CODE;")
        cpp.append(f"    header.bySize = pktSize;")
        cpp.append(f"    header.byType = game::PacketID::{enum_name};\n")
        cpp.append(f"    int headerSize = sizeof(PACKET_HEADER);\n")
        cpp.append(f"    CPacket* Packet = packetPool.Alloc();")
        cpp.append(f"    Packet->PutData((const char*)&header, headerSize);\n")
        cpp.append(f"    char buffer[512];")
        cpp.append(f"    pkt.SerializeToArray(buffer, pktSize);")
        cpp.append(f"    Packet->PutData(buffer, pktSize);\n")
        cpp.append(f"    for (auto& Sector : pSector->GetAroundSectorList())")
        cpp.append(f"    {{")
        cpp.append(f"        for (auto& Object : Sector->GetSectorObjectMap())")
        cpp.append(f"        {{")
        cpp.append(f"            if (pSession == Object.second->m_pSession)")
        cpp.append(f"                continue;\n")
        cpp.append(f"            UnicastPacket(Object.second->m_pSession, &header, Packet);")
        cpp.append(f"        }}")
        cpp.append(f"    }}\n")
        cpp.append(f"    Packet->Clear();")
        cpp.append(f"    packetPool.Free(Packet);")
        cpp.append("}\n")

    return '\n'.join(cpp)

def main():
    proto_file = "Protocol.proto"
    out_h = "MakePacket.h"
    out_cpp = "MakePacket.cpp"

    messages, enum_dict = parse_proto(proto_file)

    Path(out_h).write_text(generate_header(messages), encoding='utf-8')
    Path(out_cpp).write_text(generate_cpp(messages, enum_dict), encoding='utf-8')

    print(f"Generated {out_h} and {out_cpp} from {proto_file}")

if __name__ == "__main__":
    main()
