#pragma once

enum eventType { create, gravity_on, wait, finishAll, show, hide};
enum objType { buton, label};

// Класс описывающий игровые события. 
// Не используеться
class MVBEvent
{
	std::string _argString;
	float _argFloat;
	FPoint _argPos;
	eventType _eventType;

public:

	std::string getArgString();
	float getArgFloat();
	FPoint getArgPos();
	eventType getEventType();
	
	void setArgString(const std::string&);
	void setArgFloat(float);
	void setArgPos(const FPoint&);
	void setEventType(eventType);

	MVBEvent();
	~MVBEvent();
};

