#include "pch.h"
#include "MakePacket.h"
#include "SessionManager.h"
#include "Packet.h"
#include "Player.h"
#include "Room.h"
#include "MemoryPoolManager.h"
#include "Protobuf/Protocol.pb.h"

void SC_ATTACK_FOR_All(CSession* pSession, UINT32 playerId, float normalX, float normalY, float normalZ, float posX, float posY, float posZ)
{
    game::SC_ATTACK pkt;

    pkt.set_playerid(playerId);
    pkt.set_normalx(normalX);
    pkt.set_normaly(normalY);
    pkt.set_normalz(normalZ);
    pkt.set_posx(posX);
    pkt.set_posy(posY);
    pkt.set_posz(posZ);

    int pktSize = pkt.ByteSizeLong();

    PACKET_HEADER header;
    header.byCode = dfNETWORK_PACKET_CODE;
    header.bySize = pktSize;
    header.byType = game::PacketID::SC_Attack;

    int headerSize = sizeof(PACKET_HEADER);
    CPacket* Packet = packetPool.Alloc();

    char buffer[512];
    pkt.SerializeToArray(buffer, pktSize);
    Packet->PutData(buffer, pktSize);

    BroadcastData(pSession, Packet, Packet->GetDataSize());
    Packet->Clear();
    packetPool.Free(Packet);
}

void SC_ATTACK_FOR_SINGLE(CSession* pSession, UINT32 playerId, float normalX, float normalY, float normalZ, float posX, float posY, float posZ)
{
    game::SC_ATTACK pkt;

    pkt.set_playerid(playerId);
    pkt.set_normalx(normalX);
    pkt.set_normaly(normalY);
    pkt.set_normalz(normalZ);
    pkt.set_posx(posX);
    pkt.set_posy(posY);
    pkt.set_posz(posZ);

    int pktSize = pkt.ByteSizeLong();

    PACKET_HEADER header;
    header.byCode = dfNETWORK_PACKET_CODE;
    header.bySize = pktSize;
    header.byType = game::PacketID::SC_Attack;

    int headerSize = sizeof(PACKET_HEADER);
    CPacket* Packet = packetPool.Alloc();

    char buffer[512];
    pkt.SerializeToArray(buffer, pktSize);
    Packet->PutData(buffer, pktSize);

    UnicastPacket(pSession, &header, Packet);
    Packet->Clear();
    packetPool.Free(Packet);
}

void SC_ATTACK_FOR_AROUND(CSession* pSession, CRoom* pRoom, UINT32 playerId, float normalX, float normalY, float normalZ, float posX, float posY, float posZ)
{
    game::SC_ATTACK pkt;

    pkt.set_playerid(playerId);
    pkt.set_normalx(normalX);
    pkt.set_normaly(normalY);
    pkt.set_normalz(normalZ);
    pkt.set_posx(posX);
    pkt.set_posy(posY);
    pkt.set_posz(posZ);

    int pktSize = pkt.ByteSizeLong();

    PACKET_HEADER header;
    header.byCode = dfNETWORK_PACKET_CODE;
    header.bySize = pktSize;
    header.byType = game::PacketID::SC_Attack;

    int headerSize = sizeof(PACKET_HEADER);
    CPacket* Packet = packetPool.Alloc();

    char buffer[512];
    pkt.SerializeToArray(buffer, pktSize);
    Packet->PutData(buffer, pktSize);

    for (auto& player : pRoom->m_activePlayers)
    {
        if (pSession == player->m_pSession)
            continue;
        UnicastPacket(player->m_pSession, &header, Packet);
    }
    Packet->Clear();
    packetPool.Free(Packet);
}

void SC_CHANGE_WEAPON_FOR_All(CSession* pSession, UINT32 playerId, UINT32 weapon)
{
    game::SC_CHANGE_WEAPON pkt;

    pkt.set_playerid(playerId);
    pkt.set_weapon(weapon);

    int pktSize = pkt.ByteSizeLong();

    PACKET_HEADER header;
    header.byCode = dfNETWORK_PACKET_CODE;
    header.bySize = pktSize;
    header.byType = game::PacketID::SC_ChangeWeapon;

    int headerSize = sizeof(PACKET_HEADER);
    CPacket* Packet = packetPool.Alloc();

    char buffer[512];
    pkt.SerializeToArray(buffer, pktSize);
    Packet->PutData(buffer, pktSize);

    BroadcastData(pSession, Packet, Packet->GetDataSize());
    Packet->Clear();
    packetPool.Free(Packet);
}

void SC_CHANGE_WEAPON_FOR_SINGLE(CSession* pSession, UINT32 playerId, UINT32 weapon)
{
    game::SC_CHANGE_WEAPON pkt;

    pkt.set_playerid(playerId);
    pkt.set_weapon(weapon);

    int pktSize = pkt.ByteSizeLong();

    PACKET_HEADER header;
    header.byCode = dfNETWORK_PACKET_CODE;
    header.bySize = pktSize;
    header.byType = game::PacketID::SC_ChangeWeapon;

    int headerSize = sizeof(PACKET_HEADER);
    CPacket* Packet = packetPool.Alloc();

    char buffer[512];
    pkt.SerializeToArray(buffer, pktSize);
    Packet->PutData(buffer, pktSize);

    UnicastPacket(pSession, &header, Packet);
    Packet->Clear();
    packetPool.Free(Packet);
}

void SC_CHANGE_WEAPON_FOR_AROUND(CSession* pSession, CRoom* pRoom, UINT32 playerId, UINT32 weapon)
{
    game::SC_CHANGE_WEAPON pkt;

    pkt.set_playerid(playerId);
    pkt.set_weapon(weapon);

    int pktSize = pkt.ByteSizeLong();

    PACKET_HEADER header;
    header.byCode = dfNETWORK_PACKET_CODE;
    header.bySize = pktSize;
    header.byType = game::PacketID::SC_ChangeWeapon;

    int headerSize = sizeof(PACKET_HEADER);
    CPacket* Packet = packetPool.Alloc();

    char buffer[512];
    pkt.SerializeToArray(buffer, pktSize);
    Packet->PutData(buffer, pktSize);

    for (auto& player : pRoom->m_activePlayers)
    {
        if (pSession == player->m_pSession)
            continue;
        UnicastPacket(player->m_pSession, &header, Packet);
    }
    Packet->Clear();
    packetPool.Free(Packet);
}

void SC_CHARACTER_DOWN_FOR_All(CSession* pSession, UINT32 playerId, UINT32 teamID)
{
    game::SC_CHARACTER_DOWN pkt;

    pkt.set_playerid(playerId);
    pkt.set_teamid(teamID);

    int pktSize = pkt.ByteSizeLong();

    PACKET_HEADER header;
    header.byCode = dfNETWORK_PACKET_CODE;
    header.bySize = pktSize;
    header.byType = game::PacketID::SC_CharacterDown;

    int headerSize = sizeof(PACKET_HEADER);
    CPacket* Packet = packetPool.Alloc();

    char buffer[512];
    pkt.SerializeToArray(buffer, pktSize);
    Packet->PutData(buffer, pktSize);

    BroadcastData(pSession, Packet, Packet->GetDataSize());
    Packet->Clear();
    packetPool.Free(Packet);
}

void SC_CHARACTER_DOWN_FOR_SINGLE(CSession* pSession, UINT32 playerId, UINT32 teamID)
{
    game::SC_CHARACTER_DOWN pkt;

    pkt.set_playerid(playerId);
    pkt.set_teamid(teamID);

    int pktSize = pkt.ByteSizeLong();

    PACKET_HEADER header;
    header.byCode = dfNETWORK_PACKET_CODE;
    header.bySize = pktSize;
    header.byType = game::PacketID::SC_CharacterDown;

    int headerSize = sizeof(PACKET_HEADER);
    CPacket* Packet = packetPool.Alloc();

    char buffer[512];
    pkt.SerializeToArray(buffer, pktSize);
    Packet->PutData(buffer, pktSize);

    UnicastPacket(pSession, &header, Packet);
    Packet->Clear();
    packetPool.Free(Packet);
}

void SC_CHARACTER_DOWN_FOR_AROUND(CSession* pSession, CRoom* pRoom, UINT32 playerId, UINT32 teamID)
{
    game::SC_CHARACTER_DOWN pkt;

    pkt.set_playerid(playerId);
    pkt.set_teamid(teamID);

    int pktSize = pkt.ByteSizeLong();

    PACKET_HEADER header;
    header.byCode = dfNETWORK_PACKET_CODE;
    header.bySize = pktSize;
    header.byType = game::PacketID::SC_CharacterDown;

    int headerSize = sizeof(PACKET_HEADER);
    CPacket* Packet = packetPool.Alloc();

    char buffer[512];
    pkt.SerializeToArray(buffer, pktSize);
    Packet->PutData(buffer, pktSize);

    for (auto& player : pRoom->m_activePlayers)
    {
        if (pSession == player->m_pSession)
            continue;
        UnicastPacket(player->m_pSession, &header, Packet);
    }
    Packet->Clear();
    packetPool.Free(Packet);
}

void SC_CHARACTER_KILL_LOG_FOR_All(CSession* pSession, std::vector<PlayerInfo>& playerInfoList)
{
    game::SC_CHARACTER_KILL_LOG pkt;

    for (const auto& data : playerInfoList) {
        game::PlayerInfo* item = pkt.add_playerinfolist();
        item->set_playerid(data.playerId);
        game::KDAInfo* nested = item->mutable_kda();
        nested->set_kill(data.kda.kill);
        nested->set_death(data.kda.death);
        nested->set_assist(data.kda.assist);
    }

    int pktSize = pkt.ByteSizeLong();

    PACKET_HEADER header;
    header.byCode = dfNETWORK_PACKET_CODE;
    header.bySize = pktSize;
    header.byType = game::PacketID::SC_CharacterKillLog;

    int headerSize = sizeof(PACKET_HEADER);
    CPacket* Packet = packetPool.Alloc();

    char buffer[512];
    pkt.SerializeToArray(buffer, pktSize);
    Packet->PutData(buffer, pktSize);

    BroadcastData(pSession, Packet, Packet->GetDataSize());
    Packet->Clear();
    packetPool.Free(Packet);
}

void SC_CHARACTER_KILL_LOG_FOR_SINGLE(CSession* pSession, std::vector<PlayerInfo>& playerInfoList)
{
    game::SC_CHARACTER_KILL_LOG pkt;

    for (const auto& data : playerInfoList) {
        game::PlayerInfo* item = pkt.add_playerinfolist();
        item->set_playerid(data.playerId);
        game::KDAInfo* nested = item->mutable_kda();
        nested->set_kill(data.kda.kill);
        nested->set_death(data.kda.death);
        nested->set_assist(data.kda.assist);
    }

    int pktSize = pkt.ByteSizeLong();

    PACKET_HEADER header;
    header.byCode = dfNETWORK_PACKET_CODE;
    header.bySize = pktSize;
    header.byType = game::PacketID::SC_CharacterKillLog;

    int headerSize = sizeof(PACKET_HEADER);
    CPacket* Packet = packetPool.Alloc();

    char buffer[512];
    pkt.SerializeToArray(buffer, pktSize);
    Packet->PutData(buffer, pktSize);

    UnicastPacket(pSession, &header, Packet);
    Packet->Clear();
    packetPool.Free(Packet);
}

void SC_CHARACTER_KILL_LOG_FOR_AROUND(CSession* pSession, CRoom* pRoom, std::vector<PlayerInfo>& playerInfoList)
{
    game::SC_CHARACTER_KILL_LOG pkt;

    for (const auto& data : playerInfoList) {
        game::PlayerInfo* item = pkt.add_playerinfolist();
        item->set_playerid(data.playerId);
        game::KDAInfo* nested = item->mutable_kda();
        nested->set_kill(data.kda.kill);
        nested->set_death(data.kda.death);
        nested->set_assist(data.kda.assist);
    }

    int pktSize = pkt.ByteSizeLong();

    PACKET_HEADER header;
    header.byCode = dfNETWORK_PACKET_CODE;
    header.bySize = pktSize;
    header.byType = game::PacketID::SC_CharacterKillLog;

    int headerSize = sizeof(PACKET_HEADER);
    CPacket* Packet = packetPool.Alloc();

    char buffer[512];
    pkt.SerializeToArray(buffer, pktSize);
    Packet->PutData(buffer, pktSize);

    for (auto& player : pRoom->m_activePlayers)
    {
        if (pSession == player->m_pSession)
            continue;
        UnicastPacket(player->m_pSession, &header, Packet);
    }
    Packet->Clear();
    packetPool.Free(Packet);
}

void SC_CREATE_MY_CHARACTER_FOR_All(CSession* pSession, UINT32 playerId, UINT32 posIndex, UINT32 maxHP, UINT32 teamID)
{
    game::SC_CREATE_MY_CHARACTER pkt;

    pkt.set_playerid(playerId);
    pkt.set_posindex(posIndex);
    pkt.set_maxhp(maxHP);
    pkt.set_teamid(teamID);

    int pktSize = pkt.ByteSizeLong();

    PACKET_HEADER header;
    header.byCode = dfNETWORK_PACKET_CODE;
    header.bySize = pktSize;
    header.byType = game::PacketID::SC_CreateMyCharacter;

    int headerSize = sizeof(PACKET_HEADER);
    CPacket* Packet = packetPool.Alloc();

    char buffer[512];
    pkt.SerializeToArray(buffer, pktSize);
    Packet->PutData(buffer, pktSize);

    BroadcastData(pSession, Packet, Packet->GetDataSize());
    Packet->Clear();
    packetPool.Free(Packet);
}

void SC_CREATE_MY_CHARACTER_FOR_SINGLE(CSession* pSession, UINT32 playerId, UINT32 posIndex, UINT32 maxHP, UINT32 teamID)
{
    game::SC_CREATE_MY_CHARACTER pkt;

    pkt.set_playerid(playerId);
    pkt.set_posindex(posIndex);
    pkt.set_maxhp(maxHP);
    pkt.set_teamid(teamID);

    int pktSize = pkt.ByteSizeLong();

    PACKET_HEADER header;
    header.byCode = dfNETWORK_PACKET_CODE;
    header.bySize = pktSize;
    header.byType = game::PacketID::SC_CreateMyCharacter;

    int headerSize = sizeof(PACKET_HEADER);
    CPacket* Packet = packetPool.Alloc();

    char buffer[512];
    pkt.SerializeToArray(buffer, pktSize);
    Packet->PutData(buffer, pktSize);

    UnicastPacket(pSession, &header, Packet);
    Packet->Clear();
    packetPool.Free(Packet);
}

void SC_CREATE_MY_CHARACTER_FOR_AROUND(CSession* pSession, CRoom* pRoom, UINT32 playerId, UINT32 posIndex, UINT32 maxHP, UINT32 teamID)
{
    game::SC_CREATE_MY_CHARACTER pkt;

    pkt.set_playerid(playerId);
    pkt.set_posindex(posIndex);
    pkt.set_maxhp(maxHP);
    pkt.set_teamid(teamID);

    int pktSize = pkt.ByteSizeLong();

    PACKET_HEADER header;
    header.byCode = dfNETWORK_PACKET_CODE;
    header.bySize = pktSize;
    header.byType = game::PacketID::SC_CreateMyCharacter;

    int headerSize = sizeof(PACKET_HEADER);
    CPacket* Packet = packetPool.Alloc();

    char buffer[512];
    pkt.SerializeToArray(buffer, pktSize);
    Packet->PutData(buffer, pktSize);

    for (auto& player : pRoom->m_activePlayers)
    {
        if (pSession == player->m_pSession)
            continue;
        UnicastPacket(player->m_pSession, &header, Packet);
    }
    Packet->Clear();
    packetPool.Free(Packet);
}

void SC_CREATE_OTHER_CHARACTER_FOR_All(CSession* pSession, UINT32 playerId, UINT32 posIndex, UINT32 maxHP, UINT32 curHP, std::string name, KDAInfo kdaInfo, UINT32 weapon, UINT32 teamID)
{
    game::SC_CREATE_OTHER_CHARACTER pkt;

    pkt.set_playerid(playerId);
    pkt.set_posindex(posIndex);
    pkt.set_maxhp(maxHP);
    pkt.set_curhp(curHP);
    pkt.set_name(name);
    {
        game::KDAInfo* sub = pkt.mutable_kdainfo();
        sub->set_kill(kdaInfo.kill);
        sub->set_death(kdaInfo.death);
        sub->set_assist(kdaInfo.assist);
    }
    pkt.set_weapon(weapon);
    pkt.set_teamid(teamID);

    int pktSize = pkt.ByteSizeLong();

    PACKET_HEADER header;
    header.byCode = dfNETWORK_PACKET_CODE;
    header.bySize = pktSize;
    header.byType = game::PacketID::SC_CreateOtherCharacter;

    int headerSize = sizeof(PACKET_HEADER);
    CPacket* Packet = packetPool.Alloc();

    char buffer[512];
    pkt.SerializeToArray(buffer, pktSize);
    Packet->PutData(buffer, pktSize);

    BroadcastData(pSession, Packet, Packet->GetDataSize());
    Packet->Clear();
    packetPool.Free(Packet);
}

void SC_CREATE_OTHER_CHARACTER_FOR_SINGLE(CSession* pSession, UINT32 playerId, UINT32 posIndex, UINT32 maxHP, UINT32 curHP, std::string name, KDAInfo kdaInfo, UINT32 weapon, UINT32 teamID)
{
    game::SC_CREATE_OTHER_CHARACTER pkt;

    pkt.set_playerid(playerId);
    pkt.set_posindex(posIndex);
    pkt.set_maxhp(maxHP);
    pkt.set_curhp(curHP);
    pkt.set_name(name);
    {
        game::KDAInfo* sub = pkt.mutable_kdainfo();
        sub->set_kill(kdaInfo.kill);
        sub->set_death(kdaInfo.death);
        sub->set_assist(kdaInfo.assist);
    }
    pkt.set_weapon(weapon);
    pkt.set_teamid(teamID);

    int pktSize = pkt.ByteSizeLong();

    PACKET_HEADER header;
    header.byCode = dfNETWORK_PACKET_CODE;
    header.bySize = pktSize;
    header.byType = game::PacketID::SC_CreateOtherCharacter;

    int headerSize = sizeof(PACKET_HEADER);
    CPacket* Packet = packetPool.Alloc();

    char buffer[512];
    pkt.SerializeToArray(buffer, pktSize);
    Packet->PutData(buffer, pktSize);

    UnicastPacket(pSession, &header, Packet);
    Packet->Clear();
    packetPool.Free(Packet);
}

void SC_CREATE_OTHER_CHARACTER_FOR_AROUND(CSession* pSession, CRoom* pRoom, UINT32 playerId, UINT32 posIndex, UINT32 maxHP, UINT32 curHP, std::string name, KDAInfo kdaInfo, UINT32 weapon, UINT32 teamID)
{
    game::SC_CREATE_OTHER_CHARACTER pkt;

    pkt.set_playerid(playerId);
    pkt.set_posindex(posIndex);
    pkt.set_maxhp(maxHP);
    pkt.set_curhp(curHP);
    pkt.set_name(name);
    {
        game::KDAInfo* sub = pkt.mutable_kdainfo();
        sub->set_kill(kdaInfo.kill);
        sub->set_death(kdaInfo.death);
        sub->set_assist(kdaInfo.assist);
    }
    pkt.set_weapon(weapon);
    pkt.set_teamid(teamID);

    int pktSize = pkt.ByteSizeLong();

    PACKET_HEADER header;
    header.byCode = dfNETWORK_PACKET_CODE;
    header.bySize = pktSize;
    header.byType = game::PacketID::SC_CreateOtherCharacter;

    int headerSize = sizeof(PACKET_HEADER);
    CPacket* Packet = packetPool.Alloc();

    char buffer[512];
    pkt.SerializeToArray(buffer, pktSize);
    Packet->PutData(buffer, pktSize);

    for (auto& player : pRoom->m_activePlayers)
    {
        if (pSession == player->m_pSession)
            continue;
        UnicastPacket(player->m_pSession, &header, Packet);
    }
    Packet->Clear();
    packetPool.Free(Packet);
}

void SC_ITEM_PICK_FAIL_FOR_All(CSession* pSession, UINT32 playerId, UINT32 itemId)
{
    game::SC_ITEM_PICK_FAIL pkt;

    pkt.set_playerid(playerId);
    pkt.set_itemid(itemId);

    int pktSize = pkt.ByteSizeLong();

    PACKET_HEADER header;
    header.byCode = dfNETWORK_PACKET_CODE;
    header.bySize = pktSize;
    header.byType = game::PacketID::SC_ItemPickFail;

    int headerSize = sizeof(PACKET_HEADER);
    CPacket* Packet = packetPool.Alloc();

    char buffer[512];
    pkt.SerializeToArray(buffer, pktSize);
    Packet->PutData(buffer, pktSize);

    BroadcastData(pSession, Packet, Packet->GetDataSize());
    Packet->Clear();
    packetPool.Free(Packet);
}

void SC_ITEM_PICK_FAIL_FOR_SINGLE(CSession* pSession, UINT32 playerId, UINT32 itemId)
{
    game::SC_ITEM_PICK_FAIL pkt;

    pkt.set_playerid(playerId);
    pkt.set_itemid(itemId);

    int pktSize = pkt.ByteSizeLong();

    PACKET_HEADER header;
    header.byCode = dfNETWORK_PACKET_CODE;
    header.bySize = pktSize;
    header.byType = game::PacketID::SC_ItemPickFail;

    int headerSize = sizeof(PACKET_HEADER);
    CPacket* Packet = packetPool.Alloc();

    char buffer[512];
    pkt.SerializeToArray(buffer, pktSize);
    Packet->PutData(buffer, pktSize);

    UnicastPacket(pSession, &header, Packet);
    Packet->Clear();
    packetPool.Free(Packet);
}

void SC_ITEM_PICK_FAIL_FOR_AROUND(CSession* pSession, CRoom* pRoom, UINT32 playerId, UINT32 itemId)
{
    game::SC_ITEM_PICK_FAIL pkt;

    pkt.set_playerid(playerId);
    pkt.set_itemid(itemId);

    int pktSize = pkt.ByteSizeLong();

    PACKET_HEADER header;
    header.byCode = dfNETWORK_PACKET_CODE;
    header.bySize = pktSize;
    header.byType = game::PacketID::SC_ItemPickFail;

    int headerSize = sizeof(PACKET_HEADER);
    CPacket* Packet = packetPool.Alloc();

    char buffer[512];
    pkt.SerializeToArray(buffer, pktSize);
    Packet->PutData(buffer, pktSize);

    for (auto& player : pRoom->m_activePlayers)
    {
        if (pSession == player->m_pSession)
            continue;
        UnicastPacket(player->m_pSession, &header, Packet);
    }
    Packet->Clear();
    packetPool.Free(Packet);
}

void SC_ITEM_PICK_SUCCESS_FOR_All(CSession* pSession, UINT32 playerId, UINT32 itemId)
{
    game::SC_ITEM_PICK_SUCCESS pkt;

    pkt.set_playerid(playerId);
    pkt.set_itemid(itemId);

    int pktSize = pkt.ByteSizeLong();

    PACKET_HEADER header;
    header.byCode = dfNETWORK_PACKET_CODE;
    header.bySize = pktSize;
    header.byType = game::PacketID::SC_ItemPickSuccess;

    int headerSize = sizeof(PACKET_HEADER);
    CPacket* Packet = packetPool.Alloc();

    char buffer[512];
    pkt.SerializeToArray(buffer, pktSize);
    Packet->PutData(buffer, pktSize);

    BroadcastData(pSession, Packet, Packet->GetDataSize());
    Packet->Clear();
    packetPool.Free(Packet);
}

void SC_ITEM_PICK_SUCCESS_FOR_SINGLE(CSession* pSession, UINT32 playerId, UINT32 itemId)
{
    game::SC_ITEM_PICK_SUCCESS pkt;

    pkt.set_playerid(playerId);
    pkt.set_itemid(itemId);

    int pktSize = pkt.ByteSizeLong();

    PACKET_HEADER header;
    header.byCode = dfNETWORK_PACKET_CODE;
    header.bySize = pktSize;
    header.byType = game::PacketID::SC_ItemPickSuccess;

    int headerSize = sizeof(PACKET_HEADER);
    CPacket* Packet = packetPool.Alloc();

    char buffer[512];
    pkt.SerializeToArray(buffer, pktSize);
    Packet->PutData(buffer, pktSize);

    UnicastPacket(pSession, &header, Packet);
    Packet->Clear();
    packetPool.Free(Packet);
}

void SC_ITEM_PICK_SUCCESS_FOR_AROUND(CSession* pSession, CRoom* pRoom, UINT32 playerId, UINT32 itemId)
{
    game::SC_ITEM_PICK_SUCCESS pkt;

    pkt.set_playerid(playerId);
    pkt.set_itemid(itemId);

    int pktSize = pkt.ByteSizeLong();

    PACKET_HEADER header;
    header.byCode = dfNETWORK_PACKET_CODE;
    header.bySize = pktSize;
    header.byType = game::PacketID::SC_ItemPickSuccess;

    int headerSize = sizeof(PACKET_HEADER);
    CPacket* Packet = packetPool.Alloc();

    char buffer[512];
    pkt.SerializeToArray(buffer, pktSize);
    Packet->PutData(buffer, pktSize);

    for (auto& player : pRoom->m_activePlayers)
    {
        if (pSession == player->m_pSession)
            continue;
        UnicastPacket(player->m_pSession, &header, Packet);
    }
    Packet->Clear();
    packetPool.Free(Packet);
}

void SC_ITEM_SPAWNED_FOR_All(CSession* pSession, UINT32 itemId, UINT32 itemType, UINT32 itemPosIndex)
{
    game::SC_ITEM_SPAWNED pkt;

    pkt.set_itemid(itemId);
    pkt.set_itemtype(itemType);
    pkt.set_itemposindex(itemPosIndex);

    int pktSize = pkt.ByteSizeLong();

    PACKET_HEADER header;
    header.byCode = dfNETWORK_PACKET_CODE;
    header.bySize = pktSize;
    header.byType = game::PacketID::SC_ItemSpawned;

    int headerSize = sizeof(PACKET_HEADER);
    CPacket* Packet = packetPool.Alloc();

    char buffer[512];
    pkt.SerializeToArray(buffer, pktSize);
    Packet->PutData(buffer, pktSize);

    BroadcastData(pSession, Packet, Packet->GetDataSize());
    Packet->Clear();
    packetPool.Free(Packet);
}

void SC_ITEM_SPAWNED_FOR_SINGLE(CSession* pSession, UINT32 itemId, UINT32 itemType, UINT32 itemPosIndex)
{
    game::SC_ITEM_SPAWNED pkt;

    pkt.set_itemid(itemId);
    pkt.set_itemtype(itemType);
    pkt.set_itemposindex(itemPosIndex);

    int pktSize = pkt.ByteSizeLong();

    PACKET_HEADER header;
    header.byCode = dfNETWORK_PACKET_CODE;
    header.bySize = pktSize;
    header.byType = game::PacketID::SC_ItemSpawned;

    int headerSize = sizeof(PACKET_HEADER);
    CPacket* Packet = packetPool.Alloc();

    char buffer[512];
    pkt.SerializeToArray(buffer, pktSize);
    Packet->PutData(buffer, pktSize);

    UnicastPacket(pSession, &header, Packet);
    Packet->Clear();
    packetPool.Free(Packet);
}

void SC_ITEM_SPAWNED_FOR_AROUND(CSession* pSession, CRoom* pRoom, UINT32 itemId, UINT32 itemType, UINT32 itemPosIndex)
{
    game::SC_ITEM_SPAWNED pkt;

    pkt.set_itemid(itemId);
    pkt.set_itemtype(itemType);
    pkt.set_itemposindex(itemPosIndex);

    int pktSize = pkt.ByteSizeLong();

    PACKET_HEADER header;
    header.byCode = dfNETWORK_PACKET_CODE;
    header.bySize = pktSize;
    header.byType = game::PacketID::SC_ItemSpawned;

    int headerSize = sizeof(PACKET_HEADER);
    CPacket* Packet = packetPool.Alloc();

    char buffer[512];
    pkt.SerializeToArray(buffer, pktSize);
    Packet->PutData(buffer, pktSize);

    for (auto& player : pRoom->m_activePlayers)
    {
        if (pSession == player->m_pSession)
            continue;
        UnicastPacket(player->m_pSession, &header, Packet);
    }
    Packet->Clear();
    packetPool.Free(Packet);
}

void SC_KEY_INPUT_FOR_All(CSession* pSession, UINT32 playerId, UINT32 keyW, UINT32 keyA, UINT32 keyS, UINT32 keyD, UINT32 rotateAxisX, UINT32 rotateAxisY, UINT32 Jump)
{
    game::SC_KEY_INPUT pkt;

    pkt.set_playerid(playerId);
    pkt.set_keyw(keyW);
    pkt.set_keya(keyA);
    pkt.set_keys(keyS);
    pkt.set_keyd(keyD);
    pkt.set_rotateaxisx(rotateAxisX);
    pkt.set_rotateaxisy(rotateAxisY);
    pkt.set_jump(Jump);

    int pktSize = pkt.ByteSizeLong();

    PACKET_HEADER header;
    header.byCode = dfNETWORK_PACKET_CODE;
    header.bySize = pktSize;
    header.byType = game::PacketID::SC_KeyInput;

    int headerSize = sizeof(PACKET_HEADER);
    CPacket* Packet = packetPool.Alloc();

    char buffer[512];
    pkt.SerializeToArray(buffer, pktSize);
    Packet->PutData(buffer, pktSize);

    BroadcastData(pSession, Packet, Packet->GetDataSize());
    Packet->Clear();
    packetPool.Free(Packet);
}

void SC_KEY_INPUT_FOR_SINGLE(CSession* pSession, UINT32 playerId, UINT32 keyW, UINT32 keyA, UINT32 keyS, UINT32 keyD, UINT32 rotateAxisX, UINT32 rotateAxisY, UINT32 Jump)
{
    game::SC_KEY_INPUT pkt;

    pkt.set_playerid(playerId);
    pkt.set_keyw(keyW);
    pkt.set_keya(keyA);
    pkt.set_keys(keyS);
    pkt.set_keyd(keyD);
    pkt.set_rotateaxisx(rotateAxisX);
    pkt.set_rotateaxisy(rotateAxisY);
    pkt.set_jump(Jump);

    int pktSize = pkt.ByteSizeLong();

    PACKET_HEADER header;
    header.byCode = dfNETWORK_PACKET_CODE;
    header.bySize = pktSize;
    header.byType = game::PacketID::SC_KeyInput;

    int headerSize = sizeof(PACKET_HEADER);
    CPacket* Packet = packetPool.Alloc();

    char buffer[512];
    pkt.SerializeToArray(buffer, pktSize);
    Packet->PutData(buffer, pktSize);

    UnicastPacket(pSession, &header, Packet);
    Packet->Clear();
    packetPool.Free(Packet);
}

void SC_KEY_INPUT_FOR_AROUND(CSession* pSession, CRoom* pRoom, UINT32 playerId, UINT32 keyW, UINT32 keyA, UINT32 keyS, UINT32 keyD, UINT32 rotateAxisX, UINT32 rotateAxisY, UINT32 Jump)
{
    game::SC_KEY_INPUT pkt;

    pkt.set_playerid(playerId);
    pkt.set_keyw(keyW);
    pkt.set_keya(keyA);
    pkt.set_keys(keyS);
    pkt.set_keyd(keyD);
    pkt.set_rotateaxisx(rotateAxisX);
    pkt.set_rotateaxisy(rotateAxisY);
    pkt.set_jump(Jump);

    int pktSize = pkt.ByteSizeLong();

    PACKET_HEADER header;
    header.byCode = dfNETWORK_PACKET_CODE;
    header.bySize = pktSize;
    header.byType = game::PacketID::SC_KeyInput;

    int headerSize = sizeof(PACKET_HEADER);
    CPacket* Packet = packetPool.Alloc();

    char buffer[512];
    pkt.SerializeToArray(buffer, pktSize);
    Packet->PutData(buffer, pktSize);

    for (auto& player : pRoom->m_activePlayers)
    {
        if (pSession == player->m_pSession)
            continue;
        UnicastPacket(player->m_pSession, &header, Packet);
    }
    Packet->Clear();
    packetPool.Free(Packet);
}

void SC_ON_ACCEPT_FOR_All(CSession* pSession, UINT32 playerId)
{
    game::SC_ON_ACCEPT pkt;

    pkt.set_playerid(playerId);

    int pktSize = pkt.ByteSizeLong();

    PACKET_HEADER header;
    header.byCode = dfNETWORK_PACKET_CODE;
    header.bySize = pktSize;
    header.byType = game::PacketID::SC_OnAccept;

    int headerSize = sizeof(PACKET_HEADER);
    CPacket* Packet = packetPool.Alloc();

    char buffer[512];
    pkt.SerializeToArray(buffer, pktSize);
    Packet->PutData(buffer, pktSize);

    BroadcastData(pSession, Packet, Packet->GetDataSize());
    Packet->Clear();
    packetPool.Free(Packet);
}

void SC_ON_ACCEPT_FOR_SINGLE(CSession* pSession, UINT32 playerId)
{
    game::SC_ON_ACCEPT pkt;

    pkt.set_playerid(playerId);

    int pktSize = pkt.ByteSizeLong();

    PACKET_HEADER header;
    header.byCode = dfNETWORK_PACKET_CODE;
    header.bySize = pktSize;
    header.byType = game::PacketID::SC_OnAccept;

    int headerSize = sizeof(PACKET_HEADER);
    CPacket* Packet = packetPool.Alloc();

    char buffer[512];
    pkt.SerializeToArray(buffer, pktSize);
    Packet->PutData(buffer, pktSize);

    UnicastPacket(pSession, &header, Packet);
    Packet->Clear();
    packetPool.Free(Packet);
}

void SC_ON_ACCEPT_FOR_AROUND(CSession* pSession, CRoom* pRoom, UINT32 playerId)
{
    game::SC_ON_ACCEPT pkt;

    pkt.set_playerid(playerId);

    int pktSize = pkt.ByteSizeLong();

    PACKET_HEADER header;
    header.byCode = dfNETWORK_PACKET_CODE;
    header.bySize = pktSize;
    header.byType = game::PacketID::SC_OnAccept;

    int headerSize = sizeof(PACKET_HEADER);
    CPacket* Packet = packetPool.Alloc();

    char buffer[512];
    pkt.SerializeToArray(buffer, pktSize);
    Packet->PutData(buffer, pktSize);

    for (auto& player : pRoom->m_activePlayers)
    {
        if (pSession == player->m_pSession)
            continue;
        UnicastPacket(player->m_pSession, &header, Packet);
    }
    Packet->Clear();
    packetPool.Free(Packet);
}

void SC_POS_INTERPOLATION_FOR_All(CSession* pSession, float posX, float posY, float posZ)
{
    game::SC_POS_INTERPOLATION pkt;

    pkt.set_posx(posX);
    pkt.set_posy(posY);
    pkt.set_posz(posZ);

    int pktSize = pkt.ByteSizeLong();

    PACKET_HEADER header;
    header.byCode = dfNETWORK_PACKET_CODE;
    header.bySize = pktSize;
    header.byType = game::PacketID::SC_PosInterpolation;

    int headerSize = sizeof(PACKET_HEADER);
    CPacket* Packet = packetPool.Alloc();

    char buffer[512];
    pkt.SerializeToArray(buffer, pktSize);
    Packet->PutData(buffer, pktSize);

    BroadcastData(pSession, Packet, Packet->GetDataSize());
    Packet->Clear();
    packetPool.Free(Packet);
}

void SC_POS_INTERPOLATION_FOR_SINGLE(CSession* pSession, float posX, float posY, float posZ)
{
    game::SC_POS_INTERPOLATION pkt;

    pkt.set_posx(posX);
    pkt.set_posy(posY);
    pkt.set_posz(posZ);

    int pktSize = pkt.ByteSizeLong();

    PACKET_HEADER header;
    header.byCode = dfNETWORK_PACKET_CODE;
    header.bySize = pktSize;
    header.byType = game::PacketID::SC_PosInterpolation;

    int headerSize = sizeof(PACKET_HEADER);
    CPacket* Packet = packetPool.Alloc();

    char buffer[512];
    pkt.SerializeToArray(buffer, pktSize);
    Packet->PutData(buffer, pktSize);

    UnicastPacket(pSession, &header, Packet);
    Packet->Clear();
    packetPool.Free(Packet);
}

void SC_POS_INTERPOLATION_FOR_AROUND(CSession* pSession, CRoom* pRoom, float posX, float posY, float posZ)
{
    game::SC_POS_INTERPOLATION pkt;

    pkt.set_posx(posX);
    pkt.set_posy(posY);
    pkt.set_posz(posZ);

    int pktSize = pkt.ByteSizeLong();

    PACKET_HEADER header;
    header.byCode = dfNETWORK_PACKET_CODE;
    header.bySize = pktSize;
    header.byType = game::PacketID::SC_PosInterpolation;

    int headerSize = sizeof(PACKET_HEADER);
    CPacket* Packet = packetPool.Alloc();

    char buffer[512];
    pkt.SerializeToArray(buffer, pktSize);
    Packet->PutData(buffer, pktSize);

    for (auto& player : pRoom->m_activePlayers)
    {
        if (pSession == player->m_pSession)
            continue;
        UnicastPacket(player->m_pSession, &header, Packet);
    }
    Packet->Clear();
    packetPool.Free(Packet);
}

void SC_SEND_MESSAGE_FOR_All(CSession* pSession, UINT32 playerId, std::string message)
{
    game::SC_SEND_MESSAGE pkt;

    pkt.set_playerid(playerId);
    pkt.set_message(message);

    int pktSize = pkt.ByteSizeLong();

    PACKET_HEADER header;
    header.byCode = dfNETWORK_PACKET_CODE;
    header.bySize = pktSize;
    header.byType = game::PacketID::SC_SendMessage;

    int headerSize = sizeof(PACKET_HEADER);
    CPacket* Packet = packetPool.Alloc();

    char buffer[512];
    pkt.SerializeToArray(buffer, pktSize);
    Packet->PutData(buffer, pktSize);

    BroadcastData(pSession, Packet, Packet->GetDataSize());
    Packet->Clear();
    packetPool.Free(Packet);
}

void SC_SEND_MESSAGE_FOR_SINGLE(CSession* pSession, UINT32 playerId, std::string message)
{
    game::SC_SEND_MESSAGE pkt;

    pkt.set_playerid(playerId);
    pkt.set_message(message);

    int pktSize = pkt.ByteSizeLong();

    PACKET_HEADER header;
    header.byCode = dfNETWORK_PACKET_CODE;
    header.bySize = pktSize;
    header.byType = game::PacketID::SC_SendMessage;

    int headerSize = sizeof(PACKET_HEADER);
    CPacket* Packet = packetPool.Alloc();

    char buffer[512];
    pkt.SerializeToArray(buffer, pktSize);
    Packet->PutData(buffer, pktSize);

    UnicastPacket(pSession, &header, Packet);
    Packet->Clear();
    packetPool.Free(Packet);
}

void SC_SEND_MESSAGE_FOR_AROUND(CSession* pSession, CRoom* pRoom, UINT32 playerId, std::string message)
{
    game::SC_SEND_MESSAGE pkt;

    pkt.set_playerid(playerId);
    pkt.set_message(message);

    int pktSize = pkt.ByteSizeLong();

    PACKET_HEADER header;
    header.byCode = dfNETWORK_PACKET_CODE;
    header.bySize = pktSize;
    header.byType = game::PacketID::SC_SendMessage;

    int headerSize = sizeof(PACKET_HEADER);
    CPacket* Packet = packetPool.Alloc();

    char buffer[512];
    pkt.SerializeToArray(buffer, pktSize);
    Packet->PutData(buffer, pktSize);

    for (auto& player : pRoom->m_activePlayers)
    {
        if (pSession == player->m_pSession)
            continue;
        UnicastPacket(player->m_pSession, &header, Packet);
    }
    Packet->Clear();
    packetPool.Free(Packet);
}

void SC_SHOT_HIT_FOR_All(CSession* pSession, UINT32 playerId, UINT32 hp)
{
    game::SC_SHOT_HIT pkt;

    pkt.set_playerid(playerId);
    pkt.set_hp(hp);

    int pktSize = pkt.ByteSizeLong();

    PACKET_HEADER header;
    header.byCode = dfNETWORK_PACKET_CODE;
    header.bySize = pktSize;
    header.byType = game::PacketID::SC_ShotHit;

    int headerSize = sizeof(PACKET_HEADER);
    CPacket* Packet = packetPool.Alloc();

    char buffer[512];
    pkt.SerializeToArray(buffer, pktSize);
    Packet->PutData(buffer, pktSize);

    BroadcastData(pSession, Packet, Packet->GetDataSize());
    Packet->Clear();
    packetPool.Free(Packet);
}

void SC_SHOT_HIT_FOR_SINGLE(CSession* pSession, UINT32 playerId, UINT32 hp)
{
    game::SC_SHOT_HIT pkt;

    pkt.set_playerid(playerId);
    pkt.set_hp(hp);

    int pktSize = pkt.ByteSizeLong();

    PACKET_HEADER header;
    header.byCode = dfNETWORK_PACKET_CODE;
    header.bySize = pktSize;
    header.byType = game::PacketID::SC_ShotHit;

    int headerSize = sizeof(PACKET_HEADER);
    CPacket* Packet = packetPool.Alloc();

    char buffer[512];
    pkt.SerializeToArray(buffer, pktSize);
    Packet->PutData(buffer, pktSize);

    UnicastPacket(pSession, &header, Packet);
    Packet->Clear();
    packetPool.Free(Packet);
}

void SC_SHOT_HIT_FOR_AROUND(CSession* pSession, CRoom* pRoom, UINT32 playerId, UINT32 hp)
{
    game::SC_SHOT_HIT pkt;

    pkt.set_playerid(playerId);
    pkt.set_hp(hp);

    int pktSize = pkt.ByteSizeLong();

    PACKET_HEADER header;
    header.byCode = dfNETWORK_PACKET_CODE;
    header.bySize = pktSize;
    header.byType = game::PacketID::SC_ShotHit;

    int headerSize = sizeof(PACKET_HEADER);
    CPacket* Packet = packetPool.Alloc();

    char buffer[512];
    pkt.SerializeToArray(buffer, pktSize);
    Packet->PutData(buffer, pktSize);

    for (auto& player : pRoom->m_activePlayers)
    {
        if (pSession == player->m_pSession)
            continue;
        UnicastPacket(player->m_pSession, &header, Packet);
    }
    Packet->Clear();
    packetPool.Free(Packet);
}
