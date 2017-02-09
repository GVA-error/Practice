#pragma once
#include "GameSettings.h"

// ����� �������� ���������� ������� (������ ���������� ��������)
class DeltaIJBox
{
public:
	bool next(); // ����� ��� ���� ���������� ��������� �������

	int get_di(); // ���������� �� ��� Y
	int get_dj(); // ���������� �� ��� �
	int get_db(); // ����� ���� �����
	int get_dp(); // ����� ���������

	void reset();
	void setMaxTry(int maxTry); 

	DeltaIJBox(GameSettings::Ptr settings, int maxJ, int maxI, int maxB, int maxP, int dj, int di, int db, int dp);
	~DeltaIJBox();
private:
	int _curTry;
	int _maxTry;

	int _di; // ���������� �� y ����������
	int _dj; // ... �
	int _db; // ���� �����
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
