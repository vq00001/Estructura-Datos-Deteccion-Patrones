#include "suffix_tree.hpp"
#include <iostream>
#include <algorithm>
#include <stack>

/*
Segunda versión del suffix_tree implementado mediante algoritmo de ukkonen, en los primeros commits se intentó 
implementar mediante la explicación del concepto y tutoriales presentados por GeekForGeeks: https://www.geeksforgeeks.org/dsa/ukkonens-suffix-tree-construction-part-1/ (parte 1 a 6)
En esa implementación se buscaba abordar el codigo de otra manera pero bajo el mismo concepto usando las hojas en vez del remainSuffixCounter,
al final se implementa esta segunda versión mas tradicional y asemejada al codigo entregado en la parte 6 ya que despues
de algunas pruebas demostro ser algo mejor que la primera versión hecha
*/


//Constructor para los nodos del arbol de sufijos
SuffixTreeNode::SuffixTreeNode(int start, int* end, SuffixTreeNode* link)
    : start(start), end(end), suffixLink(link), suffixIndex(-1) {}

//Funcion para retornar el largo de la arista padre de un nodo.
int SuffixTreeNode::getLength() {
    return *end - start + 1;
}

//Funcion para corroborar que el nodo es una hoja.
bool SuffixTreeNode::isLeaf() {
    return childrens.empty();
}

//Constructor del arbol segun algoritmo de Ukkonen
//Algoritmo de ukkonen se basa en fases con extensiones, cada fase representa una nueva letra del texto
//y en cada extension de esa fase se expande todas las hojas actuales hasta la nueva (Pero puede dejar implicitas algunas)
//Se compone de 3 reglas: 1)Expandir todas las hojas ya existentes con el nuevo caracter la fase
//2)Crear nuevas hojas para los sufijos que no existan
//3)Si el sufijo ya esta implicitamente no hacer nada
//Luego de aplicar la regla 3 todas las expansiones siguientes tambien usaran la regla 3 asi que se puede romper el loop en el primero
//Para evitar muchos movimientos en el arbol se usa el SuffixLink de los nodos y walkDown, walkDown da saltos para bajar rapido
//y suffixLink son como "portales" que indican el siguiente nodo que deberia ser analizado en las expansiones.
SuffixTree::SuffixTree(std::string& texto) {
    //Concatenación de caracter especial
    this->text = texto + static_cast<char>(3);
    //Inicializa raiz y leafEnd para que todas las hojas tengan la misma referencia a un end y se pueda actualizar sin 
    //iterar por todas (Regla 1 del algoritmo de ukkonen)
    int* endRoot = new int(-1);
    root = new SuffixTreeNode(-1, endRoot, nullptr);
    root->suffixLink = root;
    leafEnd = new int(-1);

    //Inicialización del punto activo, representa el caracter apuntado de cada iteración (Expansión en el algoritmo de ukkonen)
    activeNode = root;
    activeEdge = -1;
    activeLength = 0;
    //RemainingSuffixCount representa la cantidad de sufijos que ya estan en el arbol pero implicitamente (Es como si hubieran sufijos que se hubieran quedado en trabajo pendiente)
    remainingSuffixCount = 0;
    lastNewNode = nullptr;

    int size = text.size();

    for (int i = 0; i < size; ++i) {
        //Suma de leafEnd (Regla 1 de Ukkonen)
        (*leafEnd)++;
        remainingSuffixCount++;
        lastNewNode = nullptr;

        while (remainingSuffixCount > 0) {
            //Obtención del caracter apuntado
            if (activeLength == 0) activeEdge = i;
            char currentChar = text[activeEdge];

            //Regla 2 De ukkonen cuando el hijo NO existe (O sea no hay ninguna arista hija que empiece por el caracter apuntado) . Crea una hoja desde 0 con una arista asociada a la letra
            if (activeNode->childrens.count(currentChar) == 0) {
                SuffixTreeNode* newLeaf = new SuffixTreeNode(i, leafEnd, root);
                newLeaf->suffixIndex = i - remainingSuffixCount + 1;
                activeNode->childrens[currentChar] = newLeaf;

                if (lastNewNode != nullptr) {
                    lastNewNode->suffixLink = activeNode;
                    lastNewNode = nullptr;
                }
            } else {
                //En caso de si existir el hijo avanzar por los nodos segun el punto activo
                //Se dan saltos rapidamente entre nodos con walkDown 
                SuffixTreeNode* next = activeNode->childrens[currentChar];
                if (walkDown(next)) continue;

                //Si se da cuenta que el sufijo buscado ya está previamente 
                //se aplica la regla 3 De ukkonen (Todas las expansiones pendientes están implicitas)
                if (text[next->start + activeLength] == text[i]) {
                    if (lastNewNode != nullptr && activeNode != root) {
                        lastNewNode->suffixLink = activeNode;
                        lastNewNode = nullptr;
                    }
                    activeLength++;
                    break;
                }

                //Si no está implicito pero si está parcialmente el sufijo es necesario crear un nodo intermedio
                //Se crea una nueva hoja para el sufijo actual y la hoja antigua se conecta al nodo intermedio
                //Regla 2 de ukkonen con nodos intermedios
                int* splitEnd = new int(next->start + activeLength - 1);
                SuffixTreeNode* split = new SuffixTreeNode(next->start, splitEnd, root);
                activeNode->childrens[currentChar] = split;

                SuffixTreeNode* newLeaf = new SuffixTreeNode(i, leafEnd, root);
                newLeaf->suffixIndex = i - remainingSuffixCount + 1;
                split->childrens[text[i]] = newLeaf;

                next->start += activeLength;
                split->childrens[text[next->start]] = next;

                //Se guarda el nodo intermedio como el ultimo nodo nuevo para enlazarlo mas tarde mediante suffixLink
                if (lastNewNode != nullptr) lastNewNode->suffixLink = split;
                lastNewNode = split;
            }
            //Si llega hasta aca es porque se creo una nueva hoja (Otro sufijo explicito) asi que se reduce el SuffixCount
            remainingSuffixCount--;

            //Actualización del punto activo segun lo que establece el algoritmo 
            if (activeNode == root && activeLength > 0) {
                activeLength--;
                activeEdge = i - remainingSuffixCount + 1;
            } else if (activeNode != root) {
                activeNode = activeNode->suffixLink;
            }
        }
    }
}

//Función para saltar nodos rapidamente sin tener que recorrer sus aristas.
bool SuffixTree::walkDown(SuffixTreeNode* node) {
    int edgeLength = node->getLength();
    if (activeLength >= edgeLength) {
        activeEdge += edgeLength;
        activeLength -= edgeLength;
        activeNode = node;
        return true;
    }
    return false;
}

//Función para la busqueda del arbol,de forma recursiva busca las hojas y al llegar introduce el indice del sufijo asociado
void SuffixTree::doTransversal(SuffixTreeNode* node, std::vector<int>& posiciones) {
    if (node->isLeaf()) {
        posiciones.push_back(node->suffixIndex);
        return;
    }
    for (auto& [_, child] : node->childrens) {
        doTransversal(child, posiciones);
    }
}

//Función de busqueda con el arbol, baja por los nodos coincidentes hasta que el patrón concuerde, de ser así accede a 
//todas las hojas a las que se pueda acceder desde el ulitmo nodo que concordo con el patrón encontrado y coloca el indice
//en el vector de posiciones a retornar
std::vector<int> SuffixTree::search(const std::string& pattern) {
    std::vector<int> posiciones;
    SuffixTreeNode* currentNode = root;
    int edgeLength, j = 0;

    while (j < pattern.size()) {
        if (currentNode->childrens.count(pattern[j]) == 0) return posiciones;
        SuffixTreeNode* next = currentNode->childrens[pattern[j]];
        edgeLength = next->getLength();

        for (int k = 0; k < edgeLength && j < pattern.size(); ++k, ++j) {
            if (text[next->start + k] != pattern[j]) return posiciones;
        }
        currentNode = next;
    }
    doTransversal(currentNode, posiciones);
    return posiciones;
}

//Funcion para liberar los nodos de manera iterativa con stack para evitar desborde de pila.
void SuffixTree::freeNode(SuffixTreeNode* start) {
    std::stack<SuffixTreeNode*> pila;
    pila.push(start);

    while (!pila.empty()) {
        SuffixTreeNode* node = pila.top();
        pila.pop();

        for (auto& [_, child] : node->childrens) {
            pila.push(child);
        }

        if (node->end != leafEnd) delete node->end;
        delete node;
    }
}

SuffixTree::~SuffixTree() {
    freeNode(root);
    delete leafEnd;
}
