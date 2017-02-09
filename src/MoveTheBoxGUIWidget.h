#pragma once
#include "GameFileReader.h"
#include "GameField.h"
#include "DinamicButton.h"
#include "SizableButton.h"
#include "DinamicButtonMoveOut.h"
#include "LevelList.h"
#include "labelView.h"
#include "Box.h"
// тут удобно брать текстуры
// http://www.youtube.com/watch?v=zfxE0JKvI5c
// http://www.youtube.com/watch?v=Yz16z0SLOlw

// сигнал HandleGuiEvents??
// оптимизировать функции деревьев???

// в данный момент 4 рандомный нормальный уровень

// замки можно взорвать и разрубить топором (чтобы открыть)
// бомбы взрываються каскадом, 
// Бомбы можно разрубить тапором (без взрыва)
// большой ящик не ломаем, игрок его не может передвинуть

// Текстуры брал из видео (поэтому качество среднее) 
// текстуры могут иметь любое разрешение и пропорции, главное чтобы пропорции элементов в текстуре относительно друг друга не менялись
// не нашел текстуры кнопки ретрая, поэтому использую текстуру Done

class MoveTheBoxGUIWidget :
	public GUI::Widget
{
public:
	MoveTheBoxGUIWidget(const std::string& name, rapidxml::xml_node<>* elem);

	void Draw();
	void Update(float dt);

	void AcceptMessage(const Message& message);

	bool MouseDown(const IPoint& mouse_pos);

	void MouseMove(const IPoint& mouse_pos);
	void MouseUp(const IPoint& mouse_pos);

	// инициализация игры
private:
	void Init();

	void fillInTextures();

	// временная
	void LoadTestLvl();

private:
	const FRect _btnSmall = FRect(0, 50, 0, 50);
	const FRect _btnNormal = FRect(0, 150, 0, 150);
	const float _btnDistance = 10; // растояние между  кнопками
	const float _maxSpeedButton = 100;
	const float _backgroundScale = 1.5; // масштаб фона
	const float _effSwapSize = 60;
	const float _timeFastShowSellectBox = 1.5f; // время показа подсказки о выборе ящика
	const bool _alwaysHelp = true; // подсказки бесконечны 
	int _normalRandomLVL; // уровень который будет генериться случайно 
	//bool _needShowLoadMessage; // В данной версии всё делаетсья в одном потоке, поэтому используеться для показа надписей типа "Loading..."

	// максимальное количество подсказок и помощи на уровне
	// обнуляются при загрузке уровня
	int _curBackMove; 
	int _curHelpNumber;
	int _curMove; 

	// позиция кнопки хода назад нужна для эффекта указания на кнопку, при тупиковой ситуации 
	FPoint _posBackMove;

	enum gameStance {mainMenue, starting , gaming, finishing};
	gameStance _curGameStance;

	SizableButton::Ptr _btnDone;
	LabelView::Ptr _messagePanel;
	std::string _gameMess; // текущее игровое сообщение

	float _timer;
	int _curLevelNumber;

	GameFileReader _fileReader;

	// задний фон
	Render::Texture* _texBackGround;
	FPoint _backgroundPos; // позиция фона
	

	// текстуры кнопок уровней
	Render::Texture* _texLevelList;

	// текстуры ящиков
	Render::Texture* _texBox;

	// текстуры кнопок
	std::map <btnType, Render::Texture*> _texBtn;

	ParticleEffect *_effBoxHide;
	// определяеться как кнопка (часть GUI)
	FRect _axeRect = FRect(0, 55, 0, 45);
	ParticleEffect *_effAxe;
	bool _usedAxe;
	

	GameSettings::Ptr _gameSettings;
	GameField::Ptr _gameField;
	LevelList::Ptr _levelList;

	DinamicButtonMoveFromOut::PtrList _buttonListGameGui;

	EffectsContainer _effCont;	

	bool haveBackMove();
	bool haveHelp();

	// присваиваем сообщение по умолчанию игровому сообщению
	void setDafaustGameMessage();
	void showLoadMessage();

	// показывает эффект подсказки для ящиков 
	void showSwap(const Swap&);

	void UpdateGameGui(float dt);
	void UpdateMainMenue(float dt);

	void drawBackground(); // прорисовка фона
	void drawGameGui();    // прорисовка кнопок, ящиков и надписей в игре
	void drawMainMenue();  // прорисовка меню выбора уровней

	// Загружает интерфейс игры
	void addGameGUI();
	// загружает интерфейс главного меню
	void addMainMenueGui();

	// ...показывает
	void showGameGUI();
	void showMainMenueGui();

	// ... скрывает
	void hideAllGui();
	void hideGameGUI();
	void hideMainMenueGui();

	void destroyGUIAll();

	// обработка событий отпускания клавиш
	void handleButtonUnpress();
	void handleButtonUnpressGameGui();
	void handleButtonUnpressMainMenue();

	// ... нажатия
	void handleButtonPress();
	void handleButtonPressGameGui();
	void handleButtonPressMainMenue();

	void soundOff();
	void soundOn();

	// если не будем генерить как то по особому уровни (от пользовательских параметров) можно перенести в GameField
	// сейчас генерация зависит от номера уровня (последний генерим случайно, прописанно в конструкторе)
	void loadLevel(int number, bool retry = false);
	void loadLevel(std::string nameLevel);
	void loadNextLevel(); // грузит следующий уровень, если уровень максимальный грузит 24
	void reloadLevel();

	void setReTray();
	void setDone();

	bool notHaveAction();

	// предстартовая подготовка завершена
	bool startingIsFinish();
	// .. пред завершающая
	bool finishingIsFinish();

	std::string generateLevelName(int number);
};

