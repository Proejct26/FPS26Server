
#pragma once
#include <vector>
#include <queue>
#include "Player.h"

class MatchSystem {
public:
    MatchSystem();
    MatchSystem(int roomId);

    void OnPlayerJoin(CPlayer* player);
    void OnPlayerLeave(int playerId);

    int GetPlayerCount() const;
    const std::vector<CPlayer*>& GetRedTeam() const;
    const std::vector<CPlayer*>& GetBlueTeam() const;

private:
    int m_roomId;
    std::vector<CPlayer*> m_red;
    std::vector<CPlayer*> m_blue;

    std::queue<int> m_redSpawnIndex;
    std::queue<int> m_blueSpawnIndex;
};
