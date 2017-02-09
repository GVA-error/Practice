#include "stdafx.h"
#include "labelView.h"


LabelView::LabelView(const std::string& label, const FPoint& centrPos)
{
	setText(label);
	setPos(centrPos);
	_show = false;
}


LabelView::~LabelView()
{
}

void LabelView::onShow()
{
	_show = true;
}

void LabelView::onHide()
{
	_show = false;
}

void LabelView::setPos(const FPoint& centrPos)
{
	_curPos = centrPos;
}

FPoint LabelView::getPos() const
{
	return _curPos;
}

void LabelView::setText(const std::string& label)
{
	_curText = label;
}

void LabelView::draw()
{
	if (_show)// && _curPos.y > 0) 
	{
		Render::BindFont(s_stdFont);
		Render::PrintString(_curPos, _curText, 1.f, CenterAlign); // тут почему то иногда вылетает (15 lvl разбивание и резкий выход)
	}
}