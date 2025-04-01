#include "MatchSystem.h"

MatchSystem::MatchSystem()
{
}

MatchSystem::MatchSystem(int roomId) : m_roomId(roomId) {
    m_red.reserve(5);
    m_blue.reserve(5);

    m_red.clear();
    m_blue.clear();
}

void MatchSystem::OnPlayerJoin(CPlayer* player) {
    if (m_red.size() <= m_blue.size()) {
        m_red.push_back(player);
        player->SetTeamId(0);
    }
    else {
        m_blue.push_back(player);
        player->SetTeamId(1);
    }
}

void MatchSystem::OnPlayerLeave(int playerId) {
    auto removePlayer = [playerId](std::vector<CPlayer*>& team) {
        team.erase(std::remove_if(team.begin(), team.end(), [playerId](CPlayer* p) {
            return p->GetId() == playerId;
            }), team.end());
        };
    removePlayer(m_red);
    removePlayer(m_blue);
}

int MatchSystem::GetPlayerCount() const {
    return static_cast<int>(m_red.size() + m_blue.size());
}

const std::vector<CPlayer*>& MatchSystem::GetRedTeam() const { return m_red; }
const std::vector<CPlayer*>& MatchSystem::GetBlueTeam() const { return m_blue; }
