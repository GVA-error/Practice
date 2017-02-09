#pragma once
#include "GamePosition.h"
#include "GameSettings.h"
#include "GameTree.h"
#include "MxField.h"

// ��� ����� ������ ����

class Solver : public MxField
{
public:
	typedef std::shared_ptr <Solver> Ptr;

		// maxDeepPath - ������������ ������ �������� ���� (������������ ������� ��������) 
		// GamePosition._maxNumMove - ������ ������� ���� (��� ����������)
	GameTree::Ptr getGameTree(GamePosition& curPos);

	Solver(GameSettings::Ptr);
	~Solver();
protected:
	int _boxTypesCount;	// ���������� ����� ������������ ������

	// ������������ � ������� �������
	GameTree::Ptr makeFullGameTree(PtrMx curPos, int numMove, int& bestPath);
	// ���������� ������ ��������� ������
	Swap::List getSwapList(PtrMx);

};
