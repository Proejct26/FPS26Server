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


static CSectorManager& sectorManager = CSectorManager::GetInstance();
static CObjectManager& objectManager = CObjectManager::GetInstance();
static CTimerManager& timerManager = CTimerManager::GetInstance();
static LogManager& logManager = LogManager::GetInstance();

int g_iSyncCount = 0;


std::string makeDebugLog(long sectorPosX, long sectorPosY, unsigned char direction, unsigned short posX, unsigned short posY, unsigned int hp)
{
    std::stringstream ss;
    ss << "[ SECTOR : " << sectorPosX << ", " << sectorPosY << " ] [ direction : " << direction << " ] [ curPos : " << posX << ", " << posY << " ] [ hp : " << hp << " ]";

    return ss.str();
}

bool PacketProc(CSession* pSession, PACKET_TYPE packetType, CPacket* pPacket)
{
    //pSession->pObj->m_packetQueue.enqueue(std::make_tuple(packetType, timerManager.GetCurrServerTime(), pSession->SessionID));

    switch (packetType)
    {
    case PACKET_TYPE::CS_MOVE_START:
    {
        UINT8 Direction;
        UINT16 X;
        UINT16 Y;

        *pPacket >> Direction;
        *pPacket >> X;
        *pPacket >> Y;

        return CS_MOVE_START(pSession, Direction, X, Y);
    }
    break;
    case PACKET_TYPE::CS_MOVE_STOP:
    {
        UINT8 Direction;
        UINT16 X;
        UINT16 Y;

        *pPacket >> Direction;
        *pPacket >> X;
        *pPacket >> Y;

        return CS_MOVE_STOP(pSession, Direction, X, Y);
    }
    break;
    case PACKET_TYPE::CS_ATTACK1:
    {
        UINT8 Direction;
        UINT16 X;
        UINT16 Y;

        *pPacket >> Direction;
        *pPacket >> X;
        *pPacket >> Y;

        return CS_ATTACK1(pSession, Direction, X, Y);
    }
    break;
    case PACKET_TYPE::CS_ATTACK2:
    {
        UINT8 Direction;
        UINT16 X;
        UINT16 Y;

        *pPacket >> Direction;
        *pPacket >> X;
        *pPacket >> Y;

        return CS_ATTACK2(pSession, Direction, X, Y);
    }
    break;
    case PACKET_TYPE::CS_ATTACK3:
    {
        UINT8 Direction;
        UINT16 X;
        UINT16 Y;

        *pPacket >> Direction;
        *pPacket >> X;
        *pPacket >> Y;

        return CS_ATTACK3(pSession, Direction, X, Y);
    }
    break;
    case PACKET_TYPE::CS_ECHO:
    {
        UINT32		Time;

        *pPacket >> Time;

        return CS_ECHO(pSession, Time);
    }
    break;
    case PACKET_TYPE::CS_SYNC:
    {
        UINT16 X;
        UINT16 Y;

        *pPacket >> X;
        *pPacket >> Y;

        return CS_SYNC(pSession, X, Y);
    }
    break;
    default:
        return false;
        break;
    }
    return false;
}

bool CS_MOVE_START(CSession* pSession, UINT8 direction, UINT16 x, UINT16 y)
{
    // �޽��� ���� �α� Ȯ��
    // ==========================================================================================================
    // ������ ��ġ�� ���� ��Ŷ�� ��ġ���� �ʹ� ū ���̰� ���ٸ� �������                           .
    // �� ������ ��ǥ ����ȭ ������ �ܼ��� Ű���� ���� (Ŭ����Ʈ�� ��ó��, ������ �� �ݿ�) �������
    // Ŭ���̾�Ʈ�� ��ǥ�� �״�� �ϴ� ����� ���ϰ� ����.
    // ���� �¶��� �����̶�� Ŭ���̾�Ʈ���� �������� �����ϴ� ����� ���ؾ���.
    // ������ ������ ������ �������� �ϰ� �����Ƿ� �������� ������ Ŭ���̾�Ʈ ��ǥ�� �ϵ��� �Ѵ�.
    // ==========================================================================================================

    CPlayer* pPlayer = static_cast<CPlayer*>(pSession->pObj);

    UINT16 posX, posY;
    pPlayer->getPosition(posX, posY);


    
    pSession->debugLogQueue.enqueue(std::make_tuple(
        pSession->pObj->m_curSectorPos.x,
        pSession->pObj->m_curSectorPos.y,
        direction,
        x,
        y,
        static_cast<CPlayer*>(pSession->pObj)->GetHp()
    ));




    // Ŭ���̾�Ʈ�� ��ǥ�� ������ �߸��Ǿ����� ������ ��ǥ�� �´ٰ� �����ϱ� ������ ��ũ �޽����� ������ �����ϴ� ��ǥ�� �־� Ŭ�� ���߰� �Ѵ�.
    if (
        std::abs(posX - x) > dfERROR_RANGE ||
        std::abs(posY - y) > dfERROR_RANGE
        )
    {
        // Ŭ�󿡰� �������� �÷��̾��� ��ġ�� ������.
        SC_SYNC_FOR_SINGLE(pSession, pPlayer->m_ID, posX, posY); 
        g_iSyncCount++;

        // ��ũ�� �� ������ ã�� ���� ���͸� ����
        timerManager.PrintJitterStats();

        // ������ Ŭ�󿡼� �÷��̾��� ��ġ�� ������ �� ��� ��ũ�� ������ Ȯ��.
        logManager.LogDebug(
            "Player\n"
            "Server\nPos : ", posX, ", ", posY, "  Direction : ", static_cast<int>(pPlayer->GetDirection()), "\n"
            "Client\nPos : ", x, ", ", y, "  Direction : ", static_cast<int>(direction), "\n"
            "Pre Sector Index : ", pPlayer->m_preSectorPos.x, ", ", pPlayer->m_preSectorPos.x, "\n"
            "Cur Sector Index : ", pPlayer->m_curSectorPos.x, ", ", pPlayer->m_curSectorPos.x, "\n"
            "Cur Sector PlayerCnt : ", sectorManager.GetSectorInfo(pPlayer->m_curSectorPos.x, pPlayer->m_curSectorPos.y)->GetSectorObjectMap().size(), "\n\n"
        );
    }
    // �״��� ������ ũ�� ���� ��� Ŭ���� ��ġ�� ������ �Ͼ��ش�.
    else
    {
        // ==========================================================================================================
        // ������ ����. ���� �������� ���۹�ȣ�� ���Ⱚ. ���ο��� �ٶ󺸴� ���⵵ ����
        // ==========================================================================================================
        pPlayer->SetPosition(x, y);
        pPlayer->SetDirection(direction);
    }


    // ==========================================================================================================
    // ����ڸ� ������, ���� �������� ��� ����ڿ��� ��Ŷ�� �Ѹ�.
    // ==========================================================================================================

    // ���� ������Ʈ�� ��ġ�� �������� ����
    CSector* pCurSector = sectorManager.GetSectorInfo(pPlayer->m_curSectorPos.x, pPlayer->m_curSectorPos.y);
    SC_MOVE_START_FOR_AROUND(pSession, pCurSector, pPlayer->m_ID, pPlayer->GetDirection(), x, y);

    //=====================================================================================================================================
    // �̵� ���� ������ �˸�
    //=====================================================================================================================================
    pPlayer->SetFlag(FLAG_MOVING, true);

    return true;
}

bool CS_MOVE_STOP(CSession* pSession, UINT8 direction, UINT16 x, UINT16 y)
{
    // ���� ���õ� Ŭ���̾�Ʈ�� �������� �������� ���� ���̶� ��û
    // 1. ���� ������ ó��
    // 2. PACKET_SC_MOVE_STOP �� ��ε�ĳ����
    // 3. ���� ������ �̵� ���� ������ �˸�

    CPlayer* pPlayer = static_cast<CPlayer*>(pSession->pObj);

    UINT16 posX, posY;
    pPlayer->getPosition(posX, posY);


    pSession->debugLogQueue.enqueue(std::make_tuple(
        pSession->pObj->m_curSectorPos.x,
        pSession->pObj->m_curSectorPos.y,
        direction,
        x,
        y,
        static_cast<CPlayer*>(pSession->pObj)->GetHp()
    ));




    // Ŭ���̾�Ʈ�� ��ǥ�� ������ �߸��Ǿ����� ������ ��ǥ�� �´ٰ� �����ϱ� ������ ��ũ �޽����� ������ �����ϴ� ��ǥ�� �־� Ŭ�� ���߰� �Ѵ�.
    if (
        std::abs(posX - x) > dfERROR_RANGE ||
        std::abs(posY - y) > dfERROR_RANGE
        )
    {
        // Ŭ�󿡰� �������� �÷��̾��� ��ġ�� ������.
        SC_SYNC_FOR_SINGLE(pSession, pPlayer->m_ID, posX, posY);
        g_iSyncCount++;

        // ��ũ�� �� ������ ã�� ���� ���͸� ����
        timerManager.PrintJitterStats();

        // ������ Ŭ�󿡼� �÷��̾��� ��ġ�� ������ �� ��� ��ũ�� ������ Ȯ��.
        logManager.LogDebug(
            "Player\n"
            "Server\nPos : ", posX, ", ", posY, "  Direction : ", static_cast<int>(pPlayer->GetDirection()), "\n"
            "Client\nPos : ", x, ", ", y, "  Direction : ", static_cast<int>(direction), "\n"
            "Pre Sector Index : ", pPlayer->m_preSectorPos.x, ", ", pPlayer->m_preSectorPos.x, "\n"
            "Cur Sector Index : ", pPlayer->m_curSectorPos.x, ", ", pPlayer->m_curSectorPos.x, "\n"
            "Cur Sector PlayerCnt : ", sectorManager.GetSectorInfo(pPlayer->m_curSectorPos.x, pPlayer->m_curSectorPos.y)->GetSectorObjectMap().size(), "\n\n"
        );
    }
    // �״��� ������ ũ�� ���� ��� Ŭ���� ��ġ�� ������ �Ͼ��ش�.
    else
    {
        //=====================================================================================================================================
        // 1. ���� ������ ó��
        //=====================================================================================================================================
        // Ŭ���� ��ġ�� �������� ���߱� ���� 
        pPlayer->SetDirection(direction);
        pPlayer->SetPosition(x, y);
    }

    //=====================================================================================================================================
    // 2. PACKET_SC_MOVE_STOP �� ��ε�ĳ����
    //=====================================================================================================================================

    // ���� ������Ʈ�� ��ġ�� �������� ����
    CSector* pCurSector = sectorManager.GetSectorInfo(pPlayer->m_curSectorPos.x, pPlayer->m_curSectorPos.y);
    SC_MOVE_STOP_FOR_AROUND(pSession, pCurSector, pPlayer->m_ID, pPlayer->GetDirection(), x, y);

    //=====================================================================================================================================
    // 3. ���� ������ �̵� ���� ������ �˸�
    //=====================================================================================================================================
    pPlayer->SetFlag(FLAG_MOVING, false);

    return true;
}

bool CS_ATTACK1(CSession* pSession, UINT8 direction, UINT16 x, UINT16 y)
{
    // Ŭ���̾�Ʈ�� ���� ���� �޽����� ����.
    // g_clientList�� ��ȸ�ϸ� ���� 1�� ������ �����ؼ� �������� �־���.
    // 1. dfPACKET_SC_ATTACK1 �� ��ε�ĳ����
    // 2. ���ݹ��� ĳ���͸� �˻�. �˻��� �����ϸ� 3, 4�� ���� ����
    // 3. dfPACKET_SC_DAMAGE �� ��ε�ĳ����
    // 4. ���� ü���� 0 ���Ϸ� �������ٸ� dfPACKET_SC_DELETE_CHARACTER �� ��ε�ĳ�����ϰ�, �������� ������ �� �ֵ��� �� -> �� �κ��� �������� ó���ϵ��� �ٲ�.

    CPlayer* pPlayer = static_cast<CPlayer*>(pSession->pObj);

    //=====================================================================================================================================
    // 1. dfPACKET_SC_ATTACK1 �� ��ε�ĳ����
    //=====================================================================================================================================
    //pPlayer->SetPosition(x, y);

    CSector* pCurSector = sectorManager.GetSectorInfo(pPlayer->m_curSectorPos.x, pPlayer->m_curSectorPos.y);
    SC_ATTACK1_FOR_AROUND(pSession, pCurSector, pPlayer->m_ID, pPlayer->GetDirection(), x, y);

    //=====================================================================================================================================
    // 2. ���ݹ��� ĳ���͸� �˻�. �˻��� �����ϸ� 3, 4�� ���� ����
    //=====================================================================================================================================

    // ���� �ٶ󺸴� ���⿡ ���� ���� ������ �޶���.
    UINT16 left, right, top, bottom;
    UINT16 posX, posY;
    pPlayer->getPosition(posX, posY);

    // ������ �ٶ󺸰� �־��ٸ�
    if (pPlayer->GetFacingDirection() == dfPACKET_MOVE_DIR_LL)
    {
        left = posX - dfATTACK1_RANGE_X;
        right = posX;
    }
    // �������� �ٶ󺸰� �־��ٸ�
    else
    {
        left = posX;
        right = posX + dfATTACK1_RANGE_X;
    }

    top = posY - dfATTACK1_RANGE_Y;
    bottom = posY + dfATTACK1_RANGE_Y;

    CPlayer* pOtherPlayer;
   
    for (auto& Sector : pCurSector->GetAroundSectorList())
    {
        bool bBreak = false;
        for (auto& Object : Sector->GetSectorObjectMap())
        {
            pOtherPlayer = static_cast<CPlayer*>(Object.second);

            if (pPlayer == pOtherPlayer)
                continue;

            pOtherPlayer->getPosition(posX, posY);

            // �ٸ� �÷��̾��� ��ǥ�� ���� ������ ���� ���
            if (posX >= left && posX <= right &&
                posY >= top && posY <= bottom)
            {
                //=====================================================================================================================================
                // 3. dfPACKET_SC_DAMAGE �� ��ε�ĳ����
                //=====================================================================================================================================
                // 1�� �������� �Ե��� ��
                pOtherPlayer->Damaged(dfATTACK1_DAMAGE);

                SC_DAMAGE_FOR_AROUND(nullptr, pCurSector, pSession->pObj->m_ID, pOtherPlayer->m_ID, pOtherPlayer->GetHp());
                bBreak = true;
                break;
            }
        }

        if (bBreak)
            break;
    }

    return true;
}

bool CS_ATTACK2(CSession* pSession, UINT8 direction, UINT16 x, UINT16 y)
{
    // Ŭ���̾�Ʈ�� ���� ���� �޽����� ����.
    // g_clientList�� ��ȸ�ϸ� ���� 2�� ������ �����ؼ� �������� �־���.
    // 1. dfPACKET_SC_ATTACK2 �� ��ε�ĳ����
    // 2. ���ݹ��� ĳ���͸� �˻�. �˻��� �����ϸ� 3, 4�� ���� ����
    // 3. dfPACKET_SC_DAMAGE �� ��ε�ĳ����
    // 4. ���� ü���� 0 ���Ϸ� �������ٸ� dfPACKET_SC_DELETE_CHARACTER �� ��ε�ĳ�����ϰ�, �������� ������ �� �ֵ��� ��

    CPlayer* pPlayer = static_cast<CPlayer*>(pSession->pObj);

    //=====================================================================================================================================
    // 1. dfPACKET_SC_ATTACK2 �� ��ε�ĳ����
    //=====================================================================================================================================
    //pPlayer->SetPosition(x, y);

    CSector* pCurSector = sectorManager.GetSectorInfo(pPlayer->m_curSectorPos.x, pPlayer->m_curSectorPos.y);
    SC_ATTACK2_FOR_AROUND(pSession, pCurSector, pPlayer->m_ID, pPlayer->GetDirection(), x, y);

    //=====================================================================================================================================
    // 2. ���ݹ��� ĳ���͸� �˻�. �˻��� �����ϸ� 3, 4�� ���� ����
    //=====================================================================================================================================

    // ���� �ٶ󺸴� ���⿡ ���� ���� ������ �޶���.
    UINT16 left, right, top, bottom;
    UINT16 posX, posY;
    pPlayer->getPosition(posX, posY);

    // ������ �ٶ󺸰� �־��ٸ�
    if (pPlayer->GetFacingDirection() == dfPACKET_MOVE_DIR_LL)
    {
        left = posX - dfATTACK2_RANGE_X;
        right = posX;
    }
    // �������� �ٶ󺸰� �־��ٸ�
    else
    {
        left = posX;
        right = posX + dfATTACK2_RANGE_X;
    }

    top = posY - dfATTACK2_RANGE_Y;
    bottom = posY + dfATTACK2_RANGE_Y;

    CPlayer* pOtherPlayer;
    for (auto& Sector : pCurSector->GetAroundSectorList())
    {
        bool bBreak = false;
        for (auto& Object : Sector->GetSectorObjectMap())
        {
            pOtherPlayer = static_cast<CPlayer*>(Object.second);

            if (pPlayer == pOtherPlayer)
                continue;

            pOtherPlayer->getPosition(posX, posY);

            // �ٸ� �÷��̾��� ��ǥ�� ���� ������ ���� ���
            if (posX >= left && posX <= right &&
                posY >= top && posY <= bottom)
            {
                //=====================================================================================================================================
                // 3. dfPACKET_SC_DAMAGE �� ��ε�ĳ����
                //=====================================================================================================================================
                // 1�� �������� �Ե��� ��
                pOtherPlayer->Damaged(dfATTACK2_DAMAGE);

                SC_DAMAGE_FOR_AROUND(nullptr, pCurSector, pSession->pObj->m_ID, pOtherPlayer->m_ID, pOtherPlayer->GetHp());
                bBreak = true;
                break;
            }
        }

        if (bBreak)
            break;
    }

    return true;
}

bool CS_ATTACK3(CSession* pSession, UINT8 direction, UINT16 x, UINT16 y)
{
    // Ŭ���̾�Ʈ�� ���� ���� �޽����� ����.
    // g_clientList�� ��ȸ�ϸ� ���� 3�� ������ �����ؼ� �������� �־���.
    // 1. dfPACKET_SC_ATTACK3 �� ��ε�ĳ����
    // 2. ���ݹ��� ĳ���͸� �˻�. �˻��� �����ϸ� 3, 4�� ���� ����
    // 3. dfPACKET_SC_DAMAGE �� ��ε�ĳ����
    // 4. ���� ü���� 0 ���Ϸ� �������ٸ� dfPACKET_SC_DELETE_CHARACTER �� ��ε�ĳ�����ϰ�, �������� ������ �� �ֵ��� ��

    CPlayer* pPlayer = static_cast<CPlayer*>(pSession->pObj);

    //=====================================================================================================================================
    // 1. dfPACKET_SC_ATTACK3 �� ��ε�ĳ����
    //=====================================================================================================================================
    //pPlayer->SetPosition(x, y);

    CSector* pCurSector = sectorManager.GetSectorInfo(pPlayer->m_curSectorPos.x, pPlayer->m_curSectorPos.y);
    SC_ATTACK3_FOR_AROUND(pSession, pCurSector, pPlayer->m_ID, pPlayer->GetDirection(), x, y);

    //=====================================================================================================================================
    // 2. ���ݹ��� ĳ���͸� �˻�. �˻��� �����ϸ� 3, 4�� ���� ����
    //=====================================================================================================================================

    // ���� �ٶ󺸴� ���⿡ ���� ���� ������ �޶���.
    UINT16 left, right, top, bottom;
    UINT16 posX, posY;
    pPlayer->getPosition(posX, posY);

    // ������ �ٶ󺸰� �־��ٸ�
    if (pPlayer->GetFacingDirection() == dfPACKET_MOVE_DIR_LL)
    {
        left = posX - dfATTACK3_RANGE_X;
        right = posX;
    }
    // �������� �ٶ󺸰� �־��ٸ�
    else
    {
        left = posX;
        right = posX + dfATTACK3_RANGE_X;
    }

    top = posY - dfATTACK3_RANGE_Y;
    bottom = posY + dfATTACK3_RANGE_Y;

    CPlayer* pOtherPlayer;
    for (auto& Sector : pCurSector->GetAroundSectorList())
    {
        bool bBreak = false;
        for (auto& Object : Sector->GetSectorObjectMap())
        {
            pOtherPlayer = static_cast<CPlayer*>(Object.second);

            if (pPlayer == pOtherPlayer)
                continue;

            pOtherPlayer->getPosition(posX, posY);

            // �ٸ� �÷��̾��� ��ǥ�� ���� ������ ���� ���
            if (posX >= left && posX <= right &&
                posY >= top && posY <= bottom)
            {
                //=====================================================================================================================================
                // 3. dfPACKET_SC_DAMAGE �� ��ε�ĳ����
                //=====================================================================================================================================
                // 1�� �������� �Ե��� ��
                pOtherPlayer->Damaged(dfATTACK3_DAMAGE);

                SC_DAMAGE_FOR_AROUND(nullptr, pCurSector, pSession->pObj->m_ID, pOtherPlayer->m_ID, pOtherPlayer->GetHp());
                bBreak = true;
                break;
            }
        }

        if (bBreak)
            break;
    }

    return true;
}

bool CS_ECHO(CSession* pSession, UINT32 time)
{
    SC_ECHO_FOR_SINGLE(pSession, time);

    return true;
}

bool CS_SYNC(CSession* pSession, UINT16 x, UINT16 y)
{
    // �ƹ��͵� ����

    return true;
}

void DisconnectSessionProc(CSession* pSession)
{
    // ������ ������ �ִ� ������Ʈ�� �������� �ʴ´ٸ�, ���ʿ� ������ ���� ��ü�� ���� ���� ���� ���̹Ƿ� �ѱ�
    if (pSession->pObj == nullptr)
        return;

    CPlayer* pPlayer = static_cast<CPlayer*>(pSession->pObj);

    CSector* pCurSector = sectorManager.GetSectorInfo(pPlayer->m_curSectorPos.x, pPlayer->m_curSectorPos.y);
    SC_DELETE_CHARACTER_FOR_AROUND(nullptr, pCurSector, pPlayer->m_ID);

    sectorManager.DeleteObjectFromSector(pSession->pObj); // ���� �Ŵ������� ����
    objectManager.DeleteObject(pSession->pObj); // ������Ʈ �Ŵ������� ����

    playerPool.Free(static_cast<CPlayer*>(pSession->pObj)); // �÷��̾� ����
}
