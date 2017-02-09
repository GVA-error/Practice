#include "stdafx.h"
#include "BigBox.h"


BigBox::BigBox(const IPoint& pos, GameSettings::Ptr gameSettings, LogicField::Ptr logic, Render::Texture* tex)
	: Box(pos, gameSettings, logic, tex, boxType::BrownBox)
{

}


BigBox::~BigBox()
{

}
