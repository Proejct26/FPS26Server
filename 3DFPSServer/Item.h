#pragma once

enum class ItemType {
    HealPack,
    InfiniteAmmo,
    MoveSpeed,
    JumpPower,
    DoubleJump,

    END
};

class Item {
public:
    Item(ItemType t, float px, float py, float pz);

private:
    static int m_gItemId;   // 전역 아이템 id

public:
    int id;                 // 아이템 id
    ItemType type;          // 아이템 타입
    float x, y, z;          // 위치
};