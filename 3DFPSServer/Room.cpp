#include "Room.h"
#include <Windows.h>
#include <algorithm>

#include "MakePacket.h"

extern unsigned int g_iTime;

CRoom::CRoom()
{
}

CRoom::CRoom(int id) : m_roomId(id), m_matchSystem(id) {
    std::vector<std::tuple<float, float, float>> spawnPoints = {
        {2.0f, 0.0f, 2.0f}, {5.0f, 0.0f, 5.0f}, {8.0f, 0.0f, 1.0f}
    };
    m_itemSpawner.Init(spawnPoints);

    m_activePlayers.clear();
    m_waitingPlayers.clear();
}

bool CRoom::AddPlayer(CPlayer* player) {
    if (IsFull()) return false;

    m_waitingPlayers.push_back(player);
    player->SetRoomId(m_roomId);
    player->SetCurGameState(PLAYER_GAME_STATE::WAITING);

    m_matchSystem.OnPlayerJoin(player);

    return true;
}

void CRoom::RemovePlayer(int playerId) {
    m_matchSystem.OnPlayerLeave(playerId);
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
    m_itemSpawner.Update(deltaTime);
}

bool CRoom::IsFull() const {
    return GetPlayerCount() >= MAX_PLAYERS;
}

bool CRoom::OnItemPickupRequest(int playerId, int itemId) {
    // �߸��� �÷��̾ �� ���
    CPlayer* player = FindPlayerById(playerId);
    if (!player) return false;  // �̰� ��������� ���� �߸��� ���̱⿡ ���ʿ��� return �Ǹ� ������ ���ߴ°� �´�.

    // �������� ���� ���
    Item* item = m_itemSpawner.GetItemById(itemId);
    if (!item) {
        // ��ȯ�޴��ʿ��� �÷��̾�� ������ ȹ�� ���� ��Ŷ ����
        return false;
    }
    else
    {
        // ������ �����
        m_itemSpawner.RemoveItemById(itemId);

        // ��ȯ�޴��ʿ��� �÷��̾�� ������ ȹ�� ���� ��Ŷ ����
        return true;
    }
}

void CRoom::StartGame() {
    for (CPlayer* player : m_waitingPlayers) {
        // SendPacket(player, GameStartPacket());
        m_activePlayers.push_back(player);
    }
    m_waitingPlayers.clear();

    // Ȱ�� �÷��̾�� ��� ���� ����
    for (CPlayer* from : m_activePlayers) {
        for (CPlayer* to : m_activePlayers) {
            if (from == to) continue;
            // SendPacket(to, PlayerInfoPacket(from));
        }
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
