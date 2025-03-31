#pragma once
#include "Room.h"
#include "Singleton.h"

class CRoomManager : public SingletonBase<CRoomManager> {
    friend class SingletonBase<CRoomManager>;

public:
    explicit CRoomManager() noexcept;
    ~CRoomManager() noexcept;

    // ���� �����ڿ� ���� �����ڸ� �����Ͽ� ���� ����
    CRoomManager(const CRoomManager&) = delete;
    CRoomManager& operator=(const CRoomManager&) = delete;

public:
    void InitRooms() {
        for (int i = 0; i < MAX_ROOM_COUNT; ++i) {
            m_rooms[i] = CRoom(i);  // ���� �޸𸮿� ���� ����
        }
    }

    CRoom* FindAvailableRoom() {
        for (int i = 0; i < MAX_ROOM_COUNT; ++i) {
            if (!m_rooms[i].IsFull()) {
                return &m_rooms[i];
            }
        }
        return nullptr;
    }

    CRoom* GetRoomById(int roomId) {
        if (roomId < 0 || roomId >= MAX_ROOM_COUNT)
            return nullptr;
        return &m_rooms[roomId];
    }

    int GetRoomCount() const { return MAX_ROOM_COUNT; }

private:
    static constexpr int MAX_ROOM_COUNT = 10;
    CRoom m_rooms[MAX_ROOM_COUNT];  // ���� �迭�� ���� (���� �޸�)
};
