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
	typedef std::vector <IPoint> posList; // ������ �������
	// �������� ���������� (������ ��� ��������� ������� ��� �������)
	struct primitive
	{
		posList needClear;   // ����� ������� ������ ���� �� ������
		posList needBe;		 // �� ���� �������� ������ ���� ����� �� ����������
		posList needUpAndCreate;  // ������ ������� � ������� �� ���� ����� ������� (�� ������ ���������) � ��������� �����
		Swap::List needSwap; //��� ����� ��������
		posList willCreate;  // ������ ������������� ����������� ������
	};
	/*
	�������� ����������:
		0) ��������� needClear � needBe
		1) ������� � ��������� (������� ������, ��������� �����)
		2) ��������
		3) ��������� (����� �����)
	*/
	
	GameSettings::Ptr _settings;
	PtrMx mxField;
	PtrMx tmpMxField; // ��� �������������� ����������� ��� tmpMxField (������� ��� ����� �������������� ���������� � try)

	GameFileReader _fileReader;

	int _maxCascadeSize = 3; // ������������ ���������� ��������
	//int _boxTypesCount;		 // ���������� ����� ������������ ������

	short prstn_changeTypePrimitive;
	short prstn_axePrimitive;
	short prstn_bombPrimitive;
	short prstn_lockPrimitive;
	short prstn_bigPrimitive;

	void generateMove(int boxTypesCount = 3);

	// ������ ���������� try ��������� �������������� � ������� tmpMxField

	// ���������� ������� � ������� (��������) ����� ����������� ��� ������
	bool tryUseCascade(const IPoint& pos, int size = 3, bool vertical = false, int type = 1);
	// ������� ��������� �������� 
	bool tryUsePrimitive(const IPoint& ptimitivePos, primitive p, int bType);

	// �������� �������� � ������ �������
	void offsetPrimitive(const IPoint& offset, primitive&);

	bool tryUpAndCreate(const posList&, int type);
	void tryCreate(const posList&, int type);

	void useChanges();
	void undoChanges();
	
	// ����������� ��������� 
	// 43% SeamplePrimitive
	// 17% ChangeTypePrimitive
	// 10% AxePrimitive
	// 10% BombPrimitive
	// 10% LockPrimitive
	// 10% BigBoxPrimitive
	int generateCascadeType(); // ���������� ��� ������� 
	// ������� ��������� ����������
	// length - �������� 5
	// swapDirrect - �������� 4 + (5 - length) * 2 // ���� �� ������ �������
	primitive createSeamplePrimitive(bool vertical, int length);					// 0 ���
	primitive createChangeTypePrimitive();										    // 1 ���
	primitive createAxePrimitive();													// 2 ���
	primitive createBombPrimitive();												// 3 ���
	primitive createLockPrimitive();												// 4 ���
	primitive createBigBoxPrimitive();												// 5 ���

	posList generateCascade(bool vertical, int length);

	void upPos(PtrMx, const IPoint& pos); //��������� ����� ������� � ������� pos

	// �������� �������������� - �� ��������� ��������� ���������� ����� ����� (������������� ��� ��������)
	// ������ ��� ������ ���� �� �������
	bool cascadeIsOriginal(PtrMx, const posList&);
	
	// IPoint sizeConvexHull(const posList&); // ���� ������� ��������� (������ �������� �������� ������������� ����� �����)
	int getCountClearOnX(PtrMx mx, int j); // ���������� ��������� ������ � � ����������� j
	posList getFreeTopPos(PtrMx mx);       // ������ ����� ������� ��������� ������� (���� �������)
	posList getRelatedPoint(const posList& pList); // ������ ������� ������ ������� ������ pList
	
};
/*
xx x samplePrimitive



*/