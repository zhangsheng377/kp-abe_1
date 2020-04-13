/*
 * pp.h
 *
 *  Created on: 2018骞?3鏈?14鏃?
 *      Author: root
 */
#include <vector>
#include "Node.h"
#include "clt13.h"
#include "gmp.h"

#ifndef PP_H_
#define PP_H_

class PublicKey
{
public:
    int top_level;
    int attrNumber;
    clt_pp_t *pp;
    clt_state_t *sk;
    //mpz_t MSK;                     // Master Key
    clt_elem_t *MSK; // Master Key
    struct MyMpz_t
    {
        mpz_t t;
    };
    //存h1 到hn,是属性需要的东西
    //std::vector<MyMpz_t> attributes;
    MyMpz_t attributes[100];
    clt_elem_t *encodingOfa; // gk的阿发
    PublicKey() {}
    PublicKey(int attrNumber)
    {
        this->attrNumber = attrNumber;
        //attributes.resize(attrNumber);
        encodingOfa = clt_elem_new();
        MSK = clt_elem_new();

        printf("PublicKey init****************************************\n");
    }
    ~PublicKey()
    {
        if (pp != NULL)
        {
            clt_pp_free(pp);
        }
        if (sk != NULL)
        {
            clt_state_free(sk);
        }
        if (encodingOfa != NULL)
        {
            clt_elem_free(encodingOfa);
        }
        if (MSK != NULL)
        {
            clt_elem_free(MSK);
        }
    }
    mpz_t *GetAttribute(int index)
    {
        return &(attributes[index].t);
    }
};
class CT
{
public:
    mpz_t CM;
    mpz_t gs;
    mpz_t *ci;
    int attrNumber;
    clt_elem_t *result;
    CT(int attrNumber)
    {
        this->attrNumber = attrNumber;
        result = clt_elem_new();
        ci = (mpz_t *)malloc(sizeof(mpz_t) * attrNumber);
    }

    ~CT()
    {

        if (CM != NULL)
        {
            mpz_clears(CM, NULL);
        }
        if (gs != NULL)
        {
            mpz_clears(gs, NULL);
        }
        if (ci != NULL)
        {
            free(ci);
        }
    }
};
class ssk
{
public:
    int nodeNumber;
    mpz_t kh;
    mpz_t skUnion[100]; //放每个输入，节点的kw
    int skStartIndex[100];
    ssk() {}
    ssk(int nodeNumber)
    {
        this->nodeNumber = nodeNumber;
    }
    ~ssk()
    {
        if (kh != NULL)
        {
            mpz_clears(kh, NULL);
        }
    }
};

#endif
