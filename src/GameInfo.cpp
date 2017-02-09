#include "stdafx.h"
#include "GameInfo.h"

GameInfo gameInfo;

void GameInfo::Load()
{
	std::string dataPath = getAppDataPath();
	std::string fulNameGameInfo = dataPath + s_GameInfoXml;
	if (!File::ExistsInFs(fulNameGameInfo)) {
		return;
	}
	Xml::RapidXmlDocument doc(fulNameGameInfo);
	rapidxml::xml_node<>* root = doc.first_node();
	if (!root) 
	{ 
		Assert(false); 
		return; 
	}
	rapidxml::xml_node<> *globalsXml = root->first_node("Globals");
	if (globalsXml)
	{
		_globals.Load(globalsXml);
	}
}

void GameInfo::Save()
{
	rapidxml::xml_document<char> doc;
	rapidxml::xml_node<char>* root = Xml::NewNode(&doc, "root");
	rapidxml::xml_node<> *globalsXml = Xml::NewNode(root, "Globals");
	_globals.Save(globalsXml);

	std::ofstream file;
	std::string dataPath = getAppDataPath();
	std::string fulNameGameInfo = dataPath + s_GameInfoXml;
	file.open(fulNameGameInfo);
	file << doc;

}

int GameInfo::getGlobalInt(const std::string &name, int def)
{
	return _globals.getInt(name, def);
}

void GameInfo::setGlobalInt(const std::string &name, int value)
{
	_globals.setInt(name, value);
}
