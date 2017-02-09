#include "stdafx.h"
#include "DinamicShowHide.h"


DinamicShowHide::DinamicShowHide()
{
	_curShowStance = showStance::default;
	_timer = 0;
}

DinamicShowHideBubble::DinamicShowHideBubble()
	: DinamicShowHide()
{
	_minScale = 0.7;
}

DinamicShowHideBubble::~DinamicShowHideBubble()
{

}

DinamicShowHideMoveFromOut::DinamicShowHideMoveFromOut()
	: DinamicShowHide()
{

}

void DinamicShowHideBubble::setMinScale(float scale)
{
	_minScale = scale;
}

float DinamicShowHideBubble::getCurScale()
{
	return _curScale;
}

bool DinamicShowHide::isDefaultState() const
{
	return _curShowStance == default;
}

bool DinamicShowHide::isShowing() const
{
	return _curShowStance == showing;
}

bool DinamicShowHide::isHiding() const
{
	return _curShowStance == hiding;
}

void DinamicShowHide::show()
{
	if (_curShowStance != showing)
	{
		_timer = 0;
		_curShowStance = showing;
		onShow();
	}
}

void DinamicShowHide::hide()
{
	if (_curShowStance != hiding)
	{
		_timer = 0;
		_curShowStance = hiding;
	}
}

void DinamicShowHide::update(float dt)
{
	_kTime = _timer / _maxShowStanceTime;
	if (_timer > _maxShowStanceTime)
	{
		_kTime = 1.0f;
		if (_curShowStance == hiding)
			onHide();
		_curShowStance = default;
	}

	if (_curShowStance == hiding)
		_kTime = 1 - _kTime;

	float curAlpha = math::lerp(0.0f, 1.0f, _kTime);
	setAlpha(curAlpha);
	
	_timer += dt;
}

void DinamicShowHideBubble::update(float dt)
{
	DinamicShowHide::update(dt);
	_curScale = math::lerp(_minScale, 1.0f, _kTime);
	scale(_curScale);
}

void DinamicShowHideMoveFromOut::update(float dt)
{
	DinamicShowHide::update(dt);
	_vDPos = _realPos - _showPos;
	_vDPos *= DinamicShowHide::_kTime;
	FPoint curPos = _showPos + _vDPos;
	setPos(curPos);
}

void DinamicShowHideMoveFromOut::show(const FPoint& showPos)
{
	DinamicShowHide::show();
	_realPos = getPos();
	_showPos = showPos;
	setPos(_showPos);
}

void DinamicShowHideMoveFromOut::hide(const FPoint& hidePos)
{
	DinamicShowHide::hide();
	_realPos = getPos();
	_showPos = hidePos;
}

FastShowAndHide::FastShowAndHide()
{
	_timer = _hideTime = 0;
}

void FastShowAndHide::show(float timeBeforeHide)
{
	_timer = 0;
	_hideTime = timeBeforeHide;
}

void FastShowAndHide::update(float dt)
{
	if (_timer >= _hideTime)
		return;
	_timer += dt;
	if (_timer >= _hideTime)
		hide();
}