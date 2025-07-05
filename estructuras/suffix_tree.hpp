#ifndef SUFFIXTREE_HPP
#define SUFFIXTREE_HPP
#include <unordered_map>
#include <string>
#include <vector>

class SuffixTreeNode{
public:
	std::unordered_map<int,SuffixTreeNode*> childrens;
	SuffixTreeNode* suffixLink;
	int start;
	int* end;
	int suffixIndex;

	SuffixTreeNode(int start, int* end, SuffixTreeNode* link);
	//~SuffixTreeNode();
	//void addChildren(char key);
	//void linkNode(SuffixTreeNode* node);
	int getLength();
	bool isLeaf();
};


class SuffixTree {
private:
	SuffixTreeNode* root;
	std::string text;
	int* leafEnd;
	void doTransversal(SuffixTreeNode* node, std::vector<int>& posiciones);
	//treeExpansion(int pos, int& hojas);

public:
	SuffixTree(std::string& text);
	~SuffixTree();
	std::vector<int> search(const std::string& pattern);
	
};

#endif