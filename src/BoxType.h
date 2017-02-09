#pragma once

// ���� ������ (�������� �������)
enum boxType {
	RedBox, BlueBox, BrownBox, GreenBox, YellowBox, QuestionBox, Destoyed,
	RedBoxBomb, BlueBoxBomb, BrownBoxBomb, GreenBoxBomb, YellowBoxBomb, Bomb,
	RedBoxLock, BlueBoxLock, BrownBoxLock, GreenBoxLock, YellowBoxLock, Lock, Unlock,
	RedBoxBig, BlueBoxBig, BrownBoxBig, GreenBoxBig, YellowBoxBig, grow
};

// ���� ������ � ������ � ����
class BoxType
{
public:
	// �������� �����
	static int getColorType(int type);

	// �������� ���� �����
	static bool typeIsBig(int type);
	static bool typeIsBomb(int type);
	static bool typeIsLock(int type);

	// ����� ���� �����
	static int toBomb(int type);
	static int toLock(int type);
	static int toBig(int type);
	static int unLock(int type);

private:
	BoxType();
	~BoxType();
};

