#pragma once
#include "Box.h"

// ������ ���������� � ������� �������
class GamePosition
{
public:
	Box::boxInfoList boxList; // ������ ������
	int _numMove; // ���������� ���������
	int _maxNumMove; // ������������ ���������� �����

	GamePosition();
	~GamePosition();
	
};