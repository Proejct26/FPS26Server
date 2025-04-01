#pragma once

#include "ProtoStruct.h"

class CSession;
class CRoom;

void SC_ATTACK_FOR_All(CSession* pSession, UINT32 playerId, UINT32 hittedTargetId, float normalX, float normalY, float normalZ, float posX, float posY, float posZ);
void SC_ATTACK_FOR_SINGLE(CSession* pSession, UINT32 playerId, UINT32 hittedTargetId, float normalX, float normalY, float normalZ, float posX, float posY, float posZ);
void SC_ATTACK_FOR_AROUND(CSession* pSession, CRoom* pRoom, UINT32 playerId, UINT32 hittedTargetId, float normalX, float normalY, float normalZ, float posX, float posY, float posZ);

void SC_CHANGE_WEAPON_FOR_All(CSession* pSession, UINT32 playerId, UINT32 weapon);
void SC_CHANGE_WEAPON_FOR_SINGLE(CSession* pSession, UINT32 playerId, UINT32 weapon);
void SC_CHANGE_WEAPON_FOR_AROUND(CSession* pSession, CRoom* pRoom, UINT32 playerId, UINT32 weapon);

void SC_CHARACTER_DOWN_FOR_All(CSession* pSession, UINT32 playerId, UINT32 teamID);
void SC_CHARACTER_DOWN_FOR_SINGLE(CSession* pSession, UINT32 playerId, UINT32 teamID);
void SC_CHARACTER_DOWN_FOR_AROUND(CSession* pSession, CRoom* pRoom, UINT32 playerId, UINT32 teamID);

void SC_CHARACTER_KILL_LOG_FOR_All(CSession* pSession, std::vector<PlayerInfo>& playerInfoList);
void SC_CHARACTER_KILL_LOG_FOR_SINGLE(CSession* pSession, std::vector<PlayerInfo>& playerInfoList);
void SC_CHARACTER_KILL_LOG_FOR_AROUND(CSession* pSession, CRoom* pRoom, std::vector<PlayerInfo>& playerInfoList);

void SC_CREATE_MY_CHARACTER_FOR_All(CSession* pSession, UINT32 playerId, UINT32 posIndex, UINT32 maxHP, UINT32 teamID);
void SC_CREATE_MY_CHARACTER_FOR_SINGLE(CSession* pSession, UINT32 playerId, UINT32 posIndex, UINT32 maxHP, UINT32 teamID);
void SC_CREATE_MY_CHARACTER_FOR_AROUND(CSession* pSession, CRoom* pRoom, UINT32 playerId, UINT32 posIndex, UINT32 maxHP, UINT32 teamID);

void SC_CREATE_OTHER_CHARACTER_FOR_All(CSession* pSession, UINT32 playerId, UINT32 posIndex, UINT32 maxHP, UINT32 curHP, std::string name, KDAInfo kdaInfo, UINT32 weapon, UINT32 teamID);
void SC_CREATE_OTHER_CHARACTER_FOR_SINGLE(CSession* pSession, UINT32 playerId, UINT32 posIndex, UINT32 maxHP, UINT32 curHP, std::string name, KDAInfo kdaInfo, UINT32 weapon, UINT32 teamID);
void SC_CREATE_OTHER_CHARACTER_FOR_AROUND(CSession* pSession, CRoom* pRoom, UINT32 playerId, UINT32 posIndex, UINT32 maxHP, UINT32 curHP, std::string name, KDAInfo kdaInfo, UINT32 weapon, UINT32 teamID);

void SC_ITEM_PICK_FAIL_FOR_All(CSession* pSession, UINT32 playerId, UINT32 itemId);
void SC_ITEM_PICK_FAIL_FOR_SINGLE(CSession* pSession, UINT32 playerId, UINT32 itemId);
void SC_ITEM_PICK_FAIL_FOR_AROUND(CSession* pSession, CRoom* pRoom, UINT32 playerId, UINT32 itemId);

void SC_ITEM_PICK_SUCCESS_FOR_All(CSession* pSession, UINT32 playerId, UINT32 itemId);
void SC_ITEM_PICK_SUCCESS_FOR_SINGLE(CSession* pSession, UINT32 playerId, UINT32 itemId);
void SC_ITEM_PICK_SUCCESS_FOR_AROUND(CSession* pSession, CRoom* pRoom, UINT32 playerId, UINT32 itemId);

void SC_ITEM_SPAWNED_FOR_All(CSession* pSession, UINT32 itemId, UINT32 itemType, UINT32 itemPosIndex);
void SC_ITEM_SPAWNED_FOR_SINGLE(CSession* pSession, UINT32 itemId, UINT32 itemType, UINT32 itemPosIndex);
void SC_ITEM_SPAWNED_FOR_AROUND(CSession* pSession, CRoom* pRoom, UINT32 itemId, UINT32 itemType, UINT32 itemPosIndex);

void SC_KEY_INPUT_FOR_All(CSession* pSession, UINT32 playerId, UINT32 keyW, UINT32 keyA, UINT32 keyS, UINT32 keyD, UINT32 rotateAxisX, UINT32 rotateAxisY, UINT32 Jump);
void SC_KEY_INPUT_FOR_SINGLE(CSession* pSession, UINT32 playerId, UINT32 keyW, UINT32 keyA, UINT32 keyS, UINT32 keyD, UINT32 rotateAxisX, UINT32 rotateAxisY, UINT32 Jump);
void SC_KEY_INPUT_FOR_AROUND(CSession* pSession, CRoom* pRoom, UINT32 playerId, UINT32 keyW, UINT32 keyA, UINT32 keyS, UINT32 keyD, UINT32 rotateAxisX, UINT32 rotateAxisY, UINT32 Jump);

void SC_ON_ACCEPT_FOR_All(CSession* pSession, UINT32 playerId);
void SC_ON_ACCEPT_FOR_SINGLE(CSession* pSession, UINT32 playerId);
void SC_ON_ACCEPT_FOR_AROUND(CSession* pSession, CRoom* pRoom, UINT32 playerId);

void SC_POS_INTERPOLATION_FOR_All(CSession* pSession, UINT32 playerId, float posX, float posY, float posZ);
void SC_POS_INTERPOLATION_FOR_SINGLE(CSession* pSession, UINT32 playerId, float posX, float posY, float posZ);
void SC_POS_INTERPOLATION_FOR_AROUND(CSession* pSession, CRoom* pRoom, UINT32 playerId, float posX, float posY, float posZ);

void SC_SEND_MESSAGE_FOR_All(CSession* pSession, UINT32 playerId, std::string message);
void SC_SEND_MESSAGE_FOR_SINGLE(CSession* pSession, UINT32 playerId, std::string message);
void SC_SEND_MESSAGE_FOR_AROUND(CSession* pSession, CRoom* pRoom, UINT32 playerId, std::string message);

void SC_SHOT_HIT_FOR_All(CSession* pSession, UINT32 playerId, UINT32 hp);
void SC_SHOT_HIT_FOR_SINGLE(CSession* pSession, UINT32 playerId, UINT32 hp);
void SC_SHOT_HIT_FOR_AROUND(CSession* pSession, CRoom* pRoom, UINT32 playerId, UINT32 hp);
