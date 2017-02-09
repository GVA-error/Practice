#pragma once
#include "SizableButton.h"
#include "GameSettings.h"

// ������ ����������� � 1

// ������������ �� ������ c����� �������, ������� �� ����
class LevelList
{
public: 
	typedef std::shared_ptr <LevelList> Ptr;

	int selectedLevel();

	void show();
	void hide();
	
	bool isHide() const;
	bool isShow() const;

	bool isHiding() const;

	void update(float dt);
	void draw();

	// ��������� ��� ��������� ������� ����
	void press();

	LevelList(GameSettings::Ptr, Render::Texture*, int availableLvl = -1);
	~LevelList();

private:
	// ����� ������� ������
	const int _firstLevelNum = 1;
	const float _buttonDistance = 10; // ��������� ����� ��������

	void generateLevelList(const FPoint& levlListPos, GameSettings::Ptr settings, Render::Texture* _tex);
	void refreashAvailableLvl();

	SizableButton::PtrList _levelList;
	GameSettings::Ptr _settings;

	int _levelSellected;
	int _maxILevel;
	int _maxJLevel;
	int _maxLevel;
	int _availableLvl; // ���������� ��������� �������
};

