#pragma once

// расшир€юща€с€/сужающа€с€ система точек
class ExtensibleSystemPoint
{
public:
	
	// ”становить центр системы
	void setCentr(const FPoint&);
	
	// scale = масштаб системы
	// в зависимости от масштаба, возвращает соста€ние точек в системе
	// originalPoint - положение точки при масштабе 1.0
	FPoint getPoint(const FPoint& originalPoint, float scale);
	FRect getRect(const FRect& originalPoint, float scale);
	FRect getRectInZeroPoint(const FRect& originalPoint, float scale);

	ExtensibleSystemPoint(const FPoint& centr = FPoint(0, 0));
	~ExtensibleSystemPoint();
private:

	FPoint _centr;
};

