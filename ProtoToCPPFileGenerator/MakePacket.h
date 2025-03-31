#pragma once

#include "ProtoStruct.h"

class CSession;
class CRoom;

void SC_ATTACK_FOR_All(CSession* pSession, UINT32 playerId);
void SC_ATTACK_FOR_SINGLE(CSession* pSession, UINT32 playerId);
void SC_ATTACK_FOR_AROUND(CSession* pSession, CRoom* pRoom, UINT32 playerId);

void SC_CHANGE_WEAPON_FOR_All(CSession* pSession, UINT32 playerId, UINT32 weapon);
void SC_CHANGE_WEAPON_FOR_SINGLE(CSession* pSession, UINT32 playerId, UINT32 weapon);
void SC_CHANGE_WEAPON_FOR_AROUND(CSession* pSession, CRoom* pRoom, UINT32 playerId, UINT32 weapon);

void SC_CHARACTER_DOWN_FOR_All(CSession* pSession, UINT32 playerId);
void SC_CHARACTER_DOWN_FOR_SINGLE(CSession* pSession, UINT32 playerId);
void SC_CHARACTER_DOWN_FOR_AROUND(CSession* pSession, CRoom* pRoom, UINT32 playerId);

void SC_CHARACTER_KILL_LOG_FOR_All(CSession* pSession, std::vector<PlayerInfo>& playerInfoList);
void SC_CHARACTER_KILL_LOG_FOR_SINGLE(CSession* pSession, std::vector<PlayerInfo>& playerInfoList);
void SC_CHARACTER_KILL_LOG_FOR_AROUND(CSession* pSession, CRoom* pRoom, std::vector<PlayerInfo>& playerInfoList);

void SC_CREATE_MY_CHARACTER_FOR_All(CSession* pSession, UINT32 playerId, UINT32 posX, UINT32 posY, UINT32 posZ, UINT32 dirX, UINT32 dirY, UINT32 dirZ, UINT32 maxHP);
void SC_CREATE_MY_CHARACTER_FOR_SINGLE(CSession* pSession, UINT32 playerId, UINT32 posX, UINT32 posY, UINT32 posZ, UINT32 dirX, UINT32 dirY, UINT32 dirZ, UINT32 maxHP);
void SC_CREATE_MY_CHARACTER_FOR_AROUND(CSession* pSession, CRoom* pRoom, UINT32 playerId, UINT32 posX, UINT32 posY, UINT32 posZ, UINT32 dirX, UINT32 dirY, UINT32 dirZ, UINT32 maxHP);

void SC_CREATE_OTHER_CHARACTER_FOR_All(CSession* pSession, UINT32 playerId, UINT32 posX, UINT32 posY, UINT32 posZ, UINT32 dirX, UINT32 dirY, UINT32 dirZ, UINT32 maxHP, UINT32 curHP, std::string name, KDAInfo kdaInfo);
void SC_CREATE_OTHER_CHARACTER_FOR_SINGLE(CSession* pSession, UINT32 playerId, UINT32 posX, UINT32 posY, UINT32 posZ, UINT32 dirX, UINT32 dirY, UINT32 dirZ, UINT32 maxHP, UINT32 curHP, std::string name, KDAInfo kdaInfo);
void SC_CREATE_OTHER_CHARACTER_FOR_AROUND(CSession* pSession, CRoom* pRoom, UINT32 playerId, UINT32 posX, UINT32 posY, UINT32 posZ, UINT32 dirX, UINT32 dirY, UINT32 dirZ, UINT32 maxHP, UINT32 curHP, std::string name, KDAInfo kdaInfo);

void SC_GRENADEEXPLOSITIONPOS_FOR_All(CSession* pSession, UINT32 posX, UINT32 posY, UINT32 posZ);
void SC_GRENADEEXPLOSITIONPOS_FOR_SINGLE(CSession* pSession, UINT32 posX, UINT32 posY, UINT32 posZ);
void SC_GRENADEEXPLOSITIONPOS_FOR_AROUND(CSession* pSession, CRoom* pRoom, UINT32 posX, UINT32 posY, UINT32 posZ);

void SC_KEY_INPUT_FOR_All(CSession* pSession, UINT32 playerId, UINT32 keyW, UINT32 keyA, UINT32 keyS, UINT32 keyD, UINT32 rotateAxisX, UINT32 rotateAxisY, UINT32 Jump);
void SC_KEY_INPUT_FOR_SINGLE(CSession* pSession, UINT32 playerId, UINT32 keyW, UINT32 keyA, UINT32 keyS, UINT32 keyD, UINT32 rotateAxisX, UINT32 rotateAxisY, UINT32 Jump);
void SC_KEY_INPUT_FOR_AROUND(CSession* pSession, CRoom* pRoom, UINT32 playerId, UINT32 keyW, UINT32 keyA, UINT32 keyS, UINT32 keyD, UINT32 rotateAxisX, UINT32 rotateAxisY, UINT32 Jump);

void SC_POS_INTERPOLATION_FOR_All(CSession* pSession, UINT32 posX, UINT32 posY, UINT32 posZ);
void SC_POS_INTERPOLATION_FOR_SINGLE(CSession* pSession, UINT32 posX, UINT32 posY, UINT32 posZ);
void SC_POS_INTERPOLATION_FOR_AROUND(CSession* pSession, CRoom* pRoom, UINT32 posX, UINT32 posY, UINT32 posZ);

void SC_SHOT_HIT_FOR_All(CSession* pSession, UINT32 playerId, UINT32 hp);
void SC_SHOT_HIT_FOR_SINGLE(CSession* pSession, UINT32 playerId, UINT32 hp);
void SC_SHOT_HIT_FOR_AROUND(CSession* pSession, CRoom* pRoom, UINT32 playerId, UINT32 hp);

void SC_THROW_GRENADE_FOR_All(CSession* pSession, UINT32 posX, UINT32 posY, UINT32 posZ, UINT32 dirX, UINT32 dirY, UINT32 dirZ);
void SC_THROW_GRENADE_FOR_SINGLE(CSession* pSession, UINT32 posX, UINT32 posY, UINT32 posZ, UINT32 dirX, UINT32 dirY, UINT32 dirZ);
void SC_THROW_GRENADE_FOR_AROUND(CSession* pSession, CRoom* pRoom, UINT32 posX, UINT32 posY, UINT32 posZ, UINT32 dirX, UINT32 dirY, UINT32 dirZ);
