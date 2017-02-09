#include "stdafx.h"
#include "MoveTheBoxGUIWidget.h"

MoveTheBoxGUIWidget::MoveTheBoxGUIWidget(const std::string& name, rapidxml::xml_node<>* elem)
	: Widget(name)
	, _timer(0)
{
	Init();
}

void MoveTheBoxGUIWidget::Init()
{
	_effAxe = _effBoxHide = nullptr;
	_curGameStance = mainMenue;
	_curLevelNumber = 0;
	_curBackMove = _curHelpNumber = 0;
	_curMove = 0;

	fillInTextures();
	_gameSettings = std::make_shared<GameSettings>();
	_gameField = std::make_shared<GameField>(_gameSettings, _texBox);

	_backgroundPos = FPoint(0, 0); 
	IRect backRect = _texBackGround->getBitmapRect();

	_gameMess = ""; 
	_normalRandomLVL = _gameSettings->getMaxLevel();

	addGameGUI();

	addMainMenueGui();

	showMainMenueGui();
}

void MoveTheBoxGUIWidget::fillInTextures()
{
	_texBackGround = Core::resourceManager.Get<Render::Texture>(s_Background);
	_texBox = Core::resourceManager.Get<Render::Texture>(s_Boxes);
	_texLevelList = Core::resourceManager.Get<Render::Texture>(s_LevelList);

	_texBtn[btn_sound] = Core::resourceManager.Get<Render::Texture>(s_SoundButton);
	_texBtn[btnSmall_mainMenue] = Core::resourceManager.Get<Render::Texture>(s_MainMenueButton);
	_texBtn[btn_backMove] = Core::resourceManager.Get<Render::Texture>(s_BackMoveButton);
	_texBtn[btn_done] = Core::resourceManager.Get<Render::Texture>(s_DoneButton);
	_texBtn[btn_help] = Core::resourceManager.Get<Render::Texture>(s_HelpButton);
	_texBtn[btn_retry] = Core::resourceManager.Get<Render::Texture>(s_retryButton);
}

// В этой функции много констант, можно збавиться узнав размеры текстур эффектов
void MoveTheBoxGUIWidget::showSwap(const Swap& sw)
{
	FPoint APos;
	bool swapUp = false;
	if (sw.A.x == sw.B.x)
		swapUp = true;
	if (sw.A.x <= sw.B.x && sw.A.y <= sw.B.y)
		APos = _gameField->getRealPos(sw.A);
	else
		APos = _gameField->getRealPos(sw.B);

	FPoint drawPos = APos;
	ParticleEffect* effSwap;
	if (sw.changeType >= 0)
		effSwap = _effCont.AddEffect(s_EffSwapType).get();
	else if (swapUp)
		effSwap = _effCont.AddEffect(s_EffSwapY).get();
	else
		effSwap = _effCont.AddEffect(s_EffSwapX).get();
	// в случаи ошибки указываем на верх
	if (sw == Swap::gerErrorSwap())
	{
		drawPos.x = _posBackMove.x + 15;
		drawPos.y = _posBackMove.y - _effSwapSize; 
	}
	else if (sw.changeType >= 0)
	{
		FPoint drawPosTypeSellect;
		drawPos.x += _effSwapSize / 8; 
		if (sw.changeType != boxType::Destoyed)
		{
			_gameField->fastShowSellect(APos, _timeFastShowSellectBox);
			drawPosTypeSellect = _gameField->getPosBoxTypeInSellect((boxType)sw.changeType);
		}
		else
		{
			drawPosTypeSellect = _axeRect.CenterPoint();
			std::swap(drawPosTypeSellect, drawPos);
		}
		drawPosTypeSellect.x += 4;
		ParticleEffect* effSellectTime;
		effSellectTime = _effCont.AddEffect(s_EffSwapTypeOffsetTime).get();
		effSellectTime->SetPos(drawPosTypeSellect);
		effSwap->SetScale(0.7);
		effSellectTime->Reset();
	}
	else if (!swapUp)
		drawPos.x += _effSwapSize / 2; 
	effSwap->SetScale(0.7);
	effSwap->SetPos(drawPos);
	effSwap->Reset();
}

bool MoveTheBoxGUIWidget::haveBackMove()
{
	return _curBackMove < _gameSettings->getBackStapCount();
}

bool MoveTheBoxGUIWidget::haveHelp()
{
	if (_alwaysHelp)
		return true;
	return _curHelpNumber < _gameSettings->getMaxHelpCount();
}

void MoveTheBoxGUIWidget::handleButtonPressMainMenue()
{
	_levelList->press();
	_curLevelNumber = _levelList->selectedLevel();
	if (_curLevelNumber > 0)
	{
		_curGameStance = starting;
		hideMainMenueGui();
	}
}

void MoveTheBoxGUIWidget::loadLevel(int number, bool retry)
{
	std::string levelName;
	levelName = generateLevelName(number);
	hideMainMenueGui();

	if (number == _normalRandomLVL && !retry)
		_gameField->generateNormalRandomLevel(levelName);
	else
		loadLevel(levelName);

	_gameField->show();
	showGameGUI();
}

std::string MoveTheBoxGUIWidget::generateLevelName(int number)
{
	std::string nameLevel;
	std::string s_levelNumber = utils::lexical_cast <int> (number);
	std::string prefix = _fileReader.getPrefixLevelName();
	std::string ext = _fileReader.getFileExtXMLGameLevel();
	nameLevel = prefix + s_levelNumber + ext;
	return nameLevel;
}

void MoveTheBoxGUIWidget::loadLevel(std::string nameLevel)
{
	_curBackMove = 0;
	_curHelpNumber = 0;
	_gameField->loadNewLevel(nameLevel);
}

void MoveTheBoxGUIWidget::handleButtonUnpressMainMenue()
{

}


void MoveTheBoxGUIWidget::destroyGUIAll() 
{

}

void MoveTheBoxGUIWidget::addGameGUI()
{
	float btnW = _btnSmall.Width();
	float btnH = _btnSmall.Height();
	float screenW = _gameSettings->getScreenW();
	float screenH = _gameSettings->getScreenH();
	// смещение к центру
	FPoint dCentrSmall = FPoint(_btnSmall.xEnd / 2, _btnSmall.yEnd / 2);
	FPoint dCentrNormal = FPoint(0, _btnNormal.yEnd / 2);
	
	DinamicButtonMoveFromOut::Ptr soundButton;
	float soundBtnX = screenW - _btnDistance - btnW;
	float soundBtnY = screenH - _btnDistance - btnH;
	FPoint posSoundButton(soundBtnX, soundBtnY);
	posSoundButton += dCentrSmall;
	soundButton = std::make_shared <DinamicButtonMoveFromOut>(posSoundButton, _gameSettings, _texBtn[btn_sound], s_SoundButton);
	soundButton->setCheckable();
	soundButton->setDrawRect(_btnSmall);
	soundButton->setMaxSpeed(_maxSpeedButton);
	_buttonListGameGui.push_back(soundButton);

	DinamicButtonMoveFromOut::Ptr backMove;
	float backMoveBtnX = _btnDistance;
	float backMoveBtnY = screenH - 2*_btnDistance - 2*btnH;
	_posBackMove = FPoint(backMoveBtnX, backMoveBtnY);
	_posBackMove += dCentrSmall;
	backMove = std::make_shared <DinamicButtonMoveFromOut>(_posBackMove, _gameSettings, _texBtn[btn_backMove], s_BackMoveButton);
	backMove->setUnCheckable();
	backMove->setDrawRect(_btnSmall);
	backMove->setMaxSpeed(_maxSpeedButton);
	_buttonListGameGui.push_back(backMove);

	DinamicButtonMoveFromOut::Ptr mainMenue;
	float mainMenueBtnX = _btnDistance;
	float mainMenueBtnY = screenH - _btnDistance - btnH;
	FPoint posMainMenue(mainMenueBtnX, mainMenueBtnY);
	posMainMenue += dCentrSmall;
	mainMenue = std::make_shared <DinamicButtonMoveFromOut>(posMainMenue, _gameSettings, _texBtn[btnSmall_mainMenue], s_MainMenueButton);
	mainMenue->setUnCheckable();
	mainMenue->setDrawRect(_btnSmall);
	mainMenue->setMaxSpeed(_maxSpeedButton);
	_buttonListGameGui.push_back(mainMenue);

	DinamicButtonMoveFromOut::Ptr helpButton;
	float helpBtnX = screenW - _btnDistance - btnW;
	float helpBtnY = screenH - 2 * _btnDistance - 2 * btnH;
	FPoint posHelpButton(helpBtnX, helpBtnY);
	posHelpButton += dCentrSmall;
	helpButton = std::make_shared <DinamicButtonMoveFromOut>(posHelpButton, _gameSettings, _texBtn[btn_help], s_HelpButton);
	helpButton->setUnCheckable();
	helpButton->setDrawRect(_btnSmall);
	helpButton->setMaxSpeed(_maxSpeedButton);
	_buttonListGameGui.push_back(helpButton);

	FPoint posDone = _gameSettings->getCentrField();
	posDone += dCentrNormal;
	_btnDone = std::make_shared <SizableButton>(posDone, _gameSettings, _texBtn[btn_done], s_DoneButton);
	_btnDone->setCheckable();
	_btnDone->setDrawRect(_btnNormal);

	FPoint posMess = _gameSettings->getCentrField();
	posMess -= FPoint(0, screenH / 2 - 50);
	_messagePanel = std::make_shared <LabelView>(_gameMess, posMess);
}

void MoveTheBoxGUIWidget::addMainMenueGui()
{
	_levelList = std::make_shared <LevelList> (_gameSettings, _texLevelList);
}

void MoveTheBoxGUIWidget::showGameGUI()
{
	setDafaustGameMessage();
	for (auto button : _buttonListGameGui)
	{
		std::string btnName = button->getName();
		FPoint posButton = button->getPos();
		FPoint drawPosSound;
		if (btnName == s_SoundButton || btnName == s_HelpButton || btnName == s_BackMoveButton)
			drawPosSound = posButton + 2 * FPoint(_btnSmall.Width(), 0);
		if (btnName == s_MainMenueButton)
			drawPosSound = posButton - 2 * FPoint(_btnSmall.Width(), 0);
		if (btnName == s_BackMoveButton)
			button->hide(drawPosSound);
		else
			button->show(drawPosSound);
	}
	_btnDone->hide();

	FPoint curPos = _messagePanel->getPos();
	FPoint showPos = curPos - FPoint(0, 100);
	_messagePanel->show(showPos);

	if (_effAxe)
	{
		_effAxe->Kill();
		_effAxe = nullptr;
	}

	if (_gameField->needUseAxe())
	{
		_effAxe = _effCont.AddEffect(s_EffAxe).get();
		FPoint axePos = _axeRect.CenterPoint();
		_effAxe->SetPos(axePos);
		_effAxe->SetScale(0.75);
		_effAxe->Reset();
		_usedAxe = false;
	}
}

void MoveTheBoxGUIWidget::showMainMenueGui()
{
	_levelList->show();
}

void MoveTheBoxGUIWidget::hideAllGui()
{
	hideGameGUI();
	hideMainMenueGui();
}

// задать геттер позиции появления - уничтожения
void MoveTheBoxGUIWidget::hideGameGUI()
{
	_gameField->hide();
	for (auto button : _buttonListGameGui)
	{
		std::string btnName = button->getName();
		FPoint posButton = button->getPos();
		FPoint drawPosSound;
		if (btnName == s_SoundButton || btnName == s_HelpButton)
			drawPosSound = posButton + 2 * FPoint(_btnSmall.Width(), 0);
		if (btnName == s_BackMoveButton || btnName == s_MainMenueButton)
			drawPosSound = posButton - 2 * FPoint(_btnSmall.Width(), 0);
		button->hide(drawPosSound);
	}

	FPoint curPos = _messagePanel->getPos();
	FPoint hidePos = curPos - FPoint(0, 100);
	_messagePanel->hide(hidePos);
	_btnDone->hide();
	if (_effAxe)
		_effAxe->Kill();
}

void MoveTheBoxGUIWidget::hideMainMenueGui()
{
	_levelList->hide();
}

void MoveTheBoxGUIWidget::LoadTestLvl()
{
	GamePosition testLevel;

	Box::boxInfo testBox1, testBox2, testBox3, testBox4, testBox5, testBox6, testBox7, testBox8, testBox9;

	testBox1.pos = IPoint(0, 0);
	testBox1.type = boxType::RedBox;
	testBox2.pos = IPoint(1, 1);
	testBox2.type = boxType::RedBox;
	testBox3.pos = IPoint(1, 2);
	testBox3.type = boxType::RedBox;
	testBox4.pos = IPoint(3, 1);
	testBox4.type = boxType::BlueBox;
	testBox5.pos = IPoint(0, 1);
	testBox5.type = boxType::BlueBox;
	testBox6.pos = IPoint(1, 2);
	testBox6.type = boxType::BlueBox;
	testBox7.pos = IPoint(2, 3);
	testBox7.type = boxType::YellowBox;
/*	testBox8.pos = IPoint(3, 4);
	testBox8.type = boxType::RedBox;
	testBox9.pos = IPoint(3, 3);
	testBox9.type = boxType::BlueBox;
	*/
	testLevel.boxList.push_back(testBox1);
	testLevel.boxList.push_back(testBox2);
	testLevel.boxList.push_back(testBox3);
	testLevel.boxList.push_back(testBox4);
	testLevel.boxList.push_back(testBox5);
	testLevel.boxList.push_back(testBox6);
	testLevel.boxList.push_back(testBox7);
/*	testLevel.boxList.push_back(testBox8);
	testLevel.boxList.push_back(testBox9);
	*/

	std::string levelName = generateLevelName(3);

	_fileReader.writeLevel(testLevel, levelName);

	
}

void MoveTheBoxGUIWidget::Draw()
{
	drawBackground();


	_gameField->draw();

	drawGameGui();
	drawMainMenue();

	_effCont.Draw();

	IPoint mousePos = Core::mainInput.GetMousePos();


	if (_gameSettings->showCursorPos)
	{
		Render::BindFont(s_programmMessageFont);
		float _screenW = _gameSettings->getScreenW();
		Render::PrintString(IPoint(_screenW - 100, 35), utils::lexical_cast(mousePos.x) + ", " + utils::lexical_cast(mousePos.y), 1.f, CenterAlign);
	}
}

void MoveTheBoxGUIWidget::showLoadMessage()
{
	Render::BindFont(s_stdFont);
	FPoint centrPos = _gameSettings->getCentrField();
	if (_curLevelNumber == _normalRandomLVL)
		Render::PrintString(centrPos, s_generatingField, 1.f, CenterAlign);
	else if (_curLevelNumber >= 0)
		Render::PrintString(centrPos, s_loadingField, 1.f, CenterAlign);
}

void MoveTheBoxGUIWidget::drawBackground()
{
	try
	{
		Render::device.PushMatrix();
		Render::device.MatrixScale(_backgroundScale);
		_texBackGround->Draw(_backgroundPos);
		Render::device.PopMatrix();
	}
	catch (void* e)
	{
		assert("some bade");
	}
}

void MoveTheBoxGUIWidget::drawGameGui()
{
	for (auto btn : _buttonListGameGui)
		btn->draw();

	try
	{

		if (_btnDone->isHiding() && _btnDone->isShow())
			showLoadMessage();
		else
			_messagePanel->draw();

	}
	catch (void* e)
	{
		assert("some bade");
	}

	_btnDone->draw();
}

void MoveTheBoxGUIWidget::drawMainMenue()
{
	_levelList->draw();
}

void MoveTheBoxGUIWidget::Update(float dt)
{
	_effCont.Update(dt);

	if (_curGameStance == gaming || _curGameStance == finishing)
		UpdateGameGui(dt);
	if (_curGameStance == mainMenue || _curGameStance == starting)
		UpdateMainMenue(dt);

	_timer += dt * 2;

	while (_timer > 2 * math::PI)
	{
		_timer -= 2 * math::PI;
	}

}

void MoveTheBoxGUIWidget::UpdateGameGui(float dt)
{
	_gameField->update(dt);
	int newNumMove = _gameField->getCurNumMove();
	if (_curMove != newNumMove)
	{
		_curMove = newNumMove;
		setDafaustGameMessage();	// далее сообщение может поменяться
	}

	_messagePanel->update(dt);

	// уровень перезапускаеться если потрачены все попытки и ходы назад
	if ((_gameField->levelIsDone() || notHaveAction()) && _btnDone->isHide() && !_gameField->isBusy())
	{
		if (_gameField->levelIsDone())
			setDone();
		else
			setReTray();
		hideGameGUI();
		_btnDone->show();
	}
	
	for (auto button : _buttonListGameGui)
	{
		if (button->isDefaultState() && _curGameStance == gaming && _btnDone->isHide())
		{
			std::string btnName = button->getName();
			FPoint posButton = button->getPos();
			FPoint drawPosSound;
			if (btnName == s_HelpButton)
				drawPosSound = posButton + FPoint(_btnSmall.Width(), 0);
			if (btnName == s_BackMoveButton)
				drawPosSound = posButton - FPoint(_btnSmall.Width(), 0);

			if (btnName == s_BackMoveButton)
				if (_gameField->isFirstMove())
				{
					if (button->isShow())
						button->hide();
				}
				else if (button->isHide() && haveBackMove())
					button->show(drawPosSound);
			
			if (btnName == s_HelpButton)
			{
				if (haveHelp() && button->isHide())
					button->show(drawPosSound);
			}
		}
		button->update(dt);
	}

	_btnDone->update(dt);

	if (_btnDone->isPress() && !_btnDone->isShow())
	{
		if (_gameField->levelIsDone())
			loadNextLevel();
		else 
			reloadLevel();
		_btnDone->setUnPress();
	}

	if (_curGameStance == finishing)
		if (finishingIsFinish())
		{
			showMainMenueGui();
			_curGameStance = mainMenue;
		}

	_messagePanel->setText(_gameMess);

}

void MoveTheBoxGUIWidget::reloadLevel()
{
	loadLevel(_curLevelNumber, true);
}

bool MoveTheBoxGUIWidget::notHaveAction()
{
	return !_gameField->haveMove() && _curBackMove >= _gameSettings->getMaxHelpCount();
}

void MoveTheBoxGUIWidget::setReTray()
{
	Render::Texture* texRetry = _texBtn[btn_retry];
	_btnDone->setTex(texRetry);
}

void MoveTheBoxGUIWidget::setDone()
{
	Render::Texture* texRetry = _texBtn[btn_done];
	_btnDone->setTex(texRetry);
}

void MoveTheBoxGUIWidget::loadNextLevel()
{
	_curLevelNumber++;
	int aLvl = _gameSettings->getAvailableLvl();
	int maxLvl = _gameSettings->getMaxLevel();
	if (_curLevelNumber > maxLvl) 
		_curLevelNumber=24; 
	if (_curLevelNumber > aLvl)
		_gameSettings->incAvailableLvl();
	loadLevel(_curLevelNumber);

}

void MoveTheBoxGUIWidget::UpdateMainMenue(float dt)
{
	_levelList->update(dt);

	if (_curGameStance == starting)
		if (startingIsFinish())
		{
			loadLevel(_curLevelNumber, true);
			_curGameStance = gaming;
		}

}

bool MoveTheBoxGUIWidget::startingIsFinish()
{
	bool f_startimgIsFinish = _levelList->isHide();
	return f_startimgIsFinish;
}

bool MoveTheBoxGUIWidget::finishingIsFinish()
{
	if (_gameField->isShow())
		return false;
	if (_btnDone->isShow())
		return false;
	for (auto button : _buttonListGameGui)
		if (button->isShow())
			return false;
	return true;
}

bool MoveTheBoxGUIWidget::MouseDown(const IPoint &mouse_pos)
{
	if (_gameField->isBusy()) // пользователь должен дождаться конца происходящего на поле
		return false;

	handleButtonPress();

	if (Core::mainInput.GetMouseLeftButton() && _effAxe)
		if (_axeRect.Contains(mouse_pos) && _gameField->haveMove())
			_usedAxe = true;
		else if (_usedAxe)
			if (_gameField->destroyBox((FPoint)mouse_pos, true))
			{
				_effAxe->SetPos(_axeRect.CenterPoint());
				//_effAxe = nullptr; если закоментить некоторые уровни придуться пересчитывать (т.е. в которых топор используеться 2 и более раз)
				ParticleEffect* effAxeKick = _effCont.AddEffect(s_EffAxeKick).get();
				effAxeKick->SetPos(mouse_pos);
				effAxeKick->Reset();
				_usedAxe = false;
			}
	
	if (Core::mainInput.GetMouseRightButton() && _effAxe)
	{
		_effAxe->SetPos(_axeRect.CenterPoint());
		_usedAxe = false;
	}

	return false;
}

void MoveTheBoxGUIWidget::handleButtonPressGameGui()
{

	if (Core::mainInput.GetMouseLeftButton())
		_gameField->pressLB();
	else if (Core::mainInput.GetMouseRightButton())
		_gameField->pressRB();

	for (auto button : _buttonListGameGui)
	{
		if (!button->isRealise())
			continue;
		std::string btnName = button->getName();
		bool fPressed = button->isPress();
		if (btnName == s_SoundButton)
		{
			button->setPress();
			if (fPressed)
				soundOff();
			else
				soundOn();
		}
		else
			button->setPress();
	}
	if (_btnDone->isRealise())
	{
		_btnDone->setPress();
		_btnDone->hide();
	}
}

void MoveTheBoxGUIWidget::handleButtonUnpressGameGui()
{
	for (auto button : _buttonListGameGui)
		if (button->isShow())
		{
			std::string btnName = button->getName();
			bool fPressed = button->isPress();
			if (fPressed && button->isDefaultState()) // исчезающие или появляющиеся кнопки в игре не кликабельны
				if (btnName == s_BackMoveButton)
				{
					if (_gameField->isGaming())
						_gameField->backMove();
					button->setUnPress();
					_curBackMove++;
					if (!haveBackMove())
						button->hide();
					_gameMess = "Ok. Try again";
				}
				else if (btnName == s_MainMenueButton)
				{
					hideGameGUI();
					_curGameStance = finishing;
				}
				else if (btnName == s_HelpButton)
				{
					Swap helpSwap = _gameField->getNextSwap();
					showSwap(helpSwap);
					_curHelpNumber++;
					if (!haveHelp())
						button->hide();
					_gameMess = "Watch attentively...";
				}
		}

}

void MoveTheBoxGUIWidget::handleButtonPress()
{
	if (_curGameStance == gaming)
		handleButtonPressGameGui();
	if (_curGameStance == mainMenue)
		handleButtonPressMainMenue();
}


void MoveTheBoxGUIWidget::handleButtonUnpress()
{

	if (_curGameStance == gaming)
		handleButtonUnpressGameGui();
	if (_curGameStance == mainMenue)
		handleButtonUnpressMainMenue();
}

void MoveTheBoxGUIWidget::setDafaustGameMessage()
{
	if (_gameField->haveMove())
	{
		int curMove = _gameField->getCurNumMove();
		int maxMove = _gameField->getMaxNumMove();
		_gameMess = "move " + utils::lexical_cast<int>(curMove)+"/" + utils::lexical_cast<int>(maxMove);
	}
	else
		if (_gameField->levelIsDone())
			_gameMess = "Exelent";
		else
			_gameMess = "It was last move";
}

void MoveTheBoxGUIWidget::soundOff()
{

}

void MoveTheBoxGUIWidget::soundOn()
{

}

void MoveTheBoxGUIWidget::MouseMove(const IPoint &mouse_pos)
{
	if (_usedAxe && _effAxe)
		_effAxe->SetPos(mouse_pos);
}

void MoveTheBoxGUIWidget::MouseUp(const IPoint &mouse_pos)
{
	_gameField->unPressLB();
	handleButtonUnpress();
}

void MoveTheBoxGUIWidget::AcceptMessage(const Message& message)
{

}
