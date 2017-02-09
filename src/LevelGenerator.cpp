#include "stdafx.h"
#include "LevelGenerator.h"

LevelGenerator::LevelGenerator(GameSettings::Ptr settings)
	: Solver(settings)
{
	_settings = settings;
	prstn_axePrimitive = settings->getPrstn_axePrimitive();
	prstn_changeTypePrimitive = settings->getPrstn_changeTypePrimitive();
	prstn_bombPrimitive = settings->getPrstn_bombPrimitive();
	prstn_lockPrimitive = settings->getPrstn_lockPrimitive();
	prstn_bigPrimitive = settings->getPrstn_bigPrimitive();
}


LevelGenerator::~LevelGenerator()
{

}

GamePosition LevelGenerator::generate(int boxTypesCount, int moveCountToWin, std::string nameLevel)
{
	std::string beginMessage;
	beginMessage += " start generate level: " + nameLevel;
	beginMessage += ", boxTypesCount: " + utils::lexical_cast<int> (boxTypesCount);
	beginMessage += ", moveCountToWin: " + utils::lexical_cast<int> (moveCountToWin);
	writeInLog(beginMessage);

	GamePosition newGamePos;
	GameTree::Ptr gameTree = nullptr;

	int numberTry = 0;
	int maxCountTry = 100;
	while (!gameTree)
	{
		numberTry++;
		if (numberTry >= maxCountTry)
		{
			writeInLog("Not can create level (Many try)");
			break;
		}
		mxField = createMX();
		tmpMxField = createMX();

		for (int i = 0; i < moveCountToWin; i++)
			generateMove(boxTypesCount);

		newGamePos = createPosOnMx(mxField);
		newGamePos._numMove = 0;
		newGamePos._maxNumMove = moveCountToWin;

		writeInLog("Start generate tree ");
		gameTree = getGameTree(newGamePos);
	}


	if (newGamePos._maxNumMove != moveCountToWin)
		writeInLog("Warning! newGamePos._maxNumMove != moveCountToWin ");

	// проверка позиции, на "проходимость"
	if (!gameTree)
	{
		writeInLog("ERROR! gameTree not generate ");
		return newGamePos;
	}
	std::string treeName = GameTree::getTreeName(nameLevel);
	GameTree::save(gameTree, treeName);
	_fileReader.writeLevel(newGamePos, nameLevel);

	int vCount = GameTree::varyableCount(gameTree);

	std::string endMessage;
	endMessage += "sucessfull! Level have " + utils::lexical_cast <int> (vCount)+" varyables win \n";
	endMessage += "    first varyable is " + gameTree->sons.front()->lastSwap.toString() + "\n";
	endMessage += "    faster path to win is " + utils::lexical_cast <int> (newGamePos._maxNumMove)+"\n";
	writeInLog(endMessage);

	deleteMX(mxField);
	deleteMX(tmpMxField);
	GameTree::freeTree(gameTree);
	return newGamePos;
}


void LevelGenerator::generateMove(int boxTypesCount)
{
	// в случаи неудачи двигаемьс€ дальше (по сдвигу)
	DeltaIJBox d(_settings, 3, 3, boxTypesCount, 12, 0, 0, 0, 0);
	d.next();
	posList pList = getFreeTopPos(mxField);
	primitive p;
	int maxCaskadeSize = _settings->getMaxCascadeSiae();
	int cascadeSize = math::random(1, maxCaskadeSize);
	// генерируем несколько каскадов
	int curNumCascade = 0;
	bool needUndo = false;
	int pType = generateCascadeType();
	int dp = d.get_dj() % pList.size();
	IPoint firstPos = pList[dp];
	copyMX(mxField, tmpMxField);
	while (true)
	{ 
		if (!d.next())
			return;
		pList = getFreeTopPos(tmpMxField);
		dp = d.get_dj() % pList.size();
		int bType = d.get_db() % boxTypesCount;
		bool f_vertical = false;
		if (math::random(0, 1) == 0)
			f_vertical = true;
		int size = 3 + math::random(0, 1);

		IPoint curPos = firstPos;
		curPos.y -= d.get_di() % (pList[dp].y + 1);

		// каскад сильно не смещаем
		curPos.x += math::random(-2, 2);
		if (curNumCascade < cascadeSize)
		{
			if (tryUseCascade(curPos, size, f_vertical, bType))
				curNumCascade++;
			continue;
		}
		if (pType == 0)
			p = createSeamplePrimitive(f_vertical, size);
		else if (pType == 1)
			p = createChangeTypePrimitive();
		else if (pType == 2)
			p = createAxePrimitive();
		else if (pType == 3)
			p = createBombPrimitive();
		else if (pType == 4)
			p = createLockPrimitive();
		else if (pType == 5)
			p = createBigBoxPrimitive();
		else
			assert(false);
		if (!tryUsePrimitive(curPos, p, bType))
		{
			undoChanges();
			curNumCascade = 0;
			continue;
		}
		break;
	}
	useChanges();
}

void LevelGenerator::useChanges()
{
	copyMX(tmpMxField, mxField);
}

void LevelGenerator::undoChanges()
{
	copyMX(mxField, tmpMxField);
}

// позицию считаем по кольцу вычитов
// тип по остатку
bool LevelGenerator::tryUsePrimitive(const IPoint& ptimitivePos, primitive p, int bType)
{
	offsetPrimitive(ptimitivePos, p);

	for (auto pos : p.needBe)
		if (!posInMx(pos) || isClear(tmpMxField, pos))
			return false;

	for (auto pos : p.needClear)
		if (!posInMx(pos) || !isClear(tmpMxField, pos))
			return false;

	if (!tryUpAndCreate(p.needUpAndCreate, bType))
		return false;

	for (auto& sw : p.needSwap)
	{
		if (!posInMx(sw.A) || !posInMx(sw.B))
			return false;
		makeSwap(tmpMxField, sw);
	}

	for (auto pos : p.willCreate)
		if (!posInMx(pos))
			return false;
		else if (!isClear(tmpMxField, pos))
			return false;
		else
			createBox(tmpMxField, pos.x, pos.y, bType);

	handleGravity(tmpMxField);
	if (handleMatch(tmpMxField))
		return false;
	return true;
}

void LevelGenerator::offsetPrimitive(const IPoint& offset, primitive& p)
{
	for (auto& pos = p.needBe.begin(); pos != p.needBe.end();pos++)
		*pos += offset;
	for (auto& pos = p.needClear.begin(); pos != p.needClear.end(); pos++)
		*pos += offset;
	for (auto& pos = p.needUpAndCreate.begin(); pos != p.needUpAndCreate.end(); pos++)
		*pos += offset;
	for (auto& pos = p.willCreate.begin(); pos != p.willCreate.end(); pos++)
		*pos += offset;
	for (auto& sw = p.needSwap.begin(); sw != p.needSwap.end(); sw++)
	{
		(*sw).A += offset;
		(*sw).B += offset;
	}
}

bool LevelGenerator::tryUseCascade(const IPoint& pos, int size, bool vertical, int type)
{
	LevelGenerator::posList needUpPos;
	if (vertical)	// забиваем вертикаль
		for (int up = 0; up < size; up++)
		{
			IPoint posUp = pos;
			posUp.y += up;
			needUpPos.push_back(posUp);
		}
	else	// забиваем горизонталь
		if (pos.x + size >= _maxJ)
			return false;
		else
			for (int dj = 0; dj < size; dj++)
			{
				IPoint curPos = pos;
				curPos.x += dj;
				needUpPos.push_back(curPos);
			}
	if (!tryUpAndCreate(needUpPos, type))
		return false;

	if (!cascadeIsOriginal(tmpMxField, needUpPos))
		return false;

	// добавл€ем несколько позиций слева
	for (int dx = 0; dx < 3; dx ++)
	{
		IPoint curPos = pos;
		curPos.x -= dx;
		if (curPos.x>0)
			needUpPos.push_back(curPos);
	}

	return true;
}

LevelGenerator::posList LevelGenerator::getRelatedPoint(const posList& pList)
{
	posList relatedPoint;
	for (auto point : pList)
	{
		int xPoint = point.x;
		int yPoint = point.y;
		for (int dx = -1; dx <= 1; dx++)
			for (int dy = -1; dy <= 1; dy++)
				if (dx*dy==0)
				{
					IPoint rPoint(xPoint + dx, yPoint + dy);
					if (std::find(pList.begin(), pList.end(), rPoint) == pList.end())
						relatedPoint.push_back(rPoint);
				}
	}
	return relatedPoint;
}

bool LevelGenerator::cascadeIsOriginal(PtrMx mx, const posList& pList)
{
	IPoint somePoint = pList.front();
	int typeCascade = mx[somePoint.x][somePoint.y];

	posList relatedPoint = getRelatedPoint(pList);

	// смотрим чтобы р€дом не было точек такого же цвета
	for (auto rPoint : relatedPoint)
		if (posIsType(mx, rPoint, typeCascade))
			return false;
	return true;
}

bool LevelGenerator::tryUpAndCreate(const posList& posList, int type)
{
	for (auto curPos : posList)
		if (posInMx(curPos))
		{
			int freeUp = getCountClearOnX(tmpMxField, curPos.x);
			if (freeUp == 0) // уперлись в потолок
				return false;
			if (curPos.y > 0)
				if (isClear(tmpMxField, curPos.x, curPos.y - 1)) // висим
					return false;

		}
		else
			return false;
	for (auto curPos : posList)
	{
		upPos(tmpMxField, curPos);
		createBox(tmpMxField, curPos, type);
	}
	return true;
}

void LevelGenerator::tryCreate(const posList& posList, int type)
{
	if (type == 6)
		assert(false);
	for (auto curPos : posList)
		createBox(tmpMxField, curPos.x, curPos.y, type);
}

void LevelGenerator::upPos(PtrMx mx, const IPoint& pos) //поднимает €щики начина€ с позиции
{
	// сдвиг пузырьком
	for (int i = _maxI - 1; i >= pos.y + 1; i--)
	{
		Swap sw;
		sw.A = IPoint(pos.x, i);
		sw.B = IPoint(pos.x, i-1);
		makeSwap(mx, sw);
	}
}

int LevelGenerator::getCountClearOnX(PtrMx mx, int j)
{
	int count = 0;
	for (int i = 0; i < _maxI; i++)
		if (isClear(mx, j, i))
			count++;
	return count;
}

LevelGenerator::posList LevelGenerator::getFreeTopPos(PtrMx mx)
{
	posList top;
	for (int j = 0; j < _maxJ; j++)
	{
		int i = 0;
		for (; i < _maxI; i++)
			if (isClear(mx, j, i))
				break;
		if (i < _maxI)
		{
			IPoint curTopPos;
			curTopPos.x = j;
			curTopPos.y = i;
			top.push_back(curTopPos);
		}
	}
	return top;
}

int LevelGenerator::generateCascadeType()
{
	int kCascadeType = math::random(0, 100);
	int curPersentMax = prstn_changeTypePrimitive;
	if (kCascadeType <= curPersentMax)
		return 1;
	curPersentMax += prstn_axePrimitive;
	if (kCascadeType <= curPersentMax)
		return 2;
	curPersentMax += prstn_bombPrimitive;
	if (kCascadeType <= curPersentMax)
		return 3;
	curPersentMax += prstn_lockPrimitive;
	if (kCascadeType <= curPersentMax)
		return 4;
	curPersentMax += prstn_bigPrimitive;
	if (kCascadeType <= curPersentMax)
		return 5;
	if (curPersentMax > 100)
		assert(false); 
	return 0;
}

// функции генерации

LevelGenerator::posList LevelGenerator::generateCascade(bool vertical, int length)
{
	posList cascadeList;

	IPoint dp;
	if (vertical)
		dp = IPoint(0, 1);
	else
		dp = IPoint(1, 0);

	for (int l = 0; l < length; l++)
		cascadeList.push_back(dp*l);

	return cascadeList;
}

LevelGenerator::primitive LevelGenerator::createSeamplePrimitive(bool vertical, int length)
{
	if (length > 5)
		assert(false);

	primitive seamplePrimitive;

	Swap sw;
	sw.A = IPoint(0, 0);
	sw.B = IPoint(!vertical, vertical);

	seamplePrimitive.needSwap.push_back(sw);

	return seamplePrimitive;
}

// @ - changeTypePrimitive

LevelGenerator::primitive LevelGenerator::createChangeTypePrimitive()
{
	primitive changeTypePrimitive;

	Swap sw;
	//sw.A = IPoint(0, 0);
	//sw.B = IPoint(0, 0);
	sw.changeType = boxType::QuestionBox;
	changeTypePrimitive.needSwap.push_back(sw);

	changeTypePrimitive.needBe.push_back(IPoint(0, 0));

	return changeTypePrimitive;
}

LevelGenerator::primitive LevelGenerator::createAxePrimitive()
{
	primitive changeAxePrimitive;

	changeAxePrimitive.needUpAndCreate.push_back(IPoint(0, 0));

	return changeAxePrimitive;
}

LevelGenerator::primitive LevelGenerator::createBombPrimitive()
{
	primitive BombPrimitive;
	
	bool primitivDirrectIsVertical = math::random(0, 1);
	// генеиуем своп чтобы сломать позицию
	IPoint posA(0, 0); // внутренн€€ позици€ 
	posA.x = math::random(-primitivDirrectIsVertical, primitivDirrectIsVertical);
	posA.y = math::random(-!primitivDirrectIsVertical, !primitivDirrectIsVertical);
	IPoint posB; // внешнн€€ позици€ 
	posB.x = math::random(-!primitivDirrectIsVertical, !primitivDirrectIsVertical);
	posB.y = math::random(-primitivDirrectIsVertical, primitivDirrectIsVertical);

	// пака только ставим бомбы...
	Swap sw; // позци€ 00 00
	sw.changeType = boxType::Bomb;
	BombPrimitive.needSwap.push_back(sw);

	BombPrimitive.needBe.push_back(IPoint(0, 0));
	

	Swap swBomb;
	swBomb.A = posA;
	swBomb.B = posB;
	BombPrimitive.needSwap.push_back(swBomb);



	return BombPrimitive;
}

LevelGenerator::primitive LevelGenerator::createLockPrimitive()
{
	primitive changeBombPrimitive;

	bool primitivDirrectIsVertical = math::random(0, 1);
	// генеиуем своп чтобы сломать позицию
	IPoint posA(0, 0); // внутренн€€ позици€ 
	posA.x = math::random(-primitivDirrectIsVertical, primitivDirrectIsVertical);
	posA.y = math::random(-!primitivDirrectIsVertical, !primitivDirrectIsVertical);
	IPoint posB; // внешнн€€ позици€ 
	posB.x = math::random(-!primitivDirrectIsVertical, !primitivDirrectIsVertical);
	posB.y = math::random(-primitivDirrectIsVertical, primitivDirrectIsVertical);

	IPoint dp = IPoint(!primitivDirrectIsVertical, primitivDirrectIsVertical);
	changeBombPrimitive.needUpAndCreate.push_back(-dp);
	changeBombPrimitive.needUpAndCreate.push_back(dp);

	Swap sw; // позци€ 00 00
	sw.changeType = boxType::Lock;
	changeBombPrimitive.needSwap.push_back(sw);

	changeBombPrimitive.needBe.push_back(IPoint(0, 0));


	Swap swBomb;
	swBomb.A = posA;
	swBomb.B = posB;
	changeBombPrimitive.needSwap.push_back(swBomb);

	return changeBombPrimitive;
}

/*
сейчас просто создаЄт конструцию вида

х
’х

*/
LevelGenerator::primitive LevelGenerator::createBigBoxPrimitive()
{
	primitive bigBoxPrimitive;
	for (int j = 0; j<2; j++)
		for (int i = 0; i < 2; i++)
		{
			IPoint curPos = IPoint(j, i);
			bigBoxPrimitive.needUpAndCreate.push_back(curPos);

			Swap setBig;
			setBig.changeType = grow;
			setBig.A = setBig.B = curPos;
		}

	bigBoxPrimitive.needUpAndCreate.push_back(IPoint(1, 2));
	bigBoxPrimitive.needUpAndCreate.push_back(IPoint(2, 0));

	return bigBoxPrimitive;
}