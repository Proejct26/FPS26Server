#pragma once
#include <vector>
#include <tuple>
#include "Item.h"

class ItemSpawner {
public:
    void Init(const std::vector<std::tuple<float, float, float>>& spawnPoints);
    void Update(float deltaTime);

    const std::vector<Item>& Items() const;
    bool RemoveItemById(int id);
    Item* GetItemById(int id);

private:
    std::vector<std::tuple<float, float, float>> m_spawnPoints;
    std::vector<Item> m_items;
    float m_timer = 0.0f;
    float m_spawnInterval = 10.0f;

    ItemType GetRandomItemType();
};