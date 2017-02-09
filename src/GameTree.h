#pragma once
#include <fstream>
#include <string>

#include "swap.h"

// ����� ��� ������ � ���������
// ���������� ������ ������ ����������� 
// ��� �������� � ����� �����������/���������� typedef Ptr; makeNode(const Swap&); freeNode(Ptr node); freeTree(Ptr root);
class GameTree
{
public:
	// ���� �������� ������ ����
	const std::string treeExt = ".gtr";

	class Node;
	typedef Node* Ptr;
	class Node
	{
	public:
		Swap lastSwap; // ���� ���������� � ���� �������
		std::vector < Ptr > sons;

		static Ptr makeNode(const Swap&);
		static void freeNode(Ptr node);

	private:
		Node(){};
		~Node(){};
	};


	static void freeTree(Ptr root);

	// ����������, ������ ������
	static void save(Ptr tree, std::string fileName);
	static Ptr load(std::string fileName);

	// ���������� ��������� �� �������, �� ���� history 
	static Ptr getSoneOnHistory(Swap::List history, Ptr root);

	static std::string getTreeName(const std::string&); // ��� ����� � ������� ��������� ������ ������ � ��������� levelName

	// ������� ������ (�� ������� ���������� �������)
	// ���������� true ���� ������ ������ ��������
	// ������� ��� � ���������� ������� ��� �������
	static bool freeTree(Ptr root, int curDeep, int maxDeep);
	
	static bool havePathWithOutAxe(Ptr root);
	static bool freeTreeWithAxe(Ptr& root); // ������� ��� ���� ������� � ������� 

	// ���������� ���������� ��������� ����������� (���������� �������)
	static int varyableCount(Ptr root);

	// ������� ������
	static int getDeep(Ptr root, int curDeep = 0);

	GameTree();
	~GameTree();

protected:
	static void varyableCount(Ptr root, int& vCount);

	static void write(Ptr tree, std::ofstream& out);
	static Ptr read(std::ifstream& out);
};

