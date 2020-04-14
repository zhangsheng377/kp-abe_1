/*
 * Node.h
 *
 *  Created on: 2018年3月14日
 *      Author: root
 */

#ifndef NODE_H_
#define NODE_H_

#include <gmp.h>

struct Node
{
public:
    int index;
    Node *parent;
    Node *leftsons;
    Node *rightson;
    int Nodetype;
public:
    void setParent(Node *parented) { parent = parented; }
    Node *getParent() { return this->parent; }
    void setleftsons(Node *lson) { this->leftsons = lson; }
    void setrightson(Node *rson) { this->rightson = rson; }

    void setType(int type) { Nodetype = type; }
    int getType() { return this->Nodetype; }

    virtual ~Node() {}

    Node *getleft() { return this->leftsons; }
    Node *getright() { return this->rightson; }
};
#endif
