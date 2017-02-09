#pragma once
#include "GamePosition.h"
#include "GameSettings.h"
#include "GameTree.h"
#include "MxField.h"

// даёт самый лучший своп

class Solver : public MxField
{
public:
	typedef std::shared_ptr <Solver> Ptr;

		// maxDeepPath - максимальная длинна искомого пути (максимальная глубина рекурсии) 
		// GamePosition._maxNumMove - длинна лучшего пути (уже найденного)
	GameTree::Ptr getGameTree(GamePosition& curPos);

	Solver(GameSettings::Ptr);
	~Solver();
protected:
	int _boxTypesCount;	// количество типов используемых ящиков

	// присоеденяет к вершине сыновей
	GameTree::Ptr makeFullGameTree(PtrMx curPos, int numMove, int& bestPath);
	// возвращает список доступных свопов
	Swap::List getSwapList(PtrMx);

};
