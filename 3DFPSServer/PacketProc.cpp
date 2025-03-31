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
    // ������ ����ٴ� ���� ���� ������ �����ؾ��Ѵٴ� �ǹ�. ���� ���� ó��

    return;
}

bool CS_ATTACK(CSession* pSession, bool bAttack, UINT32 normalX, UINT32 normalY, UINT32 normalZ, UINT32 posX, UINT32 posY, UINT32 posZ)
{
    // Ŭ���̾�Ʈ�� ���� ���� �� �������� ��Ŷ. ���� ��� �÷��̾�鿡�� �����ؾ��Ѵ�.

    // 1. ����� �÷��̾� ����
    CPlayer* pPlayer = static_cast<CPlayer*>(pSession->pObj);

    // 2. �� ���� �˻�
    CRoom* pRoom = roomManager.GetRoomById(pPlayer->GetRoomId());

    // 3. �濡 �ִ� ��� �÷��̾�鿡�� ��Ŷ ����. ��, �ڱ� �ڽ��� �����Ѵ�.
    for (const auto& activePlayer : pRoom->GetActivePlayers())
    {
        // CS_POS_INTERPOLATION�� Ŭ���̾�Ʈ���� ������ �����ٴ� ���� activePlayer��� �ǹ�
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
    // ���⸦ ��ü�Ѵٴ� �ǹ�

    // 1. ����� �÷��̾� ����
    CPlayer* pPlayer = static_cast<CPlayer*>(pSession->pObj);

    // 2. �� ���� �˻�
    CRoom* pRoom = roomManager.GetRoomById(pPlayer->GetRoomId());

    // 3. �濡 �ִ� ��� �÷��̾�鿡�� ��Ŷ ����. ��, �ڱ��ڽ��� ����
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
    // ���� �۾� x

    return false;
}

bool CS_ITEM_PICKED(CSession* pSession, UINT32 itemId)
{
    // Ŭ���̾�Ʈ�� �������� �ֿ��� �� �����ϴ� ��Ŷ

    // 1. ����� �÷��̾� ����
    CPlayer* pPlayer = static_cast<CPlayer*>(pSession->pObj);

    // 2. �� ���� �˻�
    CRoom* pRoom = roomManager.GetRoomById(pPlayer->GetRoomId());
    
    // 3. ������ ȹ�� ��û �õ�
    if (pRoom->OnItemPickupRequest(pPlayer->m_ID, itemId))
    {
        // ȹ�� ����
        SC_ITEM_PICK_SUCCESS_FOR_SINGLE(pSession, pPlayer->m_ID, itemId);
    }
    else
    {
        // ȹ�� ����
        SC_ITEM_PICK_FAIL_FOR_SINGLE(pSession, pPlayer->m_ID, itemId);
    }

    return true;
}

bool CS_KEY_INPUT(CSession* pSession, UINT32 keyW, UINT32 keyA, UINT32 keyS, UINT32 keyD, UINT32 rotateAxisX, UINT32 rotateAxisY, UINT32 Jump)
{
    // KeyInput ������ �ڽ��� ������ ���� �濡 �ִ� ��� �̵鿡�� �����Ѵ�.

    // 1. ����� �÷��̾� ����
    CPlayer* pPlayer = static_cast<CPlayer*>(pSession->pObj);

    // 2. �ڱ� �ڽſ��� �Էµ� Ű ����, ȸ�� ���� ����
    std::array<bool, (int)PRESS_KEY::END> keyStates{};

    keyStates[(int)PRESS_KEY::W] = keyW;
    keyStates[(int)PRESS_KEY::A] = keyA;
    keyStates[(int)PRESS_KEY::S] = keyS;
    keyStates[(int)PRESS_KEY::D] = keyD;
    keyStates[(int)PRESS_KEY::JUMP] = Jump;
    pPlayer->SetMovementKeys(keyStates);

    pPlayer->SetRotationAxisXY(rotateAxisX, rotateAxisY);

    // 3. �� ���� �˻�
    CRoom* pRoom = roomManager.GetRoomById(pPlayer->GetRoomId());

    // 4. �濡 �ִ� ��� �÷��̾�鿡�� ��Ŷ ����
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
    // ��ġ ������ �濡 �ִ� ��� �÷��̾�鿡�� ���۵Ǿ���Ѵ�.

    // 1. ����� �÷��̾� ����
    CPlayer* pPlayer = static_cast<CPlayer*>(pSession->pObj);

    // 2. �� ���� �˻�
    CRoom* pRoom = roomManager.GetRoomById(pPlayer->GetRoomId());

    // 3. �濡 �ִ� ��� �÷��̾�鿡�� ��Ŷ ����. ��, �ڱ� �ڽ��� �����Ѵ�.
    for (const auto& activePlayer : pRoom->GetActivePlayers())
    {
        // CS_POS_INTERPOLATION�� Ŭ���̾�Ʈ���� ������ �����ٴ� ���� activePlayer��� �ǹ�
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
    // ����� ��û ��Ŷ
    
    // ���� �÷��̾�� Room�� waiting�� ���� �ְ�, ����� ��û�� active�� �̵��Ѵ�.
    
    // 1. ����� �÷��̾� ����
    CPlayer* pPlayer = static_cast<CPlayer*>(pSession->pObj);

    // 2. waiting���� active�� �̵�
    CRoom* pRoom = roomManager.GetRoomById(pPlayer->GetRoomId());
    pRoom->MoveToActive(pPlayer->m_ID);

    // 3. �������� Ŭ���̾�Ʈ���� �ڽ��� �����϶�� �޽��� ����
    SC_CREATE_MY_CHARACTER_FOR_SINGLE(pSession, pPlayer->m_ID,
        0,                      // ���� �� id�� ���� ��ȣ�� �ο��ؾ������� �ϴ� 0���� �ο�
        0, 0, 1,                // ���� �̰͵� ���� ���� ���� ���͸� �ο��ؾ������� �ϴ� +z �������� �ο�
        100,                    // �ִ� HP
        pPlayer->GetTeamId()    // �� ID
    );

    return false;
}

bool CS_SEND_NICKNAME(CSession* pSession, std::string name)
{
    // �г����� �Դٴ� ���� ���� waiting���� active�� �ű�ٴ� �ǹ�

    // 1. ����� �÷��̾� ����
    CPlayer* pPlayer = static_cast<CPlayer*>(pSession->pObj);

    // 2. �̸� �ο�
    pPlayer->SetName(name);

    // 3. waiting���� active�� �̵�
    CRoom* pRoom = roomManager.GetRoomById(pPlayer->GetRoomId());
    pRoom->MoveToActive(pPlayer->m_ID);

    // 4. �������� Ŭ���̾�Ʈ���� �ڽ��� �����϶�� �޽��� ����
    SC_CREATE_MY_CHARACTER_FOR_SINGLE(pSession, pPlayer->m_ID,
        0,                      // ���� �� id�� ���� ��ȣ�� �ο��ؾ������� �ϴ� 0���� �ο�
        0, 0, 1,                // ���� �̰͵� ���� ���� ���� ���͸� �ο��ؾ������� �ϴ� +z �������� �ο�
        100,                    // �ִ� HP
        pPlayer->GetTeamId()    // �� ID
    );

    return true;
}

bool CS_SHOT_HIT(CSession* pSession, UINT32 playerId, UINT32 hp)
{
    // Ŭ���̾�Ʈ �ʿ��� �������� �������� �� �÷��̾� ������ ����

    // 1. ����� �÷��̾� ����
    CPlayer* pPlayer = static_cast<CPlayer*>(pSession->pObj);

    // 2. �� ���� ����
    CRoom* pRoom = roomManager.GetRoomById(pPlayer->GetRoomId());

    // 3. Ÿ�� �÷��̾� ���� ����
    CPlayer* pTargetPlayer = pRoom->FindPlayerById(playerId);
    
    // Ÿ�� �÷��̾ �����Ѵٸ�
    if (!pTargetPlayer)
    {
        // ü���� 0 ���� �۴ٸ�
        if (hp <= 0)
        {
            // �÷��̾�� �װ�, active���� waiting ���·� �����
            pRoom->MoveToWaiting(playerId);

            // �÷��̾ �ٿ�Ǿ����� ���� ��� �÷��̾�鿡�� ����
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
            // �ش� �÷��̾�� ������ �ο�
            pTargetPlayer->SetCurHp(hp);

            // ���� ������ ���� ��� �÷��̾�鿡�� ����
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
    // ���� �۾� x

    return false;
}
