/*
 * Gk.h
 *
 *  Created on: 2018年3月14日
 *      Author: root
 */
#include <vector>
#include "Node.h"

#ifndef GK_H_
#define GK_H_

struct Gk
{
public:
    int nodeNumb;
    //Node nodes[100];
    std::vector<Node> nodes;
    Gk(int nodeNumb)
    {
        this->nodeNumb = nodeNumb;
        nodes.resize(nodeNumb);
    }
    Node* get(){
        return &(nodes[0]);
    }
};

#endif /* GK_H_ */
