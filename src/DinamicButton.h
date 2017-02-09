#pragma once
#include <vector>
#include "GameSettings.h"
#include "ViewObject.h"
#include "ExtensibleSystemPoint.h"

// Динамическая кнопка. При наведении переходит в состояние onRealise и меняет текстуру
class DinamicButton : public ViewObject
{
public:
	typedef std::shared_ptr <DinamicButton> Ptr;
	typedef std::vector <Ptr> PtrList;
	std::string getName();

	bool isRealise();
	bool isPress();
	void setPress();
	void setUnPress();

	void setCheckable();
	void setUnCheckable();

	/*override*/ void draw(int j=1); // перегружаем перерисовку, теперь в зависимости от состояния 

	DinamicButton(const FPoint& pos, GameSettings::Ptr gameSettings, Render::Texture* tex, std::string name = "some_button");
	~DinamicButton();

private:
	enum stance { onRealise, onPress, default };

	bool f_isChecked;

	//bool _hiding; // процесс закрытия
	std::string _name;
	stance _curStance;

	// отвечает за визуальное поведение кнопки
	stance getCurStance();


};

