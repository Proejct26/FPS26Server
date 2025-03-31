#pragma once
#include "Room.h"
#include "Singleton.h"

class CRoomManager : public SingletonBase<CRoomManager> {
    friend class SingletonBase<CRoomManager>;

public:
    explicit CRoomManager() noexcept;
    ~CRoomManager() noexcept;

    // 복사 생성자와 대입 연산자를 삭제하여 복사 방지
    CRoomManager(const CRoomManager&) = delete;
    CRoomManager& operator=(const CRoomManager&) = delete;

public:
    void InitRooms() {
        for (int i = 0; i < MAX_ROOM_COUNT; ++i) {
            m_rooms[i] = CRoom(i);  // 연속 메모리에 직접 생성
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
    CRoom m_rooms[MAX_ROOM_COUNT];  // 고정 배열로 관리 (연속 메모리)
};
