#pragma once
#include "SizableButton.h"
#include "GameSettings.h"

// уровни номеруються с 1

// формирование на экране cписка уровней, реакция на клик
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

	// вызыывать для обработки нажатия мыши
	void press();

	LevelList(GameSettings::Ptr, Render::Texture*, int availableLvl = -1);
	~LevelList();

private:
	// номер первого уровня
	const int _firstLevelNum = 1;
	const float _buttonDistance = 10; // растояние между кнопками

	void generateLevelList(const FPoint& levlListPos, GameSettings::Ptr settings, Render::Texture* _tex);
	void refreashAvailableLvl();

	SizableButton::PtrList _levelList;
	GameSettings::Ptr _settings;

	int _levelSellected;
	int _maxILevel;
	int _maxJLevel;
	int _maxLevel;
	int _availableLvl; // количество доступных уровней
};

