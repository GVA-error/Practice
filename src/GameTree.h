#pragma once
#include <fstream>
#include <string>

#include "swap.h"

// класс для работы с деревьями
// информацию храним голыми указателями 
// для перехода к умным перегрузить/переписать typedef Ptr; makeNode(const Swap&); freeNode(Ptr node); freeTree(Ptr root);
class GameTree
{
public:
	// файл хранящий дерево игры
	const std::string treeExt = ".gtr";

	class Node;
	typedef Node* Ptr;
	class Node
	{
	public:
		Swap lastSwap; // своп приводящий к этой вершине
		std::vector < Ptr > sons;

		static Ptr makeNode(const Swap&);
		static void freeNode(Ptr node);

	private:
		Node(){};
		~Node(){};
	};


	static void freeTree(Ptr root);

	// сохранение, чтение дерева
	static void save(Ptr tree, std::string fileName);
	static Ptr load(std::string fileName);

	// возвращает указатель на потомка, по пути history 
	static Ptr getSoneOnHistory(Swap::List history, Ptr root);

	static std::string getTreeName(const std::string&); // имя файла в котором храниться дерево уровня с назввание levelName

	// удаляет дерево (не удаляет выйгрышные позиции)
	// возвращает true если удалил дерево поностью
	// считаем что у выйгрышной вершины нет сыновей
	static bool freeTree(Ptr root, int curDeep, int maxDeep);
	
	static bool havePathWithOutAxe(Ptr root);
	static bool freeTreeWithAxe(Ptr& root); // удаляет все пути решания с топором 

	// возвращает количество вариантов прохождения (количество листьев)
	static int varyableCount(Ptr root);

	// глубина дерева
	static int getDeep(Ptr root, int curDeep = 0);

	GameTree();
	~GameTree();

protected:
	static void varyableCount(Ptr root, int& vCount);

	static void write(Ptr tree, std::ofstream& out);
	static Ptr read(std::ifstream& out);
};

