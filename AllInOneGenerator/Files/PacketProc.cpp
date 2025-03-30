#include "pch.h"
#include "PacketProc.h"
#include "Packet.h"
#include "MakePacket.h"

#include "SessionManager.h"

#include "ObjectManager.h"

#include "SectorManager.h"
#include "Sector.h"

#include "Player.h"

#include "MemoryPoolManager.h"
#include "TimerManager.h"
#include "LogManager.h"

#include "Protobuf/Protocol.pb.h"

static CSectorManager& sectorManager = CSectorManager::GetInstance();
static CObjectManager& objectManager = CObjectManager::GetInstance();
static CTimerManager& timerManager = CTimerManager::GetInstance();
static LogManager& logManager = LogManager::GetInstance();

int g_iSyncCount = 0;

bool PacketProc(CSession* pSession, game::PacketID packetType, CPacket* pPacket)
{
    switch (packetType)
    {
    case game::PacketID::CS_Attack:
    {
        bool bAttack;

        game::CS_ATTACK pkt;
        pkt.ParseFromArray(pPacket->GetBufferPtr(), pPacket->GetDataSize());

        bAttack = pkt.battack();

        return CS_ATTACK(pSession, bAttack);
    }
    break;
    case game::PacketID::CS_ChangeWeapon:
    {
        UINT32 weapon;

        game::CS_CHANGE_WEAPON pkt;
        pkt.ParseFromArray(pPacket->GetBufferPtr(), pPacket->GetDataSize());

        weapon = pkt.weapon();

        return CS_CHANGE_WEAPON(pSession, weapon);
    }
    break;
    case game::PacketID::CS_GrenadeExplositionPos:
    {
        UINT32 posX;
        UINT32 posY;
        UINT32 posZ;

        game::CS_GRENADE_EXPLOSITION_POS pkt;
        pkt.ParseFromArray(pPacket->GetBufferPtr(), pPacket->GetDataSize());

        posX = pkt.posx();
        posY = pkt.posy();
        posZ = pkt.posz();

        return CS_GRENADE_EXPLOSITION_POS(pSession, posX, posY, posZ);
    }
    break;
    case game::PacketID::CS_KeyInput:
    {
        UINT32 keyW;
        UINT32 keyA;
        UINT32 keyS;
        UINT32 keyD;
        UINT32 rotateAxisX;
        UINT32 rotateAxisY;
        UINT32 Jump;

        game::CS_KEY_INPUT pkt;
        pkt.ParseFromArray(pPacket->GetBufferPtr(), pPacket->GetDataSize());

        keyW = pkt.keyw();
        keyA = pkt.keya();
        keyS = pkt.keys();
        keyD = pkt.keyd();
        rotateAxisX = pkt.rotateaxisx();
        rotateAxisY = pkt.rotateaxisy();
        Jump = pkt.jump();

        return CS_KEY_INPUT(pSession, keyW, keyA, keyS, keyD, rotateAxisX, rotateAxisY, Jump);
    }
    break;
    case game::PacketID::CS_OnAccept:
    {
        std::string name;

        game::CS_ON_ACCEPT pkt;
        pkt.ParseFromArray(pPacket->GetBufferPtr(), pPacket->GetDataSize());

        name = pkt.name();

        return CS_ON_ACCEPT(pSession, name);
    }
    break;
    case game::PacketID::CS_PosInterpolation:
    {
        UINT32 posX;
        UINT32 posY;
        UINT32 posZ;

        game::CS_POS_INTERPOLATION pkt;
        pkt.ParseFromArray(pPacket->GetBufferPtr(), pPacket->GetDataSize());

        posX = pkt.posx();
        posY = pkt.posy();
        posZ = pkt.posz();

        return CS_POS_INTERPOLATION(pSession, posX, posY, posZ);
    }
    break;
    case game::PacketID::CS_ShotHit:
    {
        UINT32 playerId;
        UINT32 hp;

        game::CS_SHOT_HIT pkt;
        pkt.ParseFromArray(pPacket->GetBufferPtr(), pPacket->GetDataSize());

        playerId = pkt.playerid();
        hp = pkt.hp();

        return CS_SHOT_HIT(pSession, playerId, hp);
    }
    break;
    case game::PacketID::CS_ThrowGrenade:
    {
        UINT32 posX;
        UINT32 posY;
        UINT32 posZ;
        UINT32 dirX;
        UINT32 dirY;
        UINT32 dirZ;

        game::CS_THROW_GRENADE pkt;
        pkt.ParseFromArray(pPacket->GetBufferPtr(), pPacket->GetDataSize());

        posX = pkt.posx();
        posY = pkt.posy();
        posZ = pkt.posz();
        dirX = pkt.dirx();
        dirY = pkt.diry();
        dirZ = pkt.dirz();

        return CS_THROW_GRENADE(pSession, posX, posY, posZ, dirX, dirY, dirZ);
    }
    break;
    default:
        break;
    }
    return false;
}

void DisconnectSessionProc(CSession* pSession)
{
    return;
}
bool CS_ATTACK(CSession* pSession, bool bAttack)
{
    return false;
}

bool CS_CHANGE_WEAPON(CSession* pSession, UINT32 weapon)
{
    return false;
}

bool CS_GRENADE_EXPLOSITION_POS(CSession* pSession, UINT32 posX, UINT32 posY, UINT32 posZ)
{
    return false;
}

bool CS_KEY_INPUT(CSession* pSession, UINT32 keyW, UINT32 keyA, UINT32 keyS, UINT32 keyD, UINT32 rotateAxisX, UINT32 rotateAxisY, UINT32 Jump)
{
    return false;
}

bool CS_ON_ACCEPT(CSession* pSession, std::string name)
{
    return false;
}

bool CS_POS_INTERPOLATION(CSession* pSession, UINT32 posX, UINT32 posY, UINT32 posZ)
{
    return false;
}

bool CS_SHOT_HIT(CSession* pSession, UINT32 playerId, UINT32 hp)
{
    return false;
}

bool CS_THROW_GRENADE(CSession* pSession, UINT32 posX, UINT32 posY, UINT32 posZ, UINT32 dirX, UINT32 dirY, UINT32 dirZ)
{
    return false;
}
