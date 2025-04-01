
#include "Player.h"
#include "SessionManager.h"
#include "MakePacket.h"

CPlayer::CPlayer(float _x, float _y, float _z, UINT8 _hp) noexcept
	: CObject{ _x, _y, _z }
{
    m_maxHp = _hp;
    m_curHp = m_maxHp;

    m_eCurPlayerGameState = PLAYER_GAME_STATE::END;

    memset(&m_pressedKey, 0, sizeof(m_pressedKey));
}

CPlayer::~CPlayer()
{
}

void CPlayer::Update(void)
{ 
    __super::Update();
}

void CPlayer::LateUpdate(void)
{
    __super::LateUpdate();
}

void CPlayer::Init(float _x, float _y, float _z, UINT8 _hp)
{
    __super::Init();

    m_x = _x;
    m_y = _y;
    m_z = _z;

    m_maxHp = _hp;
    m_curHp = m_maxHp;

    m_kdaInfo.kill = 0;
    m_kdaInfo.death = 0;
    m_kdaInfo.assist = 0;

    m_rotationAxisX = 0;      // x, y축 회전값
    m_rotationAxisY = 0;

    m_roomId = -1;
    m_teamId = -1;
    m_spawnPosIndex = -1;

    m_weaponInfo = 0;     // 가지고 있는 무기 번호

    memset(m_pressedKey, 0, sizeof(bool) * (int)PRESS_KEY::END);

    m_eCurPlayerGameState = PLAYER_GAME_STATE::END;
}

std::array<bool, (int)PRESS_KEY::END> CPlayer::GetMovementKeys() const {
    std::array<bool, (int)PRESS_KEY::END> result{};
    for (int i = 0; i < (int)PRESS_KEY::END; ++i) {
        result[i] = m_pressedKey[i];
    }
    return result;
}

void CPlayer::SetMovementKeys(const std::array<bool, (int)PRESS_KEY::END>& keys) {
    m_pressedKey[(int)PRESS_KEY::W] = keys[0];
    m_pressedKey[(int)PRESS_KEY::A] = keys[1];
    m_pressedKey[(int)PRESS_KEY::S] = keys[2];
    m_pressedKey[(int)PRESS_KEY::D] = keys[3];
    m_pressedKey[(int)PRESS_KEY::JUMP] = keys[4];
}