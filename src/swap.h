#pragma once
#include "Box.h"

// Описывает изменение ящиков
// изменения типа, позиции
class Swap
{
public:
	typedef std::list <Swap> List;
	bool error;
	IPoint A;
	IPoint B;
	int changeType; // тип при замене цвета

	void toFile(std::ofstream& out);
	void fromFile(std::ifstream& in);

	std::string toString();

	static Swap Swap::getZeroSwap();
	static Swap Swap::gerErrorSwap();

	Swap(std::ifstream& in);
	Swap();
	~Swap();
	friend bool operator == (const Swap& firstPos, const Swap& secondPos)
	{
		return (firstPos.A == secondPos.A && firstPos.B == secondPos.B ||
			firstPos.A == secondPos.B && firstPos.B == secondPos.A ) && 
			firstPos.error == secondPos.error && 
			firstPos.changeType == secondPos.changeType;
	}
};