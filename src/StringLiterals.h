#pragma once
#include <string>

// даёт биекцию строка - тип объекта

// типы кнопок
enum btnType { btn_start, btn_exit, btn_level, btnSmall_mainMenue, btn_backMove, btn_sound, btn_done, btn_help, btn_retry };

enum lblType { text, lvl };

// названия текстур для кнопок
const std::string s_SoundButton = "SoundButton";
const std::string s_BackMoveButton = "BackMove";
const std::string s_DoneButton = "Done";
const std::string s_retryButton = "Done"; // нужна текстура ретрая... не могу найти видео с ней.
const std::string s_MainMenueButton = "MainMenue";
const std::string s_HelpButton = "help";
const std::string s_Bomb = "bomb";
const std::string s_Lock = "lock";

const std::string s_GameInfoXml = "GameInfo.xml";

const std::string s_LevelList = "LevelList";
const std::string s_Background = "BackGround";
const std::string s_effBoxHide = "BoxHide";
const std::string s_availableLvl = "AvailableLvl";

const std::string s_EffSwapX = "swapX";
const std::string s_EffSwapY = "swapY";
const std::string s_EffSwapType = "swapType";
const std::string s_EffSwapTypeOffsetTime = "swapTypeOffsetTime";
const std::string s_EffAxe = "Axe";
const std::string s_EffAxeKick = "AxeKick";
const std::string s_EffBomb = "Bomb";
const std::string s_EffExplosion = "BombEnd";
const std::string s_EffLock = "Lock";
const std::string s_EffUnLock = "LockEnd";

const std::string s_NormalRandom = "normalRandom";
const std::string s_EasyRandom = "easyRandom";
const std::string s_HardRandom = "hardRandom";

const std::string s_programmMessageFont = "arial";
const std::string s_stdFont = "stdGameFont";

const std::string s_generatingField = "genegating...";
const std::string s_loadingField = "loading...";

const std::string s_Boxes = "Boxes";

// путь до папки с пользовательской информацией
static std::string getAppDataPath()
{
	std::string dirname = IO::Path::GetSpecialFolderPath(SpecialFolder::LocalDocuments) + "\\Playrix\\" + Core::Application::APPLICATION_NAME + "\\";
	if (!IO::Directory::Exists(dirname)) {
		IO::Directory::CreateDirectories(dirname);
	}
	return dirname;
}
