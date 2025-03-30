#pragma once

struct KDAInfoData {
    UINT32 kill;
    UINT32 death;
    UINT32 assist;
};

struct PlayerInfoData {
    UINT32 playerId;
    KDAInfoData kda;
};
