#include "Item.h"

int Item::m_gItemId = 1;

Item::Item(ItemType t, float px, float py, float pz)
    : type(t), x(px), y(py), z(pz)
{
    id = m_gItemId++;
}
