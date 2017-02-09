#pragma once
#include "DinamicButton.h"
#include "ExtensibleSystemPoint.h"
#include "DinamicShowHide.h"

// Увеличевающаяся со временем кнопка 
// если указан центр системы, то часть системы, иначе центр системы = центру текстуры кнопки (ьудет просто увеличиваться и уменьшаться)
class SizableButton : public DinamicButton, public ExtensibleSystemPoint, public  DinamicShowHideBubble
{
public:
	typedef std::shared_ptr <SizableButton> Ptr;
	typedef std::vector <Ptr> PtrList;

	void update(float dt);

	void onShow();
	void onHide();

	/*overrided*/ void show();
	/*overrided*/ void hide();
	/*overrided*/ void setPos(const FPoint&);
	/*overrided*/ void setAlpha(float a);
	/*overrided*/ void scale(float scale);

	void setDrawRect(const FRect&);

	SizableButton(const FPoint& pos, GameSettings::Ptr gameSettings, Render::Texture* tex, std::string name = "some_button");
	~SizableButton();
private:
	FRect _originalDrawRect;
	FPoint _originalPos;

};

