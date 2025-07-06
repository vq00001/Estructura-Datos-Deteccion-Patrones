#include "suffix_tree.hpp"
#include <string>
#include <vector>
#include <algorithm>
#include <iostream>

using namespace std;

SuffixTreeNode::SuffixTreeNode(int start,int* end,SuffixTreeNode* link){
	this->start = start;
	this->end = end;
	this->suffixIndex = start;
	this->suffixLink = link;
}

int SuffixTreeNode::getLength(){
	return *end - start + 1;
}

bool SuffixTreeNode::isLeaf(){
	return this->suffixIndex >= 0;
}


SuffixTree::SuffixTree(string& texto){
	this->text = texto + static_cast<char>(28);
	int* endRoot = new int(-2); 
	root = new SuffixTreeNode(-1,endRoot,nullptr);
	int hojas = 0;
	SuffixTreeNode* lastNewNode= nullptr;
	SuffixTreeNode* activeNode = root;
	int activeEdge = -1;
	int activeLength= 0;
	leafEnd = new int(-1);
	int textSize = text.size();
	for (int i = 0; i < textSize; ++i){
		(*leafEnd)++;
		lastNewNode = nullptr;
		for (int j = hojas; j <= i; ++j){
			//aplicar regla 2 y regla 3
			if(activeLength == 0) activeEdge = i;
			//cout << activeEdge << " despues" << endl;
			if(activeNode->childrens.count(text[activeEdge]) == 0){
				//crear hoja
				SuffixTreeNode* newLeaf = new SuffixTreeNode(i,leafEnd,root);
				newLeaf->suffixIndex = hojas;
				hojas++;
				activeNode->childrens[text[activeEdge]] = newLeaf;

				if (lastNewNode != nullptr) {
                	lastNewNode->suffixLink = activeNode;
                	lastNewNode = nullptr;
            	}
			} else{
				SuffixTreeNode* next = activeNode->childrens.at(text[activeEdge]);
				if(activeLength >= next->getLength()){
					activeEdge += next->getLength();
					activeLength = activeLength - next->getLength();
					activeNode = next;
					j--;
					continue;
				}
				//regla 3 de ukkonen
 				if(text[next->start + activeLength] == text[i]){
					if(lastNewNode != nullptr && activeNode != root){
						lastNewNode->suffixLink = activeNode;
						lastNewNode = nullptr;
					}
					activeLength++;
					break;
				}

				int* newEnd = new int(next->start + activeLength - 1);
				SuffixTreeNode* interNode = new SuffixTreeNode(next->start,newEnd,root);
				interNode->suffixIndex = -1;
				next->start += activeLength;
				interNode->childrens[text[next->start]] = next;
				activeNode->childrens[text[activeEdge]] = interNode;
				SuffixTreeNode* newLeaf = new SuffixTreeNode(i,leafEnd,root);
				newLeaf->suffixIndex = hojas;
				hojas++;
				interNode->childrens[text[i]] = newLeaf ;
				if(lastNewNode != nullptr){
					lastNewNode->suffixLink = interNode;
				}
				lastNewNode = interNode;
			}
			if(activeNode == root && activeLength > 0){
				activeLength--;
				activeEdge = hojas;
			} else if(activeNode != root){
				activeNode = activeNode->suffixLink;
			}
		}
	}
}

void freeNode(SuffixTreeNode* node){
	if(node->isLeaf()){
		delete node;
		return;
	}
	for (auto it = node->childrens.begin(); it != node->childrens.end() ; it++){
		freeNode(it->second);
	}
	delete node->end ;
	delete node;
}

SuffixTree::~SuffixTree(){
	freeNode(root);
	delete leafEnd;
}

void SuffixTree::doTransversal(SuffixTreeNode* node, vector<int>& posiciones){
	if(node->isLeaf()){
		posiciones.push_back(node->suffixIndex);
		return;
	}
	for (auto it = node->childrens.begin(); it != node->childrens.end(); it++){
		doTransversal(it->second,posiciones);
	}
}

vector<int> SuffixTree::search(const string& pattern){
	vector<int> posiciones;
	SuffixTreeNode* activeNode = root;
	int idxEdge= 0;
	int patternSize= pattern.size();
	if (pattern.size() >= text.size()) return posiciones; //Recordar que el texto tiene 1 caracter adicional al original
	for (int i = 0; i < patternSize; ++i){
		if(*(activeNode->end) < idxEdge+ activeNode->start){
			//cout << "efectivamente" << endl;
			if(activeNode->childrens.count(pattern[i]) != 0){
				activeNode = activeNode->childrens.at(pattern[i]);
				idxEdge = 0;
			}
			else return posiciones;
		}
		if(text[activeNode->start + idxEdge] != pattern[i]){
			return posiciones;
		}
		idxEdge++;
	}
	doTransversal(activeNode,posiciones);
	sort(posiciones.begin(),posiciones.end());
	return posiciones;
}