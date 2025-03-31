#include "pch.h"
#include "PacketProc.h"
#include "Packet.h"
#include "MakePacket.h"

#include "SessionManager.h"

#include "ObjectManager.h"

#include "Player.h"

#include "MemoryPoolManager.h"
#include "TimerManager.h"
#include "LogManager.h"
#include "RoomManager.h"

#include "Protobuf/Protocol.pb.h"

static CObjectManager& objectManager = CObjectManager::GetInstance();
static CTimerManager& timerManager = CTimerManager::GetInstance();
static LogManager& logManager = LogManager::GetInstance();
static CRoomManager& roomManager = CRoomManager::GetInstance();

int g_iSyncCount = 0;

bool PacketProc(CSession* pSession, game::PacketID packetType, CPacket* pPacket)
{
    switch (packetType)
    {
    case game::PacketID::CS_Attack:
    {
        bool bAttack;
        UINT32 normalX;
        UINT32 normalY;
        UINT32 normalZ;
        UINT32 posX;
        UINT32 posY;
        UINT32 posZ;

        game::CS_ATTACK pkt;
        pkt.ParseFromArray(pPacket->GetBufferPtr(), pPacket->GetDataSize());

        bAttack = pkt.battack();
        normalX = pkt.normalx();
        normalY = pkt.normaly();
        normalZ = pkt.normalz();
        posX = pkt.posx();
        posY = pkt.posy();
        posZ = pkt.posz();

        return CS_ATTACK(pSession, bAttack, normalX, normalY, normalZ, posX, posY, posZ);
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
    case game::PacketID::CS_ItemPicked:
    {
        UINT32 itemId;

        game::CS_ITEM_PICKED pkt;
        pkt.ParseFromArray(pPacket->GetBufferPtr(), pPacket->GetDataSize());

        itemId = pkt.itemid();

        return CS_ITEM_PICKED(pSession, itemId);
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
    case game::PacketID::CS_RequestRestart:
    {
        UINT32 playerId;
        UINT32 weapon;

        game::CS_REQUEST_RESTART pkt;
        pkt.ParseFromArray(pPacket->GetBufferPtr(), pPacket->GetDataSize());

        playerId = pkt.playerid();
        weapon = pkt.weapon();

        return CS_REQUEST_RESTART(pSession, playerId, weapon);
    }
    break;
    case game::PacketID::CS_SendNickname:
    {
        std::string name;

        game::CS_SEND_NICKNAME pkt;
        pkt.ParseFromArray(pPacket->GetBufferPtr(), pPacket->GetDataSize());

        name = pkt.name();

        return CS_SEND_NICKNAME(pSession, name);
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
    // 연결이 끊겼다는 것은 세션 접속을 해제해야한다는 의미. 추후 관련 처리

    return;
}

bool CS_ATTACK(CSession* pSession, bool bAttack, UINT32 normalX, UINT32 normalY, UINT32 normalZ, UINT32 posX, UINT32 posY, UINT32 posZ)
{
    // 클라이언트가 총을 쐈을 때 보내지는 패킷. 방의 모든 플레이어들에게 전송해야한다.

    // 1. 연결된 플레이어 추출
    CPlayer* pPlayer = static_cast<CPlayer*>(pSession->pObj);

    // 2. 방 정보 검색
    CRoom* pRoom = roomManager.GetRoomById(pPlayer->GetRoomId());

    // 3. 방에 있는 모든 플레이어들에게 패킷 전송. 단, 자기 자신은 제외한다.
    for (const auto& activePlayer : pRoom->GetActivePlayers())
    {
        // CS_POS_INTERPOLATION를 클라이언트에서 서버로 보낸다는 것은 activePlayer라는 의미
        if (activePlayer == pPlayer)
            continue;

        SC_POS_INTERPOLATION_FOR_SINGLE(activePlayer->m_pSession, posX, posY, posZ);
    }
    for (const auto& waitingPlayer : pRoom->GetWaitingPlayers())
    {
        SC_POS_INTERPOLATION_FOR_SINGLE(waitingPlayer->m_pSession, posX, posY, posZ);
    }
    return true;
}

bool CS_CHANGE_WEAPON(CSession* pSession, UINT32 weapon)
{
    // 무기를 교체한다는 의미

    // 1. 연결된 플레이어 추출
    CPlayer* pPlayer = static_cast<CPlayer*>(pSession->pObj);

    // 2. 방 정보 검색
    CRoom* pRoom = roomManager.GetRoomById(pPlayer->GetRoomId());

    // 3. 방에 있는 모든 플레이어들에게 패킷 전송. 단, 자기자신은 제외
    for (const auto& activePlayer : pRoom->GetActivePlayers())
    {
        SC_CHANGE_WEAPON_FOR_SINGLE(activePlayer->m_pSession, pPlayer->m_ID, weapon);
    }
    for (const auto& waitingPlayer : pRoom->GetWaitingPlayers())
    {
        SC_CHANGE_WEAPON_FOR_SINGLE(waitingPlayer->m_pSession, pPlayer->m_ID, weapon);
    }

    return true;
}

bool CS_GRENADE_EXPLOSITION_POS(CSession* pSession, UINT32 posX, UINT32 posY, UINT32 posZ)
{
    // 아직 작업 x

    return false;
}

bool CS_ITEM_PICKED(CSession* pSession, UINT32 itemId)
{
    // 클라이언트가 아이템을 주웠을 때 전송하는 패킷

    // 1. 연결된 플레이어 추출
    CPlayer* pPlayer = static_cast<CPlayer*>(pSession->pObj);

    // 2. 방 정보 검색
    CRoom* pRoom = roomManager.GetRoomById(pPlayer->GetRoomId());
    
    // 3. 아이템 획득 요청 시도
    if (pRoom->OnItemPickupRequest(pPlayer->m_ID, itemId))
    {
        // 획득 성공
        SC_ITEM_PICK_SUCCESS_FOR_SINGLE(pSession, pPlayer->m_ID, itemId);
    }
    else
    {
        // 획득 실패
        SC_ITEM_PICK_FAIL_FOR_SINGLE(pSession, pPlayer->m_ID, itemId);
    }

    return true;
}

bool CS_KEY_INPUT(CSession* pSession, UINT32 keyW, UINT32 keyA, UINT32 keyS, UINT32 keyD, UINT32 rotateAxisX, UINT32 rotateAxisY, UINT32 Jump)
{
    // KeyInput 정보는 자신을 포함한 같은 방에 있는 모든 이들에게 전송한다.

    // 1. 연결된 플레이어 추출
    CPlayer* pPlayer = static_cast<CPlayer*>(pSession->pObj);

    // 2. 자기 자신에게 입력된 키 정보, 회전 정보 대입
    std::array<bool, (int)PRESS_KEY::END> keyStates{};

    keyStates[(int)PRESS_KEY::W] = keyW;
    keyStates[(int)PRESS_KEY::A] = keyA;
    keyStates[(int)PRESS_KEY::S] = keyS;
    keyStates[(int)PRESS_KEY::D] = keyD;
    keyStates[(int)PRESS_KEY::JUMP] = Jump;
    pPlayer->SetMovementKeys(keyStates);

    pPlayer->SetRotationAxisXY(rotateAxisX, rotateAxisY);

    // 3. 방 정보 검색
    CRoom* pRoom = roomManager.GetRoomById(pPlayer->GetRoomId());

    // 4. 방에 있는 모든 플레이어들에게 패킷 전송
    for (const auto& activePlayer : pRoom->GetActivePlayers())
    {
        SC_KEY_INPUT_FOR_SINGLE(activePlayer->m_pSession, pPlayer->m_ID, keyW, keyA, keyS, keyD, rotateAxisX, rotateAxisY, Jump);
    }
    for (const auto& waitingPlayer : pRoom->GetWaitingPlayers())
    {
        SC_KEY_INPUT_FOR_SINGLE(waitingPlayer->m_pSession, pPlayer->m_ID, keyW, keyA, keyS, keyD, rotateAxisX, rotateAxisY, Jump);
    }

    return true;
}

bool CS_POS_INTERPOLATION(CSession* pSession, UINT32 posX, UINT32 posY, UINT32 posZ)
{
    // 위치 정보는 방에 있는 모든 플레이어들에게 전송되어야한다.

    // 1. 연결된 플레이어 추출
    CPlayer* pPlayer = static_cast<CPlayer*>(pSession->pObj);

    // 2. 방 정보 검색
    CRoom* pRoom = roomManager.GetRoomById(pPlayer->GetRoomId());

    // 3. 방에 있는 모든 플레이어들에게 패킷 전송. 단, 자기 자신은 제외한다.
    for (const auto& activePlayer : pRoom->GetActivePlayers())
    {
        // CS_POS_INTERPOLATION를 클라이언트에서 서버로 보낸다는 것은 activePlayer라는 의미
        if (activePlayer == pPlayer)
            continue;

        SC_POS_INTERPOLATION_FOR_SINGLE(activePlayer->m_pSession, posX, posY, posZ);
    }
    for (const auto& waitingPlayer : pRoom->GetWaitingPlayers())
    {
        SC_POS_INTERPOLATION_FOR_SINGLE(waitingPlayer->m_pSession, posX, posY, posZ);
    }

    return true;
}

bool CS_REQUEST_RESTART(CSession* pSession, UINT32 playerId, UINT32 weapon)
{
    // 재시작 요청 패킷
    
    // 현재 플레이어는 Room의 waiting에 속해 있고, 재시작 요청시 active로 이동한다.
    
    // 1. 연결된 플레이어 추출
    CPlayer* pPlayer = static_cast<CPlayer*>(pSession->pObj);

    // 2. waiting에서 active로 이동
    CRoom* pRoom = roomManager.GetRoomById(pPlayer->GetRoomId());
    pRoom->MoveToActive(pPlayer->m_ID);

    // 3. 서버에서 클라이언트에게 자신을 생성하라는 메시지 전송
    SC_CREATE_MY_CHARACTER_FOR_SINGLE(pSession, pPlayer->m_ID,
        0,                      // 원래 팀 id에 따라서 번호를 부여해야하지만 일단 0으로 부여
        0, 0, 1,                // 원래 이것도 팀에 따라서 방향 벡터를 부여해야하지만 일단 +z 단위벡터 부여
        100,                    // 최대 HP
        pPlayer->GetTeamId()    // 팀 ID
    );

    return false;
}

bool CS_SEND_NICKNAME(CSession* pSession, std::string name)
{
    // 닉네임이 왔다는 것은 이제 waiting에서 active로 옮긴다는 의미

    // 1. 연결된 플레이어 추출
    CPlayer* pPlayer = static_cast<CPlayer*>(pSession->pObj);

    // 2. 이름 부여
    pPlayer->SetName(name);

    // 3. waiting에서 active로 이동
    CRoom* pRoom = roomManager.GetRoomById(pPlayer->GetRoomId());
    pRoom->MoveToActive(pPlayer->m_ID);

    // 4. 서버에서 클라이언트에게 자신을 생성하라는 메시지 전송
    SC_CREATE_MY_CHARACTER_FOR_SINGLE(pSession, pPlayer->m_ID,
        0,                      // 원래 팀 id에 따라서 번호를 부여해야하지만 일단 0으로 부여
        0, 0, 1,                // 원래 이것도 팀에 따라서 방향 벡터를 부여해야하지만 일단 +z 단위벡터 부여
        100,                    // 최대 HP
        pPlayer->GetTeamId()    // 팀 ID
    );

    return true;
}

bool CS_SHOT_HIT(CSession* pSession, UINT32 playerId, UINT32 hp)
{
    // 클라이언트 쪽에서 서버에게 데미지를 준 플레이어 정보를 전송

    // 1. 연결된 플레이어 추출
    CPlayer* pPlayer = static_cast<CPlayer*>(pSession->pObj);

    // 2. 방 정보 추출
    CRoom* pRoom = roomManager.GetRoomById(pPlayer->GetRoomId());

    // 3. 타겟 플레이어 정보 추출
    CPlayer* pTargetPlayer = pRoom->FindPlayerById(playerId);
    
    // 타겟 플레이어가 존재한다면
    if (!pTargetPlayer)
    {
        // 체력이 0 보다 작다면
        if (hp <= 0)
        {
            // 플레이어는 죽고, active에서 waiting 상태로 변경됨
            pRoom->MoveToWaiting(playerId);

            // 플레이어가 다운되었음을 방의 모든 플레이어들에게 전송
            for (const auto& activePlayer : pRoom->GetActivePlayers())
            {
                SC_CHARACTER_DOWN_FOR_SINGLE(activePlayer->m_pSession, playerId, pTargetPlayer->GetTeamId());
            }
            for (const auto& waitingPlayer : pRoom->GetWaitingPlayers())
            {
                SC_CHARACTER_DOWN_FOR_SINGLE(waitingPlayer->m_pSession, playerId, pTargetPlayer->GetTeamId());
            }
        }
        else
        {
            // 해당 플레이어에게 데미지 부여
            pTargetPlayer->SetCurHp(hp);

            // 관련 정보를 방의 모든 플레이어들에게 전송
            for (const auto& activePlayer : pRoom->GetActivePlayers())
            {
                SC_SHOT_HIT_FOR_SINGLE(activePlayer->m_pSession, playerId, hp);
            }
            for (const auto& waitingPlayer : pRoom->GetWaitingPlayers())
            {
                SC_SHOT_HIT_FOR_SINGLE(waitingPlayer->m_pSession, playerId, hp);
            }
        }
    }

    return true;
}

bool CS_THROW_GRENADE(CSession* pSession, UINT32 posX, UINT32 posY, UINT32 posZ, UINT32 dirX, UINT32 dirY, UINT32 dirZ)
{
    // 아직 작업 x

    return false;
}
