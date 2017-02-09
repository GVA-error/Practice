#include "stdafx.h"
#include "Delta.h"

// хорошо бы усложнить перебор
bool DeltaIJBox::next()
{
	if (_curTry >= _maxTry)
		return false;
	_dj = math::random(0, _maxJ);
	_di = math::random(0, _maxI);
	_db = math::random(0, _maxB);
	_dp = math::random(0, _maxP);
	_curTry++;
	return true;
}

int DeltaIJBox::get_di()
{
	return _di;
}

int DeltaIJBox::get_dj()
{
	return _dj;
}

int DeltaIJBox::get_db()
{
	return _db;
}

int DeltaIJBox::get_dp()
{
	return _dp;
}

void DeltaIJBox::reset()
{
	_di = _diStart;
	_dj = _djStart;
	_db = _dbStart;
	_dp = _dpStart;
}

void DeltaIJBox::setMaxTry(int maxTry)
{
	_maxTry = maxTry;
}

DeltaIJBox::DeltaIJBox(GameSettings::Ptr settings, int maxJ, int maxI, int maxB, int maxP, int dj, int di, int db, int dp)
{
	_diStart = _di = di;
	_djStart = _dj = dj;
	_dbStart = _db = db;
	_dpStart = _dp = dp;
	_maxI = maxI;
	_maxJ = maxJ;
	_maxB = maxB;
	_maxP = maxP;
	_curTry = 0;
	_maxTry = settings->getMaxNumTryInDelta();
}

DeltaIJBox::~DeltaIJBox()
{

}