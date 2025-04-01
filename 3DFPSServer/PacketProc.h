#pragma once

#include "Session.h"
#include "Protobuf/Protocol.pb.h"

extern int g_iSyncCount;

class CObject;
class CSession;

bool PacketProc(CSession* pSession, game::PacketID packetType, CPacket* pPacket);
void DisconnectSessionProc(CSession* pSession);

bool CS_ATTACK(CSession* pSession, bool bAttack, float normalX, float normalY, float normalZ, float posX, float posY, float posZ);
bool CS_CHANGE_WEAPON(CSession* pSession, UINT32 weapon);
bool CS_ITEM_PICKED(CSession* pSession, UINT32 itemId);
bool CS_KEY_INPUT(CSession* pSession, UINT32 keyW, UINT32 keyA, UINT32 keyS, UINT32 keyD, UINT32 rotateAxisX, UINT32 rotateAxisY, UINT32 Jump);
bool CS_POS_INTERPOLATION(CSession* pSession, float posX, float posY, float posZ);
bool CS_REQUEST_RESTART(CSession* pSession, UINT32 playerId, UINT32 weapon);
bool CS_SEND_MESSAGE(CSession* pSession, UINT32 playerId, std::string message);
bool CS_SEND_NICKNAME(CSession* pSession, std::string name);
bool CS_SHOT_HIT(CSession* pSession, UINT32 playerId, UINT32 hp);