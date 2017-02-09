#pragma once
#include "GameSettings.h"

// класс перебора отклонений величин (сейчас перебирает случайно)
class DeltaIJBox
{
public:
	bool next(); // вернёт лож поле нескольких неудачных попыток

	int get_di(); // отклонение по оси Y
	int get_dj(); // отклонение по оси Х
	int get_db(); // номер типа ящика
	int get_dp(); // номер примитива

	void reset();
	void setMaxTry(int maxTry); 

	DeltaIJBox(GameSettings::Ptr settings, int maxJ, int maxI, int maxB, int maxP, int dj, int di, int db, int dp);
	~DeltaIJBox();
private:
	int _curTry;
	int _maxTry;

	int _di; // отклонение по y координате
	int _dj; // ... х
	int _db; // типу ящика
	int _dp;

	int _maxI;
	int _maxJ;
	int _maxB;
	int _maxP;

	int _diStart;
	int _djStart;
	int _dbStart;
	int _dpStart;

};
