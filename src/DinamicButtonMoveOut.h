#pragma once
#include "DinamicButton.h"
#include "DinamicShowHide.h"

// динамическ€ кнопка котора€ выезжает при по€влении, при исчезновении обратно заезжает
class DinamicButtonMoveFromOut : public DinamicButton, public DinamicShowHideMoveFromOut
{
public:
	typedef std::shared_ptr <DinamicButtonMoveFromOut> Ptr;
	typedef std::vector <Ptr> PtrList;

	void onShow();
	void onHide();

	/*overrided*/ void show(const FPoint& showPos = FPoint(-1, -1));
	/*overrided*/ void hide(const FPoint& showPos = FPoint(-1, -1));

	/*overrided*/ void update(float dt);
	///*overrided*/ void setAlpha(int); без альфы пооему лучше

	void setPos(const FPoint&);
	FPoint getPos() const;

	DinamicButtonMoveFromOut(const FPoint& pos, GameSettings::Ptr gameSettings, Render::Texture* tex, std::string name = "some_button");
	~DinamicButtonMoveFromOut();
};

