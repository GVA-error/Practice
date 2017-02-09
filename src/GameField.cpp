#include "stdafx.h"
#include "GameField.h"


GameField::GameField(GameSettings::Ptr gameSettings, Render::Texture* texBox)
{
	FieldTune(gameSettings);
	_texBox = texBox;
	_gravityOn = true;
	dPosOnShowHide = FPoint(gameSettings->getScreenW(), 0);
	_solver = std::make_shared <Solver> (gameSettings);
	_levelGenerator = std::make_shared <LevelGenerator>(gameSettings);
	_questionBoxMenue = std::make_shared <QuestionBoxMenue>(gameSettings, texBox);
}

void GameField::FieldTune(GameSettings::Ptr gameSettings)
{
	if (!gameSettings)
		Assert(false);
	_settings = gameSettings;
	_logic = std::make_shared<LogicField>(_settings);
	_boxA = nullptr;
	_boxB = nullptr;

	int maxI = _settings->getMaxIField();
	int maxJ = _settings->getMaxJField();
	_boxPtrMx.reserve(maxJ);
	for (int j = 0; j< maxJ; j++) // update сначала для нижних (для более плавной гравитации)
	{
		_boxPtrMx.push_back(std::vector<Box::Ptr>(maxI));
			for (auto& box : _boxPtrMx[j])
				box = nullptr;
	}
	float wCell = _logic->getWCell();
	float hCell = _logic->getHCell();
	absMaxDPos.x = wCell * kForSwap;
	absMaxDPos.y = hCell * kForSwap;
	_boxesCount = 0;
	_gameTree = nullptr;
}

GameField::~GameField()
{
	if (_gameTree)
	{
		GameTree::freeTree(_gameTree);
		_gameTree = nullptr;
	}
}

void GameField::explosion(const IPoint& pos)
{
	for (int dj = -1; dj <= 1; dj++)
		for (int di = -1; di <= 1; di++)
			if (di*dj == 0)
			{
				IPoint ePos = pos + IPoint(dj, di);
				if (_logic->posIsCorrect(ePos))
					{
						Box::Ptr box = _boxPtrMx[ePos.x][ePos.y];
						if (box)
							if (box->isLockIt())
							{
								box->unLockIt();
								showUnlockEffInPos(ePos);
							}
							else if (box->isBig())
								continue;
							else 
							{
								destroyBox(ePos);
								if (box->isBond() && ePos != pos)
									explosion(ePos);
							}
					}
			}
	FPoint expPos = _logic->getCentrCell(pos);
	ParticleEffect* effExplosion = _effTemp.AddEffect(s_EffExplosion).get();
	effExplosion->SetPos(expPos);
	effExplosion->Reset();
}

int GameField::getMaxNumMove()
{
	return _maxNumMove;
}

int GameField::getCurNumMove()
{
	return _moveStack.size();
}

bool GameField::destroyBox(const FPoint& boxPos, bool withAxe)
{
	IPoint p = _logic->getPosInMxField(boxPos);
	return destroyBox(p, withAxe);
}

bool GameField::destroyBox(const IPoint& p, bool withAxe)
{
	bool f_destroyed = false;
	if (!_logic->posIsCorrect(p))
		return false;
	Box::Ptr box = _boxPtrMx[p.x][p.y];
	if (box && box->isBig() && withAxe)
		return false;
	if (box)
		if (box->isLive())
		{
			if (withAxe)
			{
				saveForBackMove();
				Swap axeSwap;
				axeSwap.A = axeSwap.B = p;
				axeSwap.changeType = boxType::Destoyed;
				_historySwaps.push_back(axeSwap);
			}
			if (box->isLockIt())
			{
				box->unLockIt();
				showUnlockEffInPos(p);
				return false;
			}
			f_destroyed = true;
			std::list <IPoint> bPoss = box->getAllPos();
			for (auto p : bPoss)
			{
				_boxPtrMx[p.x][p.y] = nullptr;
				showHideEffInPos(p);
			}
			box->finish();
			refreashBoxCount();
		}
	return f_destroyed;
}

void GameField::refreashBoxCount()
{
	destroyFinished(_boxPtrList);
	_boxesCount = _boxPtrList.size(); ///// будут баги с завершением уровня поверить _boxesCount

}

void GameField::generateEasyRandomLevel(std::string name)
{
	int moveount = 1;
	int rand = math::random(0, 1000);
	if (rand % 3 == 0)
		moveount++;
	if (rand % 5 == 0)
		moveount++;
	GamePosition level = _levelGenerator->generate(3, moveount, name);
	loadNewLevel(level, name);
}


void GameField::generateNormalRandomLevel(std::string name)
{
	int moveount = 2;
	int rand = math::random(0, 1000);
	if (rand % 3 == 0)
		moveount++;
	if (rand % 4 == 0)
		moveount++;

	GamePosition level = _levelGenerator->generate(5, 3, name);
	loadNewLevel(level, name); 
}
void GameField::generateHardRandomLevel(std::string name)
{
	int moveount = 4;
	int rand = math::random(0, 1000);
	if (rand % 4 == 0)
		moveount++;
	GamePosition level = _levelGenerator->generate(5, moveount, name);
	loadNewLevel(level, name);

}

void GameField::loadNewLevel(const std::string& nameLevel)
{
	GamePosition level = _fileReader.readLevel(nameLevel);
	loadNewLevel(level, nameLevel);
}

void GameField::loadNewLevel(const GamePosition& level, const std::string& nameLevel)
{
	_nameLevel = nameLevel;
	clear();
	_effTemp.KillAllEffects();
	clearBackStap();
	_historySwaps.clear();
	createLevel(level);
	loadLevelTree();
	_maxNumMove = GameTree::getDeep(_gameTree);
	refreashBoxCount();
}

bool GameField::haveMove()
{
	int curMove = getCurNumMove();
	int maxMove = getMaxNumMove();
	if (curMove < maxMove)
		return true;
	return false;
}

void GameField::loadLevelTree()
{

	GamePosition curPos = getGamePosition();
	
	curPos._numMove = 0;
	curPos._maxNumMove = 5; 

	if (_gameTree)
	{
		GameTree::freeTree(_gameTree);
		_gameTree = nullptr;
	}
	std::string gameTreeName = GameTree::getTreeName(_nameLevel);
	if (!File::ExistsInFs(gameTreeName))
	{
		_gameTree = _solver->getGameTree(curPos);
		GameTree::save(_gameTree, gameTreeName);
	}
	else
		_gameTree = GameTree::load(gameTreeName);
	
	_havePathWithOutAxe = GameTree::havePathWithOutAxe(_gameTree);
}

bool GameField::needUseAxe()
{
	return !_havePathWithOutAxe;
}

Swap GameField::getNextSwap()
{
	GameTree::Ptr curSone = GameTree::getSoneOnHistory(_historySwaps, _gameTree);

	Swap nextSwap = Swap::gerErrorSwap();
	if (curSone)
		if (!curSone->sons.empty())
			nextSwap = curSone->sons.front()->lastSwap;
	return nextSwap;
}

FPoint GameField::getRealPos(const IPoint& fieldPos)
{
	FPoint realPos = _logic->getCentrCell(fieldPos);
	return realPos;
}

void GameField::show()
{
	_show = true;
	for (auto box : _boxPtrList)
	{
		FPoint curPos = box->getPos();
		FPoint dPos = box->getDPos();
		box->show(curPos + dPosOnShowHide);
	}
}

void GameField::hide()
{
	_show = false;
	for (auto box : _boxPtrList)
	{
		FPoint dPos = box->getDPos();
		FPoint curPos = box->getPos();
		box->hide(curPos - dPosOnShowHide);
	}
}

bool GameField::isHide()
{
	for (auto box : _boxPtrList)
		if (box->isShow())
			return false;
	return !_show;
}

bool GameField::isShow()
{
	return !isHide();
}

void GameField::backMove()
{
	if (_moveStack.empty())
		return;
	if (_historySwaps.empty())
		assert(false); // не правильно щитаеться история или сохраняются ходы назад 
	GamePosition lastSavePos = _moveStack.top();
	createLevel(lastSavePos);
	_moveStack.pop();
	_historySwaps.pop_back();
	show();
}

void GameField::saveForBackMove()
{
	destroyFinished(_boxPtrList);
	GamePosition curGamePos = getGamePosition();
	_moveStack.push(curGamePos);
}

GamePosition GameField::getGamePosition()
{
	GamePosition curGamePos;
	curGamePos.boxList.reserve(_boxPtrList.size());
	for (auto box : _boxPtrList)
		if (box->isLive())
		{
			Box::boxInfo boxInfo = box->getInfo();
			curGamePos.boxList.push_back(boxInfo);
		}
	return curGamePos;
}

bool GameField::levelIsDone()
{
	bool f_levelIsDone = !isBusy() && !isGaming();
	return f_levelIsDone;
}

bool GameField::isGaming()
{
	return _boxesCount > 0;
}

void GameField::doMatch()
{
	if (isBusy())
		return;

	GamePosition curPos = getGamePosition();
	MxField::Match curMatch = _solver->handleMatch(curPos);

	for (auto i_curPos = curMatch.begin(); i_curPos != curMatch.end(); i_curPos++)
	{
		IPoint curPos = *i_curPos;
		if (std::find(curMatch.begin(), i_curPos, curPos) == i_curPos) // чтобы дважды не обрабатывать матчи
		{
			Box::Ptr box = _boxPtrMx[curPos.x][curPos.y];
			if (!box)
				continue;
			int type = box->getType();
			if (BoxType::typeIsBomb(type))
				explosion(curPos);
			else if (box->isLockIt())
			{
				showUnlockEffInPos(curPos);
				box->unLockIt();
			}
			else
				destroyBox(curPos);
		}
	}
}

void GameField::showHideEffInPos(const IPoint& p)
{
	ParticleEffect* hideEff = _effTemp.AddEffect(s_effBoxHide).get();
	FPoint effPos = _logic->getCentrCell(p);
	hideEff->SetPos(effPos);
	hideEff->Reset();
}

void GameField::showUnlockEffInPos(const IPoint& p)
{
	ParticleEffect* hideEff = _effTemp.AddEffect(s_EffUnLock).get();
	FPoint effPos = _logic->getCentrCell(p);
	hideEff->SetScale(0.7f);
	hideEff->SetPos(effPos);
	hideEff->Reset();
}

bool GameField::isFirstMove()
{
	return _moveStack.empty();
}

bool GameField::isBusy()
{
	if (!_effTemp.IsFinished())
		return true;
	for (auto box : _boxPtrList)
		if (box->isBusy())
			return true;
	return false;
}

void GameField::SetGravityOn()
{
	_gravityOn = true;
}

void GameField::SetGravityOff()
{
	_gravityOn = false;
}

bool GameField::GravityIsOn() const
{
	return _gravityOn;
}

bool GameField::GravityIsOff() const
{
	return !_gravityOn;
}

void GameField::update(float dt)
{
	_effTemp.Update(dt);
	if (_effTemp.IsFinished())
		_gravityOn = true;
	else
		_gravityOn = false;
	updateBoxSellectMenue(dt);
	updateBoxs(dt);
	updateSwapBox(dt);
}

void GameField::updateBoxSellectMenue(float dt)
{
	// флаг момента исчезновения // можно оптимизиовать, чеез включение флага в QuestionBoxMenue
	bool f_showingBeforUpdate = _questionBoxMenue->isShow();
	if (!f_showingBeforUpdate)
		return;
	bool f_someSellect = _questionBoxMenue->boxTypeIsSellected();
	if (f_someSellect)
		if (f_showingBeforUpdate)
			_questionBoxMenue->hide();
	_questionBoxMenue->update(dt);
	
	if (!f_someSellect)
		return;
	bool f_showingAfterUpdate = _questionBoxMenue->isShow();
	// обработка исчезновения
	if (f_showingBeforUpdate != f_showingAfterUpdate)
	{
		boxType sellectedType = (boxType)_questionBoxMenue->getLastSellectedBoxType();
		FPoint qBoxPos = _questionBoxMenue->getQBoxPos();
		IPoint qBoxPosInField = _logic->getPosInMxField(qBoxPos);
		setTypeBox(qBoxPosInField, sellectedType);
	}
}

void GameField::setTypeBox(const IPoint& boxPos, boxType type)
{
	saveForBackMove();
	Swap sw;
	sw.changeType = type;
	sw.A = sw.B = boxPos;
	_historySwaps.push_back(sw);
	_boxPtrMx[boxPos.x][boxPos.y]->setType(type);
	doMatch();
}

void GameField::updateBoxs(float dt)
{
	for (auto& column : _boxPtrMx) // update сначала для нижних (для более плавной гравитации)
		for (int i = 0; i < column.size(); i++)
		{
			Box::Ptr box = column[i];
			if (box)
			{
				bool fOldBeasy = box->isBusy();
				IPoint oldFieldPos = box->getFieldPos();
				box->update(dt, _gravityOn);
				if (box->isHiding() || box->isShowing())
					continue;
				bool fNewBeasy = box->isBusy();
				IPoint newFieldPos = box->getFieldPos();
				// Смена позиций ящиков
				if (oldFieldPos != newFieldPos)
					changePosInMx(oldFieldPos, newFieldPos, box);
				// при смене занятости проверяем матчи
				if (fOldBeasy == true && fNewBeasy == false)
					doMatch();
			}
		}
}

void GameField::changePosInMx(IPoint oldPos, IPoint newPos, Box::Ptr box)
{
	// новая позиция поля должна быть свободна
	if (!box->isDefaultState())
		return;
	Box::Ptr boxOnNewPos = _boxPtrMx[newPos.x][newPos.y];
	if (boxOnNewPos != nullptr)
		Assert(boxOnNewPos->isFinished()); // видимо пытаемься установить 2 ящика на одну позиию, возможно из-за не верной генерации ящика 2х2

	for (int dx = 0; dx <= 1; dx++)
		for (int dy = 0; dy <= 1; dy++)
		{
			_boxPtrMx[oldPos.x + dx][oldPos.y + dy] = nullptr;
			if (!box->isBig())
				dx = dy = 7; // выход из цикла
		}

	if (box->isLive())
		for (int dx = 0; dx<=1; dx++)
			for (int dy = 0; dy <= 1; dy++)
			{
				_boxPtrMx[newPos.x+dx][newPos.y+dy] = box;
				if (!box->isBig())
					return;
			}

}

void GameField::makeSwap(const IPoint& posA, const IPoint& posB)
{
	if (!_logic->posIsCorrect(posA) || !_logic->posIsCorrect(posB))
		return;
	_boxA = _boxPtrMx[posA.x][posA.y];
	_boxB = _boxPtrMx[posB.x][posB.y];
	if (_boxA && !_boxA->isBig())
	{
		_boxA->setFieldPos(posB);
	}
	if (_boxB && !_boxB->isBig())
	{
		_boxB->setFieldPos(posA);
	}
	_boxPtrMx[posA.x][posA.y] = _boxB;
	_boxPtrMx[posB.x][posB.y] = _boxA;
	gameMoveNew();
	refreashSlowMove();
	Swap sw;
	sw.A = posA;
	sw.B = posB;
	_historySwaps.push_back(sw);
}

void  GameField::gameMoveNew()
{
	doMatch();
}

void GameField::refreashSlowMove()
{
	OnSlowMoveForAll();
}

void GameField::updateSwapBox(float dt)
{
	if (!_boxA)
		return;
	if (_boxA->isBig())
		assert(false); // мы их делали не кликабельными
	FPoint dPos = _boxA->getDPosOnMousePos();
	bool f_timeToSwap = false;
	if (abs(dPos.x) > absMaxDPos.x)
	{
		dPos.x = absMaxDPos.x * math::sign(dPos.x);
		f_timeToSwap = true;
	}

	if (abs(dPos.y) > absMaxDPos.y)
	{
		dPos.y = absMaxDPos.y * math::sign(dPos.y);
		f_timeToSwap = true;
	}

	if (!_boxA->dPosIsCorrect(dPos))
		dPos = FPoint(0.0f, 0.0f);

	_boxA->setDPos(dPos);
	if (_boxB) // пoка обнуляем смещение 
		_boxB->setDPos(FPoint(0.0f, 0.0f));
	IPoint FieldPosB = _boxA->tranformDPosOnFieldPos();
	IPoint FieldPosA = _boxA->getFieldPos();
	if (_logic->posIsCorrect(FieldPosB))
		_boxB = _boxPtrMx[FieldPosB.x][FieldPosB.y];
	else
		_boxB = nullptr;

	if (!_boxB && dPos.y > 0 || _boxB && _boxB->isBig() || _boxB && _boxB->isLockIt()) // не можем смещаться вверх и к большим ящикам
	{
		dPos = FPoint(0.0f, 0.0f);
		_boxA->setDPos(dPos);
		f_timeToSwap = false;
		if (_boxB && _boxB->isBig())
			return;
	}
	if (FieldPosA == FieldPosB)
		_boxB = nullptr;
	if (_boxB)
		_boxB->setDPos(-dPos);
	if (f_timeToSwap) 
	{
		if (_logic->posIsCorrect(FieldPosA) && _logic->posIsCorrect(FieldPosB) && FieldPosA != FieldPosB)
		{
			saveForBackMove();
			makeSwap(FieldPosA, FieldPosB);
		}
		unPressLB();
	}
}

void GameField::draw()
{
	for (auto& box : _boxPtrList)
		box->draw();
	_effTemp.Draw();
	_questionBoxMenue->draw();
}

void GameField::createLevel(const GamePosition& position)
{
	clear();
	for (auto box : position.boxList)
		createBox(box);
}

void GameField::clear()
{
	for (auto& colunm : _boxPtrMx)
		for (auto& cell : colunm)
			cell = nullptr;
	_boxPtrList.clear();
	_logic->reset();
	_effTemp.KillAllEffects(); // кастыль
}

void GameField::clearBackStap()
{
	while (!_moveStack.empty())
		_moveStack.pop();
}

void GameField::createBox(const Box::boxInfo& box)
{
	IPoint pos = box.pos;
	boxType type = box.type;

	if (!_logic->posIsCorrect(pos))
		return;

	Render::Texture* _texLabel = nullptr;
	if (BoxType::typeIsBomb(type))
		_texLabel = Core::resourceManager.Get<Render::Texture>(s_Bomb);
	if (BoxType::typeIsLock(type))
		_texLabel = Core::resourceManager.Get<Render::Texture>(s_Lock);

	Box::Ptr newBox = std::make_shared<Box>(pos, _settings, _logic, _texBox, type, _texLabel);
	
	IPoint fieldPos = newBox->getFieldPos();

	changePosInMx(fieldPos, fieldPos, newBox);
	
	_boxPtrList.push_back(newBox);
}

void GameField::pressLB()
{
	// не реагируем пока на поле что то делается
	if (isBusy() || !haveMove())
		return;
	unPressLB();
	
	// не обрабатываем нажатия на ящики, пока выбираем ящик
	if (_questionBoxMenue->isShow())
	{
		_questionBoxMenue->press();
		return;
	}
	for (auto& box : _boxPtrList)
		if (box->isRealise())
		{
			if (box->isLockIt() || box->isBig())
				continue;
			_boxA = box;
			box->setPress();
		}
	OnSlowMoveForAll();
	_boxB = nullptr;
}


void GameField::OnSlowMoveForAll()
{
	for (auto box : _boxPtrList)
	{
		FPoint pathPoint = box->getPos() + box->getDPos();
		box->setSlowPointPath(pathPoint);
	}
}

void GameField::OffSlowMoveForAll()
{
	for (auto box : _boxPtrList)
		box->unSetSlowPointPath();
}

void GameField::unPressLB()
{
	if (_boxA && !_boxA->isBig())
	{
		_boxA->setDPos(FPoint(0, 0));
	}
	if (_boxB && !_boxB->isBig())
	{
		_boxB->setDPos(FPoint(0, 0));
	}
	_boxA = nullptr;
	_boxB = nullptr;
}

void GameField::pressRB()
{
	if (isBusy() || !haveMove())
		return;
	unPressRB();
	for (auto& box : _boxPtrList)
		if (box->isRealise())
			if (box->getType() == boxType::QuestionBox)
			{
				FPoint boxPos = box->getPos();
				_questionBoxMenue->unPress();
				_questionBoxMenue->show(boxPos);
			}
}

void GameField::unPressRB()
{

}

void GameField::fastShowSellect(const FPoint& pos, float timeBeforHide)
{
	_questionBoxMenue->show(pos, true, timeBeforHide);
}

FPoint GameField::getPosBoxTypeInSellect(boxType type)
{
	return _questionBoxMenue->getPosBoxType(type);
}