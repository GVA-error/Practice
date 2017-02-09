#pragma once
// ����� ������� �������, � ��������� ��������� � ������������ �� ��������

// ������� ����� ��� ��������, ������� ����� �������� ���������� �� ����� �� ��������
class DinamicShowHide
{
public:
	void show();
	void hide();

	// ��� ������ ��� ���������, ������������
	virtual void onShow() = 0;
	virtual void onHide() = 0;
	virtual void setAlpha(float){}; // ������������� ������������ � ����������� �� ���������

	bool isDefaultState() const;
	bool isHiding() const;
	bool isShowing() const;

	void update(float dt);

	DinamicShowHide();
	virtual ~DinamicShowHide(){};
protected:
	enum showStance { default, showing, hiding, showed, hided };
	const float _maxShowStanceTime = 0.5f; // ������ ��� ������� � ���� ����������� �� ���� � ���� �����
	float _timer;
	float _kTime;
	showStance _curShowStance;


};

// ������ ����������� ��� ���
// ��������� - ��������� 
// ������������ - ������
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

// ��������� - �������� �� ���� ������ (����� showPos) 
// ������������ - �������� ������� (� ����� hidePos)
class DinamicShowHideMoveFromOut : public DinamicShowHide
{
public:
	void update(float dt);

	// ����� �����������
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

// ����� ������� ������� hide ����� ��������� ����� ����� ������ show
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