#include "stdafx.h"
#include "GameSettings.h"

GameSettings::GameSettings()
{
	IRect wRect = Core::appInstance->GetMainWindow()->GetClientSizes();
	_screenW = wRect.width;
	_screenH = wRect.height;
	_fieldRect = FRect(50, _screenW - 50, 54, _screenH - 100);
	_LevelListRect = FRect(60, _screenW - 60, 100, _screenH - 70);
	Load();
	_availableLvl = GameInfo::getGlobalInt(s_availableLvl);
	if (_availableLvl < 1) // гейм инфо ненайден или испорчен
	{
		_availableLvl = 1;
		save();
	}
}

GameSettings::~GameSettings()
{
	save();
}

void GameSettings::save()
{
	setGlobalInt(s_availableLvl, _availableLvl);
	GameInfo::Save();
}

void GameSettings::setAvailableLvl(int value)
{
	_availableLvl = value;
	GameInfo::setGlobalInt(s_availableLvl, value);
}

short GameSettings::getPrstn_changeTypePrimitive() const
{
	return prstn_changeTypePrimitive;
}

short GameSettings::getPrstn_axePrimitive() const
{
	return prstn_axePrimitive;
}

short GameSettings::getPrstn_bombPrimitive() const
{
	return prstn_bombPrimitive;
}

short GameSettings::getPrstn_lockPrimitive() const
{
	return prstn_lockPrimitive;
}

short GameSettings::getPrstn_bigPrimitive() const
{
	return prstn_bigPrimitive;
}

int GameSettings::getMaxHelpCount() const
{
	return _maxHelp;
}

int GameSettings::getBackStapCount() const
{
	return _maxBackStap;
}

int GameSettings::getMaxCascadeSiae() const
{
	return _maxCascadeSiae;
}

float GameSettings::getV0() const
{
	return _v0;
}

int GameSettings::getMaxNumTryInDelta() const
{
	return _maxNumTryInDelta;
}

int GameSettings::getAvailableLvl() const
{
	if (_availableLvl == -1)
		return getMaxLevel();
	return _availableLvl;
}

void GameSettings::resetAvailableLvl()
{
	setAvailableLvl(1);
}

void GameSettings::incAvailableLvl()
{
	if (_availableLvl > getMaxLevel())
		return;
	_availableLvl++;
	save();
}

int GameSettings::getBoxTypesCount() const
{
	return _boxTypesCount;
}

FPoint GameSettings::getLevelListPos() const
{
	FPoint posLevelList;
	posLevelList.x = _LevelListRect.xStart;
	posLevelList.y = _LevelListRect.yStart;
	return posLevelList;
}

FPoint GameSettings::getCentrField() const
{
	FPoint centr;
	centr.x = _screenW / 2;
	centr.y = _screenH / 2;
	return centr;
}

FRect GameSettings::getLevelListRect() const
{
	return _LevelListRect;
}

int GameSettings::getMaxILevel() const
{
	return _maxILevel;
}

int GameSettings::getMaxJLevel() const
{
	return _maxJLevel;
}

int GameSettings::getMaxLevel() const
{
	return _maxLevel;
}

float GameSettings::getg() const
{
	return _g;
}

int GameSettings::getMaxJField() const
{
	return _maxJField;
}

int GameSettings::getMaxIField() const
{
	return _maxIField;
}

float GameSettings::getScreenW() const
{
	return _screenW;
}

float GameSettings::getScreenH() const
{
	return _screenH;
}

FRect GameSettings::getFieldRect() const
{
	return _fieldRect;
}