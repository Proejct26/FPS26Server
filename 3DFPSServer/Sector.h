#pragma once

class CObject;
class CRingBuffer;

class CSector
{
public:
	// ���ڷ� ��ü ���Ϳ��� ��ġ�� �ε����� ����
	explicit CSector(UINT8 Width, UINT8 Height) noexcept;
	~CSector(void);

public:
	inline std::unordered_map<UINT32, CObject*>& GetSectorObjectMap(void) { return m_sectorObjectMap; }
	void RegisterObject(CObject* pObject);
	void DeleteObject(CObject* pObject);

public:
	const std::list<CSector*>& GetAroundSectorList(void) { return m_aroundSectorlist; }

public:
	void InsertAroundSector(CSector* pSector) { m_aroundSectorlist.push_back(pSector); }

private:
	// ���ͺ��� ��� �ִ� ������Ʈ ����
	std::unordered_map<UINT32, CObject*> m_sectorObjectMap;

private:
	// UINT8�̱� ������ �ε��� ���� 0 ~ 255������ ���� �� �ִ�. �߰��� �����÷ο� ����
	UINT8 m_posX;
	UINT8 m_posY; 

private:
	std::list<CSector*> m_aroundSectorlist;	// ������ �ִ� ���� ����
};

