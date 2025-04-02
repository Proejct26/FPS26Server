#pragma once

#include "pch.h"
#include "Object.h"
//#include "DamageTracker.h"
#include <array>
#include "ProtoStruct.h"

void SendCreationPacketBetween(CObject*, CObject*);
void SendDestructionPacketBetween(CObject*, CObject*);

struct KDA {
    int kill;
    int death;
    int assist;
};

enum class PLAYER_GAME_STATE {
    WAITING,
    ACTIVE,
    END
};

enum class PRESS_KEY {
    W,
    A,
    S,
    D,
    JUMP,
    END
};

class CPlayer : public CObject {
public:
    explicit CPlayer(float _x = 0, float _y = 0, float _z = 0, UINT8 _hp = 0) noexcept;
    virtual ~CPlayer();

    // 기타 멤버 함수 선언
    virtual void Update(void) override;
    virtual void LateUpdate(void) override;

    void Init(float _x, float _y, float _z, UINT8 _hp);

public:
    int GetId() const { return m_ID; }
    int GetRoomId() const { return m_roomId; }
    int GetTeamId() const { return m_teamId; }
    int GetSpawnPosIndex() const { return m_spawnPosIndex; }
    UINT32 GetLastAttackedPlayerID() const { return m_lastAttackedPlayerID; }

    void SetRoomId(int roomId) { m_roomId = roomId; }
    void SetTeamId(int teamId) { m_teamId = teamId; }
    void SetSpawnPosIndex(int spawnPosIndex) { m_spawnPosIndex = spawnPosIndex; }
    void SetLastAttackedPlayerID(UINT32 lastAttackedPlayerID) { m_lastAttackedPlayerID = lastAttackedPlayerID; }

    void AddKill() { ++m_kdaInfo.kill; }
    void AddDeath() { ++m_kdaInfo.death; }
    void AddAssist() { ++m_kdaInfo.assist; }

    int GetKill() const { return m_kdaInfo.kill; }
    int GetDeath() const { return m_kdaInfo.death; }
    int GetAssist() const { return m_kdaInfo.assist; }
    void GetKDAInfo(KDAInfo& info) const { info = m_kdaInfo; }

    //void RecordDamage(int attackerId, float nowTime) { m_damageTracker.RecordDamage(attackerId, nowTime); }
    //void UpdateDamageHistory(float nowTime) { m_damageTracker.Update(nowTime); }
    //std::vector<int> GetAssistCandidates(float nowTime) const { return m_damageTracker.GetAssistCandidates(nowTime); }

public:
    PLAYER_GAME_STATE GetCurGameState(void) { return m_eCurPlayerGameState; }
    void SetCurGameState(PLAYER_GAME_STATE eState) { m_eCurPlayerGameState = eState; }

public:
    std::array<bool, (int)PRESS_KEY::END> GetMovementKeys() const;
    void SetMovementKeys(const std::array<bool, (int)PRESS_KEY::END>& keys);

public:
    void GetRotationAxisXY(float& AxisX, float& AxisY) const { AxisX = m_rotationAxisX; AxisY = m_rotationAxisY; }
    void SetRotationAxisXY(float AxisX, float AxisY) { m_rotationAxisX = AxisX, m_rotationAxisY = AxisY; }

public:
    UINT8 GetCurHp(void) { return m_curHp; }
    UINT8 GetMaxHp(void) { return m_maxHp; }
    void SetCurHp(UINT8 curHp) { m_curHp = curHp; }

public:
    const std::string& GetName(void) { return m_playerName; }
    void SetName(const std::string& name) { m_playerName = name; }

public:
    UINT8 GetWeaponInfo(void) { return m_weaponInfo; }
    void SetWeaponInfo(UINT8 weapon) { m_weaponInfo = weapon; }

private:
    UINT8 m_curHp = 0;          // 체력
    UINT8 m_maxHp = 0;

    float m_rotationAxisX = 0;      // x, y축 회전값
    float m_rotationAxisY = 0;

    int m_roomId = -1;
    int m_teamId = -1;
    int m_spawnPosIndex = -1;

    KDAInfo m_kdaInfo;

    UINT8 m_weaponInfo = 0;     // 가지고 있는 무기 번호

    bool m_pressedKey[(int)PRESS_KEY::END];

    // 데미지 트레커 기능은 비활성화 대신 마지막에 같이 때린 플레이어 정보를 전송
    // DamageTracker m_damageTracker;
    UINT32 m_lastAttackedPlayerID = 0;  // 가장 최근에 나를 공격한 플레이어 ID
                                        // 초기값은 0이고 플레이어 id는 1부터 시작하므로, 0이면 아무에게도 아직 맞지 않은 상태

    PLAYER_GAME_STATE m_eCurPlayerGameState;  // 시작 관련 정보, 처음이나 죽었을 때 WAITING 상태, 게임 플레이 중이면 ACTIVE로 변경됨

    std::string m_playerName;
};