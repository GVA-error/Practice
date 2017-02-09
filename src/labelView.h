#pragma once
#include "StringLiterals.h"
#include "ViewObject.h"
#include "DinamicShowHide.h"

// красиво выводит надписи на экран...
// В данный моммент, не испоьзуеться

// нужно вызывать update(dt)
class LabelView : public DinamicShowHideMoveFromOut
{
public:
	typedef std::shared_ptr<LabelView> Ptr;

	LabelView(const std::string& label, const FPoint& centrPos);
	void setText(const std::string& label);
	void draw(); // иногда в draw начинаютсья глюки
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

