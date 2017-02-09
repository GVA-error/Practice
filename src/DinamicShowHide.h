#pragma once
// Набор базовых классов, с функциями появления и исчезновения со временем

// базовый класс для объектов, которые имеют свойство появляться на экане со временем
class DinamicShowHide
{
public:
	void show();
	void hide();

	// что делать для появления, исчезновения
	virtual void onShow() = 0;
	virtual void onHide() = 0;
	virtual void setAlpha(float){}; // устанавливает прозрачность в зависимости от состояния

	bool isDefaultState() const;
	bool isHiding() const;
	bool isShowing() const;

	void update(float dt);

	DinamicShowHide();
	virtual ~DinamicShowHide(){};
protected:
	enum showStance { default, showing, hiding, showed, hided };
	const float _maxShowStanceTime = 0.5f; // сейчас все объекты в игре появляються за одно и тоже время
	float _timer;
	float _kTime;
	showStance _curShowStance;


};

// объект появляеться как шар
// появление - надувание 
// исчезновение - сдутие
class DinamicShowHideBubble : public DinamicShowHide
{
public:
	void update(float dt);
	virtual void scale(float scale) = 0;
	
	void setMinScale(float);
	float getCurScale();

	DinamicShowHideBubble();
	~DinamicShowHideBubble();
protected:
	float _minScale;
	float _curScale;
};

// появление - вылетает из края экрана (точки showPos) 
// исчезновение - залетает обратно (в точку hidePos)
class DinamicShowHideMoveFromOut : public DinamicShowHide
{
public:
	void update(float dt);

	// нужно перегрузить
	virtual void setPos(const FPoint&) = 0;
	virtual FPoint getPos() const = 0;

	void show(const FPoint& showPos);
	void hide(const FPoint& hidePos);

	DinamicShowHideMoveFromOut();
protected:
	 FPoint _vDPos;
	FPoint _showPos;
	FPoint _realPos;
};

// класс который вызовет hide через некоторое время после вызова show
class FastShowAndHide
{
public:
	void show(float timeBeforeHide);
	void update(float dt);

	virtual void hide() = 0;

	FastShowAndHide();
private:
	float _timer;
	float _hideTime;
};