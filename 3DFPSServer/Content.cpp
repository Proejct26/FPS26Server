
#include "pch.h"
#include "Content.h"
#include "Player.h"
#include "MakePacket.h"
#include "ObjectManager.h"

#include "MemoryPoolManager.h"

#include "Protobuf/Protocol.pb.h"
#include "SessionManager.h"
#include "Packet.h"

#include "RoomManager.h"

CObject* CreateAcceptObject(void)
{
    // 오브젝트 생성
    CPlayer* pPlayer = playerPool.Alloc();

    pPlayer->Init(0, 0, 0, 100);

    return pPlayer;
}

void LoginAcceptObject(CObject* pObj)
{
    CSession* pSession = pObj->m_pSession;

    // 1. 연결된 플레이어 객체 추출
    CPlayer* pPlayer = static_cast<CPlayer*>(pObj);

    // 2. 빈 룸 찾기
    CRoom* room = CRoomManager::GetInstance().FindAvailableRoom();
    if (!room)
    {
        // 서버 수용 인원 초과 처리
        return;
    }

    // 3. 룸에 플레이어 추가
    // 룸, 팀 정보를 플레이어에 주입
    // 룸에 등록되었고, 해당 플레이어에게 룸에 대한 정보가 전송될 예정
    // 처음엔 Waiting 쪽에 들어감. 플레이어가 참가할 방의 정보를 보내기 시작. 방의 정보는 [ 새로 접속한 플레이어를 제외한 나머지 플레이어들의 KDA 정보, 키 입력 정보와 위치 정보 ]
    if (!room->AddPlayer(pPlayer))
    {
        // 룸에 추가 실패 처리
        return;
    }

    // 4. 접속 성공

    //=====================================================================================================================================
    // 4-1. 기존 룸에 있던 플레이어의 초기 정보를 새로 접속한 플레이어에게 전송
    //=====================================================================================================================================

    // 룸에 있던 activePlayer들의 정보를 새로 룸에 접속한 플레이어에게 전송
    UINT16 posX, posY, posZ;
    UINT32 rotationAxisX, rotationAxisY;
    KDAInfo kdaInfo;
    for (const auto& activePlayer : room->GetActivePlayers())
    {
        activePlayer->getPosition(posX, posY, posZ);
        activePlayer->GetRotationAxisXY(rotationAxisX, rotationAxisY);
        activePlayer->GetKDAInfo(kdaInfo);

        SC_CREATE_OTHER_CHARACTER_FOR_SINGLE(
            pSession, activePlayer->m_ID, 
            posX, posY, posZ, 
            rotationAxisX, rotationAxisY, 
            activePlayer->GetMaxHp(), activePlayer->GetCurHp(),
            activePlayer->GetName(), kdaInfo,
            activePlayer->GetWeaponInfo(), activePlayer->GetTeamId());
    }
}