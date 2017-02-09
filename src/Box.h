#pragma once
#include "DinamicButton.h"
#include "GameSettings.h"
#include "LogicField.h"
#include "StringLiterals.h"
#include "DinamicButtonMoveOut.h"
#include "BoxType.h"

// ������ ���������� ������������� ������
// ������ �� ������� ������ LogicField
class Box : public DinamicButtonMoveFromOut
{
public:
	//�������� ���������� � �����
	struct boxInfo
	{
		IPoint pos;
		boxType type;
	};

	typedef std::vector < Box::boxInfo > boxInfoList;

	typedef std::shared_ptr<Box> Ptr;
	typedef std::vector < Box::Ptr > PtrList;
	typedef std::vector < PtrList > PtrMX;
	
	Box(const IPoint& pos, GameSettings::Ptr gameSettings, LogicField::Ptr, Render::Texture* tex, boxType, Render::Texture* texLabel = nullptr);
	~Box();

	void unLockIt();
	bool isLockIt();

	bool isBond();
	bool isBig();

	void update(float dt, bool gravityIsOn = true);
	void draw();

	FPoint getBot();

	void finish();
	bool isFinished();

	/*override*/ void setPress();
	void setDPos(const FPoint&);
	FPoint getDPos() const;
	void setFieldPos(const IPoint&);

	// ��������� ������� ������� ������� ���� � ��� ��� ���� ����� �������� �� ������� 
	IPoint getDPosOnMousePos();
	// �������� DPos ����������� ��� ������ getDPosOnMousePos() ��������� ������� ������
	IPoint tranformDPosOnFieldPos() const;

	IPoint getFieldPos();

	boxType getType();
	boxInfo getInfo();

	bool isBusy() const;
	// �������� �� ����� ��������
	bool dPosIsCorrect(const FPoint& dPos) const;

	void setType(boxType type);

	// ���������� ��� ������� ���������� ������
	std::list <IPoint> getAllPos();
private:

	std::vector <int> _ID;
	boxType _type;
	float _g;
	float _v0;
	float _speed;
	float _HDrawRect;
	float _WDrawRect;

	// �������� �� ��������� ����������� (��� ���������� � ������� ���������� ����� ��������� �� ����� �������� �������)
	// ��� �������� ������� �� ������ �� ������ � ��������� � ������� ������
	FPoint _dPos;

	IPoint _posWhenPress;

	LogicField::Ptr _logic;

	GameSettings::Ptr _settings;

	// � ������� ������ �������� ������ � default, dead, falling  
	enum stance { default, dead, falling, hiding, showing };
	stance _curStance;

	stance getCurStance() const;

	// ��������� ���������� �������� ������� (�� ���������� � ��������� ��������)
	void draw(const FPoint& dPos);
	void update(const FPoint& dPos, float dt);

	void logicMove();

	FRect getDrawRect() const;

};

