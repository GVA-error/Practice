#include "stdafx.h"
#include "SizableButton.h"


SizableButton::SizableButton(const FPoint& pos, GameSettings::Ptr gameSettings, Render::Texture* tex, std::string name)
	: DinamicButton(pos, gameSettings, tex, name),
	ExtensibleSystemPoint(),
	DinamicShowHideBubble()
{
	FRect drawRect = ViewObject::getDrawRect();
	setDrawRect(drawRect);
	_curShowStance = showStance::default;
	setPos(pos);
}

void SizableButton::setPos(const FPoint& pos)
{
	_originalPos = pos;
	setCentr(pos);
	DinamicButton::setPos(pos);
}

void SizableButton::setAlpha(float a)
{
	ViewObject::setAlpha(a);
}

SizableButton::~SizableButton()
{

}

void SizableButton::setDrawRect(const FRect& rect)
{
	DinamicButton::setDrawRect(rect);
	_originalDrawRect = ViewObject::getDrawRect();;
}


void SizableButton::show()
{
	DinamicShowHideBubble::show();
}

void SizableButton::hide()
{
	DinamicShowHideBubble::hide();
}

void SizableButton::update(float dt)
{
	DinamicShowHideBubble::update(dt);
}

void SizableButton::scale(float scale)
{
	FRect newDrawRect = getRectInZeroPoint(_originalDrawRect, scale);
	ViewObject::setDrawRect(newDrawRect);
	FPoint curPos = getPoint(_originalPos, scale);
	DinamicButton::setPos(curPos);
}

void SizableButton::onShow()
{
	DinamicButton::show();
}

void SizableButton::onHide()
{
	DinamicButton::hide();
}