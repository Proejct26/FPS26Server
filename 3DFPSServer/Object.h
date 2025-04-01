#pragma once

class CSession;
class CPacket;

#include "CircularQueue.h"

#include "NetIOManager.h"

// 기본 CObject 클래스 정의
class CObject {
public:
    explicit CObject(float _x = 0, float _y = 0, float _z = 0) noexcept;
    virtual ~CObject() = default;

    // 메모리 풀에서 사용할 용도
    virtual void Init(void);

    // 업데이트 함수 (서버 로직에서 오브젝트 상태를 갱신하는 데 사용)
    virtual void Update(void);
    virtual void LateUpdate(void);

    // 움직임 관련 함수
    virtual void Move(void);


public:
    // 위치 설정 및 가져오기
    constexpr void SetPosition(float _x, float _y, float _z) {
        m_x = _x;
        m_y = _y;
        m_z = _z;
    }

    constexpr void getPosition(float& _x, float& _y, float& _z) const {
        _x = m_x;
        _y = m_y;
        _z = m_z;
    }

public:
    inline bool isDead(void) { return m_bDead; }
    
private:
    void CheckTimeout(void);    // 추기적으로 Timeout을 확인하기 위해 불러주는 함수
    void SetCurTimeout(void);   // Timeout 값을 설정하는 함수. 패킷 처리할 때 마다 호출해서 Timeout 갱신
    friend bool PacketProc(CSession* pSession, game::PacketID packetType, CPacket* pPacket);
    friend void CNetIOManager::netProc_Recv(CSession* pSession);

public:
    CSession* m_pSession;   // 오브젝트와 연결된 세션
    UINT32 m_ID;            // ID

public:
    float m_x, m_y, m_z;   // 현재 위치(3차원)
    bool m_bDead;           // 죽었는지 여부

private:
    // Timeout 관련
    UINT32 m_lastTimeoutCheckTime; // timegettime 기준 값

private:
    static UINT32 g_ID; // ID
    static UINT32 m_maxLastTimeoutCheckTime; // timegettime 기준 값
};