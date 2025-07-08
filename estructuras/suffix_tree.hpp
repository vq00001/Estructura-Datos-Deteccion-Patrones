#ifndef SUFFIXTREE_HPP
#define SUFFIXTREE_HPP
#include <unordered_map>
#include <string>
#include <vector>
#include <stack>
//Clase para representar los nodos del arbol de sufijos
class SuffixTreeNode {
public:
    std::unordered_map<char, SuffixTreeNode*> childrens; //Unordered map para asociar los nodos hijos al caracter con el que empieza su arista asociada
    SuffixTreeNode* suffixLink; //Enlace a otro nodo para recorrer mas rapido el arbol
    int start; // caracter de inicio de su arista padre (El numero es el indice en el texto)
    int* end; //Caracter final de la arista padre (El numero es el indice en el texto)
    int suffixIndex; //Indice que indica el indice en el que empieza el sufijo (Solo para las hojas)

    SuffixTreeNode(int start, int* end, SuffixTreeNode* link);
    int getLength();
    bool isLeaf();
};

class SuffixTree {
private:
    SuffixTreeNode* root; //Nodo raiz
    std::string text; //texto con la concatenación
    int* leafEnd; //puntero al end de los hojas

    void doTransversal(SuffixTreeNode* node, std::vector<int>& posiciones);
    void freeNodeIterative(SuffixTreeNode* root);
    bool walkDown(SuffixTreeNode* node);
    //Atributos del punto activo
    SuffixTreeNode* activeNode; 
    int activeEdge;
    int activeLength;
    int remainingSuffixCount;
    //referencia al ulitmo nodo interno creado para enlazar mediante suffixLink
    SuffixTreeNode* lastNewNode;

public:
    SuffixTree(std::string& text);
    ~SuffixTree();
    std::vector<int> search(const std::string& pattern);
};

#endif