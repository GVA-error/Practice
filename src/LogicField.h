#pragma once
#include "GameSettings.h"
#include <algorithm>
#include <iterator> 
#include <utility> 

// класс предоставл€ющий пространственную логику пол€
// оперирует точками, но не размерами
// основнык методы move и getFieldPos
class LogicField
{
public:
	typedef std::shared_ptr <LogicField> Ptr;

	// позици€ в координатах пол€
	// в этой функции можно дополнительно учесть погрешность флоата
	IPoint getPosInMxField(const FPoint&absPos) const;

	// координаты левого нижнего угла €чейки
	FPoint roundedPos(const FPoint&) const;
	FPoint roundedPos(const IPoint&) const;
	
	// возвращает центр €чейки
	FPoint getCentrCell(const IPoint&) const;

	// проверка вхождени€ точки в €чейку
	bool posInCell(const FPoint& pos, const IPoint& fieldPos) const;

	// может ли объект с неким ID встать на €чейку
	bool posIsFree(const IPoint&, int ID) const;
	bool posIsFree(const FPoint&, int ID) const;

	void move(int ID, const FPoint& newPos); // перемещает объект
	void create(int ID, const FPoint& pos); // делает первый мув, чтобы забить место
	void destoy(int ID);

	// проверить наличие ID
	bool haveID(int ID);
	bool posIsCorrect(const IPoint&) const;
	bool posIsCorrect(const FPoint&) const;

	// убирает объекты с логического пол€ (не сбрасывает логику расположени€ границ €чеек)
	void reset();

	float getHCell();
	float getWCell();

	int getMaxI();
	int getMaxJ();

	IPoint getFieldPos(int ID);

	FRect getCellRect();

	bool isFly(int ID);

	int getID(const IPoint&) const;

	// генерирует новый ID (возвращает не зан€тый)
	int generateNewID();

	LogicField(GameSettings::Ptr);
	~LogicField();

protected:
	void retuneLogic(GameSettings::Ptr);
private:
	const int maxID = 100;
	FPoint _fieldPos;
	FRect _fieldRect;

	int _maxI, _maxJ;

	std::map <int, FPoint> _objRealPos;
	std::map <int, IPoint> _objFieldPos;

	//std::map <IPoint, int> _fieldPosObj;

	FRect _CellRect; // пр€моугольник одной €чейки (остальные можно получить сдвигом)


};

