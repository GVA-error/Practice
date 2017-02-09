#pragma once
#include "StringLiterals.h"
#include "ViewObject.h"
#include "DinamicShowHide.h"

// ������� ������� ������� �� �����...
// � ������ �������, �� ������������

// ����� �������� update(dt)
class LabelView : public DinamicShowHideMoveFromOut
{
public:
	typedef std::shared_ptr<LabelView> Ptr;

	LabelView(const std::string& label, const FPoint& centrPos);
	void setText(const std::string& label);
	void draw(); // ������ � draw ����������� �����
	/*override*/ void setPos(const FPoint& centrPos);
	/*override*/ FPoint getPos() const;
	/*override*/ void onShow();
	/*override*/ void onHide();

	~LabelView();
private:
	std::string _curText;
	FPoint _curPos;
	bool _show;
};

