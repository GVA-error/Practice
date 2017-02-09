#pragma once
#include "GameInfo.h"
#include "StringLiterals.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

// ��������� ������� ���������
class GameSettings : protected GameInfo
{
public:
	typedef std::shared_ptr <GameSettings> Ptr;

	const bool showCursorPos = false;

	void save();

	void setAvailableLvl(int);
	void resetAvailableLvl(); // ������������� ���������� ��������� ������� � 1

	float getScreenW() const;
	float getScreenH() const;
	FRect getFieldRect() const;
	FRect getLevelListRect() const;
	FPoint getCentrField() const;

	FPoint getLevelListPos() const;

	float getg() const;
	float getV0()const;

	// I - � ��������� � �������, J - ��������� � �������
	int getMaxIField() const;
	int getMaxJField() const;

	int getMaxILevel() const;
	int getMaxJLevel() const;
	int getMaxLevel() const;

	int getAvailableLvl() const;
	void incAvailableLvl();

	int getBoxTypesCount() const;

	short getPrstn_changeTypePrimitive() const;
	short getPrstn_axePrimitive() const;
	short getPrstn_bombPrimitive() const;
	short getPrstn_lockPrimitive() const;
	short getPrstn_bigPrimitive() const;

	int getMaxHelpCount() const;
	int getBackStapCount() const;

	int getMaxCascadeSiae() const;
	int getMaxNumTryInDelta() const;

	GameSettings();
	~GameSettings();
private:

	int _screenW;
	int _screenH;

	// ��������� ���������� �������
	const float _g = 400;
	const float _v0 = 77; // ��������� (�����������) �������� ����� (���� ��������� ������� ������� ����� ��������...) 
	// ������������ �������� � ������� �� ������ ����� ��������� ������� ViewObject::_defaultMaxSpeed, ������ �������� �������� ����� ���� ������

	//const bool 
	const int _maxHelp = 2; // ������������ ���������� ������� �� ���������
	const int _maxBackStap = 5; // .. ��� �����

	// ������� ����
	const int _maxJField = 7;
	const int _maxIField = 8;

	// ������� ������� ������ ������
	const int _maxILevel = 6; 
	const int _maxJLevel = 4;
	const int _maxLevel = 24;

	// ��������� ��������� �������
	const int _maxCascadeSiae = 2; // ������������ ������ ������� (����������� ������� ���������� ����� �������� ����)
	const int _maxNumTryInDelta = 700; // ������������ ���������� ������� ��������� ��� Delta

	// �������� ��������� ���� ��� ����� ��������� ��� ��������� ����
	// ��������� ������� ���������� ������ ������ (�� ��������)
	const short prstn_changeTypePrimitive = 17;
	const short prstn_axePrimitive = 10;
	const short prstn_bombPrimitive = 10;
	const short prstn_lockPrimitive = 10;
	// 5 ��� ��� ������ �������
	const short prstn_bigPrimitive = 5;

	const int _defaultAvailableLvl = -1; // �� ��������� ��� �������
	int _availableLvl; // ���������� ��������� �������, ������ �� GameInfo.xml

	// ���������� ����� ������
	const int _boxTypesCount = 6;

	// ������������� �������� ����, ����� � ������������
	FRect _fieldRect;
	FRect _LevelListRect;
};

