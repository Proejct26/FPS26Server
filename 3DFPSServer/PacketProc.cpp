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

#include "EncodingUtils.h"

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
        UINT32 hittedTargetId;
        float normalX;
        float normalY;
        float normalZ;
        float posX;
        float posY;
        float posZ;

        game::CS_ATTACK pkt;
        pkt.ParseFromArray(pPacket->GetBufferPtr(), pPacket->GetDataSize());

        hittedTargetId = pkt.hittedtargetid();
        normalX = pkt.normalx();
        normalY = pkt.normaly();
        normalZ = pkt.normalz();
        posX = pkt.posx();
        posY = pkt.posy();
        posZ = pkt.posz();

        return CS_ATTACK(pSession, hittedTargetId, normalX, normalY, normalZ, posX, posY, posZ);
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
        bool keyW;
        bool keyA;
        bool keyS;
        bool keyD;
        float rotateAxisX;
        float rotateAxisY;
        UINT32 Jump;
        float normalX;
        float normalY;
        float normalZ;

        game::CS_KEY_INPUT pkt;
        pkt.ParseFromArray(pPacket->GetBufferPtr(), pPacket->GetDataSize());

        keyW = pkt.keyw();
        keyA = pkt.keya();
        keyS = pkt.keys();
        keyD = pkt.keyd();
        rotateAxisX = pkt.rotateaxisx();
        rotateAxisY = pkt.rotateaxisy();
        Jump = pkt.jump();
        normalX = pkt.normalx();
        normalY = pkt.normaly();
        normalZ = pkt.normalz();

        return CS_KEY_INPUT(pSession, keyW, keyA, keyS, keyD, rotateAxisX, rotateAxisY, Jump, normalX, normalY, normalZ);
    }
    break;
    case game::PacketID::CS_PosInterpolation:
    {
        float posX;
        float posY;
        float posZ;

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
    case game::PacketID::CS_SendMessageAll:
    {
        UINT32 playerId;
        std::string message;

        game::CS_SEND_MESSAGE_ALL pkt;
        pkt.ParseFromArray(pPacket->GetBufferPtr(), pPacket->GetDataSize());

        playerId = pkt.playerid();
        message = pkt.message();

        return CS_SEND_MESSAGE_ALL(pSession, playerId, message);
    }
    break;
    case game::PacketID::CS_SendMessageTeam:
    {
        UINT32 playerId;
        std::string message;

        game::CS_SEND_MESSAGE_TEAM pkt;
        pkt.ParseFromArray(pPacket->GetBufferPtr(), pPacket->GetDataSize());

        playerId = pkt.playerid();
        message = pkt.message();

        return CS_SEND_MESSAGE_TEAM(pSession, playerId, message);
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
    default:
        break;
    }
    return false;
}

void DisconnectSessionProc(CSession* pSession)
{
    // ������ ����ٴ� ���� ���� ������ �����ؾ��Ѵٴ� �ǹ�

    // 1. ����� �÷��̾� ����
    CPlayer* pPlayer = static_cast<CPlayer*>(pSession->pObj);

    // 2. �� ���� �˻�
    CRoom* pRoom = roomManager.GetRoomById(pPlayer->GetRoomId());

    // 3. ���� �÷��̾ ������ ��� �÷��̾�� �ش� ĳ���� �������� �˸��� ��Ŷ ����. ��, �ڱ� �ڽ��� �����Ѵ�. 
    
    UINT32 assistID = 1;  // ��ý�Ʈ ���� �ӽ�
    for (const auto& activePlayer : pRoom->m_activePlayers)
    {
        if (activePlayer == pPlayer)
            continue;

        // ĳ������ hp�� 0�� �Ǿ����� �˸��� ��Ŷ�� ����. ���� ���̳� hp�� 0�� �� ���̳� ���� �ǹ̷� ���
        SC_CHARACTER_DOWN_FOR_SINGLE(activePlayer->m_pSession, pPlayer->m_ID, pPlayer->GetTeamId(), assistID);
    }
    for (const auto& waitingPlayer : pRoom->m_activePlayers)
    {
        if (waitingPlayer == pPlayer)
            continue;

        // ĳ������ hp�� 0�� �Ǿ����� �˸��� ��Ŷ�� ����. ���� ���̳� hp�� 0�� �� ���̳� ���� �ǹ̷� ���
        SC_CHARACTER_DOWN_FOR_SINGLE(waitingPlayer->m_pSession, pPlayer->m_ID, pPlayer->GetTeamId(), assistID);
    }

    // 4. ���� ����
    pRoom->RemovePlayer(pPlayer->m_ID);

    return;
}

bool CS_ATTACK(CSession* pSession, UINT32 hittedTargetId, float normalX, float normalY, float normalZ, float posX, float posY, float posZ)
{
    // Ŭ���̾�Ʈ�� ���� ���� �� �������� ��Ŷ. ���� ��� �÷��̾�鿡�� �����ؾ��Ѵ�.

    // 1. ����� �÷��̾� ����
    CPlayer* pPlayer = static_cast<CPlayer*>(pSession->pObj);

    // 2. �� ���� �˻�
    CRoom* pRoom = roomManager.GetRoomById(pPlayer->GetRoomId());

    // 3. �濡 �ִ� ��� �÷��̾�鿡�� ��Ŷ ����. ��, �ڱ� �ڽ��� �����Ѵ�.
    for (const auto& activePlayer : pRoom->m_activePlayers)
    {
        if (activePlayer == pPlayer)
            continue;

        SC_ATTACK_FOR_SINGLE(activePlayer->m_pSession, pPlayer->m_ID, hittedTargetId, normalX, normalY, normalZ, posX, posY, posZ);
    }
    for (const auto& waitingPlayer : pRoom->m_activePlayers)
    {
        SC_ATTACK_FOR_SINGLE(waitingPlayer->m_pSession, pPlayer->m_ID, hittedTargetId, normalX, normalY, normalZ, posX, posY, posZ);
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
    for (const auto& activePlayer : pRoom->m_activePlayers)
    {
        SC_CHANGE_WEAPON_FOR_SINGLE(activePlayer->m_pSession, pPlayer->m_ID, weapon);
    }
    for (const auto& waitingPlayer : pRoom->m_activePlayers)
    {
        SC_CHANGE_WEAPON_FOR_SINGLE(waitingPlayer->m_pSession, pPlayer->m_ID, weapon);
    }

    return true;
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

bool CS_KEY_INPUT(CSession* pSession, bool keyW, bool keyA, bool keyS, bool keyD, float rotateAxisX, float rotateAxisY, UINT32 Jump, float normalX, float normalY, float normalZ)
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
    for (const auto& activePlayer : pRoom->m_activePlayers)
    {
        SC_KEY_INPUT_FOR_SINGLE(activePlayer->m_pSession, pPlayer->m_ID, keyW, keyA, keyS, keyD, rotateAxisX, rotateAxisY, Jump, normalX, normalY, normalZ);
    }
    for (const auto& waitingPlayer : pRoom->m_waitingPlayers)
    {
        SC_KEY_INPUT_FOR_SINGLE(waitingPlayer->m_pSession, pPlayer->m_ID, keyW, keyA, keyS, keyD, rotateAxisX, rotateAxisY, Jump, normalX, normalY, normalZ);
    }

    return true;
}

bool CS_POS_INTERPOLATION(CSession* pSession, float posX, float posY, float posZ)
{
    // ��ġ ������ �濡 �ִ� ��� �÷��̾�鿡�� ���۵Ǿ���Ѵ�.

    // 1. ����� �÷��̾� ����
    CPlayer* pPlayer = static_cast<CPlayer*>(pSession->pObj);

    // 2. �� ���� �˻�
    CRoom* pRoom = roomManager.GetRoomById(pPlayer->GetRoomId());

    // 3. �濡 �ִ� ��� �÷��̾�鿡�� ��Ŷ ����. ��, �ڱ� �ڽ��� �����Ѵ�.
    for (const auto& activePlayer : pRoom->m_activePlayers)
    {
        // CS_POS_INTERPOLATION�� Ŭ���̾�Ʈ���� ������ �����ٴ� ���� activePlayer��� �ǹ�
        if (activePlayer == pPlayer)
            continue;

        SC_POS_INTERPOLATION_FOR_SINGLE(activePlayer->m_pSession, pPlayer->m_ID, posX, posY, posZ);
    }
    for (const auto& waitingPlayer : pRoom->m_waitingPlayers)
    {
        SC_POS_INTERPOLATION_FOR_SINGLE(waitingPlayer->m_pSession, pPlayer->m_ID, posX, posY, posZ);
    }

    return true;
}

bool CS_REQUEST_RESTART(CSession* pSession, UINT32 playerId, UINT32 weapon)
{
    // ����� ��û ��Ŷ
    
    // ���� �÷��̾�� Room�� waiting�� ���� �ְ�, ����� ��û�� active�� �̵��Ѵ�.
    
    // 1. ����� �÷��̾� ����
    CPlayer* pPlayer = static_cast<CPlayer*>(pSession->pObj);

    // 2. �� ���� ����
    CRoom* pRoom = roomManager.GetRoomById(pPlayer->GetRoomId());

    // 3. Waiting���� Active�� �̵�
    pRoom->MoveToActive(pPlayer->m_ID);

    // 4. �������� Ŭ���̾�Ʈ���� �ڽ��� �����϶�� �޽��� ����
    SC_CREATE_MY_CHARACTER_FOR_SINGLE(pSession, pPlayer->m_ID,
        pPlayer->GetSpawnPosIndex(),    // �� id�� ���� �ο��� ���� ��ȣ
        100,                            // �ִ� HP
        pPlayer->GetTeamId()            // �� ID
    );

    // 5. �濡 �ִ� ��� �÷��̾�鿡�� ���� ������ �÷��̾� ���� ����
    KDAInfo kdaInfo;
    for (const auto& waitingPlayer : pRoom->m_waitingPlayers)
    {
            pPlayer->GetKDAInfo(kdaInfo);

            // �ڱ� �ڽ��� ����
            if (waitingPlayer != pPlayer)
            {
                    SC_CREATE_OTHER_CHARACTER_FOR_SINGLE(
                            waitingPlayer->m_pSession,
                            pPlayer->m_ID,
                            pPlayer->GetSpawnPosIndex(),
                            100, pPlayer->GetCurHp(),
                            pPlayer->GetName(),
                            kdaInfo,
                            pPlayer->GetWeaponInfo(),
                            pPlayer->GetTeamId()
                    );
            }
    }
    for (const auto& activePlayer : pRoom->m_activePlayers)
    {
            pPlayer->GetKDAInfo(kdaInfo);

            // �ڱ� �ڽ��� ����
            if (activePlayer != pPlayer)
            {
                    SC_CREATE_OTHER_CHARACTER_FOR_SINGLE(
                            activePlayer->m_pSession,
                            pPlayer->m_ID,
                            pPlayer->GetSpawnPosIndex(),
                            100, pPlayer->GetCurHp(),
                            pPlayer->GetName(),
                            kdaInfo,
                            pPlayer->GetWeaponInfo(),
                            pPlayer->GetTeamId()
                    );
            }
    }

    return true;
}

bool CS_SEND_MESSAGE_ALL(CSession* pSession, UINT32 playerId, std::string message)
{
    // �濡 �ִ� ��� �÷��̾�� �޽����� ����

    // 1. ����� �÷��̾� ����
    CPlayer* pPlayer = static_cast<CPlayer*>(pSession->pObj);

    // 2. �� ���� ����
    CRoom* pRoom = roomManager.GetRoomById(pPlayer->GetRoomId());

    // 3. �޽��� ����. �޽����� active ������ ���� Ŭ���̾�Ʈ���� ������ �����ϱ⿡ activePlayer �鿡�Ը� ������.
    // �ڱ� �ڽ��� �޽����� �̹� �������ϱ� �ٽ� ��ȯ���� �ʴ´�.
    std::wstring wideStr;
    for (const auto& activePlayer : pRoom->m_activePlayers)
    {
        // �޽����� ���� �÷��̾ �ƴҶ�
        if (activePlayer != pPlayer)
        {
            // �Ž��� ����
            wideStr = Utf8ToWString(message);
            SC_SEND_MESSAGE_ALL_FOR_SINGLE(activePlayer->m_pSession, playerId, WStringToUtf8(wideStr));
        }
    }

    return true;
}

bool CS_SEND_MESSAGE_TEAM(CSession* pSession, UINT32 playerId, std::string message)
{
    // ���� �� �÷��̾�Ը� �޽����� ����

    // 1. ����� �÷��̾� ����
    CPlayer* pPlayer = static_cast<CPlayer*>(pSession->pObj);

    // 2. �� ���� ����
    CRoom* pRoom = roomManager.GetRoomById(pPlayer->GetRoomId());

    // 3. �޽��� ����. �޽����� active ������ ���� Ŭ���̾�Ʈ���� ������ �����ϱ⿡ activePlayer �鿡�Ը� ������.
    // �ڱ� �ڽ��� �޽����� �̹� �������ϱ� �ٽ� ��ȯ���� �ʴ´�.
    std::wstring wideStr;
    for (const auto& activePlayer : pRoom->m_activePlayers)
    {
        // ���� team�̰�, �޽����� ���� �÷��̾ �ƴҶ�
        if (activePlayer->GetTeamId() == pPlayer->GetTeamId() && activePlayer != pPlayer)
        {
            // �Ž��� ����
            wideStr = Utf8ToWString(message);
            SC_SEND_MESSAGE_TEAM_FOR_SINGLE(pSession, playerId, WStringToUtf8(wideStr));
        }
    }

    return true;
}

bool CS_SEND_NICKNAME(CSession* pSession, std::string name)
{
    // �г����� �Դٴ� ���� ���� waiting���� active�� �ű�ٴ� �ǹ�

    // 1. ����� �÷��̾� ����
    CPlayer* pPlayer = static_cast<CPlayer*>(pSession->pObj);

    // ä�� �޽��� ����
    // Ȯ�ο�. �ѱ��� ������ wideStr���� �ѱ��� Ȯ���� �� �ִ�.
    //SC_SEND_MESSAGE_FOR_SINGLE(pSession, pPlayer->m_ID, WStringToUtf8(wideStr));

    // 2. �̸� �ο�
    std::wstring wideStr = Utf8ToWString(name);
    pPlayer->SetName(WStringToUtf8(wideStr));

    // 3. waiting���� active�� �̵�
    CRoom* pRoom = roomManager.GetRoomById(pPlayer->GetRoomId());
    pRoom->MoveToActive(pPlayer->m_ID);
     
    // 4. �������� Ŭ���̾�Ʈ���� �ڽ��� �����϶�� �޽��� ����
    SC_CREATE_MY_CHARACTER_FOR_SINGLE(pSession, pPlayer->m_ID,
        pPlayer->GetSpawnPosIndex(),    // �� id�� ���� �ο��� ���� ��ȣ
        100,                    // �ִ� HP
        pPlayer->GetTeamId()    // �� ID
    );


    // 5. �濡 �ִ� ��� �÷��̾�鿡�� ���� ������ �÷��̾� ���� ����
    KDAInfo kdaInfo;
    for (const auto& waitingPlayer : pRoom->m_waitingPlayers)
    {
        pPlayer->GetKDAInfo(kdaInfo);

        // �ڱ� �ڽ��� ����
        if (waitingPlayer != pPlayer)
        {
            SC_CREATE_OTHER_CHARACTER_FOR_SINGLE(
                waitingPlayer->m_pSession,
                pPlayer->m_ID,
                pPlayer->GetSpawnPosIndex(),
                100, pPlayer->GetCurHp(),
                pPlayer->GetName(),
                kdaInfo,
                pPlayer->GetWeaponInfo(),
                pPlayer->GetTeamId()
            );
        }
    }
    for (const auto& activePlayer : pRoom->m_activePlayers)
    {
        pPlayer->GetKDAInfo(kdaInfo);

        // �ڱ� �ڽ��� ����
        if (activePlayer != pPlayer)
        {
            SC_CREATE_OTHER_CHARACTER_FOR_SINGLE(
                activePlayer->m_pSession,
                pPlayer->m_ID,
                pPlayer->GetSpawnPosIndex(),
                100, pPlayer->GetCurHp(),
                pPlayer->GetName(),
                kdaInfo,
                pPlayer->GetWeaponInfo(),
                pPlayer->GetTeamId()
            );
        }
    }

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
    if (pTargetPlayer)
    {
        // ü���� 0 ���� �۴ٸ�
        if (hp <= 0)
        {
            // ���� �ο��� kil�� �ø�
            pPlayer->AddKill();

            // ���� �ο��� death�� �ö�
            pTargetPlayer->AddDeath();

            // �������� ������ �ο��� assist�� �ø�
            CPlayer* pAssistPlayer = pRoom->FindPlayerById(pTargetPlayer->GetLastAttackedPlayerID());
            if (pAssistPlayer)
            {
                if (pPlayer != pAssistPlayer)
                    pAssistPlayer->AddAssist();
            }

            // �÷��̾�� �װ�, active���� waiting ���·� �����
            pRoom->MoveToWaiting(playerId);

            // �÷��̾ �ٿ�Ǿ����� ���� ��� �÷��̾�鿡�� ����
            std::vector<PlayerInfo> v;
            for (const auto& activePlayer : pRoom->m_activePlayers)
            {
                SC_CHARACTER_DOWN_FOR_SINGLE(activePlayer->m_pSession, playerId, pTargetPlayer->GetTeamId(), pTargetPlayer->GetLastAttackedPlayerID());
                KDAInfo kda;
                activePlayer->GetKDAInfo(kda);
                v.push_back(PlayerInfo(activePlayer->m_ID, kda));
            }
             
            for (const auto& activePlayer : pRoom->m_activePlayers)
                SC_CHARACTER_KILL_LOG_FOR_SINGLE(activePlayer->m_pSession, v);

            for (const auto& waitingPlayer : pRoom->m_waitingPlayers)
            {
                SC_CHARACTER_DOWN_FOR_SINGLE(waitingPlayer->m_pSession, playerId, pTargetPlayer->GetTeamId(), pTargetPlayer->GetLastAttackedPlayerID());
            }
        }
        else
        {
            // �ش� �÷��̾�� ������ �ο�
            pTargetPlayer->SetCurHp(hp);
            pTargetPlayer->SetLastAttackedPlayerID(pPlayer->m_ID);  // pPlayer�� ���������� pTargetPlayer�� �����ߴٰ� ����

            // ���� ������ ���� ��� �÷��̾�鿡�� ����
            for (const auto& activePlayer : pRoom->m_activePlayers)
            {
                SC_SHOT_HIT_FOR_SINGLE(activePlayer->m_pSession, playerId, hp);
            }
            for (const auto& waitingPlayer : pRoom->m_waitingPlayers)
            {
                SC_SHOT_HIT_FOR_SINGLE(waitingPlayer->m_pSession, playerId, hp);
            }
        }
    }

    return true;
}