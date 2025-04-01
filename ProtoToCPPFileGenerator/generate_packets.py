import re
from pathlib import Path

# --- 헤더에 추가할 텍스트 ---
HEADER_PREFIX = """#pragma once

#include "ProtoStruct.h"

class CSession;
class CRoom;
"""

CPP_PREFIX = """#include "pch.h"
#include "MakePacket.h"
#include "SessionManager.h"
#include "Packet.h"
#include "Player.h"
#include "Room.h"
#include "MemoryPoolManager.h"
#include "Protobuf/Protocol.pb.h"
"""

PROTO_TO_CPP_TYPE = {
    "fixed32": "UINT32",
    "sfixed32": "INT32",
    "fixed64": "UINT64",
    "sfixed64": "INT64",
    "uint32": "UINT32",
    "int32": "INT32",
    "uint64": "UINT64",
    "int64": "INT64",
    "float": "float",
    "double": "double",
    "bool": "bool",
    "string": "std::string",
}

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
    struct_pattern = re.compile(r'message\s+([A-Za-z0-9_]+)\s*{([^}]+)}', re.MULTILINE)
    messages = []
    struct_map = {}

    for struct_match in struct_pattern.finditer(text):
        name = struct_match.group(1)
        body = struct_match.group(2)
        fields = []
        for line in body.splitlines():
            repeated_match = re.match(r'\s*repeated\s+(\w+)\s+(\w+)\s*=\s*\d+;', line)
            single_match = re.match(r'\s*(\w+)\s+(\w+)\s*=\s*\d+;', line)
            if repeated_match:
                t, n = repeated_match.groups()
                fields.append((t.strip(), n.strip(), True))
            elif single_match:
                t, n = single_match.groups()
                fields.append((t.strip(), n.strip(), False))
        struct_map[name] = fields

    for message_match in message_pattern.finditer(text):
        msg_name = message_match.group(1)
        fields_block = message_match.group(2)

        fields = []
        for line in fields_block.splitlines():
            repeated_match = re.match(r'\s*repeated\s+(\w+)\s+(\w+)\s*=\s*\d+;', line)
            single_match = re.match(r'\s*(\w+)\s+(\w+)\s*=\s*\d+;', line)
            if repeated_match:
                field_type, field_name = repeated_match.groups()
                fields.append((field_type.strip(), field_name, True))
            elif single_match:
                field_type, field_name = single_match.groups()
                fields.append((field_type.strip(), field_name, False))

        messages.append((msg_name, fields))

    return messages, enum_dict, struct_map

def expand_repeated_struct(type_name, field_name, struct_map):
    lines = []
    if type_name not in struct_map:
        return [
            f"    for (const auto& data : {field_name}) {{",
            f"        auto* item = pkt.add_{field_name.lower()}();",
            f"        *item = data;",
            "    }"
        ]

    lines.append(f"    for (const auto& data : {field_name}) {{")
    lines.append(f"        game::{type_name}* item = pkt.add_{field_name.lower()}();")
    for sub_type, sub_name, _ in struct_map[type_name]:
        if sub_type in struct_map:
            lines.append(f"        game::{sub_type}* nested = item->mutable_{sub_name.lower()}();")
            for n_type, n_name, _ in struct_map[sub_type]:
                lines.append(f"        nested->set_{n_name.lower()}(data.{sub_name}.{n_name});")
        else:
            lines.append(f"        item->set_{sub_name.lower()}(data.{sub_name});")
    lines.append("    }")
    return lines

def setter_lines(fields, struct_map):
    result = []
    for field_type, field_name, is_repeated in fields:
        if is_repeated:
            result.extend(expand_repeated_struct(field_type, field_name, struct_map))
        else:
            if field_type in struct_map:
                result.append(f"    {{")
                result.append(f"        game::{field_type}* sub = pkt.mutable_{field_name.lower()}();")
                for sub_type, sub_name, _ in struct_map[field_type]:
                    if sub_type in struct_map:
                        result.append(f"        game::{sub_type}* nested = sub->mutable_{sub_name.lower()}();")
                        for n_type, n_name, _ in struct_map[sub_type]:
                            result.append(f"        nested->set_{n_name.lower()}({field_name}.{sub_name}.{n_name});")
                    else:
                        result.append(f"        sub->set_{sub_name.lower()}({field_name}.{sub_name});")
                result.append(f"    }}")
            else:
                result.append(f"    pkt.set_{field_name.lower()}({field_name});")
    return '\n'.join(result)

def generate_header(messages):
    lines = [HEADER_PREFIX]
    for msg_name, fields in messages:
        args = []
        for field_type, field_name, is_repeated in fields:
            cpp_type = PROTO_TO_CPP_TYPE.get(field_type, field_type)
            if is_repeated:
                args.append(f"std::vector<{cpp_type}>& {field_name}")
            else:
                args.append(f"{cpp_type} {field_name}")
        arg_str = ', '.join(args)
        lines.append(f"void {msg_name}_FOR_All(CSession* pSession, {arg_str});")
        lines.append(f"void {msg_name}_FOR_SINGLE(CSession* pSession, {arg_str});")
        lines.append(f"void {msg_name}_FOR_AROUND(CSession* pSession, CRoom* pRoom, {arg_str});\n")
    return '\n'.join(lines)

def generate_cpp(messages, enum_dict, struct_map):
    cpp = [CPP_PREFIX]

    for msg_name, fields in messages:
        msg_suffix = msg_name[3:]
        enum_name = f"SC_{to_pascal_case(msg_suffix)}"

        args = []
        for field_type, field_name, is_repeated in fields:
            cpp_type = PROTO_TO_CPP_TYPE.get(field_type, field_type)
            if is_repeated:
                args.append(f"std::vector<{cpp_type}>& {field_name}")
            else:
                args.append(f"{cpp_type} {field_name}")
        arg_str = ', '.join(args)
        setter_block = setter_lines(fields, struct_map)

        for suffix, body in [
            ("FOR_All", "BroadcastData(pSession, Packet, Packet->GetDataSize());"),
            ("FOR_SINGLE", "UnicastPacket(pSession, &header, Packet);"),
            ("FOR_AROUND", """for (auto& player : pRoom->m_activePlayers)
    {
        if (pSession == player->m_pSession)
            continue;
        UnicastPacket(player->m_pSession, &header, Packet);
    }""")
        ]:
            prefix = f"void {msg_name}_{suffix}(CSession* pSession, "
            if suffix == "FOR_AROUND":
                prefix += "CRoom* pRoom, "
            prefix += arg_str + ")"

            cpp.append(prefix)
            cpp.append("{")
            cpp.append(f"    game::{msg_name} pkt;\n")
            cpp.append(setter_block + "\n")
            cpp.append("    int pktSize = pkt.ByteSizeLong();\n")
            cpp.append("    PACKET_HEADER header;")
            cpp.append("    header.byCode = dfNETWORK_PACKET_CODE;")
            cpp.append("    header.bySize = pktSize;")
            cpp.append(f"    header.byType = game::PacketID::{enum_name};\n")
            cpp.append("    int headerSize = sizeof(PACKET_HEADER);")
            cpp.append("    CPacket* Packet = packetPool.Alloc();\n")
            cpp.append("    char buffer[512];")
            cpp.append("    pkt.SerializeToArray(buffer, pktSize);")
            cpp.append("    Packet->PutData(buffer, pktSize);\n")
            cpp.append(f"    {body}")
            cpp.append("    Packet->Clear();")
            cpp.append("    packetPool.Free(Packet);")
            cpp.append("}\n")

    return '\n'.join(cpp)

def main():
    proto_file = "Protocol.proto"
    out_h = "MakePacket.h"
    out_cpp = "MakePacket.cpp"

    messages, enum_dict, struct_map = parse_proto(proto_file)

    Path(out_h).write_text(generate_header(messages), encoding='utf-8')
    Path(out_cpp).write_text(generate_cpp(messages, enum_dict, struct_map), encoding='utf-8')

    print(f"Generated {out_h} and {out_cpp} from {proto_file}")

if __name__ == "__main__":
    main()
