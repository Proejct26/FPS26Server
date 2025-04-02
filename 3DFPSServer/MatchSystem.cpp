#include "MatchSystem.h"

MatchSystem::MatchSystem()
{
}

MatchSystem::MatchSystem(int roomId) : m_roomId(roomId) {
    m_red.reserve(5);
    m_blue.reserve(5);

    m_red.clear();
    m_blue.clear();

    // 스폰 데이터에 0 ~ 4까지 인덱스 삽입
    for (int i = 0; i < 5; ++i)
    {
        m_redSpawnIndex.push(i);
        m_blueSpawnIndex.push(i);
    }
}

void MatchSystem::OnPlayerJoin(CPlayer* player) {
    // 레드 팀일 경우
    if (m_red.size() <= m_blue.size()) {
        m_red.push_back(player);
        player->SetTeamId(0);   // 레드 팀일 경우 팀 ID = 0

        player->SetSpawnPosIndex(m_redSpawnIndex.front());
        m_redSpawnIndex.pop();
    }

    // 블루 팀일 경우
    else {
        m_blue.push_back(player);
        player->SetTeamId(1);   // 블루 팀일 경우 팀 ID = 1

        player->SetSpawnPosIndex(m_blueSpawnIndex.front());
        m_blueSpawnIndex.pop();
    }
}

void MatchSystem::OnPlayerLeave(int playerId) {
    auto removePlayer = [playerId](std::vector<CPlayer*>& team) {
        team.erase(std::remove_if(team.begin(), team.end(), [playerId](CPlayer* p) {
            return p->GetId() == playerId;
            }), team.end());
        };

    for (auto& redPlayer : m_red)
    {
        if (redPlayer)
        {
            // 플레이어를 찾았을 경우
            if (redPlayer->m_ID == playerId)
            {
                // 자신의 스폰 위치를 반환
                m_redSpawnIndex.push(redPlayer->GetSpawnPosIndex());
            }
        }
    }
    for (auto& bluePlayer : m_blue)
    {
        if (bluePlayer)
        {
            // 플레이어를 찾았을 경우
            if (bluePlayer->m_ID == playerId)
            {
                // 자신의 스폰 위치를 반환
                m_blueSpawnIndex.push(bluePlayer->GetSpawnPosIndex());
            }
        }
    }

    removePlayer(m_red);
    removePlayer(m_blue);
}

int MatchSystem::GetPlayerCount() const {
    return static_cast<int>(m_red.size() + m_blue.size());
}

const std::vector<CPlayer*>& MatchSystem::GetRedTeam() const { return m_red; }
const std::vector<CPlayer*>& MatchSystem::GetBlueTeam() const { return m_blue; }
