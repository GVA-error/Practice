#pragma once
#include <string>
#include <boost\filesystem.hpp>
#include "GamePosition.h"
#include "File.h"

// чтение и запись в/из xml, информации о игровой позиции
class GameFileReader
{
public:
	GamePosition readLevel(std::string fileName);
	void writeLevel(const GamePosition&, std::string fileName);

	std::string getFileExtXMLGameLevel() const;
	std::string getPrefixLevelName() const;

	GameFileReader();
	~GameFileReader();
private:
	// константы парсинга xml (выделяем на куче) 
	const char* s_titl = "LevelInfo";
	const char* s_boxInfo = "Box";
	const char* s_x = "x";
	const char* s_y = "y";
	const char* s_type = "type";
	const char* s_maxNumMove = "Moves";
	const char* s_max = "max";
	const char* s_settings = "settings";
	const char* s_boxes = "Boxes";

	const std::string _fileExtXMLGameLevel = ".lvl";
	const std::string s_prefixLevelName = "level_";

	GamePosition readXMLLevel(std::string fileName);
	void writeXMLLevel(const GamePosition&, std::string fileName);
	
};

