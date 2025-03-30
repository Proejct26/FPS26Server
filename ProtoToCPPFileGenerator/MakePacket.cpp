#include "pch.h"
#include "MakePacket.h"
#include "SessionManager.h"
#include "Packet.h"
#include "Player.h"
#include "Sector.h"
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
    Packet->PutData((const char*)&header, headerSize);

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
    Packet->PutData((const char*)&header, headerSize);

    char buffer[512];
    pkt.SerializeToArray(buffer, pktSize);
    Packet->PutData(buffer, pktSize);

    UnicastPacket(pSession, &header, Packet);

    Packet->Clear();
    packetPool.Free(Packet);
}

void SC_ATTACK_FOR_AROUND(CSession* pSession, CSector* pSector, UINT32 playerId)
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
    Packet->PutData((const char*)&header, headerSize);

    char buffer[512];
    pkt.SerializeToArray(buffer, pktSize);
    Packet->PutData(buffer, pktSize);

    for (auto& Sector : pSector->GetAroundSectorList())
    {
        for (auto& Object : Sector->GetSectorObjectMap())
        {
            if (pSession == Object.second->m_pSession)
                continue;

            UnicastPacket(Object.second->m_pSession, &header, Packet);
        }
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
    Packet->PutData((const char*)&header, headerSize);

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
    Packet->PutData((const char*)&header, headerSize);

    char buffer[512];
    pkt.SerializeToArray(buffer, pktSize);
    Packet->PutData(buffer, pktSize);

    UnicastPacket(pSession, &header, Packet);

    Packet->Clear();
    packetPool.Free(Packet);
}

void SC_CHANGE_WEAPON_FOR_AROUND(CSession* pSession, CSector* pSector, UINT32 playerId, UINT32 weapon)
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
    Packet->PutData((const char*)&header, headerSize);

    char buffer[512];
    pkt.SerializeToArray(buffer, pktSize);
    Packet->PutData(buffer, pktSize);

    for (auto& Sector : pSector->GetAroundSectorList())
    {
        for (auto& Object : Sector->GetSectorObjectMap())
        {
            if (pSession == Object.second->m_pSession)
                continue;

            UnicastPacket(Object.second->m_pSession, &header, Packet);
        }
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
    Packet->PutData((const char*)&header, headerSize);

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
    Packet->PutData((const char*)&header, headerSize);

    char buffer[512];
    pkt.SerializeToArray(buffer, pktSize);
    Packet->PutData(buffer, pktSize);

    UnicastPacket(pSession, &header, Packet);

    Packet->Clear();
    packetPool.Free(Packet);
}

void SC_CHARACTER_DOWN_FOR_AROUND(CSession* pSession, CSector* pSector, UINT32 playerId)
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
    Packet->PutData((const char*)&header, headerSize);

    char buffer[512];
    pkt.SerializeToArray(buffer, pktSize);
    Packet->PutData(buffer, pktSize);

    for (auto& Sector : pSector->GetAroundSectorList())
    {
        for (auto& Object : Sector->GetSectorObjectMap())
        {
            if (pSession == Object.second->m_pSession)
                continue;

            UnicastPacket(Object.second->m_pSession, &header, Packet);
        }
    }

    Packet->Clear();
    packetPool.Free(Packet);
}

void SC_CHARACTER_KILL_LOG_FOR_All(CSession* pSession, )
{
    game::SC_CHARACTER_KILL_LOG pkt;

    

    int pktSize = pkt.ByteSizeLong();

    PACKET_HEADER header;
    header.byCode = dfNETWORK_PACKET_CODE;
    header.bySize = pktSize;
    header.byType = game::PacketID::SC_CharacterKillLog;

    int headerSize = sizeof(PACKET_HEADER);

    CPacket* Packet = packetPool.Alloc();
    Packet->PutData((const char*)&header, headerSize);

    char buffer[512];
    pkt.SerializeToArray(buffer, pktSize);
    Packet->PutData(buffer, pktSize);

    BroadcastData(pSession, Packet, Packet->GetDataSize());

    Packet->Clear();
    packetPool.Free(Packet);
}

void SC_CHARACTER_KILL_LOG_FOR_SINGLE(CSession* pSession, )
{
    game::SC_CHARACTER_KILL_LOG pkt;

    

    int pktSize = pkt.ByteSizeLong();

    PACKET_HEADER header;
    header.byCode = dfNETWORK_PACKET_CODE;
    header.bySize = pktSize;
    header.byType = game::PacketID::SC_CharacterKillLog;

    int headerSize = sizeof(PACKET_HEADER);

    CPacket* Packet = packetPool.Alloc();
    Packet->PutData((const char*)&header, headerSize);

    char buffer[512];
    pkt.SerializeToArray(buffer, pktSize);
    Packet->PutData(buffer, pktSize);

    UnicastPacket(pSession, &header, Packet);

    Packet->Clear();
    packetPool.Free(Packet);
}

void SC_CHARACTER_KILL_LOG_FOR_AROUND(CSession* pSession, CSector* pSector, )
{
    game::SC_CHARACTER_KILL_LOG pkt;

    

    int pktSize = pkt.ByteSizeLong();

    PACKET_HEADER header;
    header.byCode = dfNETWORK_PACKET_CODE;
    header.bySize = pktSize;
    header.byType = game::PacketID::SC_CharacterKillLog;

    int headerSize = sizeof(PACKET_HEADER);

    CPacket* Packet = packetPool.Alloc();
    Packet->PutData((const char*)&header, headerSize);

    char buffer[512];
    pkt.SerializeToArray(buffer, pktSize);
    Packet->PutData(buffer, pktSize);

    for (auto& Sector : pSector->GetAroundSectorList())
    {
        for (auto& Object : Sector->GetSectorObjectMap())
        {
            if (pSession == Object.second->m_pSession)
                continue;

            UnicastPacket(Object.second->m_pSession, &header, Packet);
        }
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
    Packet->PutData((const char*)&header, headerSize);

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
    Packet->PutData((const char*)&header, headerSize);

    char buffer[512];
    pkt.SerializeToArray(buffer, pktSize);
    Packet->PutData(buffer, pktSize);

    UnicastPacket(pSession, &header, Packet);

    Packet->Clear();
    packetPool.Free(Packet);
}

void SC_CREATE_MY_CHARACTER_FOR_AROUND(CSession* pSession, CSector* pSector, UINT32 playerId, UINT32 posX, UINT32 posY, UINT32 posZ, UINT32 dirX, UINT32 dirY, UINT32 dirZ, UINT32 maxHP)
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
    Packet->PutData((const char*)&header, headerSize);

    char buffer[512];
    pkt.SerializeToArray(buffer, pktSize);
    Packet->PutData(buffer, pktSize);

    for (auto& Sector : pSector->GetAroundSectorList())
    {
        for (auto& Object : Sector->GetSectorObjectMap())
        {
            if (pSession == Object.second->m_pSession)
                continue;

            UnicastPacket(Object.second->m_pSession, &header, Packet);
        }
    }

    Packet->Clear();
    packetPool.Free(Packet);
}

void SC_CREATE_OTHER_CHARACTER_FOR_All(CSession* pSession, UINT32 playerId, UINT32 posX, UINT32 posY, UINT32 posZ, UINT32 dirX, UINT32 dirY, UINT32 dirZ, UINT32 maxHP, UINT32 curHP, UINT32 name, UINT32 kdaInfo)
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
    pkt.set_kdainfo(kdaInfo);

    int pktSize = pkt.ByteSizeLong();

    PACKET_HEADER header;
    header.byCode = dfNETWORK_PACKET_CODE;
    header.bySize = pktSize;
    header.byType = game::PacketID::SC_CreateOtherCharacter;

    int headerSize = sizeof(PACKET_HEADER);

    CPacket* Packet = packetPool.Alloc();
    Packet->PutData((const char*)&header, headerSize);

    char buffer[512];
    pkt.SerializeToArray(buffer, pktSize);
    Packet->PutData(buffer, pktSize);

    BroadcastData(pSession, Packet, Packet->GetDataSize());

    Packet->Clear();
    packetPool.Free(Packet);
}

void SC_CREATE_OTHER_CHARACTER_FOR_SINGLE(CSession* pSession, UINT32 playerId, UINT32 posX, UINT32 posY, UINT32 posZ, UINT32 dirX, UINT32 dirY, UINT32 dirZ, UINT32 maxHP, UINT32 curHP, UINT32 name, UINT32 kdaInfo)
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
    pkt.set_kdainfo(kdaInfo);

    int pktSize = pkt.ByteSizeLong();

    PACKET_HEADER header;
    header.byCode = dfNETWORK_PACKET_CODE;
    header.bySize = pktSize;
    header.byType = game::PacketID::SC_CreateOtherCharacter;

    int headerSize = sizeof(PACKET_HEADER);

    CPacket* Packet = packetPool.Alloc();
    Packet->PutData((const char*)&header, headerSize);

    char buffer[512];
    pkt.SerializeToArray(buffer, pktSize);
    Packet->PutData(buffer, pktSize);

    UnicastPacket(pSession, &header, Packet);

    Packet->Clear();
    packetPool.Free(Packet);
}

void SC_CREATE_OTHER_CHARACTER_FOR_AROUND(CSession* pSession, CSector* pSector, UINT32 playerId, UINT32 posX, UINT32 posY, UINT32 posZ, UINT32 dirX, UINT32 dirY, UINT32 dirZ, UINT32 maxHP, UINT32 curHP, UINT32 name, UINT32 kdaInfo)
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
    pkt.set_kdainfo(kdaInfo);

    int pktSize = pkt.ByteSizeLong();

    PACKET_HEADER header;
    header.byCode = dfNETWORK_PACKET_CODE;
    header.bySize = pktSize;
    header.byType = game::PacketID::SC_CreateOtherCharacter;

    int headerSize = sizeof(PACKET_HEADER);

    CPacket* Packet = packetPool.Alloc();
    Packet->PutData((const char*)&header, headerSize);

    char buffer[512];
    pkt.SerializeToArray(buffer, pktSize);
    Packet->PutData(buffer, pktSize);

    for (auto& Sector : pSector->GetAroundSectorList())
    {
        for (auto& Object : Sector->GetSectorObjectMap())
        {
            if (pSession == Object.second->m_pSession)
                continue;

            UnicastPacket(Object.second->m_pSession, &header, Packet);
        }
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
    Packet->PutData((const char*)&header, headerSize);

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
    Packet->PutData((const char*)&header, headerSize);

    char buffer[512];
    pkt.SerializeToArray(buffer, pktSize);
    Packet->PutData(buffer, pktSize);

    UnicastPacket(pSession, &header, Packet);

    Packet->Clear();
    packetPool.Free(Packet);
}

void SC_GRENADEEXPLOSITIONPOS_FOR_AROUND(CSession* pSession, CSector* pSector, UINT32 posX, UINT32 posY, UINT32 posZ)
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
    Packet->PutData((const char*)&header, headerSize);

    char buffer[512];
    pkt.SerializeToArray(buffer, pktSize);
    Packet->PutData(buffer, pktSize);

    for (auto& Sector : pSector->GetAroundSectorList())
    {
        for (auto& Object : Sector->GetSectorObjectMap())
        {
            if (pSession == Object.second->m_pSession)
                continue;

            UnicastPacket(Object.second->m_pSession, &header, Packet);
        }
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
    Packet->PutData((const char*)&header, headerSize);

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
    Packet->PutData((const char*)&header, headerSize);

    char buffer[512];
    pkt.SerializeToArray(buffer, pktSize);
    Packet->PutData(buffer, pktSize);

    UnicastPacket(pSession, &header, Packet);

    Packet->Clear();
    packetPool.Free(Packet);
}

void SC_KEY_INPUT_FOR_AROUND(CSession* pSession, CSector* pSector, UINT32 playerId, UINT32 keyW, UINT32 keyA, UINT32 keyS, UINT32 keyD, UINT32 rotateAxisX, UINT32 rotateAxisY, UINT32 Jump)
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
    Packet->PutData((const char*)&header, headerSize);

    char buffer[512];
    pkt.SerializeToArray(buffer, pktSize);
    Packet->PutData(buffer, pktSize);

    for (auto& Sector : pSector->GetAroundSectorList())
    {
        for (auto& Object : Sector->GetSectorObjectMap())
        {
            if (pSession == Object.second->m_pSession)
                continue;

            UnicastPacket(Object.second->m_pSession, &header, Packet);
        }
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
    Packet->PutData((const char*)&header, headerSize);

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
    Packet->PutData((const char*)&header, headerSize);

    char buffer[512];
    pkt.SerializeToArray(buffer, pktSize);
    Packet->PutData(buffer, pktSize);

    UnicastPacket(pSession, &header, Packet);

    Packet->Clear();
    packetPool.Free(Packet);
}

void SC_POS_INTERPOLATION_FOR_AROUND(CSession* pSession, CSector* pSector, UINT32 posX, UINT32 posY, UINT32 posZ)
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
    Packet->PutData((const char*)&header, headerSize);

    char buffer[512];
    pkt.SerializeToArray(buffer, pktSize);
    Packet->PutData(buffer, pktSize);

    for (auto& Sector : pSector->GetAroundSectorList())
    {
        for (auto& Object : Sector->GetSectorObjectMap())
        {
            if (pSession == Object.second->m_pSession)
                continue;

            UnicastPacket(Object.second->m_pSession, &header, Packet);
        }
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
    Packet->PutData((const char*)&header, headerSize);

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
    Packet->PutData((const char*)&header, headerSize);

    char buffer[512];
    pkt.SerializeToArray(buffer, pktSize);
    Packet->PutData(buffer, pktSize);

    UnicastPacket(pSession, &header, Packet);

    Packet->Clear();
    packetPool.Free(Packet);
}

void SC_SHOT_HIT_FOR_AROUND(CSession* pSession, CSector* pSector, UINT32 playerId, UINT32 hp)
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
    Packet->PutData((const char*)&header, headerSize);

    char buffer[512];
    pkt.SerializeToArray(buffer, pktSize);
    Packet->PutData(buffer, pktSize);

    for (auto& Sector : pSector->GetAroundSectorList())
    {
        for (auto& Object : Sector->GetSectorObjectMap())
        {
            if (pSession == Object.second->m_pSession)
                continue;

            UnicastPacket(Object.second->m_pSession, &header, Packet);
        }
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
    Packet->PutData((const char*)&header, headerSize);

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
    Packet->PutData((const char*)&header, headerSize);

    char buffer[512];
    pkt.SerializeToArray(buffer, pktSize);
    Packet->PutData(buffer, pktSize);

    UnicastPacket(pSession, &header, Packet);

    Packet->Clear();
    packetPool.Free(Packet);
}

void SC_THROW_GRENADE_FOR_AROUND(CSession* pSession, CSector* pSector, UINT32 posX, UINT32 posY, UINT32 posZ, UINT32 dirX, UINT32 dirY, UINT32 dirZ)
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
    Packet->PutData((const char*)&header, headerSize);

    char buffer[512];
    pkt.SerializeToArray(buffer, pktSize);
    Packet->PutData(buffer, pktSize);

    for (auto& Sector : pSector->GetAroundSectorList())
    {
        for (auto& Object : Sector->GetSectorObjectMap())
        {
            if (pSession == Object.second->m_pSession)
                continue;

            UnicastPacket(Object.second->m_pSession, &header, Packet);
        }
    }

    Packet->Clear();
    packetPool.Free(Packet);
}
