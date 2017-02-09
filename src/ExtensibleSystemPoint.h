#pragma once

// �������������/���������� ������� �����
class ExtensibleSystemPoint
{
public:
	
	// ���������� ����� �������
	void setCentr(const FPoint&);
	
	// scale = ������� �������
	// � ����������� �� ��������, ���������� ��������� ����� � �������
	// originalPoint - ��������� ����� ��� �������� 1.0
	FPoint getPoint(const FPoint& originalPoint, float scale);
	FRect getRect(const FRect& originalPoint, float scale);
	FRect getRectInZeroPoint(const FRect& originalPoint, float scale);

	ExtensibleSystemPoint(const FPoint& centr = FPoint(0, 0));
	~ExtensibleSystemPoint();
private:

	FPoint _centr;
};

