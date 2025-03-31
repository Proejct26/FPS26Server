#pragma once

#include "pch.h"
#include "Object.h"
#include "DamageTracker.h"
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
    explicit CPlayer(UINT16 _x = 0, UINT16 _y = 0, UINT16 _z = 0, UINT8 _hp = 0) noexcept;
    virtual ~CPlayer();

    // ��Ÿ ��� �Լ� ����
    virtual void Update(void) override;
    virtual void LateUpdate(void) override;

    void Init(UINT16 _x, UINT16 _y, UINT16 _z, UINT8 _hp);

public:
    int GetId() const { return m_ID; }
    int GetRoomId() const { return m_roomId; }
    int GetTeamId() const { return m_teamId; }

    void SetRoomId(int roomId) { m_roomId = roomId; }
    void SetTeamId(int teamId) { m_teamId = teamId; }

    void AddKill() { ++m_kdaInfo.kill; }
    void AddDeath() { ++m_kdaInfo.death; }
    void AddAssist() { ++m_kdaInfo.assist; }

    int GetKill() const { return m_kdaInfo.kill; }
    int GetDeath() const { return m_kdaInfo.death; }
    int GetAssist() const { return m_kdaInfo.assist; }
    void GetKDAInfo(KDAInfo& info) const { info = m_kdaInfo; }

    void RecordDamage(int attackerId, float nowTime) { m_damageTracker.RecordDamage(attackerId, nowTime); }
    void UpdateDamageHistory(float nowTime) { m_damageTracker.Update(nowTime); }
    std::vector<int> GetAssistCandidates(float nowTime) const { return m_damageTracker.GetAssistCandidates(nowTime); }

public:
    PLAYER_GAME_STATE GetCurGameState(void) { return m_eCurPlayerGameState; }
    void SetCurGameState(PLAYER_GAME_STATE eState) { m_eCurPlayerGameState = eState; }

public:
    std::array<bool, (int)PRESS_KEY::END> GetMovementKeys() const;
    void SetMovementKeys(const std::array<bool, (int)PRESS_KEY::END>& keys);

public:
    void GetRotationAxisXY(UINT32& AxisX, UINT32& AxisY) const { AxisX = m_rotationAxisX; AxisY = m_rotationAxisY; }
    void SetRotationAxisXY(UINT32 AxisX, UINT32 AxisY) { m_rotationAxisX = AxisX, m_rotationAxisY = AxisY; }

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
    UINT8 m_curHp;          // ü��
    UINT8 m_maxHp;

    UINT32 m_rotationAxisX;      // x, y�� ȸ����
    UINT32 m_rotationAxisY;

    int m_roomId = -1;
    int m_teamId = -1;

    KDAInfo m_kdaInfo;

    UINT8 m_weaponInfo;     // ������ �ִ� ���� ��ȣ

    bool m_pressedKey[(int)PRESS_KEY::END];

    DamageTracker m_damageTracker;

    PLAYER_GAME_STATE m_eCurPlayerGameState;  // ���� ���� ����, ó���̳� �׾��� �� WAITING ����, ���� �÷��� ���̸� ACTIVE�� �����

    std::string m_playerName;
};