#include "stdafx.h"
#include "GameFileReader.h"


GameFileReader::GameFileReader()
{
}


GameFileReader::~GameFileReader()
{
}

std::string GameFileReader::getFileExtXMLGameLevel() const
{
	return _fileExtXMLGameLevel;
}

std::string GameFileReader::getPrefixLevelName() const
{
	return s_prefixLevelName;
}

GamePosition GameFileReader::readLevel(std::string fileName)
{
	if (!File::ExistsInFs(fileName))
		return GamePosition();

	std::string ext = boost::filesystem::extension(fileName);
	if (ext == _fileExtXMLGameLevel)
		return readXMLLevel(fileName);

	//Assert(false);

	return GamePosition();
}

void GameFileReader::writeLevel(const GamePosition& gamePos, std::string fileName)
{

	std::string ext = boost::filesystem::extension(fileName);
	if (ext == _fileExtXMLGameLevel)
		return writeXMLLevel(gamePos, fileName);
}

void GameFileReader::writeXMLLevel(const GamePosition& gamePos, std::string fileName)
{
	rapidxml::xml_document<> doc;
	rapidxml::xml_node<>* decl = doc.allocate_node(rapidxml::node_declaration);
	decl->append_attribute(doc.allocate_attribute("encoding", "utf-8"));
	doc.append_node(decl);

	rapidxml::xml_node<>* root = doc.allocate_node(rapidxml::node_element, s_titl);
	doc.append_node(root);

	rapidxml::xml_node<>* settings = doc.allocate_node(rapidxml::node_element, s_settings);
	root->append_node(settings);

	rapidxml::xml_node<>* boxes = doc.allocate_node(rapidxml::node_element, s_boxes);
	root->append_node(boxes);


	// заполн€ем количество ходов
	rapidxml::xml_node<>* maxNumMoveXml = doc.allocate_node(rapidxml::node_element, s_maxNumMove);

	int maxNumMove = gamePos._maxNumMove;
	char* c_maxNum = doc.allocate_string(utils::lexical_cast<int>(maxNumMove).c_str());
	
	rapidxml::xml_attribute <>* attributeMax = doc.allocate_attribute(s_max, c_maxNum);
	maxNumMoveXml->append_attribute(attributeMax);
	settings->append_node(maxNumMoveXml);

	// заполн€ем список €щиков
	for (auto boxInfo : gamePos.boxList)
	{
		rapidxml::xml_node<>* boxInfoXml = doc.allocate_node(rapidxml::node_element, s_boxInfo);

		IPoint pos = boxInfo.pos;
		int x = pos.x;
		int y = pos.y;
		boxType type = boxInfo.type;

		std::string valueX = utils::lexical_cast<int>(x);
		std::string valueY = utils::lexical_cast<int>(y);
		std::string valueType = utils::lexical_cast<int>(type);

		char* c_X = doc.allocate_string(valueX.c_str());
		char* c_Y = doc.allocate_string(valueY.c_str());
		char* c_Type = doc.allocate_string(valueType.c_str());
		
		rapidxml::xml_attribute <>* attributeX = doc.allocate_attribute(s_x, c_X);
		boxInfoXml->append_attribute(attributeX);
		rapidxml::xml_attribute <>* attributeY = doc.allocate_attribute(s_y, c_Y);
		boxInfoXml->append_attribute(attributeY);
		rapidxml::xml_attribute <>* attributeType = doc.allocate_attribute(s_type, c_Type);
		boxInfoXml->append_attribute(attributeType);
		

		boxes->append_node(boxInfoXml);
	}
	
	std::string xml_as_string;
	rapidxml::print(std::back_inserter(xml_as_string), doc);

	std::ofstream file_stored(fileName);
	file_stored << doc;
	file_stored.close();
	doc.clear();

}

GamePosition GameFileReader::readXMLLevel(std::string fileName)
{
	GamePosition gamePos;
	
	if (!File::ExistsInFs(fileName))
	{
		Assert(false);
		return gamePos; // Ѕудет работать с ретурном
	}

	rapidxml::file<> file(fileName.c_str());

	rapidxml::xml_document<> doc;
	doc.parse<0>(file.data());

	rapidxml::xml_node<>* root = doc.first_node();
	if (!root) 
	{ 
		Assert(false);
		throw std::runtime_error("No root node"); 
	}

	rapidxml::xml_node<>* lvlInfo = root->first_node();
	for (; lvlInfo != NULL; lvlInfo = lvlInfo->next_sibling())
	{
		if (utils::equals(lvlInfo->name(), s_settings))
		{
			rapidxml::xml_node<>* setting = lvlInfo->first_node();
			for (; setting != NULL; setting = setting->next_sibling())
			{
				// –азличные игровые настройки добавл€ютьс€ тут
				if (utils::equals(setting->name(), s_maxNumMove))
				{
					int maxNum = utils::lexical_cast<int>(setting->first_attribute(s_max)->value());
					gamePos._maxNumMove = maxNum;
				}
			}
		}
		if (utils::equals(lvlInfo->name(), s_boxes))
		{
			rapidxml::xml_node<>* someBox = lvlInfo->first_node();
			for (; someBox != NULL; someBox = someBox->next_sibling())
			{
				// ящики
				if (utils::equals(someBox->name(), s_boxInfo))
				{
					Box::boxInfo newBoxInfo;
					int x = utils::lexical_cast<int>(someBox->first_attribute(s_x)->value());
					int y = utils::lexical_cast<int>(someBox->first_attribute(s_y)->value());
					boxType type = (boxType)utils::lexical_cast<int>(someBox->first_attribute(s_type)->value());
					newBoxInfo.pos = IPoint(x, y);
					newBoxInfo.type = type;
					gamePos.boxList.push_back(newBoxInfo);
				}
			}
		}
	}

	return gamePos;
}