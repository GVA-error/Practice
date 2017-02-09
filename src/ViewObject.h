#pragma once
#include <vector>
#include "boost\shared_ptr.hpp"

class ViewObject
{
public:
	typedef std::shared_ptr<ViewObject> Ptr;
	typedef std::vector <Ptr> PtrList;

	// прорисовка кадра i - строки, j - столбца 
	// из текстуры высотой - _iMax*frameSizeY, и шириной - _jMax*frameSizeX, 
	// где размер одного кадра = frameSizeX на frameSizeY
	// номерация i j ведёться с 1
	void draw(int i = 1, int j = 1);

	void setTex(Render::Texture* tex);

	// масштабируем объект (под размером имееться ввиду радиус) 
	void setSize(float size);
	float getCurSize()  const;
	// размер объекта с масштабом 1.0 
	float getRealSize()  const;

	// возвращает нижнюю границу прорисовки объекта
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
		
	// устанавливает точку обхода при плавном движении (если пользователь слишком резо сменил координату)
	// При неправильно заданной промежуточной точке движение идет по кратчайшему растоянию
	void setSlowPointPath(const FPoint& pathPoint = FPoint(-1.0f, -1.0f));
	// отменяем плавное движение
	void unSetSlowPointPath();

	void setMaxSpeed(float);
	// Устанавливает позицию прорисовки (пока позиция прорисовки не равна реальной позиции обьект будет на экране)
	void setDrawPos(const FPoint&);
	FPoint getDrawPos() const;
	bool drawInRealPos() const; // сравнивает позицию прорисовки и реальную

	void setDrawRect(const FRect&);
	void setDrawSize(float);

	// пододвигает позицию прорисовки к реальной позиции 
	void update(float dt); // для обновления _drawPos 

	void setPos(const FPoint&);
	FPoint getPos() const;

	void setIMax(int);
	void setJMax(int);

	void setAlpha(float alpha = 0.0f);
protected:
	float _angle; // угол в радианах
	float _scale;
	bool _isLive; // не живых не рисуем, не апдейтим, ждём пока удалят
	Render::Texture* _tex; // запоминаем сюда, заранее выделенную во вне текстуру
	int _iMax;
	int _jMax;
	float _alpha;


	Render::Texture* _texLabel; // текстура которую дополнительно рисуем поверх бокса в его центре (замок или бомта)

	// если не установлен прямоугольник для рисования, рисуем пропорционально текстуре
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
