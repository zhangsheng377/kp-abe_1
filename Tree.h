/*
 * Tree.h
 *
 *  Created on: 2018年3月14日
 *      Author: root
 */
#include <vector>
#include "Node.h"

#ifndef TREE_H_
#define TREE_H_

struct Tree
{
public:
    int nodeNumb;
    //Node nodes[100];
    std::vector<Node> nodes;
    Tree(int nodeNumb)
    {
        this->nodeNumb = nodeNumb;
        nodes.resize(nodeNumb);
    }
    Node* getRoot(){
        return &(nodes[0]);
    }
};

#endif /* TREE_H_ */
