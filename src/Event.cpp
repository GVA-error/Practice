#include "stdafx.h"
#include "Event.h"


MVBEvent::MVBEvent()
{
}


MVBEvent::~MVBEvent()
{
}

std::string MVBEvent::getArgString()
{
	return _argString;
}

float MVBEvent::getArgFloat()
{
	return _argFloat;
}

FPoint MVBEvent::getArgPos()
{
	return _argPos;
}

eventType MVBEvent::getEventType()
{
	return _eventType;
}

void MVBEvent::setArgString(const std::string& str)
{
	_argString = str;
}

void MVBEvent::setArgFloat(float value)
{
	_argFloat = value;
}

void MVBEvent::setArgPos(const FPoint& pos)
{
	_argPos = pos;
}

void MVBEvent::setEventType(eventType type)
{
	_eventType = type;
}