#pragma once
#include <vector>
#include <boost/shared_ptr.hpp>
#include <memory>
#include <stack>

#include "StringLiterals.h"
#include "LogicField.h"
#include "GameSettings.h"
#include "GamePosition.h"
#include "Solver.h"
#include "GameFileReader.h"
#include "LevelGenerator.h"
#include "QuestionBoxMenue.h"
#include "Box.h"

// �������� �� ������� ������ ��������� ����� ����
class GameField
{
public:
	typedef std::shared_ptr < GameField > Ptr; 

	void SetGravityOn();
	void SetGravityOff();
	bool GravityIsOn() const;
	bool GravityIsOff() const;

	void doMatch();

	void loadNewLevel(const std::string& nameLevel);
	void loadNewLevel(const GamePosition& level, const std::string& nameLevel);

	void generateNormalRandomLevel(std::string name);
	void generateEasyRandomLevel(std::string name);
	void generateHardRandomLevel(std::string name);

	bool isBusy(); // �� ���� ��� �� ���������� (�������, ����� �� ������� ��� ����)

	void clear();
	void saveForBackMove();
	void clearBackStap();

	void update(float dt);
	void draw();

	void show();
	void hide();

	bool isHide();
	bool isShow();

	// ������ ���
	void pressLB();
	// ���������
	void unPressLB();

	void pressRB();
	void unPressRB();

	void backMove();
	void fastShowSellect(const FPoint& pos, float timeBeforHide = 1);
	FPoint getPosBoxTypeInSellect(boxType);

	bool levelIsDone();
	bool isGaming();

	// ���������� � ��������� ����
	Swap getNextSwap();

	// ���������� ������� �� ������, �� ������� �� ����
	FPoint getRealPos(const IPoint&); // ����� ��� ������ ������������ ����� (������ �������� ����)

	// withAxe - c ������ �����
	bool destroyBox(const FPoint&, bool withAxe=false);
	bool destroyBox(const IPoint&, bool withAxe = false);

	void explosion(const IPoint&);

	bool needUseAxe();

	void FieldTune(GameSettings::Ptr gameSettings);

	int getMaxNumMove();
	int getCurNumMove();

	bool haveMove();
	bool isFirstMove();

	GameField(GameSettings::Ptr gameSettings, Render::Texture* texBox);
	~GameField();
private:
	// ��������� ������ (������������ ������� �����) ����� ��������� ���� ����� ���������� ����� �������
	const float kForSwap = 0.99;

	// �������� ��� ���������/������������ ������
	FPoint dPosOnShowHide;
	// ������������ �������� ����� (�� ����� ����), �������������� �� kForSwap
	FPoint absMaxDPos;
	std::string _nameLevel; // ������� �������� ������

    bool _gravityOn;
	bool _havePathWithOutAxe;
	bool _show;
	int _boxesCount;
	int _maxNumMove;

	GameTree::Ptr _gameTree;
	Box::PtrList _boxPtrList;
	std::stack <GamePosition> _moveStack;
	Swap::List _historySwaps;

	// � ������� ������ ���������� ������� �������� ������ ������� _boxPtrMx[column][string] ~ _boxPtrMx[x][y] 
	Box::PtrMX _boxPtrMx; // ������ ���������� ������ + �������� ����� �� ������� � ���� �����

	// ������� ���������������� ��� �������
	Box::Ptr _boxA, _boxB;

	LogicField::Ptr _logic;
	GameSettings::Ptr _settings; 
	
	// �������������� ������� ����
	Solver::Ptr _solver;
	GameFileReader _fileReader;
	LevelGenerator::Ptr _levelGenerator;
	QuestionBoxMenue::Ptr _questionBoxMenue;

	EffectsContainer _effTemp;

	Render::Texture* _texBox;

	// ������ ����� ������� (������ ������) �� ������� �������, ������ ������ ������� 
	// �� ������� ������ ����
	void createLevel(const GamePosition&);
	// ������� ���� ���� � ������ �������� ������� ���� 
	void createBox(const Box::boxInfo&);
	// ������ ��� ����� (��������� ���)
	void setTypeBox(const IPoint& boxPos, boxType);

	// ����� ������� ����� ������� ������ ����
	void gameMoveNew();
	
	void updateBoxs(float dt);
	void updateSwapBox(float dt);
	void updateBoxSellectMenue(float dt);

	void changePosInMx(IPoint oldPos, IPoint newPos, Box::Ptr);

	// ������ ����� �������
	void makeSwap(const IPoint& posA, const IPoint& posB);

	// �������� ����������� �������� (����������) ���� ��������
	void OnSlowMoveForAll(); // �������� ��� ����� ������������
	// .. ���������, ���������� ��������� ��� ��������� �������� ������� �������
	void OffSlowMoveForAll(); // ��������� ����� ����� ������� ������ (��� ������������ OnSlowMoveForAll())

	void refreashSlowMove();

	void showHideEffInPos(const IPoint&);
	void showUnlockEffInPos(const IPoint&);

	GamePosition getGamePosition();

	void loadLevelTree(); 
	void refreashBoxCount();
};
