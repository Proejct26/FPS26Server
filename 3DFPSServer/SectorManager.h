#pragma once

#include "Singleton.h"

class CSector;
class CObject;

using CreateSectorObjectCallback = void(*)(CObject*, CObject*);
using DeleteSectorObjectCallback = void(*)(CObject*, CObject*);

class CSectorManager : public SingletonBase<CSectorManager> {
private:
    friend class SingletonBase<CSectorManager>;

public:
    explicit CSectorManager() noexcept;
    ~CSectorManager() noexcept;

    // ���� �����ڿ� ���� �����ڸ� �����Ͽ� ���� ����
    CSectorManager(const CSectorManager&) = delete;
    CSectorManager& operator=(const CSectorManager&) = delete;

public:
    CSector* GetSectorInfo(UINT8 posX, UINT8 poxY) { return m_Sectors[poxY][posX]; }

    // ��ġ ������ ���ڷ� �޾� ���� �ִ� ������ �ε����� ��ȯ�ϴ� �Լ�
    static std::pair<UINT8, UINT8> GetSectorIndexFromWorldCoords(UINT16 prePosX, UINT16 prePosY);

    // ���͸� �̵��� �� �߰��Ǿ���ϴ� ���Ϳ� �Ҹ�Ǿ���ϴ� ���� ��긦 ���
    void CalculateSectorChanges(CObject* pObject);

    // �Ҹ�Ǿ���ϴ� ���͵��� ������ �ִ� ������Ʈ�鿡�� �� ������Ʈ ���� ���� ��Ŷ�� ������, �� ������Ʈ���� �ش� ���͵��� ������ �ִ� ������Ʈ�� ���� ��Ŷ�� ����
    // �����Ǿ���ϴ� ���͵��� ������ �ִ� ������Ʈ�鿡�� �� ������Ʈ ���� ���� ��Ŷ�� ������, �� ������Ʈ���� �ش� ���͵��� ������ �ִ� ������Ʈ�� ���� ��Ŷ�� ����
    // ���� ��Ŷ�� ���� �� �� ������Ʈ�� ���µ� �Բ� ������. ���������� ������ �Ǿ���ϴ� ������Ʈ�� ������ ���� �� �ش� ������Ʈ���� ���·� ���� ����.
    void ProcessSectorObjectPackets(CObject* pObject);

public:
    void RegisterObjectToSector(CObject* pObject);
    void DeleteObjectFromSector(CObject* pObject);

public:
    static void RegisterCreateSectorObjectCallback(CreateSectorObjectCallback callback) { m_callbackCreateSector = callback; }
    static void RegisterDeleteSectorObjectCallback(DeleteSectorObjectCallback callback) { m_callbackDeleteSector = callback; }

private:
    // ���� ����
    CSector* m_Sectors[dfSECTOR_HEIGHT_CNT][dfSECTOR_WIDTH_CNT];

    // �̵� ����� �����ǰų� �߰��Ǿ���� ������ �ε��� ����
    std::vector<POINT> m_sectorsToDelete;
    std::vector<POINT> m_sectorsToAdd;

    static UINT16 sectorWidth;
    static UINT16 sectorHeight;

    static CreateSectorObjectCallback m_callbackCreateSector;
    static DeleteSectorObjectCallback m_callbackDeleteSector;
};


