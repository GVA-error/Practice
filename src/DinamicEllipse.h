#pragma once
#include "GameSettings.h"
#include "ViewObject.h"

// ��������� ��������� ������������� ������� (������ � �� ��� ������� ������� - � ����������� ����� �������)
class DinamicEllipse : public ViewObject
{
public:
	void update(float dt);

	DinamicEllipse(const FPoint& pos, std::shared_ptr<GameSettings> gameSettings, Render::Texture* tex, ParticleEffect* eff = nullptr);
	~DinamicEllipse();

	void hide();
protected:
	//GameSettings* _gameSettings;
	void initVariables();

	const float _defaultMaxSize = 100;
	float _cur_radius;
	float _timer;
	float _bornTime;
	float _deathTime;
	float _maxRadius;
	float _liveTime;
	float _beginDeath; // _beginDeath � _endDeath �������� ���� ������� ������� ������ 
	float _endDeath;
	float _maxTime;
};

