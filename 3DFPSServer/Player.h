#pragma once

#include "Object.h"

#define FLAG_MOVING         0x01
#define FLAG_DEAD           0x02

void SendCreationPacketBetween(CObject*, CObject*);
void SendDestructionPacketBetween(CObject*, CObject*);

class CPlayer : public CObject {
public:
    explicit CPlayer(UINT16 _x = 0, UINT16 _y = 0, UINT8 _direction = 0, UINT8 _hp = 0) noexcept;
    virtual ~CPlayer();

    // ��Ÿ ��� �Լ� ����
    virtual void Update(void) override;
    virtual void LateUpdate(void) override;
    virtual void Move(void) override;

public:
    void Init(UINT16 _x, UINT16 _y, UINT8 _direction, UINT8 _hp);

public:
    // ���߿� �÷��̾ ������ �ٸ� ������Ʈ�� �߰��Ǹ� �� �Լ����� CObject�� �ְ�, ��ӹ��� class���� �������̵��ؼ� ���� ������ �ۼ��ϸ� �ȴ�.
    // �ϴ� �÷��̾ ������ �̷��� �ۼ�.
    friend void SendCreationPacketBetween(CObject*, CObject*);
    friend void SendDestructionPacketBetween(CObject*, CObject*);

public:
    // Getter �Լ�
    constexpr UINT8 GetHp() const { return m_hp; }
    constexpr UINT8 GetDirection() const { return m_direction; }
    constexpr UINT8 GetFacingDirection() const { return m_facingDirection; }

    // Setter �Լ�
    void SetHp(int _hp) { m_hp = _hp; }
    void SetDirection(int _direction);
    void Damaged(int _hp);
    void SetSpeed(UINT8 speedX, UINT8 speedY);

public:
    void SetFlag(UINT8 flag, bool bOnOff);
    void ToggleFlag(UINT8 flag);
    bool isBitSet(UINT8 flag);

public:
    void SetFlagField(UINT8 pField);

private:
    UINT8 m_hp;          // ü��
    UINT8 m_direction;   // ����
    UINT8 m_facingDirection;    // ĳ���Ͱ� �ٶ󺸰� �ִ� ����
    UINT8 m_speedX;     // ���ǵ�
    UINT8 m_speedY;
    UINT8 m_FlagField;  // �ൿ ���� flagField;
};