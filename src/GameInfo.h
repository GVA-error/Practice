#ifndef _GAME_INFO_H_
#define _GAME_INFO_H_

#include "DataStore.h"
#include "StringLiterals.h"

class GameInfo
{
public:
	GameInfo() {}

	void Load();
	void Save();

	int getGlobalInt(const std::string &name, int def = 0);
	void setGlobalInt(const std::string &name, int value);

private:
	DataStore _globals;
};

extern GameInfo gameInfo;

#endif