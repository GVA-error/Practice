#include "stdafx.h"
#include "GameTree.h"


GameTree::GameTree()
{
}

GameTree::~GameTree()
{
}

void GameTree::save(Ptr tree, std::string fileName)
{
	std::ofstream out(fileName);
	GameTree::write(tree, out);
	out.close();
}

bool GameTree::havePathWithOutAxe(Ptr node)
{
	if (!node)
		return false;  
	if (node->lastSwap.changeType == boxType::Destoyed)
		return false;
	bool f_havePathWithoutAxe = node->sons.empty();
	for (auto& sune : node->sons)
		f_havePathWithoutAxe |= havePathWithOutAxe(sune);
	return f_havePathWithoutAxe;
}

bool GameTree::freeTreeWithAxe(Ptr& node) // удаляет все пути решания с топором
{
	if (!node)
		return true;

	if (node->lastSwap.changeType == boxType::Destoyed)
	{
		freeTree(node);
		return true;
	}

	if (node->sons.empty())
		return false;

	for (int s = 0; s < node->sons.size();)
		if (freeTreeWithAxe(node->sons[s]))
			node->sons.erase(node->sons.begin() + s);
		else
			s++;

	if (node->sons.empty())
	{
		Node::freeNode(node);
		return true;
	}

	return false;
}

int GameTree::getDeep(Ptr root, int curDeep)
{
	if (!root)
		return 0;

	int deep = curDeep;
	for (auto sone : root->sons)
	{
		int soneDeep = getDeep(sone, curDeep + 1);
		deep = math::max(soneDeep, deep);
	}
	return deep;
}

int GameTree::varyableCount(Ptr root)
{
	int vCount = 0;
	varyableCount(root, vCount);
	return vCount;
}

void GameTree::varyableCount(Ptr root, int& vCount)
{
	if (root->sons.empty())
		vCount++;
	else
		for (auto& sone : root->sons)
			varyableCount(sone, vCount);
}

GameTree::Ptr GameTree::load(std::string fileName)
{
	std::ifstream in(fileName);
	GameTree::Ptr tree = GameTree::read(in);
	return tree;
}

// TODO надо как то по другому убирать расширения
std::string GameTree::getTreeName(const std::string& levelName)
{
	std::string treeName = levelName;
	if (levelName.size() < 4)
		Assert(false);
	for (int i = 0; i < 4; i++)
		treeName.pop_back();
	treeName += GameTree().treeExt;
	return treeName;
}

GameTree::Ptr GameTree::getSoneOnHistory(Swap::List history, Ptr root)
{
	GameTree::Ptr curSone = root;
	for (auto nodePath : history)
	{
		bool f_find = false; // найден ли очередной сын
		for (auto& sone : curSone->sons)
			if (sone->lastSwap == nodePath)
			{
				f_find = true;
				curSone = sone;
				break;
			}
		if (!f_find)
			return nullptr;
	}
	return curSone;
}

void GameTree::write(Ptr tree, std::ofstream& out)
{
	if (!tree)
		return;
	// записываем как мы сюда пришли (последнюю вершиу)
	
	tree->lastSwap.toFile(out);
	// далее количество вершин
	out << tree->sons.size() << " ";

	for (auto sone : tree->sons)
		write(sone, out);
}

GameTree::Ptr GameTree::read(std::ifstream& in)
{
	Swap sw(in);

	if (!in)
		return nullptr;

	int numSuns;
	in >> numSuns;

	GameTree::Ptr curNode = Node::makeNode(sw);

	for (int i = 0; i < numSuns; i++)
	{
		GameTree::Ptr newSone = read(in);
		curNode->sons.push_back(newSone);
	}

	return curNode;

}

GameTree::Ptr GameTree::Node::makeNode(const Swap& lastSwap)
{
	GameTree::Ptr newNode = new Node();
	newNode->lastSwap = lastSwap;
	return newNode;
}

void GameTree::freeTree(Ptr root)
{
	if (!root)
		return;
	if (!root->sons.empty())
		for (auto& node : root->sons)
			freeTree(node);
	if (!root->sons.empty())
		assert("GameTree::freeTree ERROR: Have sone after clear!");
	Node::freeNode(root);
}

bool GameTree::freeTree(Ptr root, int curDeep, int maxDeep)
{
	if (!root)
		return true;
	if (curDeep == maxDeep)
	{
		// в выйгрышной позиции нет других вариантов
		bool fWinPos = root->sons.empty();
		if (!fWinPos)
			freeTree(root);
		return !fWinPos;
	}

	if (root->sons.empty())
		assert(false); // видимо есть более короткий путь а мы и не знали...

	for (int i = 0; i < root->sons.size();)
		if (freeTree(root->sons[i], curDeep + 1, maxDeep))
			root->sons.erase(root->sons.begin() + i);
		else
			i++;

	if (root->sons.empty())
	{
		freeTree(root);
		return true;
	}
	return false;
}

void GameTree::Node::freeNode(Ptr node)
{
	delete node;
}