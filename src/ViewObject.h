#pragma once
#include <vector>
#include "boost\shared_ptr.hpp"

class ViewObject
{
public:
	typedef std::shared_ptr<ViewObject> Ptr;
	typedef std::vector <Ptr> PtrList;

	// ���������� ����� i - ������, j - ������� 
	// �� �������� ������� - _iMax*frameSizeY, � ������� - _jMax*frameSizeX, 
	// ��� ������ ������ ����� = frameSizeX �� frameSizeY
	// ��������� i j ������� � 1
	void draw(int i = 1, int j = 1);

	void setTex(Render::Texture* tex);

	// ������������ ������ (��� �������� �������� ����� ������) 
	void setSize(float size);
	float getCurSize()  const;
	// ������ ������� � ��������� 1.0 
	float getRealSize()  const;

	// ���������� ������ ������� ���������� �������
	FPoint getBot();

	bool isShow() const;
	bool isHide() const;

	bool isLive() const;
	bool isDead() const;
	void kill();

	void show();
	void hide();

	bool contains(const FPoint&) const;

	//static void destroyDead(std::list <ViewObject*> &);

	ViewObject(Render::Texture* tex, const FPoint& start_pos = FPoint(0.0, 0.0));
	virtual ~ViewObject();
		
	// ������������� ����� ������ ��� ������� �������� (���� ������������ ������� ���� ������ ����������)
	// ��� ����������� �������� ������������� ����� �������� ���� �� ����������� ���������
	void setSlowPointPath(const FPoint& pathPoint = FPoint(-1.0f, -1.0f));
	// �������� ������� ��������
	void unSetSlowPointPath();

	void setMaxSpeed(float);
	// ������������� ������� ���������� (���� ������� ���������� �� ����� �������� ������� ������ ����� �� ������)
	void setDrawPos(const FPoint&);
	FPoint getDrawPos() const;
	bool drawInRealPos() const; // ���������� ������� ���������� � ��������

	void setDrawRect(const FRect&);
	void setDrawSize(float);

	// ����������� ������� ���������� � �������� ������� 
	void update(float dt); // ��� ���������� _drawPos 

	void setPos(const FPoint&);
	FPoint getPos() const;

	void setIMax(int);
	void setJMax(int);

	void setAlpha(float alpha = 0.0f);
protected:
	float _angle; // ���� � ��������
	float _scale;
	bool _isLive; // �� ����� �� ������, �� ��������, ��� ���� ������
	Render::Texture* _tex; // ���������� ����, ������� ���������� �� ��� ��������
	int _iMax;
	int _jMax;
	float _alpha;


	Render::Texture* _texLabel; // �������� ������� ������������� ������ ������ ����� � ��� ������ (����� ��� �����)

	// ���� �� ���������� ������������� ��� ���������, ������ ��������������� ��������
	FRect getDrawRect() const;

private:
	FRect _drawRect;
	FPoint _pos;

	bool _show;
	bool f_slowPath;
	FPoint _pathPoint;
	FPoint _drawPos;
	const float _defaultMaxSpeed = 300;
	float _maxSpeed;
};
/*
template <class T> void destroyDead(std::list<std::shared_ptr<T>> &list, bool destroyAll = false)
{
	for (std::vector::iterator obj = list.begin(); obj != list.end();)
	{
		std::shared_ptr<T> curObj = *obj;
		if (curObj->isDead() || destroyAll)
		{
			auto dobj = obj;
			obj++;
			list.erase(dobj);
		}
		else
			obj++;
	}
}
*/
template <class T> void destroyFinished(std::vector<std::shared_ptr<T>> &vector, bool destroyAll = false)
{
	int size = vector.size();
	for (int i = 0; i<size;)
	{
		std::shared_ptr<T> curObj = vector[i];
		if (curObj->isFinished() || destroyAll)
		{
			std::swap(vector[i], vector[size-1]);
			vector.pop_back();
			size--;
		}
		else
			i++;
	}
}
