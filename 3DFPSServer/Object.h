#pragma once

class CSession;
class CPacket;

#include "CircularQueue.h"

#include "NetIOManager.h"

// �⺻ CObject Ŭ���� ����
class CObject {
public:
    explicit CObject(UINT16 _x = 0, UINT16 _y = 0) noexcept;
    virtual ~CObject() = default;

    // �޸� Ǯ���� ����� �뵵
    virtual void Init(void);

    // ������Ʈ �Լ� (���� �������� ������Ʈ ���¸� �����ϴ� �� ���)
    virtual void Update(void);
    virtual void LateUpdate(void);

    // ������ ���� �Լ�
    virtual void Move(void);


public:
    // ��ġ ���� �� ��������
    constexpr void SetPosition(UINT16 _x, UINT16 _y) {
        m_x = _x;
        m_y = _y;
    }

    constexpr void getPosition(UINT16& _x, UINT16& _y) const {
        _x = m_x;
        _y = m_y;
    }

public:
    inline bool isDead(void) { return m_bDead; }
    
private:
    void CheckTimeout(void);    // �߱������� Timeout�� Ȯ���ϱ� ���� �ҷ��ִ� �Լ�
    void SetCurTimeout(void);   // Timeout ���� �����ϴ� �Լ�. ��Ŷ ó���� �� ���� ȣ���ؼ� Timeout ����
    friend bool PacketProc(CSession* pSession, PACKET_TYPE packetType, CPacket* pPacket);
    friend void CNetIOManager::netProc_Recv(CSession* pSession);

public:
    CSession* m_pSession;
    UINT32 m_ID; // ID
    POINT m_preSectorPos;
    POINT m_curSectorPos;

public:
    UINT16 m_x, m_y;
    bool m_bDead;

private:
    // Timeout ����
    UINT32 m_lastTimeoutCheckTime; // timegettime ���� ��

private:
    static UINT32 g_ID; // ID
    static UINT32 m_maxLastTimeoutCheckTime; // timegettime ���� ��

public:
    // ��Ŷ Ÿ��, ��Ŷ�� �޴� �ð�, ���� ID
    CircularQueue<std::tuple<PACKET_TYPE, DWORD, UINT32>> m_packetQueue;
};