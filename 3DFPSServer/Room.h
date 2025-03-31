#pragma once
#include <vector>
#include "Player.h"
#include "MatchSystem.h"
#include "ItemSpawner.h"

class CRoom {
public:
    CRoom();
    CRoom(int id);

    bool AddPlayer(CPlayer* player);
    void RemovePlayer(int playerId);
    void Update(float deltaTime);
    bool IsFull() const;

    bool OnItemPickupRequest(int playerId, int itemId);
    void StartGame();

    void MoveToActive(int playerId);
    void MoveToWaiting(int playerId);

    int GetRoomId() const { return m_roomId; }
    int GetPlayerCount() const { return static_cast<int>(m_activePlayers.size() + m_waitingPlayers.size()); }
    const std::vector<CPlayer*>& GetActivePlayers() const { return m_activePlayers; }
    const std::vector<CPlayer*>& GetWaitingPlayers() const { return m_waitingPlayers; }

    CPlayer* FindPlayerById(int playerId);

private:
    static constexpr int MAX_PLAYERS = 10;
    int m_roomId = -1;
    std::vector<CPlayer*> m_activePlayers;
    std::vector<CPlayer*> m_waitingPlayers;
    MatchSystem m_matchSystem;
    ItemSpawner m_itemSpawner;

};