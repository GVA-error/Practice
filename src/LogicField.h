#pragma once
#include "GameSettings.h"
#include <algorithm>
#include <iterator> 
#include <utility> 

// ����� ��������������� ���������������� ������ ����
// ��������� �������, �� �� ���������
// �������� ������ move � getFieldPos
class LogicField
{
public:
	typedef std::shared_ptr <LogicField> Ptr;

	// ������� � ����������� ����
	// � ���� ������� ����� ������������� ������ ����������� ������
	IPoint getPosInMxField(const FPoint&absPos) const;

	// ���������� ������ ������� ���� ������
	FPoint roundedPos(const FPoint&) const;
	FPoint roundedPos(const IPoint&) const;
	
	// ���������� ����� ������
	FPoint getCentrCell(const IPoint&) const;

	// �������� ��������� ����� � ������
	bool posInCell(const FPoint& pos, const IPoint& fieldPos) const;

	// ����� �� ������ � ����� ID ������ �� ������
	bool posIsFree(const IPoint&, int ID) const;
	bool posIsFree(const FPoint&, int ID) const;

	void move(int ID, const FPoint& newPos); // ���������� ������
	void create(int ID, const FPoint& pos); // ������ ������ ���, ����� ������ �����
	void destoy(int ID);

	// ��������� ������� ID
	bool haveID(int ID);
	bool posIsCorrect(const IPoint&) const;
	bool posIsCorrect(const FPoint&) const;

	// ������� ������� � ����������� ���� (�� ���������� ������ ������������ ������ �����)
	void reset();

	float getHCell();
	float getWCell();

	int getMaxI();
	int getMaxJ();

	IPoint getFieldPos(int ID);

	FRect getCellRect();

	bool isFly(int ID);

	int getID(const IPoint&) const;

	// ���������� ����� ID (���������� �� �������)
	int generateNewID();

	LogicField(GameSettings::Ptr);
	~LogicField();

protected:
	void retuneLogic(GameSettings::Ptr);
private:
	const int maxID = 100;
	FPoint _fieldPos;
	FRect _fieldRect;

	int _maxI, _maxJ;

	std::map <int, FPoint> _objRealPos;
	std::map <int, IPoint> _objFieldPos;

	//std::map <IPoint, int> _fieldPosObj;

	FRect _CellRect; // ������������� ����� ������ (��������� ����� �������� �������)


};

