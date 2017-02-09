#pragma once
#include "GameInfo.h"
#include "StringLiterals.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

// различные игровые настройки
class GameSettings : protected GameInfo
{
public:
	typedef std::shared_ptr <GameSettings> Ptr;

	const bool showCursorPos = false;

	void save();

	void setAvailableLvl(int);
	void resetAvailableLvl(); // устанавливает количество доступных уровней в 1

	float getScreenW() const;
	float getScreenH() const;
	FRect getFieldRect() const;
	FRect getLevelListRect() const;
	FPoint getCentrField() const;

	FPoint getLevelListPos() const;

	float getg() const;
	float getV0()const;

	// I - х положение в матрице, J - положение в матрице
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

	// ускорение свободного падения
	const float _g = 400;
	const float _v0 = 77; // начальная (минимальная) скорость ящика (если поставить слишком большую может полететь...) 
	// Максимальная скорость с которой по экрану будут двигаться объекты ViewObject::_defaultMaxSpeed, однако реальная скорость может быть больше

	//const bool 
	const int _maxHelp = 2; // максимальное количество нажатий на подсказку
	const int _maxBackStap = 5; // .. ход назад

	// размеры поля
	const int _maxJField = 7;
	const int _maxIField = 8;

	// размеры матрици выбора уовней
	const int _maxILevel = 6; 
	const int _maxJLevel = 4;
	const int _maxLevel = 24;

	// константы генерации уровней
	const int _maxCascadeSiae = 2; // максимальный размер каскада (конструкции которая обрушиться после текущего хода)
	const int _maxNumTryInDelta = 700; // максимальное количество попыток генерации для Delta

	// проценты появления того или иного примитива при генерации хода
	// остальной процент отводиться обчным свопам (не каскадам)
	const short prstn_changeTypePrimitive = 17;
	const short prstn_axePrimitive = 10;
	const short prstn_bombPrimitive = 10;
	const short prstn_lockPrimitive = 10;
	// 5 так как иногда косячит
	const short prstn_bigPrimitive = 5;

	const int _defaultAvailableLvl = -1; // по умолчанию все достпны
	int _availableLvl; // количество доступных уровней, читаем из GameInfo.xml

	// количество видов ящиков
	const int _boxTypesCount = 6;

	// Прямоугольник игрового поля, задан в конструкторе
	FRect _fieldRect;
	FRect _LevelListRect;
};

