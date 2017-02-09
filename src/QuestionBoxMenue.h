#pragma once
#include "SizableButton.h"
#include "LogicField.h"
#include "DinamicShowHide.h"
#include "Box.h"

class QuestionBoxMenue : FastShowAndHide
{
public:
	typedef std::shared_ptr <QuestionBoxMenue> Ptr;

	int selectedBoxType();
	int getLastSellectedBoxType();
	bool boxTypeIsSellected();

	// timeFastShow // время до исчезновения при быстром появлении
	void show(const FPoint& qBoxPos, bool fastShow = false, float timeFastShow = -1);
	void hide();

	bool isHide() const;
	bool isShow() const;

	void update(float dt);
	void draw();

	// вызыывать для обработки нажатия мыши
	void press();
	void unPress();

	FPoint getQBoxPos();
	FPoint getPosBoxType(int boxType);

	QuestionBoxMenue(GameSettings::Ptr settings, Render::Texture* _tex);
	~QuestionBoxMenue();
private:
	const float _boxDistance = 5; // растояние между ящиками при выбое
	const float _offsetFromBorderScreen = 10; // растояние от кая экана
	const float _kYPos = (float)2/3; // Y позиция кнопок относитело поля 
	bool _fastShow;
	FPoint _qBoxPos;
	int _menueBoxCount; // количество кнопок выбора (типов ящиков)
	int _sellectedBox; // последний выбранный бокс

	SizableButton::PtrList _boxButtonList;
	GameSettings::Ptr _settings;

	void generateBoxList(Render::Texture* _tex);

};

