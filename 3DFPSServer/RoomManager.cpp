#include "RoomManager.h"

CRoomManager::CRoomManager() noexcept
{
}

CRoomManager::~CRoomManager() noexcept
{
	delete[] m_rooms;
}
