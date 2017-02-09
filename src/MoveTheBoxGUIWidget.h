#pragma once
#include "GameFileReader.h"
#include "GameField.h"
#include "DinamicButton.h"
#include "SizableButton.h"
#include "DinamicButtonMoveOut.h"
#include "LevelList.h"
#include "labelView.h"
#include "Box.h"
// ��� ������ ����� ��������
// http://www.youtube.com/watch?v=zfxE0JKvI5c
// http://www.youtube.com/watch?v=Yz16z0SLOlw

// ������ HandleGuiEvents??
// �������������� ������� ��������???

// � ������ ������ 4 ��������� ���������� �������

// ����� ����� �������� � ��������� ������� (����� �������)
// ����� ����������� ��������, 
// ����� ����� ��������� ������� (��� ������)
// ������� ���� �� ������, ����� ��� �� ����� �����������

// �������� ���� �� ����� (������� �������� �������) 
// �������� ����� ����� ����� ���������� � ���������, ������� ����� ��������� ��������� � �������� ������������ ���� ����� �� ��������
// �� ����� �������� ������ ������, ������� ��������� �������� Done

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

	// ������������� ����
private:
	void Init();

	void fillInTextures();

	// ���������
	void LoadTestLvl();

private:
	const FRect _btnSmall = FRect(0, 50, 0, 50);
	const FRect _btnNormal = FRect(0, 150, 0, 150);
	const float _btnDistance = 10; // ��������� �����  ��������
	const float _maxSpeedButton = 100;
	const float _backgroundScale = 1.5; // ������� ����
	const float _effSwapSize = 60;
	const float _timeFastShowSellectBox = 1.5f; // ����� ������ ��������� � ������ �����
	const bool _alwaysHelp = true; // ��������� ���������� 
	int _normalRandomLVL; // ������� ������� ����� ���������� �������� 
	//bool _needShowLoadMessage; // � ������ ������ �� ��������� � ����� ������, ������� ������������� ��� ������ �������� ���� "Loading..."

	// ������������ ���������� ��������� � ������ �� ������
	// ���������� ��� �������� ������
	int _curBackMove; 
	int _curHelpNumber;
	int _curMove; 

	// ������� ������ ���� ����� ����� ��� ������� �������� �� ������, ��� ��������� �������� 
	FPoint _posBackMove;

	enum gameStance {mainMenue, starting , gaming, finishing};
	gameStance _curGameStance;

	SizableButton::Ptr _btnDone;
	LabelView::Ptr _messagePanel;
	std::string _gameMess; // ������� ������� ���������

	float _timer;
	int _curLevelNumber;

	GameFileReader _fileReader;

	// ������ ���
	Render::Texture* _texBackGround;
	FPoint _backgroundPos; // ������� ����
	

	// �������� ������ �������
	Render::Texture* _texLevelList;

	// �������� ������
	Render::Texture* _texBox;

	// �������� ������
	std::map <btnType, Render::Texture*> _texBtn;

	ParticleEffect *_effBoxHide;
	// ������������� ��� ������ (����� GUI)
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

	// ����������� ��������� �� ��������� �������� ���������
	void setDafaustGameMessage();
	void showLoadMessage();

	// ���������� ������ ��������� ��� ������ 
	void showSwap(const Swap&);

	void UpdateGameGui(float dt);
	void UpdateMainMenue(float dt);

	void drawBackground(); // ���������� ����
	void drawGameGui();    // ���������� ������, ������ � �������� � ����
	void drawMainMenue();  // ���������� ���� ������ �������

	// ��������� ��������� ����
	void addGameGUI();
	// ��������� ��������� �������� ����
	void addMainMenueGui();

	// ...����������
	void showGameGUI();
	void showMainMenueGui();

	// ... ��������
	void hideAllGui();
	void hideGameGUI();
	void hideMainMenueGui();

	void destroyGUIAll();

	// ��������� ������� ���������� ������
	void handleButtonUnpress();
	void handleButtonUnpressGameGui();
	void handleButtonUnpressMainMenue();

	// ... �������
	void handleButtonPress();
	void handleButtonPressGameGui();
	void handleButtonPressMainMenue();

	void soundOff();
	void soundOn();

	// ���� �� ����� �������� ��� �� �� ������� ������ (�� ���������������� ����������) ����� ��������� � GameField
	// ������ ��������� ������� �� ������ ������ (��������� ������� ��������, ���������� � ������������)
	void loadLevel(int number, bool retry = false);
	void loadLevel(std::string nameLevel);
	void loadNextLevel(); // ������ ��������� �������, ���� ������� ������������ ������ 24
	void reloadLevel();

	void setReTray();
	void setDone();

	bool notHaveAction();

	// ������������� ���������� ���������
	bool startingIsFinish();
	// .. ���� �����������
	bool finishingIsFinish();

	std::string generateLevelName(int number);
};

