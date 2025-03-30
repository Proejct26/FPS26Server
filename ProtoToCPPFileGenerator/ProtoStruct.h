#pragma once

struct KDAInfo {
    UINT32 kill;
    UINT32 death;
    UINT32 assist;
};

struct PlayerInfo {
    UINT32 playerId;
    KDAInfo kda;
};
