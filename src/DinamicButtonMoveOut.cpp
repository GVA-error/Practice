#include "stdafx.h"
#include "DinamicButtonMoveOut.h"


DinamicButtonMoveFromOut::DinamicButtonMoveFromOut(const FPoint& pos, GameSettings::Ptr gameSettings, Render::Texture* tex, std::string name)
	: DinamicButton(pos, gameSettings, tex, name),
	DinamicShowHideMoveFromOut()
{

}

void DinamicButtonMoveFromOut::onShow()
{
	DinamicButton::show();
}

void DinamicButtonMoveFromOut::onHide()
{
	DinamicButton::hide();
}

void DinamicButtonMoveFromOut::update(float dt)
{
	DinamicShowHideMoveFromOut::update(dt);
	ViewObject::update(dt);
}

void DinamicButtonMoveFromOut::show(const FPoint& showPos)
{
	DinamicShowHideMoveFromOut::show(showPos);
	setDrawPos(showPos);

}

void DinamicButtonMoveFromOut::hide(const FPoint& hidePos)
{
	unSetSlowPointPath();
	FPoint cur_hidePos = hidePos;
	if (cur_hidePos == FPoint(-1, -1))
		cur_hidePos = _showPos;
	DinamicShowHideMoveFromOut::hide(cur_hidePos);
}

DinamicButtonMoveFromOut::~DinamicButtonMoveFromOut()
{

}

void DinamicButtonMoveFromOut::setPos(const FPoint& pos)
{
	ViewObject::setPos(pos);
}

FPoint DinamicButtonMoveFromOut::getPos() const
{
	return ViewObject::getPos();
}