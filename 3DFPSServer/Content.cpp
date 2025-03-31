
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
    // ������Ʈ ����
    CPlayer* pPlayer = playerPool.Alloc();

    pPlayer->Init(0, 0, 0, 100);

    return pPlayer;
}

void LoginAcceptObject(CObject* pObj)
{
    CSession* pSession = pObj->m_pSession;

    // 1. ����� �÷��̾� ��ü ����
    CPlayer* pPlayer = static_cast<CPlayer*>(pObj);

    // 2. �� �� ã��
    CRoom* room = CRoomManager::GetInstance().FindAvailableRoom();
    if (!room)
    {
        // ���� ���� �ο� �ʰ� ó��
        return;
    }

    // 3. �뿡 �÷��̾� �߰�
    // ��, �� ������ �÷��̾ ����
    // �뿡 ��ϵǾ���, �ش� �÷��̾�� �뿡 ���� ������ ���۵� ����
    // ó���� Waiting �ʿ� ��. �÷��̾ ������ ���� ������ ������ ����. ���� ������ [ ���� ������ �÷��̾ ������ ������ �÷��̾���� KDA ����, Ű �Է� ������ ��ġ ���� ]
    if (!room->AddPlayer(pPlayer))
    {
        // �뿡 �߰� ���� ó��
        return;
    }

    // 4. ���� ����

    //=====================================================================================================================================
    // 4-1. ���� �뿡 �ִ� �÷��̾��� �ʱ� ������ ���� ������ �÷��̾�� ����
    //=====================================================================================================================================

    // �뿡 �ִ� activePlayer���� ������ ���� �뿡 ������ �÷��̾�� ����
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