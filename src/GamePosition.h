#pragma once
#include "Box.h"

// хранит информацию о игровой позиции
class GamePosition
{
public:
	Box::boxInfoList boxList; // список ящиков
	int _numMove; // количество сделанных
	int _maxNumMove; // максимальное количество ходов

	GamePosition();
	~GamePosition();
	
};