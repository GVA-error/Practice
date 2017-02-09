#pragma once
#include "Box.h"

class BigBox : public Box
{

public:


	BigBox(const IPoint& pos, GameSettings::Ptr gameSettings, LogicField::Ptr, Render::Texture* tex);
	~BigBox();
};

