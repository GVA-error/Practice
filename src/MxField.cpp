#include "stdafx.h"
#include "MxField.h"


MxField::MxField(int maxJ, int maxI)
{
	_maxJ = maxJ;
	_maxI = maxI;
	_wMx = createMX();
}


MxField::~MxField()
{
	deleteMX(_wMx);
}


void MxField::handleBig(PtrMx mxField) // размещает большие в 4 клетках
{
	for (int j = 0; j<_maxJ-1; j++)
		for (int i = 0; i < _maxI-1; i++)
		{
			int curType = mxField[j][i];
			if (BoxType::typeIsBig(curType))
			{
				if (i>0 && BoxType::typeIsBig(mxField[j][i - 1]))
					continue;
				if (j>0 && BoxType::typeIsBig(mxField[j - 1][i]))
					continue;
				for (int di = 0; di <= 1; di++)
					for (int dj = 0; dj <= 1; dj++)
						mxField[j + dj][i + di] = BoxType::toBig(curType);
			}
		}
}

void MxField::handleUnBig(PtrMx mxField) // размещает большие в 1 клетке
{
	for (int j = 0; j<_maxJ - 1; j++)
		for (int i = 0; i < _maxI - 1; i++)
		{
			int curType = mxField[j][i];
			if (BoxType::typeIsBig(curType))
			{
				if (i>0 && !BoxType::typeIsBig(mxField[j][i - 1]))
					continue;
				if (j>0 && !BoxType::typeIsBig(mxField[j - 1][i]))
					continue;
				for (int di = 0; di <= 1; di++)
					for (int dj = 0; dj <= 1; dj++)
						if (di+dj == 0)
							clear(mxField, j + dj, i + di);
			}
		}

}

void MxField::copyMX(PtrMx original, PtrMx copy)
{
	for (int j = 0; j < _maxJ; j++)
		for (int i = 0; i < _maxI; i++)
			copy[j][i] = original[j][i];
}

bool MxField::posIsType(PtrMx mx, const IPoint& p, int type)
{
	return posIsType(mx, p.x, p.y, type);
}

bool MxField::posIsType(PtrMx mx, int j, int i, int type)
{
	if (!posInMx(j, i))
		return false;
	if (mx[j][i] != type)
		return false;
	return true;
}

GamePosition MxField::createPosOnMx(PtrMx mx)
{
	handleUnBig(mx);
	GamePosition newGamePos;
	for (int j = 0; j < _maxJ; j++)
		for (int i = 0; i < _maxI; i++)
		{
			int curBoxType = mx[j][i];
			if (curBoxType >= 0)
			{
				Box::boxInfo curBox;
				curBox.pos = IPoint(j, i);
				curBox.type = (boxType)curBoxType;
				newGamePos.boxList.push_back(curBox);
			}
		}
	return newGamePos;
}

bool MxField::equal(PtrMx mxA, PtrMx mxB)
{
	for (int j = 0; j < _maxJ; j++)
		for (int i = 0; i < _maxI; i++)
			if (mxA[j][i] != mxB[j][i])
				return false;
	return true;

}

bool MxField::isEmpty(PtrMx curPos)
{
	for (int j = 0; j < _maxJ; j++)
		for (int i = 0; i < _maxI; i++)
			if (curPos[j][i] >= 0)
				return false;
	return true;
}

MxField::PtrMx MxField::generateMxField(const GamePosition& gamePos)
{
	PtrMx mxField = createMX();
	for (auto box : gamePos.boxList)
		mxField[box.pos.x][box.pos.y] = box.type;
	handleBig(mxField);
	return mxField;
}

MxField::PtrMx MxField::generateMxField(PtrMx mxField)
{
	PtrMx newMxField = createMX();
	for (int j = 0; j < _maxJ; j++)
		for (int i = 0; i < _maxI; i++)
			newMxField[j][i] = mxField[j][i];
	return newMxField;
}

MxField::PtrMx MxField::createMX()
{
	PtrMx mx = new short*[_maxJ];
	for (int j = 0; j < _maxJ; j++)
	{
		mx[j] = new short[_maxI];
		for (int i = 0; i < _maxI; i++)
			mx[j][i] = -1;
	}

	return mx;
}

void MxField::deleteMX(PtrMx mx)
{
	for (int j = 0; j < _maxJ; j++)
		delete[] mx[j];
	delete[] mx;
}

Swap MxField::createSwap(int x1, int y1, int x2, int y2)
{
	Swap sw;
	sw.A = IPoint(x1, y1);
	sw.B = IPoint(x2, y2);
	sw.error = false;
	return sw;
}


MxField::PtrMx MxField::getAfterSwap(PtrMx curPos, Swap& sw)
{
	PtrMx posAfterSwap = generateMxField(curPos);
	makeSwap(posAfterSwap, sw);

	handleMove(posAfterSwap);

	return posAfterSwap;
}

void MxField::handleMove(PtrMx posAfterSwap)
{
	do
		handleGravity(posAfterSwap);
	while (handleMatch(posAfterSwap));
}

void MxField::handleGravity(PtrMx mxField)
{
	for (int j = 0; j<_maxJ; j++)
		for (int i = 0; i < _maxI;)
		{
			if (isClear(mxField, j, i))
				if (BoxType::typeIsBig(mxField[j][i + 1]))
				{
					if (j < _maxJ)
					{
						int rColor = mxField[j + 1][i + 1];
						int curColor = mxField[j][i + 1];
						int rightColorType = BoxType::getColorType(rColor);
						int curColorType = BoxType::getColorType(curColor);
						if (BoxType::typeIsBig(rColor) && rightColorType == curColorType) // сначала падает нижняя часть потом верхняя
						{
							makeSwap(mxField, j, i, j, i + 1);
							makeSwap(mxField, j + 1, i, j + 1, i + 1);
							if (i > 0)
								i--;
							else
								i++;
							continue;
						}

					}
				}
				else if (i + 1 < _maxI && !isClear(mxField, j, i + 1))
				{
					makeSwap(mxField, j, i, j, i + 1);
					if (i > 0)
						i--;
					else
						i++;
					continue;
				}
			i++;
		}
}


MxField::Match MxField::handleMatch(const GamePosition& gPos)
{
	PtrMx curPos = generateMxField(gPos);
	PtrMx oldPos = createMX();
	copyMX(curPos, oldPos);

	while (handleMatch(curPos))
		;
	Match match;
	for (auto box : gPos.boxList)
	{
		int j = box.pos.x;
		int i = box.pos.y;

		int oldState = oldPos[j][i];
		int curState = curPos[j][i];
		if (oldState != curState)
			match.push_back(box.pos);
	}
	return match;
}

bool MxField::handleMatch(PtrMx mxField)
{	
	copyMX(mxField, _wMx);
	_maxSize = 1;
	for (int i = 0; i<_maxI; i++)
		for (int j = 0; j < _maxJ; j++)
			if (BoxType::typeIsBig(_wMx[j][i]))
			{
				_maxSize = 2;
				i = j = _maxI + _maxJ; // выход из двойного цикла
			}

	bool f_match = false;
	for (int j = 0; j < _maxJ; j++)
		for (int i = 0; i < _maxJ; i++)
			f_match |= handleMatch(mxField, _wMx, j, i);

	return f_match;
}

bool MxField::handleMatch(PtrMx mxField, PtrMx watchMxField, int j, int i)
{
	bool f_match = false;
	for (int size = 1; size <= _maxSize; size++)
		for (int axis = 0; axis < 2; axis++)// добавить зависимость от сайза
			f_match |= handleMatch(mxField, watchMxField, j, i, size, axis);
	return f_match;
}

bool MxField::handleMatch(PtrMx mxField, PtrMx watchMxField, int j, int i, int size, int axis)
{
	int boxColorType = BoxType::getColorType(watchMxField[j][i]);
	int dj = !axis;
	int di = axis; 

	int maxX, maxY;
	maxX = _maxJ - j;
	maxY = _maxI - i;
	if (axis)
		std::swap(maxX, maxY);

	int a = 0;
	if (j == 2 && i == 0)
		a++;

	int x = 0;
	int y;
	int bigSizeBoxCount = 0;
	for (x = 0; x < maxX; x++)
	{
		for (y = 0; y < maxY - size + 1 && y < size; y++)
		{
			int curX = j + x*dj + y*axis;
			int curY = i + x*di + y*(!axis);
			int curColor = watchMxField[curX][curY];
			int curColorType = BoxType::getColorType(curColor);
			if (curColorType != boxColorType || isClear(curColor) || (BoxType::typeIsBig(curColor) && size != 2))
				break; // выход из цикла
			if (BoxType::typeIsBig(curColor))
				bigSizeBoxCount++;
		}
		if (y < size)
			break;
	}

	if (x >= 3 && (_maxSize == 1 || (bigSizeBoxCount == 4 && _maxSize == 2)))
	{
		for (int dl = 0; dl < x; dl++)
			for (y = 0; y < maxY - size + 1 && y < size; y++)
			{
				int curX = j + dl*dj + y*axis;
				int curY = i + dl*di + y*(!axis);
				int curType = watchMxField[curX][curY];
				int realType = mxField[curX][curY];
				if (BoxType::typeIsLock(curType) && BoxType::typeIsLock(realType) || // если мы еще не обработали замок
					!BoxType::typeIsLock(curType) && !BoxType::typeIsLock(realType)) // или там вообще нет замка
					clear(mxField, curX, curY);
				if (BoxType::typeIsBomb(curType))
					explosion(mxField, watchMxField, curX, curY);
			}
		return true;
	}
	return false;
}

void MxField::explosion(PtrMx mx, PtrMx watchMxField, const IPoint& p)
{
	explosion(mx, watchMxField, p.x, p.y);
}

void MxField::explosion(PtrMx mx, PtrMx watchMxField, int j, int i)
{
	for (int dj = -1; dj <= 1; dj++)
		for (int di = -1; di <= 1; di++)
			if (di*dj == 0)
				if (posInMx(j + dj, i + di) && !BoxType::typeIsBig(watchMxField[j + dj][i + di]))
				{
					int curX = j + dj;
					int curY = i + di;
					int curType = watchMxField[curX][curY];
					int realType = mx[curX][curY];
					if (BoxType::typeIsLock(curType) && BoxType::typeIsLock(realType) || // если мы еще не обработали замок
						!BoxType::typeIsLock(curType) && !BoxType::typeIsLock(realType)) // или там вообще нет замка
						clear(mx, j + dj, i + di);
					if (BoxType::typeIsBomb(curType))
					{
						clear(watchMxField, j + dj, i + di);
						explosion(mx, watchMxField, j + dj, i + di);
					}
				}
}

// возвращает такую же позцию как gamePos но после свопа
// gamePos._numMove--
void MxField::makeSwap(PtrMx curPos, Swap& sw)
{
	int x1 = sw.A.x;
	int x2 = sw.B.x;
	int y1 = sw.A.y;
	int y2 = sw.B.y;
	if (sw.changeType >= 0)
	{
		if (sw.changeType == boxType::Destoyed)
			clear(curPos, x1, y1);
		else if (sw.changeType == boxType::Bomb)
			curPos[x1][y1] = BoxType::toBomb(curPos[x1][y1]);
		else if (sw.changeType == boxType::Lock)
			curPos[x1][y1] = BoxType::toLock(curPos[x1][y1]);
		else if (sw.changeType == boxType::grow)
			curPos[x1][y1] = BoxType::toBig(curPos[x1][y1]);
		else if (!BoxType::typeIsBig(curPos[x1][y1]))
			curPos[x1][y1] = sw.changeType;
	}
	else
		makeSwap(curPos, x1, y1, x2, y2);
}

bool MxField::posInMx(const IPoint& pos)
{
	return posInMx(pos.x, pos.y);
}

bool MxField::posInMx(int j, int i)
{
	return j >= 0 && j < _maxJ && i >= 0 && i < _maxI;
}

void MxField::makeSwap(PtrMx curPos, int x1, int y1, int x2, int y2)
{
	if (BoxType::typeIsBig(curPos[x1][y1]) || BoxType::typeIsBig(curPos[x2][y2]))
		return;
	std::swap(curPos[x1][y1], curPos[x2][y2]);
}


bool MxField::isClear(PtrMx mx, const IPoint& p)
{
	return isClear(mx, p.x, p.y);
}

void MxField::clear(PtrMx mx, const IPoint& p)
{
	clear(mx, p.x, p.y);
}

bool MxField::isClear(PtrMx mx, int j, int i)
{
	return isClear(mx[j][i]);
}

bool MxField::isClear(int color)
{
	return color < 0;
}

void MxField::clear(PtrMx mx, int j, int i)
{
	if (BoxType::typeIsLock(mx[j][i]))
		mx[j][i] = BoxType::unLock(mx[j][i]);
	else
		mx[j][i] = -1;
}

void MxField::createBox(PtrMx mx, const IPoint& p, int type)
{
	createBox(mx, p.x, p.y, type);
}

void MxField::createBox(PtrMx mx, int j, int i, int type)
{
	mx[j][i] = type;
}