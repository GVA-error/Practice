#include "stdafx.h"
#include "DinamicButton.h"


DinamicButton::DinamicButton(const FPoint& pos, GameSettings::Ptr gameSettings, Render::Texture* tex, std::string name)
	: ViewObject(tex, pos)
{
	if (!tex || !gameSettings)
		Assert(false);
	_name = name;
	_curStance = default;

	_iMax = 3;
	_jMax = 1;

	f_isChecked = false;

}

DinamicButton::~DinamicButton()
{

}


std::string DinamicButton::getName()
{
	return _name;
}

void DinamicButton::setPress()
{
	if (_curStance != onPress)
		_curStance = onPress;
	else
		_curStance = onRealise;
}

void DinamicButton::setUnPress()
{
	_curStance = default;
}

bool DinamicButton::isPress()
{
	_curStance = getCurStance();
	return _curStance == onPress;
}

void DinamicButton::draw(int j)
{
	stance curStance = getCurStance();
	int i = curStance + 1;
	ViewObject::draw(i, j);
}

bool DinamicButton::isRealise()
{
	if (isDead() || isHide())
		return false;

	IPoint mousePos = Core::mainInput.GetMousePos();
	if (ViewObject::contains(mousePos))
		return true;
	return false;
}

DinamicButton::stance DinamicButton::getCurStance()
{
	if (isRealise())
		if (_curStance == onPress)
		{
			_curStance = onPress;
			return _curStance;
		}
		else
		{
			_curStance = onRealise;
			return _curStance;
		}
	if (_curStance != onPress || !f_isChecked)
		_curStance = default;
	return _curStance;
}

void DinamicButton::setCheckable()
{
	f_isChecked = true;
}

void DinamicButton::setUnCheckable()
{
	f_isChecked = false;
}