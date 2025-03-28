
#include "pch.h"

#include "SectorManager.h"
#include "Sector.h"

#include "ObjectManager.h"
#include "Object.h"

#include "SessionManager.h"

UINT16 CSectorManager::sectorWidth = 0;
UINT16 CSectorManager::sectorHeight = 0;

void (*CSectorManager::m_callbackCreateSector)(CObject*, CObject*) = nullptr; // �ʱ�ȭ
void (*CSectorManager::m_callbackDeleteSector)(CObject*, CObject*) = nullptr; // �ʱ�ȭ

int direction[8][2] = {
    {-1, 0},	// LL
    {-1, -1},	// LU
    {0, -1},	// UU
    {1, -1},	// RU
    {1, 0},		// RR
    {1, 1},		// RD
    {0, 1},		// DD
    {-1, 1}	    // LD
};

CSectorManager::CSectorManager() noexcept
{
	for (UINT8 i = 0; i < dfSECTOR_HEIGHT_CNT; ++i)
	{
		for (UINT8 j = 0; j < dfSECTOR_WIDTH_CNT; ++j)
		{
			m_Sectors[i][j] = new CSector(i, j);
		}
	}

    UINT8 posX, posY;
    for (UINT8 i = 0; i < dfSECTOR_HEIGHT_CNT; ++i)
    {
        for (UINT8 j = 0; j < dfSECTOR_WIDTH_CNT; ++j)
        {
            for (UINT8 k = 0; k < 8; ++k)
            {
                posX = j + direction[k][0];
                posY = i + direction[k][1];

                if (
                    (0 <= posX && posX < dfSECTOR_WIDTH_CNT) &&
                    (0 <= posY && posY < dfSECTOR_HEIGHT_CNT)
                    )
                {
                    m_Sectors[i][j]->InsertAroundSector(m_Sectors[posY][posX]);
                }
            }

            m_Sectors[i][j]->InsertAroundSector(m_Sectors[i][j]);
        }
    }

    // ���Ϳ� �� �ִ� ���� ���� ���� (�ڱ� ����)�� Ȯ��
    /*
    for (UINT8 i = 0; i < dfSECTOR_HEIGHT_CNT; ++i)
    {
        for (UINT8 j = 0; j < dfSECTOR_WIDTH_CNT; ++j)
        {
            std::cout << m_Sectors[i][j]->GetAroundSectorList().size();
        }
        std::cout << "\n";
    }
    */

	sectorWidth = (dfRANGE_MOVE_RIGHT - dfRANGE_MOVE_LEFT) / dfSECTOR_WIDTH_CNT;
	sectorHeight = (dfRANGE_MOVE_BOTTOM - dfRANGE_MOVE_TOP) / dfSECTOR_HEIGHT_CNT;

	m_sectorsToDelete.reserve(5);
	m_sectorsToAdd.reserve(5);
}

CSectorManager::~CSectorManager() noexcept
{
	for (UINT8 i = 0; i < dfSECTOR_HEIGHT_CNT; ++i)
	{
		for (UINT8 j = 0; j < dfSECTOR_WIDTH_CNT; ++j)
		{
			delete m_Sectors[i][j];
		}
	}
}

std::pair<UINT8, UINT8> CSectorManager::GetSectorIndexFromWorldCoords(UINT16 prePosX, UINT16 prePosY)
{
	return std::make_pair(prePosX / sectorWidth, prePosY / sectorHeight);
}

void CSectorManager::CalculateSectorChanges(CObject* pObject)
{
	// ������Ʈ�� ��ġ�� ����Ǿ��ٸ�
	if ((pObject->m_preSectorPos.x != pObject->m_curSectorPos.x) || (pObject->m_preSectorPos.y != pObject->m_curSectorPos.y))
	{
        m_sectorsToAdd.clear();
        m_sectorsToDelete.clear();

        int moveDirX = pObject->m_curSectorPos.x - pObject->m_preSectorPos.x;
        int moveDirY = pObject->m_curSectorPos.y - pObject->m_preSectorPos.y;

        // 65536... ���� ���� ���� ��ġ ������ �̸� �Ѿ�� �ʰ���?
        UINT16 x = pObject->m_preSectorPos.x;
        UINT16 y = pObject->m_preSectorPos.y;

        // �밢�� �̵�
        if (moveDirX != 0 && moveDirY != 0) { // �밢�� �̵�
            if (moveDirX == -1 && moveDirY == -1) { // �»�
                m_sectorsToDelete.push_back(POINT{ x + 1, y + 1 }); // ���ϴ�
                m_sectorsToDelete.push_back(POINT{ x + 1, y });     // ����
                m_sectorsToDelete.push_back(POINT{ x + 1, y - 1 }); // ����
                m_sectorsToDelete.push_back(POINT{ x, y + 1 });     // �ϴ�
                m_sectorsToDelete.push_back(POINT{ x - 1, y + 1 }); // ���ϴ�

                x = pObject->m_curSectorPos.x;
                y = pObject->m_curSectorPos.y;

                m_sectorsToAdd.push_back(POINT{ x - 1, y });     // ����
                m_sectorsToAdd.push_back(POINT{ x - 1, y + 1 }); // ���ϴ�
                m_sectorsToAdd.push_back(POINT{ x - 1, y - 1 }); // �»��
                m_sectorsToAdd.push_back(POINT{ x, y - 1 });     // ���
                m_sectorsToAdd.push_back(POINT{ x + 1, y - 1 }); // ����
            }
            else if (moveDirX == -1 && moveDirY == 1) { // ����
                m_sectorsToDelete.push_back(POINT{ x + 1, y - 1 }); // ����
                m_sectorsToDelete.push_back(POINT{ x + 1, y });     // ����
                m_sectorsToDelete.push_back(POINT{ x + 1, y + 1 }); // ���ϴ�
                m_sectorsToDelete.push_back(POINT{ x, y - 1 });     // ���
                m_sectorsToDelete.push_back(POINT{ x - 1, y - 1 }); // �»��

                x = pObject->m_curSectorPos.x;
                y = pObject->m_curSectorPos.y;

                m_sectorsToAdd.push_back(POINT{ x - 1, y });     // ����
                m_sectorsToAdd.push_back(POINT{ x - 1, y - 1 }); // �»��
                m_sectorsToAdd.push_back(POINT{ x - 1, y + 1 }); // ���ϴ�
                m_sectorsToAdd.push_back(POINT{ x, y + 1 });     // �ϴ�
                m_sectorsToAdd.push_back(POINT{ x + 1, y + 1 }); // ���ϴ�
            }
            else if (moveDirX == 1 && moveDirY == -1) { // ���
                m_sectorsToDelete.push_back(POINT{ x - 1, y + 1 }); // ���ϴ�
                m_sectorsToDelete.push_back(POINT{ x - 1, y });     // ����
                m_sectorsToDelete.push_back(POINT{ x - 1, y - 1 }); // �»��
                m_sectorsToDelete.push_back(POINT{ x, y + 1 });     // �ϴ�
                m_sectorsToDelete.push_back(POINT{ x + 1, y + 1 }); // ���ϴ�

                x = pObject->m_curSectorPos.x;
                y = pObject->m_curSectorPos.y;

                m_sectorsToAdd.push_back(POINT{ x + 1, y });     // ����
                m_sectorsToAdd.push_back(POINT{ x + 1, y - 1 }); // ����
                m_sectorsToAdd.push_back(POINT{ x + 1, y + 1 }); // ���ϴ�
                m_sectorsToAdd.push_back(POINT{ x, y - 1 });     // ���
                m_sectorsToAdd.push_back(POINT{ x - 1, y - 1 }); // �»��
            }
            else if (moveDirX == 1 && moveDirY == 1) { // ����
                m_sectorsToDelete.push_back(POINT{ x - 1, y - 1 }); // �»��
                m_sectorsToDelete.push_back(POINT{ x - 1, y });     // ����
                m_sectorsToDelete.push_back(POINT{ x - 1, y + 1 }); // ���ϴ�
                m_sectorsToDelete.push_back(POINT{ x, y - 1 });     // ���
                m_sectorsToDelete.push_back(POINT{ x + 1, y - 1 }); // ����

                x = pObject->m_curSectorPos.x;
                y = pObject->m_curSectorPos.y;

                m_sectorsToAdd.push_back(POINT{ x + 1, y });     // ����
                m_sectorsToAdd.push_back(POINT{ x + 1, y + 1 }); // ���ϴ�
                m_sectorsToAdd.push_back(POINT{ x, y + 1 });     // �ϴ�
                m_sectorsToAdd.push_back(POINT{ x - 1, y + 1 }); // ���ϴ�
                m_sectorsToAdd.push_back(POINT{ x + 1, y - 1 }); // ����
            }
        }
        // �����¿� �̵�
        else if (moveDirX != 0 || moveDirY != 0) {
            if (moveDirX == -1 && moveDirY == 0) { // ��
                m_sectorsToDelete.push_back(POINT{ x + 1, y - 1 }); // ����
                m_sectorsToDelete.push_back(POINT{ x + 1, y });     // ����
                m_sectorsToDelete.push_back(POINT{ x + 1, y + 1 }); // ���ϴ�

                x = pObject->m_curSectorPos.x;
                y = pObject->m_curSectorPos.y;

                m_sectorsToAdd.push_back(POINT{ x - 1, y - 1 }); // �»��
                m_sectorsToAdd.push_back(POINT{ x - 1, y });     // ����
                m_sectorsToAdd.push_back(POINT{ x - 1, y + 1 }); // ���ϴ�
            }
            else if (moveDirX == 1 && moveDirY == 0) { // ��
                m_sectorsToDelete.push_back(POINT{ x - 1, y - 1 }); // �»��
                m_sectorsToDelete.push_back(POINT{ x - 1, y });     // ����
                m_sectorsToDelete.push_back(POINT{ x - 1, y + 1 }); // ���ϴ�

                x = pObject->m_curSectorPos.x;
                y = pObject->m_curSectorPos.y;

                m_sectorsToAdd.push_back(POINT{ x + 1, y - 1 }); // ����
                m_sectorsToAdd.push_back(POINT{ x + 1, y });     // ����
                m_sectorsToAdd.push_back(POINT{ x + 1, y + 1 }); // ���ϴ�
            }
            else if (moveDirX == 0 && moveDirY == -1) { // ��
                m_sectorsToDelete.push_back(POINT{ x - 1, y + 1 }); // ���ϴ�
                m_sectorsToDelete.push_back(POINT{ x, y + 1 });     // �ϴ�
                m_sectorsToDelete.push_back(POINT{ x + 1, y + 1 }); // ���ϴ�

                x = pObject->m_curSectorPos.x;
                y = pObject->m_curSectorPos.y;

                m_sectorsToAdd.push_back(POINT{ x - 1, y - 1 }); // �»��
                m_sectorsToAdd.push_back(POINT{ x, y - 1 });     // ���
                m_sectorsToAdd.push_back(POINT{ x + 1, y - 1 }); // ����
            }
            else if (moveDirX == 0 && moveDirY == 1) { // �Ʒ�
                m_sectorsToDelete.push_back(POINT{ x - 1, y - 1 }); // �»��
                m_sectorsToDelete.push_back(POINT{ x, y - 1 });     // ���
                m_sectorsToDelete.push_back(POINT{ x + 1, y - 1 }); // ����

                x = pObject->m_curSectorPos.x;
                y = pObject->m_curSectorPos.y;

                m_sectorsToAdd.push_back(POINT{ x - 1, y + 1 }); // ���ϴ�
                m_sectorsToAdd.push_back(POINT{ x, y + 1 });     // �ϴ�
                m_sectorsToAdd.push_back(POINT{ x + 1, y + 1 }); // ���ϴ�
            }
        }

        // �׸��� ��踦 ����� �ʵ��� ���͸�
        m_sectorsToDelete.erase(std::remove_if(m_sectorsToDelete.begin(), m_sectorsToDelete.end(),
            [](const POINT& pos) {
                return pos.x < 0 || pos.x >= dfSECTOR_WIDTH_CNT || pos.y < 0 || pos.y >= dfSECTOR_HEIGHT_CNT;
            }), m_sectorsToDelete.end());

        m_sectorsToAdd.erase(std::remove_if(m_sectorsToAdd.begin(), m_sectorsToAdd.end(),
            [](const POINT& pos) {
                return pos.x < 0 || pos.x >= dfSECTOR_WIDTH_CNT || pos.y < 0 || pos.y >= dfSECTOR_HEIGHT_CNT;
            }), m_sectorsToAdd.end());

        // ������Ʈ ���� �� ���� ó��
        ProcessSectorObjectPackets(pObject);
	}
}

void CSectorManager::ProcessSectorObjectPackets(CObject* pMovingObject)
{
    CObject* pSectorObject;

    // �Ҹ�Ǿ���ϴ� ���͵��� ������ �ִ� ������Ʈ�鿡�� �� ������Ʈ ���� ���� ��Ŷ�� ������, �� ������Ʈ���� �ش� ���͵��� ������ �ִ� ������Ʈ�� ���� ��Ŷ�� ����
    for (auto& sector : m_sectorsToDelete)
    {
        for (auto& mapIndex : m_Sectors[sector.y][sector.x]->GetSectorObjectMap())
        {
            pSectorObject = mapIndex.second;

            // ���ο��� ���� ��Ŷ ������ ����.
            m_callbackDeleteSector(pMovingObject, pSectorObject);
        }
    }
    m_Sectors[pMovingObject->m_preSectorPos.y][pMovingObject->m_preSectorPos.x]->DeleteObject(pMovingObject);

    // �����Ǿ���ϴ� ���͵��� ������ �ִ� ������Ʈ�鿡�� �� ������Ʈ ���� ���� ��Ŷ�� ������, �� ������Ʈ���� �ش� ���͵��� ������ �ִ� ������Ʈ�� ���� ��Ŷ�� ����
    // ���� ��Ŷ�� ���� �� �� ������Ʈ�� ���µ� �Բ� ������. ���������� ������ �Ǿ���ϴ� ������Ʈ�� ������ ���� �� �ش� ������Ʈ���� ���·� ���� ����.
    for (auto& sector : m_sectorsToAdd)
    {
        for (auto& mapIndex : m_Sectors[sector.y][sector.x]->GetSectorObjectMap())
        {
            pSectorObject = mapIndex.second;

            // ���ο��� �������� ��Ŷ ������ ����.
            m_callbackCreateSector(pMovingObject, pSectorObject);
        }
    }
    m_Sectors[pMovingObject->m_curSectorPos.y][pMovingObject->m_curSectorPos.x]->RegisterObject(pMovingObject);
}

void CSectorManager::RegisterObjectToSector(CObject* pObject)
{
    // ���� ������Ʈ�� ������ �޾� ���Ϳ� ���
    UINT16 posX, posY;
    pObject->getPosition(posX, posY);
    std::pair<UINT8, UINT8> curIndex = CSectorManager::GetSectorIndexFromWorldCoords(posX, posY);
    pObject->m_curSectorPos.x = curIndex.first;
    pObject->m_curSectorPos.y = curIndex.second;

    m_Sectors[pObject->m_curSectorPos.y][pObject->m_curSectorPos.x]->RegisterObject(pObject);
}

void CSectorManager::DeleteObjectFromSector(CObject* pObject)
{
    // ���� ������Ʈ�� ������ �޾� ���Ϳ��� ����
    UINT16 posX, posY;
    m_Sectors[pObject->m_curSectorPos.y][pObject->m_curSectorPos.x]->DeleteObject(pObject);
}
