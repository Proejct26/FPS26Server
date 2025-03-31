#include "DamageTracker.h"

void DamageTracker::RecordDamage(int attackerId, float nowTime) {
    _records.push_back({ attackerId, nowTime });
}

void DamageTracker::Update(float nowTime) {
    float expire = nowTime - 2.0f;
    while (!_records.empty() && _records.front().timestamp < expire) {
        _records.pop_front();
    }
}

std::vector<int> DamageTracker::GetAssistCandidates(float nowTime, float durationSec) const {
    std::unordered_set<int> unique;
    float valid = nowTime - durationSec;
    for (auto it = _records.rbegin(); it != _records.rend(); ++it) {
        if (it->timestamp < valid) break;
        unique.insert(it->attackerId);
    }
    return std::vector<int>(unique.begin(), unique.end());
}