#include "stdafx.h"
#include "DinamicEllipse.h"
#include "GameSettings.h"

DinamicEllipse::DinamicEllipse(const FPoint& pos, std::shared_ptr<GameSettings> gameSettings, Render::Texture* tex, ParticleEffect* eff)
	: ViewObject(tex)
{
	if (!tex || !gameSettings)
	{
		return;
		Assert(false);
	}
	_timer = 0.0f;
	_cur_radius = 0;
	_pos = pos;
	_bornTime = gameSettings->getBornTime();

	_liveTime = gameSettings->getLifeTime();
	_deathTime = gameSettings->getDeathTime();

	_maxRadius = _defaultMaxSize;
	initVariables();
}

void DinamicEllipse::initVariables()
{
	if (_liveTime < 0)
		_beginDeath = _bornTime;
	else
		_beginDeath = _bornTime + _liveTime;
	_endDeath = _beginDeath + _deathTime;
	_maxTime = _endDeath + 1;


}

DinamicEllipse::~DinamicEllipse()
{

}

void DinamicEllipse::update(float dt)
{
	if (!isLive())
		return;

	// изменение радиуса во времени
	if (_timer < _bornTime || _liveTime < 0)
	{
		float kTime = _timer / _bornTime;
		if (kTime > 1.0f)
			kTime = 1.0f;
		float funcTime = math::ease(kTime, 0.0f, 1.0f);
		_cur_radius = math::lerp(0.0f, _maxRadius, funcTime);
	}
	else if (_liveTime >= 0)
		if (_timer >= _beginDeath && _timer < _endDeath)
		{

			float kTime = (_timer - _beginDeath) / (_deathTime);
			float funcTime = math::ease(kTime, 0.0f, 1.0f);
			_cur_radius = math::lerp(_maxRadius, 0.0f, funcTime);
		}
		else if (_timer > _endDeath)
			this->kill();

	// устанавливаем размер от радиуса
	setSize(_cur_radius);


	if (_timer >= _bornTime && _liveTime < 0)
		return;

	if (_timer >= _maxTime)
		_timer = 0;
	else 
		_timer += dt;
}

void DinamicEllipse::hide()
{
	_liveTime = 0;
	_timer = _beginDeath;
}