#pragma once
#include <algorithm>
#include "GamePosition.h"
#include "LogicField.h"
#include "Solver.h"
#include "GameFileReader.h"
#include "LogFuncs.h"
#include "Delta.h"

class LevelGenerator : protected Solver
{
public:
	typedef std::shared_ptr <LevelGenerator> Ptr;

	GamePosition generate(int boxTypesCount = 3, int moveCountToWin = 2, std::string nameLevel = "someLevel");

	LevelGenerator(GameSettings::Ptr);
	~LevelGenerator();
private:
	typedef std::vector <IPoint> posList; // список позиций
	// описание примитивов (правил для получения позиции для решения)
	struct primitive
	{
		posList needClear;   // точки которые должны буть не заняты
		posList needBe;		 // на этих позициях должны быть ящики до применения
		posList needUpAndCreate;  // позции начиная с которых всё выше нужно поднять (до первой свободной) и поставить ящики
		Swap::List needSwap; //что нужно поменять
		posList willCreate;  // список дополнительно создаваемых ящиков
	};
	/*
	алгоритм применения:
		0) проверить needClear и needBe
		1) Поднять и поставить (поднять старые, поставить новые)
		2) Поменять
		3) Поставить (новые ящики)
	*/
	
	GameSettings::Ptr _settings;
	PtrMx mxField;
	PtrMx tmpMxField; // все преобразования проводяться над tmpMxField (финкции для таких преобразований начинаются с try)

	GameFileReader _fileReader;

	int _maxCascadeSize = 3; // максимальное количество каскадов
	//int _boxTypesCount;		 // количество типов используемых ящиков

	short prstn_changeTypePrimitive;
	short prstn_axePrimitive;
	short prstn_bombPrimitive;
	short prstn_lockPrimitive;
	short prstn_bigPrimitive;

	void generateMove(int boxTypesCount = 3);

	// методы отмеченные try применяют преобразования к матрице tmpMxField

	// возвращает позиции в каторых (примерно) нужно расположить что нибуть
	bool tryUseCascade(const IPoint& pos, int size = 3, bool vertical = false, int type = 1);
	// пробует применить примитив 
	bool tryUsePrimitive(const IPoint& ptimitivePos, primitive p, int bType);

	// сдвигает примитив в нужную позицию
	void offsetPrimitive(const IPoint& offset, primitive&);

	bool tryUpAndCreate(const posList&, int type);
	void tryCreate(const posList&, int type);

	void useChanges();
	void undoChanges();
	
	// вероятности появления 
	// 43% SeamplePrimitive
	// 17% ChangeTypePrimitive
	// 10% AxePrimitive
	// 10% BombPrimitive
	// 10% LockPrimitive
	// 10% BigBoxPrimitive
	int generateCascadeType(); // генерирует тип каскада 
	// функции генерации примитивов
	// length - максимум 5
	// swapDirrect - максимум 4 + (5 - length) * 2 // одна из сменых позиций
	primitive createSeamplePrimitive(bool vertical, int length);					// 0 тип
	primitive createChangeTypePrimitive();										    // 1 тип
	primitive createAxePrimitive();													// 2 тип
	primitive createBombPrimitive();												// 3 тип
	primitive createLockPrimitive();												// 4 тип
	primitive createBigBoxPrimitive();												// 5 тип

	posList generateCascade(bool vertical, int length);

	void upPos(PtrMx, const IPoint& pos); //поднимает ящики начиная с позиции pos

	// проверка оригинальности - не допускает скопления одинаковых точек рядом (используеться для каскадов)
	// каскад уже должен быть на матрице
	bool cascadeIsOriginal(PtrMx, const posList&);
	
	// IPoint sizeConvexHull(const posList&); // ищем граници примитива (размер выпуклой оболочки положительных точек точек)
	int getCountClearOnX(PtrMx mx, int j); // количество свободных ящиков с Х координатой j
	posList getFreeTopPos(PtrMx mx);       // список самых верхних доступных позиций (выше пустота)
	posList getRelatedPoint(const posList& pList); // список смежных точекс точками списка pList
	
};
/*
xx x samplePrimitive



*/