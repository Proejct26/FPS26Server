#include "Room.h"
#include <Windows.h>
#include <algorithm>

#include "MakePacket.h"

extern unsigned int g_iTime;

CRoom::CRoom(int id) : m_roomId(id) {
}

void CRoom::Init(void)
{
    std::vector<std::tuple<float, float, float>> spawnPoints = {
        {2.0f, 0.0f, 2.0f}, {5.0f, 0.0f, 5.0f}, {8.0f, 0.0f, 1.0f}
    };

    try
    {
        m_pMatchSystem = new MatchSystem(m_roomId);
        m_pItemSpawner = new ItemSpawner;
    }
    catch (const std::exception& e)
    {
        std::cout << "예외 발생: " << e.what() << std::endl;
    }
    catch (...)
    {
        std::cout << "알 수 없는 예외 발생" << std::endl;
    }

    m_pItemSpawner->Init(spawnPoints);

    m_activePlayers.clear();
    m_waitingPlayers.clear();
}

bool CRoom::AddPlayer(CPlayer* player) {
    if (IsFull()) return false;

    m_waitingPlayers.push_back(player);
    player->SetRoomId(m_roomId);
    player->SetCurGameState(PLAYER_GAME_STATE::WAITING);

    m_pMatchSystem->OnPlayerJoin(player);

    return true;
}

void CRoom::RemovePlayer(int playerId) {
    m_pMatchSystem->OnPlayerLeave(playerId);
    auto remove = [playerId](std::vector<CPlayer*>& list) {
        list.erase(std::remove_if(list.begin(), list.end(),
            [playerId](CPlayer* p) { return p->GetId() == playerId; }), list.end());
        };
    remove(m_activePlayers);
    remove(m_waitingPlayers);
}

void CRoom::Update(float deltaTime) {
    float now = g_iTime;

    for (CPlayer* p : m_activePlayers) {
        p->UpdateDamageHistory(now);
    }
    m_pItemSpawner->Update(deltaTime);
}

bool CRoom::IsFull() const {
    return GetPlayerCount() >= MAX_PLAYERS;
}

bool CRoom::OnItemPickupRequest(int playerId, int itemId) {
    // 잘못된 플레이어가 올 경우
    CPlayer* player = FindPlayerById(playerId);
    if (!player) return false;  // 이건 기능적으로 완전 잘못된 것이기에 이쪽에서 return 되면 서버를 멈추는게 맞다.

    // 아이템이 없을 경우
    Item* item = m_pItemSpawner->GetItemById(itemId);
    if (!item) {
        // 반환받는쪽에서 플레이어에게 아이템 획득 실패 패킷 전송
        return false;
    }
    else
    {
        // 아이템 지우고
        m_pItemSpawner->RemoveItemById(itemId);

        // 반환받는쪽에서 플레이어에게 아이템 획득 성공 패킷 전송
        return true;
    }
}

void CRoom::MoveToActive(int playerId) {
    auto it = std::find_if(m_waitingPlayers.begin(), m_waitingPlayers.end(),
        [playerId](CPlayer* p) { return p->GetId() == playerId; });

    if (it != m_waitingPlayers.end()) {
        CPlayer* player = *it;
        m_activePlayers.push_back(player);
        m_waitingPlayers.erase(it); 

        player->SetCurGameState(PLAYER_GAME_STATE::ACTIVE);  
    }
}

void CRoom::MoveToWaiting(int playerId) {
    auto it = std::find_if(m_activePlayers.begin(), m_activePlayers.end(),
        [playerId](CPlayer* p) { return p->GetId() == playerId; });

    if (it != m_activePlayers.end()) {
        CPlayer* player = *it;
        m_waitingPlayers.push_back(*it);
        m_activePlayers.erase(it);

        player->SetCurGameState(PLAYER_GAME_STATE::WAITING); 
    }
}

CPlayer* CRoom::FindPlayerById(int playerId) {
    for (CPlayer* p : m_activePlayers) {
        if (p->GetId() == playerId) return p;
    }
    for (CPlayer* p : m_waitingPlayers) {
        if (p->GetId() == playerId) return p;
    }
    return nullptr;
}
