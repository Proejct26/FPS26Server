#pragma once
#include <deque>
#include <unordered_set>
#include <vector>

struct DamageRecord {
    int attackerId;
    float timestamp;
};

class DamageTracker {
public:
    void RecordDamage(int attackerId, float nowTime);
    void Update(float nowTime);
    std::vector<int> GetAssistCandidates(float nowTime, float durationSec = 2.0f) const;

private:
    std::deque<DamageRecord> _records;
};
