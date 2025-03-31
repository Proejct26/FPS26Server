#include "pch.h"
#include "MakePacket.h"
#include "SessionManager.h"
#include "Packet.h"
#include "Player.h"
#include "Room.h"
#include "MemoryPoolManager.h"
#include "Protobuf/Protocol.pb.h"

void SC_ATTACK_FOR_All(CSession* pSession, UINT32 playerId)
{
    game::SC_ATTACK pkt;

    pkt.set_playerid(playerId);

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

void SC_ATTACK_FOR_SINGLE(CSession* pSession, UINT32 playerId)
{
    game::SC_ATTACK pkt;

    pkt.set_playerid(playerId);

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

void SC_ATTACK_FOR_AROUND(CSession* pSession, CRoom* pRoom, UINT32 playerId)
{
    game::SC_ATTACK pkt;

    pkt.set_playerid(playerId);

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

    for (auto& player : pRoom->GetPlayers())
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

    for (auto& player : pRoom->GetPlayers())
    {
        if (pSession == player->m_pSession)
            continue;
        UnicastPacket(player->m_pSession, &header, Packet);
    }
    Packet->Clear();
    packetPool.Free(Packet);
}

void SC_CHARACTER_DOWN_FOR_All(CSession* pSession, UINT32 playerId)
{
    game::SC_CHARACTER_DOWN pkt;

    pkt.set_playerid(playerId);

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

void SC_CHARACTER_DOWN_FOR_SINGLE(CSession* pSession, UINT32 playerId)
{
    game::SC_CHARACTER_DOWN pkt;

    pkt.set_playerid(playerId);

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

void SC_CHARACTER_DOWN_FOR_AROUND(CSession* pSession, CRoom* pRoom, UINT32 playerId)
{
    game::SC_CHARACTER_DOWN pkt;

    pkt.set_playerid(playerId);

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

    for (auto& player : pRoom->GetPlayers())
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

    for (auto& player : pRoom->GetPlayers())
    {
        if (pSession == player->m_pSession)
            continue;
        UnicastPacket(player->m_pSession, &header, Packet);
    }
    Packet->Clear();
    packetPool.Free(Packet);
}

void SC_CREATE_MY_CHARACTER_FOR_All(CSession* pSession, UINT32 playerId, UINT32 posX, UINT32 posY, UINT32 posZ, UINT32 dirX, UINT32 dirY, UINT32 dirZ, UINT32 maxHP)
{
    game::SC_CREATE_MY_CHARACTER pkt;

    pkt.set_playerid(playerId);
    pkt.set_posx(posX);
    pkt.set_posy(posY);
    pkt.set_posz(posZ);
    pkt.set_dirx(dirX);
    pkt.set_diry(dirY);
    pkt.set_dirz(dirZ);
    pkt.set_maxhp(maxHP);

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

void SC_CREATE_MY_CHARACTER_FOR_SINGLE(CSession* pSession, UINT32 playerId, UINT32 posX, UINT32 posY, UINT32 posZ, UINT32 dirX, UINT32 dirY, UINT32 dirZ, UINT32 maxHP)
{
    game::SC_CREATE_MY_CHARACTER pkt;

    pkt.set_playerid(playerId);
    pkt.set_posx(posX);
    pkt.set_posy(posY);
    pkt.set_posz(posZ);
    pkt.set_dirx(dirX);
    pkt.set_diry(dirY);
    pkt.set_dirz(dirZ);
    pkt.set_maxhp(maxHP);

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

void SC_CREATE_MY_CHARACTER_FOR_AROUND(CSession* pSession, CRoom* pRoom, UINT32 playerId, UINT32 posX, UINT32 posY, UINT32 posZ, UINT32 dirX, UINT32 dirY, UINT32 dirZ, UINT32 maxHP)
{
    game::SC_CREATE_MY_CHARACTER pkt;

    pkt.set_playerid(playerId);
    pkt.set_posx(posX);
    pkt.set_posy(posY);
    pkt.set_posz(posZ);
    pkt.set_dirx(dirX);
    pkt.set_diry(dirY);
    pkt.set_dirz(dirZ);
    pkt.set_maxhp(maxHP);

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

    for (auto& player : pRoom->GetPlayers())
    {
        if (pSession == player->m_pSession)
            continue;
        UnicastPacket(player->m_pSession, &header, Packet);
    }
    Packet->Clear();
    packetPool.Free(Packet);
}

void SC_CREATE_OTHER_CHARACTER_FOR_All(CSession* pSession, UINT32 playerId, UINT32 posX, UINT32 posY, UINT32 posZ, UINT32 dirX, UINT32 dirY, UINT32 dirZ, UINT32 maxHP, UINT32 curHP, std::string name, KDAInfo kdaInfo)
{
    game::SC_CREATE_OTHER_CHARACTER pkt;

    pkt.set_playerid(playerId);
    pkt.set_posx(posX);
    pkt.set_posy(posY);
    pkt.set_posz(posZ);
    pkt.set_dirx(dirX);
    pkt.set_diry(dirY);
    pkt.set_dirz(dirZ);
    pkt.set_maxhp(maxHP);
    pkt.set_curhp(curHP);
    pkt.set_name(name);
    {
        game::KDAInfo* sub = pkt.mutable_kdainfo();
        sub->set_kill(kdaInfo.kill);
        sub->set_death(kdaInfo.death);
        sub->set_assist(kdaInfo.assist);
    }

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

void SC_CREATE_OTHER_CHARACTER_FOR_SINGLE(CSession* pSession, UINT32 playerId, UINT32 posX, UINT32 posY, UINT32 posZ, UINT32 dirX, UINT32 dirY, UINT32 dirZ, UINT32 maxHP, UINT32 curHP, std::string name, KDAInfo kdaInfo)
{
    game::SC_CREATE_OTHER_CHARACTER pkt;

    pkt.set_playerid(playerId);
    pkt.set_posx(posX);
    pkt.set_posy(posY);
    pkt.set_posz(posZ);
    pkt.set_dirx(dirX);
    pkt.set_diry(dirY);
    pkt.set_dirz(dirZ);
    pkt.set_maxhp(maxHP);
    pkt.set_curhp(curHP);
    pkt.set_name(name);
    {
        game::KDAInfo* sub = pkt.mutable_kdainfo();
        sub->set_kill(kdaInfo.kill);
        sub->set_death(kdaInfo.death);
        sub->set_assist(kdaInfo.assist);
    }

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

void SC_CREATE_OTHER_CHARACTER_FOR_AROUND(CSession* pSession, CRoom* pRoom, UINT32 playerId, UINT32 posX, UINT32 posY, UINT32 posZ, UINT32 dirX, UINT32 dirY, UINT32 dirZ, UINT32 maxHP, UINT32 curHP, std::string name, KDAInfo kdaInfo)
{
    game::SC_CREATE_OTHER_CHARACTER pkt;

    pkt.set_playerid(playerId);
    pkt.set_posx(posX);
    pkt.set_posy(posY);
    pkt.set_posz(posZ);
    pkt.set_dirx(dirX);
    pkt.set_diry(dirY);
    pkt.set_dirz(dirZ);
    pkt.set_maxhp(maxHP);
    pkt.set_curhp(curHP);
    pkt.set_name(name);
    {
        game::KDAInfo* sub = pkt.mutable_kdainfo();
        sub->set_kill(kdaInfo.kill);
        sub->set_death(kdaInfo.death);
        sub->set_assist(kdaInfo.assist);
    }

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

    for (auto& player : pRoom->GetPlayers())
    {
        if (pSession == player->m_pSession)
            continue;
        UnicastPacket(player->m_pSession, &header, Packet);
    }
    Packet->Clear();
    packetPool.Free(Packet);
}

void SC_GRENADEEXPLOSITIONPOS_FOR_All(CSession* pSession, UINT32 posX, UINT32 posY, UINT32 posZ)
{
    game::SC_GRENADEEXPLOSITIONPOS pkt;

    pkt.set_posx(posX);
    pkt.set_posy(posY);
    pkt.set_posz(posZ);

    int pktSize = pkt.ByteSizeLong();

    PACKET_HEADER header;
    header.byCode = dfNETWORK_PACKET_CODE;
    header.bySize = pktSize;
    header.byType = game::PacketID::SC_Grenadeexplositionpos;

    int headerSize = sizeof(PACKET_HEADER);
    CPacket* Packet = packetPool.Alloc();

    char buffer[512];
    pkt.SerializeToArray(buffer, pktSize);
    Packet->PutData(buffer, pktSize);

    BroadcastData(pSession, Packet, Packet->GetDataSize());
    Packet->Clear();
    packetPool.Free(Packet);
}

void SC_GRENADEEXPLOSITIONPOS_FOR_SINGLE(CSession* pSession, UINT32 posX, UINT32 posY, UINT32 posZ)
{
    game::SC_GRENADEEXPLOSITIONPOS pkt;

    pkt.set_posx(posX);
    pkt.set_posy(posY);
    pkt.set_posz(posZ);

    int pktSize = pkt.ByteSizeLong();

    PACKET_HEADER header;
    header.byCode = dfNETWORK_PACKET_CODE;
    header.bySize = pktSize;
    header.byType = game::PacketID::SC_Grenadeexplositionpos;

    int headerSize = sizeof(PACKET_HEADER);
    CPacket* Packet = packetPool.Alloc();

    char buffer[512];
    pkt.SerializeToArray(buffer, pktSize);
    Packet->PutData(buffer, pktSize);

    UnicastPacket(pSession, &header, Packet);
    Packet->Clear();
    packetPool.Free(Packet);
}

void SC_GRENADEEXPLOSITIONPOS_FOR_AROUND(CSession* pSession, CRoom* pRoom, UINT32 posX, UINT32 posY, UINT32 posZ)
{
    game::SC_GRENADEEXPLOSITIONPOS pkt;

    pkt.set_posx(posX);
    pkt.set_posy(posY);
    pkt.set_posz(posZ);

    int pktSize = pkt.ByteSizeLong();

    PACKET_HEADER header;
    header.byCode = dfNETWORK_PACKET_CODE;
    header.bySize = pktSize;
    header.byType = game::PacketID::SC_Grenadeexplositionpos;

    int headerSize = sizeof(PACKET_HEADER);
    CPacket* Packet = packetPool.Alloc();

    char buffer[512];
    pkt.SerializeToArray(buffer, pktSize);
    Packet->PutData(buffer, pktSize);

    for (auto& player : pRoom->GetPlayers())
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

    for (auto& player : pRoom->GetPlayers())
    {
        if (pSession == player->m_pSession)
            continue;
        UnicastPacket(player->m_pSession, &header, Packet);
    }
    Packet->Clear();
    packetPool.Free(Packet);
}

void SC_POS_INTERPOLATION_FOR_All(CSession* pSession, UINT32 posX, UINT32 posY, UINT32 posZ)
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

void SC_POS_INTERPOLATION_FOR_SINGLE(CSession* pSession, UINT32 posX, UINT32 posY, UINT32 posZ)
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

void SC_POS_INTERPOLATION_FOR_AROUND(CSession* pSession, CRoom* pRoom, UINT32 posX, UINT32 posY, UINT32 posZ)
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

    for (auto& player : pRoom->GetPlayers())
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

    for (auto& player : pRoom->GetPlayers())
    {
        if (pSession == player->m_pSession)
            continue;
        UnicastPacket(player->m_pSession, &header, Packet);
    }
    Packet->Clear();
    packetPool.Free(Packet);
}

void SC_THROW_GRENADE_FOR_All(CSession* pSession, UINT32 posX, UINT32 posY, UINT32 posZ, UINT32 dirX, UINT32 dirY, UINT32 dirZ)
{
    game::SC_THROW_GRENADE pkt;

    pkt.set_posx(posX);
    pkt.set_posy(posY);
    pkt.set_posz(posZ);
    pkt.set_dirx(dirX);
    pkt.set_diry(dirY);
    pkt.set_dirz(dirZ);

    int pktSize = pkt.ByteSizeLong();

    PACKET_HEADER header;
    header.byCode = dfNETWORK_PACKET_CODE;
    header.bySize = pktSize;
    header.byType = game::PacketID::SC_ThrowGrenade;

    int headerSize = sizeof(PACKET_HEADER);
    CPacket* Packet = packetPool.Alloc();

    char buffer[512];
    pkt.SerializeToArray(buffer, pktSize);
    Packet->PutData(buffer, pktSize);

    BroadcastData(pSession, Packet, Packet->GetDataSize());
    Packet->Clear();
    packetPool.Free(Packet);
}

void SC_THROW_GRENADE_FOR_SINGLE(CSession* pSession, UINT32 posX, UINT32 posY, UINT32 posZ, UINT32 dirX, UINT32 dirY, UINT32 dirZ)
{
    game::SC_THROW_GRENADE pkt;

    pkt.set_posx(posX);
    pkt.set_posy(posY);
    pkt.set_posz(posZ);
    pkt.set_dirx(dirX);
    pkt.set_diry(dirY);
    pkt.set_dirz(dirZ);

    int pktSize = pkt.ByteSizeLong();

    PACKET_HEADER header;
    header.byCode = dfNETWORK_PACKET_CODE;
    header.bySize = pktSize;
    header.byType = game::PacketID::SC_ThrowGrenade;

    int headerSize = sizeof(PACKET_HEADER);
    CPacket* Packet = packetPool.Alloc();

    char buffer[512];
    pkt.SerializeToArray(buffer, pktSize);
    Packet->PutData(buffer, pktSize);

    UnicastPacket(pSession, &header, Packet);
    Packet->Clear();
    packetPool.Free(Packet);
}

void SC_THROW_GRENADE_FOR_AROUND(CSession* pSession, CRoom* pRoom, UINT32 posX, UINT32 posY, UINT32 posZ, UINT32 dirX, UINT32 dirY, UINT32 dirZ)
{
    game::SC_THROW_GRENADE pkt;

    pkt.set_posx(posX);
    pkt.set_posy(posY);
    pkt.set_posz(posZ);
    pkt.set_dirx(dirX);
    pkt.set_diry(dirY);
    pkt.set_dirz(dirZ);

    int pktSize = pkt.ByteSizeLong();

    PACKET_HEADER header;
    header.byCode = dfNETWORK_PACKET_CODE;
    header.bySize = pktSize;
    header.byType = game::PacketID::SC_ThrowGrenade;

    int headerSize = sizeof(PACKET_HEADER);
    CPacket* Packet = packetPool.Alloc();

    char buffer[512];
    pkt.SerializeToArray(buffer, pktSize);
    Packet->PutData(buffer, pktSize);

    for (auto& player : pRoom->GetPlayers())
    {
        if (pSession == player->m_pSession)
            continue;
        UnicastPacket(player->m_pSession, &header, Packet);
    }
    Packet->Clear();
    packetPool.Free(Packet);
}
