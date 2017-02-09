#include "stdafx.h"
#include "ViewObject.h"


ViewObject::ViewObject(Render::Texture* tex, const FPoint& startPos)
{
	if (!tex)
		Assert(false);
	setTex(tex);
	_pos = startPos;
	_scale = 1.0f;
	_isLive = true;
	_angle = math::PI/2; // 90 
	_iMax = 1;
	_jMax = 1;
	_drawRect = FRect(_tex->getBitmapRect());
	_drawPos = _pos; 
	_show = false;
	_maxSpeed = _defaultMaxSpeed;
	_texLabel = nullptr;
	_alpha = 1.0f;
	unSetSlowPointPath();
}


ViewObject::~ViewObject()
{

}

FPoint ViewObject::getDrawPos() const
{
	return _drawPos;
}

void ViewObject::setIMax(int newValue)
{
	_iMax = newValue;
}

void ViewObject::setJMax(int newValue)
{
	_jMax = newValue;
}

void ViewObject::setMaxSpeed(float speed)
{
	_maxSpeed = speed;
}

void ViewObject::setDrawPos(const FPoint& pos)
{
	_drawPos = pos;
}

void ViewObject::show()
{
	_show = true;
}

void ViewObject::hide()
{
	_show = false;
}

bool ViewObject::isShow() const
{
	return (_show && isLive());
}

bool ViewObject::isHide() const
{
	return !isShow();
}

void ViewObject::setSlowPointPath(const FPoint& pPoint)
{
	f_slowPath = true;
	if (pPoint.x < 0)
		_pathPoint = _pos;
	_pathPoint = pPoint;
}

void ViewObject::unSetSlowPointPath()
{
	f_slowPath = false;
	ViewObject::_pathPoint = FPoint(-1.0f, -1.0f);
}

void ViewObject::update(float dt)
{
	if (isDead())
		return;

	// для понимания построить прямоугольный треугольник вида АВС где В - прямой. А - реальная позиция объекта, В - точка пути, С - позиция прорисовки

	// растояние которое прошло бы тело если двигалось с максимальной скоростью
	float vSped = dt * _maxSpeed;

	float hypotenuse = _pos.GetDistanceTo(_drawPos);

	float cathetusAB = _pathPoint.GetDistanceTo(_pos);
	float k;
	FPoint dDrawPos;

	if (hypotenuse > cathetusAB)
	{
		float cathetusBC = _pathPoint.GetDistanceTo(_drawPos);
		if (cathetusBC != 0)
			k = vSped / cathetusBC;
		if (cathetusBC == 0 || k > 1.0f)
		{
			_drawPos = _pathPoint;
			return;
		}
		dDrawPos = (_pathPoint - _drawPos) * k;
	}
	else
	{
		// растояние от текущей позиции прорисовки до настоящей 
		if (hypotenuse!=0)
			k = vSped / hypotenuse;
		if (hypotenuse == 0 || k > 1.0f)
		{
			_drawPos = _pos;
			return;
		}
		dDrawPos = (_pos - _drawPos) * k;
	}
	_drawPos += dDrawPos;
}

FPoint ViewObject::getBot()
{
	FRect drawRect = getDrawRect();
	FPoint botPos = _pos;
	botPos.y -= _drawRect.Height() / 2;
	return botPos;
}

bool ViewObject::contains(const FPoint& point) const
{
	IRect realTexRect = getDrawRect().Rounded();
	realTexRect.x = -realTexRect.width * 0.5f + _pos.x;
	realTexRect.y = -realTexRect.height * 0.5f + _pos.y;
	return realTexRect.Contains(point.Rounded());
}

float ViewObject::getCurSize() const
{
	float realSize = getRealSize();
	float curSize = realSize*_scale;
	return curSize;
}

void ViewObject::setSize(float size)
{
	float realSize = getRealSize();
	_scale = size / realSize;
}

float ViewObject::getRealSize() const
{
	IRect texRect = _tex->getBitmapRect();
	float h = texRect.Height();
	float w = texRect.Width();
	float realSize = math::min(w, h) / 2;
	return realSize;
}

bool ViewObject::isLive() const
{
	return _isLive;
}

bool ViewObject::isDead() const
{
	return !_isLive;
}

void ViewObject::kill()
{
	_isLive = false;
}

void ViewObject::setTex(Render::Texture* tex)
{
	_tex = tex;
}

bool ViewObject::drawInRealPos() const
{
	float distanceToRealPos = _pos.GetDistanceTo(_drawPos);
	return distanceToRealPos < 1;
}

void ViewObject::setPos(const FPoint& pos)
{
	_pos = pos;
	if (!f_slowPath)
	{
		_drawPos = _pos;
		return;
	}
}

void ViewObject::setAlpha(float alpha)
{
	_alpha = alpha;
}

FPoint ViewObject::getPos() const
{
	return _pos;
}

void ViewObject::draw(int i, int j)
{
	if (isDead() || (isHide()))
		return;
	if (_drawPos.x < 10)
		return;
	float Fangle = _angle / math::PI * 180 - 90;
	Render::device.PushMatrix();

	Render::device.MatrixTranslate(_drawPos);
	Render::device.MatrixRotate(math::Vector3(0, 0, 1), Fangle);


	FRect rect(getDrawRect());

	float xStart = ((float)j - 1.0f) / _jMax;
	float yStart = ((float)i - 1.0f) / _iMax;
	float xEnd = ((float)j) / _jMax;
	float yEnd = ((float)i) / _iMax;
	FRect uv(xStart, xEnd, yStart, yEnd);

	_tex->TranslateUV(rect, uv);

	float dx, dy;
	dx = -rect.Width() * 0.5f;
	dy = -rect.Height() * 0.5f;
	Render::device.MatrixTranslate(dx, dy, 0.0f);

	_tex->Bind();
	

	Render::BeginAlphaMul(_alpha);
	Render::DrawRect(rect, uv); // иногда рендер даёт какие то ошибки
	Render::EndAlphaMul();
	if (_texLabel)
	{
		_texLabel->Bind();
		IRect drawLabelRect = _texLabel->getBitmapRect();
		int dx = drawLabelRect.Width()/2;
		int dy = drawLabelRect.Height()/2;
		_texLabel->Draw(rect.CenterPoint() - FPoint(dx, dy));
	}

	Render::device.PopMatrix();
}

FRect ViewObject::getDrawRect() const
{
	if (_drawRect.xEnd < 0)
	{
		IRect texRect = _tex->getBitmapRect();
		FRect rect;
		rect.xEnd = _drawRect.Width() / (float)_jMax;
		rect.yEnd = _drawRect.Height() / (float)_iMax;
		return rect;
	}
	return _drawRect;
}

void ViewObject::setDrawRect(const FRect& dRect)
{
	_drawRect = dRect;
}

/*
void ViewObject::destroyDead(std::list <ViewObject*> & list)
{
	for (auto obj = list.begin(); obj != list.end();)
	{
		ViewObject* curObj = *obj;
		if (curObj->isDead())
		{
			auto dobj = obj;
			obj++;
			list.erase(dobj);
			delete curObj;
		}
		else
			obj++;
	}
}
*/