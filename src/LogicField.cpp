#include "stdafx.h"
#include "LogicField.h"


LogicField::LogicField(GameSettings::Ptr gameSettings)
{
	assert(gameSettings);
	retuneLogic(gameSettings);
}

void LogicField::retuneLogic(GameSettings::Ptr settings)
{
	reset();
	_fieldRect = settings->getFieldRect();
	float fieldW = _fieldRect.Width();
	float fieldH = _fieldRect.Height();
	_maxI = settings->getMaxIField();
	_maxJ = settings->getMaxJField();

	_CellRect.xStart = 0;
	_CellRect.yStart = 0;

	float cellW = fieldW/(float)_maxJ;
	float cellH = fieldH/(float)_maxI;

	_CellRect.xEnd = cellW;
	_CellRect.yEnd = cellH;

	_fieldPos.x = _fieldRect.xStart;
	_fieldPos.y = _fieldRect.yStart;
}

LogicField::~LogicField()
{
}

bool LogicField::isFly(int ID)
{
	IPoint curPos = getFieldPos(ID);
	if (curPos.y <= 0)
		return false;
	IPoint downPos = IPoint(curPos.x, curPos.y - 1);
	if (posIsFree(downPos, ID))
		return true;
	return false;
}

bool LogicField::posIsCorrect(const IPoint& posInField) const
{
	if (posInField.x < 0 || posInField.y < 0)
		return false;
	if (posInField.x >= _maxJ || posInField.y >= _maxI)
		return false;
	return true;
}

bool LogicField::posIsCorrect(const FPoint& pos) const
{
	FPoint fosWithOffset = pos;
	return _fieldRect.Contains(fosWithOffset);
}

int LogicField::generateNewID()
{
	int newID = 0;
	int maxExistID = -1; // максимальный сущевствующий ID
	for (auto cell : _objFieldPos)
	{
		int cellID = cell.first;
		if (cellID > maxExistID)
			maxExistID = cellID;
	}
	newID = maxExistID + 1;
	assert(newID <= maxID);
	return newID;
}

float LogicField::getHCell()
{
	return _CellRect.Height();
}

float LogicField::getWCell()
{
	return _CellRect.Width();
}

IPoint LogicField::getFieldPos(int ID)
{
	if (!haveID(ID))
		return IPoint(-1, -1);
	//if (_objFieldPos.find(ID) != _objFieldPos.end())
	return _objFieldPos[ID];
	//else
	//	assert(false);
	//return IPoint()
}

bool LogicField::posIsFree(const IPoint& posInField, int ID) const
{
	if (posInField.y < 0 || posInField.x < 0)
		return false;
	for (auto cell : _objFieldPos)
	{
		if (cell.first == ID)
			continue;
		if (cell.second == posInField)
			return false;
	}
	return true;
}

bool LogicField::posIsFree(const FPoint& pos, int ID) const
{
	IPoint posInField = getPosInMxField(pos);
	return posIsFree(posInField, ID);
}

int LogicField::getID(const IPoint& posInField) const
{
	for (auto cell : _objFieldPos)
	{
		int ID = cell.first;
		IPoint pos = cell.second;
		if (pos == posInField)
			return ID;
	}
	return -1;
}

FPoint LogicField::getCentrCell(const IPoint& posInField) const
{
	FPoint centr;
	centr.x = ((float)posInField.x + 0.5f) * _CellRect.Width();
	centr.y = ((float)posInField.y + 0.5f) * _CellRect.Height();
	centr += _fieldPos;
	return centr;
}

bool LogicField::posInCell(const FPoint& pos, const IPoint& posInField) const
{
	FRect CellRect = _CellRect;
	FPoint rounded = roundedPos(posInField);
	CellRect.xStart += rounded.x;
	CellRect.yStart += rounded.y;
	return CellRect.Contains(pos);
}


IPoint LogicField::getPosInMxField(const FPoint& absPos) const
{
	FPoint pos = absPos - _fieldPos;
	int i = pos.y / _CellRect.Height();
	int j = pos.x / _CellRect.Width();
	if (pos.y < 0)
		i = -1;
	if (pos.x < 0)
		j = -1;
	return IPoint(j, i);
}

FPoint LogicField::roundedPos(const FPoint& absPos) const
{
	IPoint posInField = getPosInMxField(absPos);
	FPoint rounded = roundedPos(posInField);
	return rounded;
}

FPoint LogicField::roundedPos(const IPoint& posInField) const
{
	FPoint roundedPos; 
	roundedPos.x = posInField.x * _CellRect.Width();
	roundedPos.y = posInField.y * _CellRect.Height();
	roundedPos += _fieldPos;
	return roundedPos;
}

void LogicField::reset()
{
	_objRealPos.clear();
	_objFieldPos.clear();
}

void LogicField::destoy(int ID)
{
	assert(haveID(ID));
	_objRealPos.erase(ID);
	_objFieldPos.erase(ID);
	assert(_objFieldPos.size() == _objRealPos.size()); // где то мы к нему обратились (к мапу). Возможно обновили позицию мёртвого объекта
}

int LogicField::getMaxI()
{
	return _maxI;
}
int LogicField::getMaxJ()
{
	return _maxJ;
}

void LogicField::move(int ID, const FPoint& pos)
{
	IPoint posInField = getPosInMxField(pos);
	_objRealPos[ID] = pos;
	_objFieldPos[ID] = posInField;
}

void LogicField::create(int ID, const FPoint& pos)
{
	assert(!haveID(ID));
	move(ID, pos);
}

bool LogicField::haveID(int ID)
{
	//return _objFieldPos.find(ID) != _objFieldPos.end();

	for (auto cell : _objFieldPos)
		if (cell.first == ID)
			return true;
	return false;

}

FRect LogicField::getCellRect()
{
	return _CellRect;
}