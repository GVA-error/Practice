#include "stdafx.h"
#include "ExtensibleSystemPoint.h"


ExtensibleSystemPoint::ExtensibleSystemPoint(const FPoint& centr)
{
	setCentr(centr);
}


ExtensibleSystemPoint::~ExtensibleSystemPoint()
{
}

FPoint ExtensibleSystemPoint::getPoint(const FPoint& point, float scale)
{
	// вектор позиции точки в системе
	FPoint posV = point - _centr;
	posV *= scale;
	FPoint newPos = posV + _centr;
	return newPos;
}

FRect ExtensibleSystemPoint::getRect(const FRect& rect, float scale)
{
	FPoint start = FPoint(rect.xStart, rect.yStart);
	FPoint end = FPoint(rect.xEnd, rect.yEnd);
	
	FPoint newStart = getPoint(start, scale);
	FPoint newEnd = getPoint(end, scale);

	FRect newRect = FRect(newStart.x, newEnd.x, newStart.y, newEnd.y);

	return newRect;
}

FRect ExtensibleSystemPoint::getRectInZeroPoint(const FRect& rect, float scale)
{
	FRect newRect = getRect(rect, scale);
	newRect.MoveTo(FPoint(0, 0));
	return newRect;
}

void ExtensibleSystemPoint::setCentr(const FPoint& centr)
{
	_centr = centr;
}