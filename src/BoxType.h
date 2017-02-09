#pragma once

// типы ящиков (обычного размера)
enum boxType {
	RedBox, BlueBox, BrownBox, GreenBox, YellowBox, QuestionBox, Destoyed,
	RedBoxBomb, BlueBoxBomb, BrownBoxBomb, GreenBoxBomb, YellowBoxBomb, Bomb,
	RedBoxLock, BlueBoxLock, BrownBoxLock, GreenBoxLock, YellowBoxLock, Lock, Unlock,
	RedBoxBig, BlueBoxBig, BrownBoxBig, GreenBoxBig, YellowBoxBig, grow
};

// типы ящиков и работа с ними
class BoxType
{
public:
	// проверка цвета
	static int getColorType(int type);

	// проверка типа ящика
	static bool typeIsBig(int type);
	static bool typeIsBomb(int type);
	static bool typeIsLock(int type);

	// смена типа ящика
	static int toBomb(int type);
	static int toLock(int type);
	static int toBig(int type);
	static int unLock(int type);

private:
	BoxType();
	~BoxType();
};

