#include "stdafx.h"
#include "LevelList.h"

LevelList::LevelList(GameSettings::Ptr settings, Render::Texture* _tex, int availableLvl)
{
	_maxILevel = settings->getMaxILevel();
	_maxJLevel = settings->getMaxJLevel();
	_maxLevel = settings->getMaxLevel(); 
	_settings = settings;
	refreashAvailableLvl();
	FPoint levlListPos = settings->getLevelListPos();
	generateLevelList(levlListPos, settings, _tex);
}

void LevelList::refreashAvailableLvl()
{
	_availableLvl = _settings->getAvailableLvl();
}

void LevelList::press()
{
	for (auto button : _levelList)
		if (button->isRealise())
			button->setPress();
}

int LevelList::selectedLevel()
{
	int sellected = _firstLevelNum;
	for (auto button : _levelList)
		if (button->isPress())
			if (sellected > _availableLvl)
				return -1;
			else
				return sellected;
		else
			sellected++;
	return -1;
}

void LevelList::update(float dt)
{
	for (auto levelButton : _levelList)
		levelButton->update(dt);
}

void LevelList::draw()
{
	int j = 1;
	for (auto levelButton : _levelList)
	{
		if (j > _availableLvl)
			levelButton->draw(1);
		else
			levelButton->draw(j+1);
		j++;
	}
}

LevelList::~LevelList()
{

}


bool LevelList::isHiding() const
{
	if (_levelList.empty())
		assert(false);
	return _levelList.front()->isHiding();
}

void LevelList::show()
{
	refreashAvailableLvl();
	for (auto button : _levelList)
		button->show();
}

void LevelList::hide()
{
	for (auto button : _levelList)
		button->hide();
}

bool LevelList::isHide() const
{
	for (auto button : _levelList)
		if (button->isHide())
			return true;
	return false;
}

bool LevelList::isShow() const
{
	return !isHide();
}

void LevelList::generateLevelList(const FPoint& levlListPos, GameSettings::Ptr settings, Render::Texture* _tex)
{
	FRect listLevelRect = settings->getLevelListRect();
	FPoint levelListCentr = listLevelRect.CenterPoint();

	FRect drawrect;
	drawrect.xEnd = listLevelRect.Width() / _maxJLevel;
	drawrect.yEnd = listLevelRect.Height() / _maxILevel;

	// сдвиг к центру относитеьно левого нижнего угла
	FPoint dPosButton;
	dPosButton.x = drawrect.Width() / 2;
	dPosButton.y = drawrect.Height() / 2;

	// уменьшаем кнопки чтобы получить растояние между кнопками
	drawrect.xEnd -= _buttonDistance;
	drawrect.yEnd -= _buttonDistance;

	int maxLevel = _maxJLevel*_maxILevel;

	for (int i = 0; i < _maxILevel; i++)
		for (int j = 0; j<_maxJLevel; j++)
		{
			FPoint buttonPos;
			buttonPos.x = (drawrect.Width() + _buttonDistance) * j;
			buttonPos.y = (drawrect.Height() + _buttonDistance) * (_maxILevel - 1 - i);
			buttonPos += dPosButton + levlListPos;

			int number = _maxJLevel * i + j;
			std::string buttonName = utils::lexical_cast<int>(number);

			SizableButton::Ptr LevelButton;
			LevelButton = std::make_shared <SizableButton>(buttonPos, settings, _tex, buttonName);
			LevelButton->setUnCheckable();
			LevelButton->setDrawRect(drawrect);
			LevelButton->setCentr(levelListCentr);

			LevelButton->setJMax(maxLevel+1); // + замок

			_levelList.push_back(LevelButton);
		}
}