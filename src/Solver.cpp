#include "stdafx.h"
#include "Solver.h"


Solver::Solver(GameSettings::Ptr settings)
	: MxField(settings->getMaxJField(), settings->getMaxIField())
{
	_boxTypesCount = settings->getBoxTypesCount();
}


Solver::~Solver()
{

}

// возвращает список доступных свопов
Swap::List Solver::getSwapList(PtrMx curPos)
{
	/*
	считаем дл€ каждой €чейки свопы либо с пустой позицией либо влево либо вверх
	либо замену цвета у вопросительных кробок
	*/
	Swap::List swList;
	for (int j = 0; j < _maxJ; j++)
		for (int i = 0; i < _maxI; i++)
		{
			int curType = curPos[j][i];
			// зан€тую вверх
			if (i + 1 < _maxI)
			{
				int upType = curPos[j][i + 1];
				if (curType != upType)
					if (!isClear(curPos, j, i + 1) &&
						!BoxType::typeIsLock(upType) && !BoxType::typeIsLock(curType) &&
						!BoxType::typeIsBig(upType) && !BoxType::typeIsBig(curType))
					{
						Swap sw = createSwap(j, i, j, i + 1);
						swList.push_back(sw);
					}
			}
			// любую влево
			if (j - 1 >= 0)
			{
				int lType = curPos[j - 1][i];
				if (curType != lType
					&& !BoxType::typeIsLock(lType) && !BoxType::typeIsLock(curType)
					&& !BoxType::typeIsBig(lType) && !BoxType::typeIsBig(curType))
				{
					Swap sw = createSwap(j, i, j - 1, i);
					swList.push_back(sw);
				}
			}
			// потдержка QuestionBox
			if (curPos[j][i] == boxType::QuestionBox)
				for (int t = 0; t < _boxTypesCount-1; t++)
				{
					Swap sw = createSwap(j, i, j, i);
					sw.changeType = t;
					swList.push_back(sw);
				}
			// потдержка топора
			if (!isClear(curPos, j, i) && !BoxType::typeIsBig(curType)) // всегда можно попробовать топором
			{
				Swap sw = createSwap(j, i, j, i);
				sw.changeType = boxType::Destoyed;
				swList.push_back(sw);
			}
		}
	return swList;
}


GameTree::Ptr Solver::getGameTree(GamePosition& curPos)
{
	PtrMx mxField = generateMxField(curPos);
	if (isEmpty(mxField))
		return nullptr;
	int minDeepToSolve = curPos._maxNumMove + 1;
	handleMove(mxField);

	GameTree::Ptr root = makeFullGameTree(mxField, curPos._numMove, minDeepToSolve);
	deleteMX(mxField);
	if (GameTree::freeTree(root, 0, minDeepToSolve))
		return nullptr;
	if (GameTree::havePathWithOutAxe(root))
		GameTree::freeTreeWithAxe(root);
	curPos._maxNumMove = minDeepToSolve;
	return root;
}

GameTree::Ptr Solver::makeFullGameTree(PtrMx curPos, int numMove, int& bestPath)
{
	if (numMove >= bestPath)
		return nullptr;

	GameTree::Ptr curNode = GameTree::Node::makeNode(Swap::getZeroSwap());

	Swap::List curSwaps = getSwapList(curPos);
	int curBestPath = bestPath;

	for (auto curSwap : curSwaps)
	{
		PtrMx newPos = getAfterSwap(curPos, curSwap);
		if (isEmpty(newPos))
		{
			bestPath = numMove + 1;
			deleteMX(newPos);
			GameTree::Ptr newSone = GameTree::Node::makeNode(curSwap);
			newSone->lastSwap = curSwap;
			curNode->sons.push_back(newSone);
			continue;
		}
		GameTree::Ptr newSone = makeFullGameTree(newPos, numMove + 1, curBestPath);
		if (newSone)
		{
			newSone->lastSwap = curSwap;
			curNode->sons.push_back(newSone);
		}
		deleteMX(newPos);
		if (curBestPath < bestPath)
			bestPath = curBestPath;
	}
	if (curNode->sons.empty())
	{
		GameTree::Node::freeNode(curNode);
		return nullptr;
	}
	return curNode;
}