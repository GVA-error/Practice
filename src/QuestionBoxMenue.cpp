#include "stdafx.h"
#include "QuestionBoxMenue.h"

QuestionBoxMenue::QuestionBoxMenue(GameSettings::Ptr settings, Render::Texture* _tex)
	: FastShowAndHide()
{
	_settings = settings;
	generateBoxList(_tex);
	_sellectedBox = -1;
	_qBoxPos = FPoint(0, 0);
	_fastShow = false;
}

QuestionBoxMenue::~QuestionBoxMenue()
{

}

int QuestionBoxMenue::selectedBoxType()
{
	for (int bType = 0; bType < _menueBoxCount; bType++)
		if (_boxButtonList[bType]->isPress())
		{
			_sellectedBox = bType;
			return bType;
		}
	return _sellectedBox;
}

bool QuestionBoxMenue::boxTypeIsSellected()
{
	if (selectedBoxType() >= 0)
		return true;
	return false;
}

int QuestionBoxMenue::getLastSellectedBoxType()
{
	return _sellectedBox;
}

void QuestionBoxMenue::show(const FPoint& qBoxPos, bool fastShow, float timeFastShow)
{
	_fastShow = fastShow;
	if (_fastShow)
	{
		_sellectedBox = -1;
		FastShowAndHide::show(timeFastShow);
	}
	_qBoxPos = qBoxPos;
	for (int bType = 0; bType < _menueBoxCount; bType++)
	{
		_boxButtonList[bType]->setCentr(_qBoxPos);
		_boxButtonList[bType]->show();
	}
}

void QuestionBoxMenue::hide()
{
	_sellectedBox = selectedBoxType();
	for (int bType = 0; bType < _menueBoxCount; bType++)
	{
		if (bType == _sellectedBox)
			_boxButtonList[bType]->setCentr(_qBoxPos);
		else
		{
			FPoint curPos = _boxButtonList[bType]->getPos();
			_boxButtonList[bType]->setCentr(curPos);
		}
		_boxButtonList[bType]->hide();
	}

}

FPoint QuestionBoxMenue::getQBoxPos()
{
	return _qBoxPos;
}

bool QuestionBoxMenue::isHide() const
{
	for (auto button : _boxButtonList)
		if (button->isShow())
			return false;
	return true;
}

bool QuestionBoxMenue::isShow() const
{
	return !isHide();
}

void QuestionBoxMenue::update(float dt)
{
	if (_fastShow)
		FastShowAndHide::update(dt);
	for (auto button : _boxButtonList)
		button->update(dt);
}

void QuestionBoxMenue::draw()
{
	for (int bType = 0; bType < _menueBoxCount; bType++)
		_boxButtonList[bType]->ViewObject::draw(1, bType + 1);
}

// вызыывать для обработки нажатия мыши
void QuestionBoxMenue::press()
{
	for (auto& button : _boxButtonList)
		if (button->isRealise())
			button->setPress();
	hide(); // если мисклик, исчезаем
}

void QuestionBoxMenue::unPress()
{
	_sellectedBox = -1;
	for (auto& button : _boxButtonList)
		if (button->isPress())
			button->setUnPress();
}

void QuestionBoxMenue::generateBoxList(Render::Texture* tex)
{
	int boxTypesCount = _settings->getBoxTypesCount();
	_menueBoxCount = boxTypesCount - 1;
	for (int bType = 0; bType < _menueBoxCount; bType++)
	{
		float screenW = _settings->getScreenW();
		float screenH = _settings->getScreenH();

		FPoint boxPos = getPosBoxType(bType); // смещение к центру

		float cellMenueW = (screenW - 2 * _offsetFromBorderScreen) / _menueBoxCount;
		float drawRectSize = cellMenueW - _boxDistance;
		FRect wrawRect = FRect(0, drawRectSize, 0, drawRectSize);

		SizableButton::Ptr newButtonBox = std::make_shared <SizableButton>(boxPos, _settings, tex);

		FPoint defaultShowHidePoint = _settings->getCentrField();

		newButtonBox->setDrawRect(wrawRect);
		newButtonBox->setCentr(defaultShowHidePoint);
		newButtonBox->setJMax(boxTypesCount);
		newButtonBox->setIMax(1);

		_boxButtonList.push_back(newButtonBox);
	}
}

FPoint QuestionBoxMenue::getPosBoxType(int boxType)
{
	float screenW = _settings->getScreenW();
	float screenH = _settings->getScreenH();
	float cellMenueW = (screenW - 2 * _offsetFromBorderScreen) / _menueBoxCount;

	float yPos = _kYPos * screenH;
	float xPos = _offsetFromBorderScreen + boxType*cellMenueW;

	FPoint boxPos = FPoint(xPos, yPos);
	boxPos += FPoint(cellMenueW / 2, cellMenueW / 2); // смещение к центру

	return boxPos;
}