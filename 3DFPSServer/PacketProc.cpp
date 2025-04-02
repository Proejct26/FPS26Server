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
    // 연결이 끊겼다는 것은 세션 접속을 해제해야한다는 의미

    // 1. 연결된 플레이어 추출
    CPlayer* pPlayer = static_cast<CPlayer*>(pSession->pObj);

    // 2. 방 정보 검색
    CRoom* pRoom = roomManager.GetRoomById(pPlayer->GetRoomId());

    // 3. 현재 플레이어를 제외한 모든 플레이어에게 해당 캐릭이 나갔음을 알리는 패킷 전송. 단, 자기 자신은 제외한다. 
    
    UINT32 assistID = 1;  // 어시스트 관련 임시
    for (const auto& activePlayer : pRoom->m_activePlayers)
    {
        if (activePlayer == pPlayer)
            continue;

        // 캐릭터의 hp가 0이 되었음을 알리는 패킷을 전송. 나간 것이나 hp가 0이 된 것이나 같은 의미로 사용
        SC_CHARACTER_DOWN_FOR_SINGLE(activePlayer->m_pSession, pPlayer->m_ID, pPlayer->GetTeamId(), assistID);
    }
    for (const auto& waitingPlayer : pRoom->m_activePlayers)
    {
        if (waitingPlayer == pPlayer)
            continue;

        // 캐릭터의 hp가 0이 되었음을 알리는 패킷을 전송. 나간 것이나 hp가 0이 된 것이나 같은 의미로 사용
        SC_CHARACTER_DOWN_FOR_SINGLE(waitingPlayer->m_pSession, pPlayer->m_ID, pPlayer->GetTeamId(), assistID);
    }

    // 4. 방을 나감
    pRoom->RemovePlayer(pPlayer->m_ID);

    return;
}

bool CS_ATTACK(CSession* pSession, UINT32 hittedTargetId, float normalX, float normalY, float normalZ, float posX, float posY, float posZ)
{
    // 클라이언트가 총을 쐈을 때 보내지는 패킷. 방의 모든 플레이어들에게 전송해야한다.

    // 1. 연결된 플레이어 추출
    CPlayer* pPlayer = static_cast<CPlayer*>(pSession->pObj);

    // 2. 방 정보 검색
    CRoom* pRoom = roomManager.GetRoomById(pPlayer->GetRoomId());

    // 3. 방에 있는 모든 플레이어들에게 패킷 전송. 단, 자기 자신은 제외한다.
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
    // 무기를 교체한다는 의미

    // 1. 연결된 플레이어 추출
    CPlayer* pPlayer = static_cast<CPlayer*>(pSession->pObj);

    // 2. 방 정보 검색
    CRoom* pRoom = roomManager.GetRoomById(pPlayer->GetRoomId());

    // 3. 방에 있는 모든 플레이어들에게 패킷 전송. 단, 자기자신은 제외
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

bool CS_KEY_INPUT(CSession* pSession, bool keyW, bool keyA, bool keyS, bool keyD, float rotateAxisX, float rotateAxisY, UINT32 Jump, float normalX, float normalY, float normalZ)
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
    // 위치 정보는 방에 있는 모든 플레이어들에게 전송되어야한다.

    // 1. 연결된 플레이어 추출
    CPlayer* pPlayer = static_cast<CPlayer*>(pSession->pObj);

    // 2. 방 정보 검색
    CRoom* pRoom = roomManager.GetRoomById(pPlayer->GetRoomId());

    // 3. 방에 있는 모든 플레이어들에게 패킷 전송. 단, 자기 자신은 제외한다.
    for (const auto& activePlayer : pRoom->m_activePlayers)
    {
        // CS_POS_INTERPOLATION를 클라이언트에서 서버로 보낸다는 것은 activePlayer라는 의미
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
    // 재시작 요청 패킷
    
    // 현재 플레이어는 Room의 waiting에 속해 있고, 재시작 요청시 active로 이동한다.
    
    // 1. 연결된 플레이어 추출
    CPlayer* pPlayer = static_cast<CPlayer*>(pSession->pObj);

    // 2. 방 정보 추출
    CRoom* pRoom = roomManager.GetRoomById(pPlayer->GetRoomId());

    // 3. Waiting에서 Active로 이동
    pRoom->MoveToActive(pPlayer->m_ID);

    // 4. 서버에서 클라이언트에게 자신을 생성하라는 메시지 전송
    SC_CREATE_MY_CHARACTER_FOR_SINGLE(pSession, pPlayer->m_ID,
        pPlayer->GetSpawnPosIndex(),    // 팀 id에 따라서 부여된 스폰 번호
        100,                            // 최대 HP
        pPlayer->GetTeamId()            // 팀 ID
    );

    // 5. 방에 있는 모든 플레이어들에게 새로 접속한 플레이어 정보 전송
    KDAInfo kdaInfo;
    for (const auto& waitingPlayer : pRoom->m_waitingPlayers)
    {
            pPlayer->GetKDAInfo(kdaInfo);

            // 자기 자신은 제외
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

            // 자기 자신은 제외
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
    // 방에 있는 모든 플레이어에게 메시지를 전송

    // 1. 연결된 플레이어 추출
    CPlayer* pPlayer = static_cast<CPlayer*>(pSession->pObj);

    // 2. 방 정보 추출
    CRoom* pRoom = roomManager.GetRoomById(pPlayer->GetRoomId());

    // 3. 메시지 전송. 메시지는 active 상태일 때만 클라이언트에서 전송이 가능하기에 activePlayer 들에게만 보낸다.
    // 자기 자신은 메시지를 이미 보냈으니깐 다시 반환하지 않는다.
    std::wstring wideStr;
    for (const auto& activePlayer : pRoom->m_activePlayers)
    {
        // 메시지를 보낸 플레이어가 아닐때
        if (activePlayer != pPlayer)
        {
            // 매시지 전송
            wideStr = Utf8ToWString(message);
            SC_SEND_MESSAGE_ALL_FOR_SINGLE(activePlayer->m_pSession, playerId, WStringToUtf8(wideStr));
        }
    }

    return true;
}

bool CS_SEND_MESSAGE_TEAM(CSession* pSession, UINT32 playerId, std::string message)
{
    // 같은 팀 플레이어에게만 메시지를 전송

    // 1. 연결된 플레이어 추출
    CPlayer* pPlayer = static_cast<CPlayer*>(pSession->pObj);

    // 2. 방 정보 추출
    CRoom* pRoom = roomManager.GetRoomById(pPlayer->GetRoomId());

    // 3. 메시지 전송. 메시지는 active 상태일 때만 클라이언트에서 전송이 가능하기에 activePlayer 들에게만 보낸다.
    // 자기 자신은 메시지를 이미 보냈으니깐 다시 반환하지 않는다.
    std::wstring wideStr;
    for (const auto& activePlayer : pRoom->m_activePlayers)
    {
        // 같은 team이고, 메시지를 보낸 플레이어가 아닐때
        if (activePlayer->GetTeamId() == pPlayer->GetTeamId() && activePlayer != pPlayer)
        {
            // 매시지 전송
            wideStr = Utf8ToWString(message);
            SC_SEND_MESSAGE_TEAM_FOR_SINGLE(pSession, playerId, WStringToUtf8(wideStr));
        }
    }

    return true;
}

bool CS_SEND_NICKNAME(CSession* pSession, std::string name)
{
    // 닉네임이 왔다는 것은 이제 waiting에서 active로 옮긴다는 의미

    // 1. 연결된 플레이어 추출
    CPlayer* pPlayer = static_cast<CPlayer*>(pSession->pObj);

    // 채팅 메시지 예제
    // 확인용. 한글을 보내면 wideStr에서 한글을 확인할 수 있다.
    //SC_SEND_MESSAGE_FOR_SINGLE(pSession, pPlayer->m_ID, WStringToUtf8(wideStr));

    // 2. 이름 부여
    std::wstring wideStr = Utf8ToWString(name);
    pPlayer->SetName(WStringToUtf8(wideStr));

    // 3. waiting에서 active로 이동
    CRoom* pRoom = roomManager.GetRoomById(pPlayer->GetRoomId());
    pRoom->MoveToActive(pPlayer->m_ID);
     
    // 4. 서버에서 클라이언트에게 자신을 생성하라는 메시지 전송
    SC_CREATE_MY_CHARACTER_FOR_SINGLE(pSession, pPlayer->m_ID,
        pPlayer->GetSpawnPosIndex(),    // 팀 id에 따라서 부여된 스폰 번호
        100,                    // 최대 HP
        pPlayer->GetTeamId()    // 팀 ID
    );


    // 5. 방에 있는 모든 플레이어들에게 새로 접속한 플레이어 정보 전송
    KDAInfo kdaInfo;
    for (const auto& waitingPlayer : pRoom->m_waitingPlayers)
    {
        pPlayer->GetKDAInfo(kdaInfo);

        // 자기 자신은 제외
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

        // 자기 자신은 제외
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
    // 클라이언트 쪽에서 서버에게 데미지를 준 플레이어 정보를 전송

    // 1. 연결된 플레이어 추출
    CPlayer* pPlayer = static_cast<CPlayer*>(pSession->pObj);

    // 2. 방 정보 추출
    CRoom* pRoom = roomManager.GetRoomById(pPlayer->GetRoomId());

    // 3. 타겟 플레이어 정보 추출
    CPlayer* pTargetPlayer = pRoom->FindPlayerById(playerId);
    
    // 타겟 플레이어가 존재한다면
    if (pTargetPlayer)
    {
        // 체력이 0 보다 작다면
        if (hp <= 0)
        {
            // 죽인 인원의 kil을 올림
            pPlayer->AddKill();

            // 죽은 인원의 death가 올라감
            pTargetPlayer->AddDeath();

            // 마지막에 공격한 인원의 assist를 올림
            CPlayer* pAssistPlayer = pRoom->FindPlayerById(pTargetPlayer->GetLastAttackedPlayerID());
            if (pAssistPlayer)
            {
                if (pPlayer != pAssistPlayer)
                    pAssistPlayer->AddAssist();
            }

            // 플레이어는 죽고, active에서 waiting 상태로 변경됨
            pRoom->MoveToWaiting(playerId);

            // 플레이어가 다운되었음을 방의 모든 플레이어들에게 전송
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
            // 해당 플레이어에게 데미지 부여
            pTargetPlayer->SetCurHp(hp);
            pTargetPlayer->SetLastAttackedPlayerID(pPlayer->m_ID);  // pPlayer가 마지막으로 pTargetPlayer를 공격했다고 갱신

            // 관련 정보를 방의 모든 플레이어들에게 전송
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