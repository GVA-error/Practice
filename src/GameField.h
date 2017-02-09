#pragma once
#include <vector>
#include <boost/shared_ptr.hpp>
#include <memory>
#include <stack>

#include "StringLiterals.h"
#include "LogicField.h"
#include "GameSettings.h"
#include "GamePosition.h"
#include "Solver.h"
#include "GameFileReader.h"
#include "LevelGenerator.h"
#include "QuestionBoxMenue.h"
#include "Box.h"

// отвечает за игровую логику поведения всего поля
class GameField
{
public:
	typedef std::shared_ptr < GameField > Ptr; 

	void SetGravityOn();
	void SetGravityOff();
	bool GravityIsOn() const;
	bool GravityIsOff() const;

	void doMatch();

	void loadNewLevel(const std::string& nameLevel);
	void loadNewLevel(const GamePosition& level, const std::string& nameLevel);

	void generateNormalRandomLevel(std::string name);
	void generateEasyRandomLevel(std::string name);
	void generateHardRandomLevel(std::string name);

	bool isBusy(); // На поле что то происходит (падение, какие то эффекты или матч)

	void clear();
	void saveForBackMove();
	void clearBackStap();

	void update(float dt);
	void draw();

	void show();
	void hide();

	bool isHide();
	bool isShow();

	// зажали ЛКМ
	void pressLB();
	// отпустили
	void unPressLB();

	void pressRB();
	void unPressRB();

	void backMove();
	void fastShowSellect(const FPoint& pos, float timeBeforHide = 1);
	FPoint getPosBoxTypeInSellect(boxType);

	bool levelIsDone();
	bool isGaming();

	// подсказака о следующем ходе
	Swap getNextSwap();

	// возвращает позицию на экране, по позиции на поле
	FPoint getRealPos(const IPoint&); // нужно для выбора неизвестного ящика (откуда вылетает меню)

	// withAxe - c топора топор
	bool destroyBox(const FPoint&, bool withAxe=false);
	bool destroyBox(const IPoint&, bool withAxe = false);

	void explosion(const IPoint&);

	bool needUseAxe();

	void FieldTune(GameSettings::Ptr gameSettings);

	int getMaxNumMove();
	int getCurNumMove();

	bool haveMove();
	bool isFirstMove();

	GameField(GameSettings::Ptr gameSettings, Render::Texture* texBox);
	~GameField();
private:
	// насколько далеко (относительно размера ящика) нужно протащить ящик чтобы защиталась смена позиции
	const float kForSwap = 0.99;

	// смещение при появлении/исчезновении ящиков
	FPoint dPosOnShowHide;
	// максимальное смещение ящика (по обоим осям), высчитываеться из kForSwap
	FPoint absMaxDPos;
	std::string _nameLevel; // текущее название уровня

    bool _gravityOn;
	bool _havePathWithOutAxe;
	bool _show;
	int _boxesCount;
	int _maxNumMove;

	GameTree::Ptr _gameTree;
	Box::PtrList _boxPtrList;
	std::stack <GamePosition> _moveStack;
	Swap::List _historySwaps;

	// у матрици первую координату считать столбцом вторую строкой _boxPtrMx[column][string] ~ _boxPtrMx[x][y] 
	Box::PtrMX _boxPtrMx; // служит хранилищем боксов + упрощает обход по матрице с низу вверх

	// объекты заимодействующие при нажатии
	Box::Ptr _boxA, _boxB;

	LogicField::Ptr _logic;
	GameSettings::Ptr _settings; 
	
	// дополнительные объекты поля
	Solver::Ptr _solver;
	GameFileReader _fileReader;
	LevelGenerator::Ptr _levelGenerator;
	QuestionBoxMenue::Ptr _questionBoxMenue;

	EffectsContainer _effTemp;

	Render::Texture* _texBox;

	// Создаём новый уровень (список ящиков) по игровой позиции, чистим старый уровень 
	// не трогаем дерево игры
	void createLevel(const GamePosition&);
	// Создайм один ящик с нужной позицией матрици поля 
	void createBox(const Box::boxInfo&);
	// меняет тип ящика (тратиться ход)
	void setTypeBox(const IPoint& boxPos, boxType);

	// будет вызвана перед началом нового хода
	void gameMoveNew();
	
	void updateBoxs(float dt);
	void updateSwapBox(float dt);
	void updateBoxSellectMenue(float dt);

	void changePosInMx(IPoint oldPos, IPoint newPos, Box::Ptr);

	// меняем ящики местами
	void makeSwap(const IPoint& posA, const IPoint& posB);

	// включить замедленное движения (прорисовки) всех объектов
	void OnSlowMoveForAll(); // включать при клике пользователя
	// .. выключить, необходимо выключать при изменении реальной позиции объекта
	void OffSlowMoveForAll(); // применять после смены позиции ящиков (или перевключить OnSlowMoveForAll())

	void refreashSlowMove();

	void showHideEffInPos(const IPoint&);
	void showUnlockEffInPos(const IPoint&);

	GamePosition getGamePosition();

	void loadLevelTree(); 
	void refreashBoxCount();
};
