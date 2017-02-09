#pragma once
#include "GamePosition.h"
#include "GameSettings.h"
#include "BoxType.h"
#include "swap.h"

// Класс для работы с матрицей игрового поля 
// матрицу хранит голым указателем // не очень безопасно, однако от этого класса требуется скорость, т.к. матрица используется при построении дерева игры
// для перехода к умным перегрузить/переписать typedef PtrMx; createMX(); deleteMX(PtrMx);
class MxField
{
public:
	typedef std::vector <IPoint> Match;
	Match handleMatch(const GamePosition&);

	MxField(int maxJ, int maxI);
	~MxField();

protected:
	typedef short** PtrMx;
	int _maxI;
	int _maxJ; 
	int _maxSize; // максимальная толщина, 2 если есть 2х2

	PtrMx _wMx;

	GamePosition createPosOnMx(PtrMx);

	Swap createSwap(int x1, int y1, int x2, int y2);

	// возвращает такую же позцию как gamePos но после свопа
	// (делает ход)
	PtrMx getAfterSwap(PtrMx, Swap&);
	void makeSwap(PtrMx, Swap&);
	void makeSwap(PtrMx curPos, int x1, int y1, int x2, int y2);

	void handleMove(PtrMx);

	void handleGravity(PtrMx);
	bool handleMatch(PtrMx);
	bool handleMatch(PtrMx mxField, PtrMx watchMxField, int j, int i);
	bool handleMatch(PtrMx, PtrMx watchMxField, int j, int i, int size, int axis);

	void handleBig(PtrMx mxField); // размещает большие в 4 клетках
	void handleUnBig(PtrMx mxField); // размещает большие в 1 клетке

	// считаем отицательные значения пустыми
	PtrMx generateMxField(const GamePosition& gamePos);
	PtrMx generateMxField(PtrMx curPos);
	void copyMX(PtrMx original, PtrMx copy);
	PtrMx createMX();
	void deleteMX(PtrMx);

	bool isEmpty(PtrMx curPos);

	bool equal(PtrMx, PtrMx);

	bool isClear(PtrMx, const IPoint&);
	bool isClear(PtrMx, int j, int i);
	bool isClear(int color);
	void clear(PtrMx, const IPoint&);
	void clear(PtrMx, int j, int i);

	void explosion(PtrMx, PtrMx watchMxField, const IPoint&);
	void explosion(PtrMx, PtrMx watchMxField, int j, int i);

	bool posIsType(PtrMx, const IPoint&, int type);
	bool posIsType(PtrMx, int j, int i, int type);

	bool posInMx(const IPoint& pos);
	bool posInMx(int j, int i);

	void createBox(PtrMx, const IPoint&, int);
	void createBox(PtrMx, int j, int i, int);
};