#include "ItemSpawner.h"
#include <cstdlib>
#include <ctime>
#include <algorithm>

void ItemSpawner::Init(const std::vector<std::tuple<float, float, float>>& spawnPoints) {
    m_spawnPoints = spawnPoints;
    m_items.clear();
}

void ItemSpawner::Update(float deltaTime) {
    m_timer += deltaTime;
    if (m_timer >= m_spawnInterval && !m_spawnPoints.empty()) {
        m_timer = 0.0f;
        int index = rand() % m_spawnPoints.size();
        auto [x, y, z] = m_spawnPoints[index];
        m_items.emplace_back(GetRandomItemType(), x, y, z);
    }
}

const std::vector<Item>& ItemSpawner::Items() const {
    return m_items;
}

bool ItemSpawner::RemoveItemById(int id) {
    auto it = std::remove_if(m_items.begin(), m_items.end(),
        [id](const Item& item) { return item.id == id; });
    if (it != m_items.end()) {
        m_items.erase(it, m_items.end());
        return true;
    }
    return false;
}

Item* ItemSpawner::GetItemById(int id) {
    for (auto& item : m_items) {
        if (item.id == id) return &item;
    }
    return nullptr;
}

ItemType ItemSpawner::GetRandomItemType() {
    int totalTypes = static_cast<int>(ItemType::END);
    int randIndex = rand() % totalTypes;
    return static_cast<ItemType>(randIndex);
}