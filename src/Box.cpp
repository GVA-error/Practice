#include "stdafx.h"
#include "Box.h"

Box::Box(const IPoint& pos, GameSettings::Ptr gameSettings, LogicField::Ptr logic, Render::Texture* tex, boxType type, Render::Texture* texLabel)
	: DinamicButtonMoveFromOut(pos, gameSettings, tex)
{
	if (!tex || !gameSettings || !logic)
		Assert(false);

	_dPos = FPoint(0.0f, 0.0f);
	_logic = logic;
	_type = type;

	FRect drawRect = _logic->getCellRect();
	setDrawRect(drawRect);
	_HDrawRect = drawRect.Height();
	_WDrawRect = drawRect.Width();

	for (int dj = 0; dj <= 1; dj++)
		for (int di = 0; di<=1; di++)
		{
			_ID.push_back(_logic->generateNewID());
			_logic->create(_ID.back(), FPoint(0, 0));
			if (!BoxType::typeIsBig(type))
				di = dj = 7; // выходим из цикла, нужен только 1 ID
		}

	FPoint curPos = _logic->getCentrCell(pos);
	setPos(curPos);
	logicMove();
		
	_speed = 0;
	_g = gameSettings->getg();
	_v0 = gameSettings->getV0();

	_posWhenPress = getPos().Rounded();

	_settings = gameSettings;

	_jMax = gameSettings->getBoxTypesCount();
	_iMax = 1;

	_texLabel = texLabel;

	if (BoxType::typeIsBig(_type))
	{
		FRect newDrawRect = drawRect.Scaled(2.0);
		newDrawRect.MoveTo(_logic->getCellRect().CenterPoint());
		setDrawRect(newDrawRect);
	}
}

Box::~Box()
{

}

std::list <IPoint> Box::getAllPos()
{
	std::list <IPoint> bPoss;
	for (auto id : _ID)
		if (_logic->haveID(id))
		{
			IPoint nextPos = _logic->getFieldPos(id);
			bPoss.push_back(nextPos);
		}
	return bPoss;
}

bool Box::isBig()
{
	return BoxType::typeIsBig(_type);
}

void Box::setType(boxType type)
{
	_type = type;
}

void Box::finish()
{
	if (isDead())
		return;
	for (auto id : _ID)
		_logic->destoy(id);
	ViewObject::kill();
}
/*
void Box::onShow()
{
	ViewObject::show();
}

void Box::onHide()
{
	ViewObject::hide();
}
*/
bool Box::isFinished()
{
	if (!isBusy() && isDead())
		return true;
	return false;
}

bool Box::isBusy() const
{
	if (!isDefaultState())
		return true;
	if (!ViewObject::drawInRealPos())
		return true;
	stance curStance = getCurStance();
	FPoint drawPos = ViewObject::getDrawPos();
	FPoint pos = ViewObject::getPos();
	return curStance == falling || 
		_logic->isFly(_ID.front()) ||
		pos.GetDistanceTo(drawPos) > 1; // мало ли округление какое
}

Box::boxInfo Box::getInfo()
{
	boxInfo curInfo;
	IPoint fieldPos = _logic->getFieldPos(_ID.front());
	curInfo.pos = fieldPos;
	curInfo.type = _type;
	return curInfo;
}

bool Box::dPosIsCorrect(const FPoint& dPos) const
{
	FRect drawRect = ViewObject::getDrawRect();
	float wRect = drawRect.Width();
	float hRect = drawRect.Height();
	FPoint pos = getPos();
	drawRect.xStart = pos.x - wRect / 2 + dPos.x;
	drawRect.yStart = pos.y - hRect / 2 + dPos.y;
	drawRect.xEnd = pos.x + wRect / 2 + dPos.x;
	drawRect.yEnd = pos.y + hRect / 2 + dPos.y;

	std::vector <FPoint> vertexes;
	vertexes.reserve(4);
	for (int i = 0; i < 2; i++)
		for (int j = 0; j < 2; j++)
		{
			FPoint curPos;
			curPos.x = drawRect.xStart + j * drawRect.Width();
			curPos.y = drawRect.yStart + i * drawRect.Height();
			vertexes.push_back(curPos);
		}
	for (auto pos : vertexes)
		if (!_logic->posIsCorrect(pos))
			return false;
	return true;
}

FPoint Box::getDPos() const
{
	return _dPos;
}

boxType Box::getType()
{
	return _type;
}

IPoint Box::getFieldPos()
{
	return _logic->getFieldPos(_ID.front());
}

void Box::update(float dt, bool gravityIsOn)
{
	if (isDefaultState())
	{
		FPoint newPos = getPos();
		if (gravityIsOn)
		{
			float dy = (_v0 + _speed) * dt + _g * dt * dt / 2;
			newPos.y -= dy;
		}

		FPoint floorNewPosBox = newPos;
		floorNewPosBox.y -= _HDrawRect / 2.0f;

		bool f_canFall = _logic->posIsFree(floorNewPosBox, _ID.front());
		if (_ID.size() > 1)
		{
			FPoint rightFloor = floorNewPosBox;
			rightFloor.x += _WDrawRect;
			f_canFall &= _logic->posIsFree(rightFloor, _ID[1]);
		}

		if (f_canFall)
		{
			ViewObject::setPos(newPos);
			_speed += _g * dt;
			logicMove();
		}
		else // если не можем падать зафиксируем позицию
		{
			IPoint fieldPos = _logic->getFieldPos(_ID.front());
			ViewObject::setPos(_logic->getCentrCell(fieldPos));
			_speed = 0;
		}
	}
	update(_dPos, dt);
}

FPoint Box::getBot()
{
	FPoint bot = ViewObject::getBot();
	if (isBig())
	{
		FRect drawRect = _logic->getCellRect();
		bot = ViewObject::getPos(); 
	}
	return bot;
}

void Box::logicMove()
{
	FPoint bot = getBot();
	for (int di = 0; di <= 1; di++)
		for (int dj = 0; dj <= 1; dj++)
			if (_ID.size() > di * 2 + dj)
			{
				FPoint cellSize = _logic->getCellRect().CenterPoint() * 2;
				FPoint curBot = bot + FPoint(cellSize.x*dj, cellSize.y*di);
				_logic->move(_ID[di * 2 + dj], curBot);
			}
			else
				return;
}

void Box::unLockIt()
{
	_type = (boxType)BoxType::unLock(_type);
	_texLabel = nullptr;
}

bool Box::isLockIt()
{
	return BoxType::typeIsLock(_type);
}

bool Box::isBond()
{
	return BoxType::typeIsBomb(_type);
}

void Box::setPress()
{
	// залоченные и большие не кликабельны
	if (isLockIt() || isBig())
		return;
	_posWhenPress = Core::mainInput.GetMousePos();
	DinamicButton::setPress();
}

void Box::setDPos(const FPoint& dPos)
{
	if (isBig())
		return;
	_dPos = dPos;
}

void Box::setFieldPos(const IPoint& pos)
{
	ViewObject::setPos(_logic->getCentrCell(pos));
	logicMove();
}

IPoint Box::getDPosOnMousePos()
{
	IPoint mousePos = Core::mainInput.GetMousePos();
	IPoint dPos = mousePos - _posWhenPress;
	if (abs(dPos.x) < abs(dPos.y))
		dPos.x = 0;
	else
		dPos.y = 0;
	return dPos;
}

IPoint Box::tranformDPosOnFieldPos() const
{
	IPoint dFieldPos;
	dFieldPos.x = math::sign(_dPos.x);
	dFieldPos.y = math::sign(_dPos.y);
	
	IPoint fieldPos = _logic->getFieldPos(_ID.front());
	fieldPos += dFieldPos;
	return fieldPos;
}

Box::stance Box::getCurStance() const
{
	stance curStance;
	if (isDead())
		curStance = stance::dead;
	else if 
		(_speed > 0 || (_ID.size() > 1 && _logic->isFly(_ID.front()) && _logic->isFly(_ID[1])))
			curStance = stance::falling;
	else
		curStance = stance::default;
	return curStance;
}

void Box::draw()
{
	draw(_dPos); 
}

void Box::draw(const FPoint& dPos)
{
	FPoint oldPos = getPos();
	ViewObject::setPos(oldPos + _dPos);
	int colorType = BoxType::getColorType(_type);
	ViewObject::draw(1, colorType + 1);
	FPoint newPos = getPos();
	ViewObject::setPos(newPos - _dPos);
}

void Box::update(const FPoint& dPos, float dt)
{
	FPoint oldPos = getPos();
	ViewObject::setPos(oldPos + _dPos);
	if (_curShowStance != default)
		DinamicButtonMoveFromOut::update(dt);
	else
		ViewObject::update(dt);
	FPoint newPos = getPos();
	ViewObject::setPos(newPos - _dPos);
	
}
