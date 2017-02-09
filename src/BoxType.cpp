#include "stdafx.h"
#include "BoxType.h"


BoxType::BoxType()
{
}


BoxType::~BoxType()
{
}

bool BoxType::typeIsBig(int type)
{
	return type >= RedBoxBig && type <= YellowBoxBig;
}

bool BoxType::typeIsBomb(int type)
{
	return type >= RedBoxBomb && type <= YellowBoxBomb;
}

bool BoxType::typeIsLock(int type)
{
	return type >= RedBoxLock && type <= YellowBoxLock;
}

int BoxType::getColorType(int type)
{
	if (typeIsBomb(type))
		type -= RedBoxBomb;
	if (typeIsLock(type))
		type -= RedBoxLock;
	if (typeIsBig(type))
		type -= RedBoxBig;

	return type;
}

int BoxType::toBomb(int type)
{
	if (type == boxType::QuestionBox)
		type--;
	int dType;
	if (typeIsBomb(type))
		dType = 0;
	else if (typeIsLock(type))
		dType = RedBoxBomb - RedBoxLock;
	if (typeIsBig(type))
		dType = RedBoxBomb - RedBoxBig;
	else
		dType = RedBoxBomb - RedBox;
	return type + dType;
}

int BoxType::toLock(int type)
{
	if (type == boxType::QuestionBox)
		type--;
	int dType;
	if (typeIsLock(type))
		dType = 0;
	else if (typeIsBomb(type))
		dType = RedBoxLock - RedBoxBomb;
	if (typeIsBig(type))
		dType = RedBoxLock - RedBoxBig;
	else
		dType = RedBoxLock - RedBox;
	return type + dType;
}

int BoxType::toBig(int type)
{
	if (type == boxType::QuestionBox)
		type--;
	int dType;
	if (typeIsBig(type))
		dType = 0;
	else if (typeIsLock(type))
		dType = RedBoxBig - RedBoxLock;
	else if (typeIsBomb(type))
		dType = RedBoxBig - RedBoxBomb;
	else
		dType = RedBoxBig - RedBox;
	return type + dType;
}

int BoxType::unLock(int type)
{
	if (type == boxType::QuestionBox)
		assert(false);
	int dType;
	if (!typeIsLock(type))
		return type;
	dType = RedBoxLock - RedBox;
	return type - dType;
}
