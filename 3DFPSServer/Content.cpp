
#include "pch.h"
#include "Content.h"
#include "Player.h"
#include "MakePacket.h"
#include "ObjectManager.h"
#include "SectorManager.h"
#include "Sector.h" 

#include "MemoryPoolManager.h"

#include "Protobuf/Protocol.pb.h"
#include "SessionManager.h"
#include "Packet.h"

CObject* CreateAcceptObject(void)
{
    // ������Ʈ ���� �� ������Ʈ�� ���Ϳ� ���
    CPlayer* pPlayer = playerPool.Alloc();
    pPlayer->Init(
        //rand() % 30 + 20,
        //rand() % 30 + 20,
        rand() % (dfRANGE_MOVE_RIGHT - dfRANGE_MOVE_LEFT - 10) + dfRANGE_MOVE_LEFT + 10,
        rand() % (dfRANGE_MOVE_BOTTOM - dfRANGE_MOVE_TOP - 10) + dfRANGE_MOVE_TOP + 10,
        dfPACKET_MOVE_DIR_LL,
        100
    );

    pPlayer->SetSpeed(dfSPEED_PLAYER_X, dfSPEED_PLAYER_Y);

    return pPlayer;
}

void LoginAcceptObject(CObject* pObj)
{
    CSession* pSession = pObj->m_pSession;
    CPlayer* pPlayer = static_cast<CPlayer*>(pObj);

    //std::string payload;
    //pkt.SerializeToString(&payload);

    char buffer[256];   // �ִ� ��Ŷ ������. ���Ƿ� 256���� �����ص�.
    game::SC_CreateMyCharacter pkt;
    pkt.set_id(pPlayer->m_ID);
    pkt.set_posx(10);
    pkt.set_posy(20);
    pkt.set_posz(30);
    pkt.set_rotatex(0);
    pkt.set_rotatey(90);
    pkt.set_rotatez(0);
    pkt.set_name("TestPlayer");

    int size = pkt.ByteSizeLong(); // ����ȭ ũ�� ���� ���
    pkt.SerializeToArray(buffer, size); // ���⼭ buffer�� �ٷ� ��


    PACKET_HEADER header;
    CPacket* Packet = packetPool.Alloc();

    Packet->PutData(buffer, size);

    header.byCode = dfNETWORK_PACKET_CODE;
    header.bySize = Packet->GetDataSize();
    header.byType = dfPACKET_SC_CREATE_MY_CHARACTER;

    UnicastPacket(pSession, &header, Packet);

    Packet->Clear();
    packetPool.Free(Packet);

    ////=====================================================================================================================================
    //// 1. ����� ���ǿ� PACKET_SC_CREATE_MY_CHARACTER �� ����
    ////=====================================================================================================================================
    //UINT16 posX, posY;
    //pPlayer->getPosition(posX, posY);
    //SC_CREATE_MY_CHARACTER_FOR_SINGLE(pSession, pPlayer->m_ID, pPlayer->GetDirection(), posX, posY, pPlayer->GetHp());

    ////=====================================================================================================================================
    //// 2. PACKET_SC_CREATE_OTHER_CHARACTER �� ���� ����� ������ ������ ��� ���� ���͵鿡�� send
    ////=====================================================================================================================================

    //static CSectorManager& sectorManager = CSectorManager::GetInstance();

    //// ���� ������Ʈ�� ��ġ�� �������� ����
    //CSector* pCurSector = sectorManager.GetSectorInfo(pPlayer->m_curSectorPos.x, pPlayer->m_curSectorPos.y);

    //// ���� ������Ʈ ������ ������ ���͵鿡 �ִ� ������Ʈ�鿡 ���� ������ ������ ������ �߰�
    //SC_CREATE_OTHER_CHARACTER_AROUND(pSession, pCurSector, pPlayer->m_ID, pPlayer->GetDirection(), posX, posY, pPlayer->GetHp());


    ////=====================================================================================================================================
    //// 3. PACKET_SC_CREATE_OTHER_CHARACTER �� ���� ���͵鿡 �ִ� ��� ĳ���� �������� ��� ���� ����� ���ǿ��� ����
    ////=====================================================================================================================================

    //// ���� ������ �ִ� ���� ����Ʈ ����
    //auto& sectorList = pCurSector->GetAroundSectorList();

    //// ���ο� ������ �õ��ϴ� Ŭ���̾�Ʈ�� ���� ������ �ִ� ��� ������Ʈ���� ���� �߰�
    //CPlayer* pSectorPlayer;
    //for (auto& sector : sectorList)
    //{
    //    for (auto& Object : sector->GetSectorObjectMap())
    //    {
    //        if (Object.second == pObj)
    //            continue;

    //        pSectorPlayer = static_cast<CPlayer*>(Object.second);
    //        pSectorPlayer->getPosition(posX, posY);

    //        SC_CREATE_OTHER_CHARACTER_FOR_SINGLE(pSession, pSectorPlayer->m_ID, pSectorPlayer->GetDirection(), posX, posY, pSectorPlayer->GetHp());

    //        // �����̰� �ִ� ��Ȳ�̶��
    //        if (pSectorPlayer->isBitSet(FLAG_MOVING))
    //        {
    //            SC_MOVE_START_FOR_SINGLE(pSession, pSectorPlayer->m_ID, pSectorPlayer->GetDirection(), posX, posY);
    //        }
    //    }
    //}
}