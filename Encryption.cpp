//============================================================================
// Name        : Encryption.cpp
// Author      :
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include "Node.h"
#include "Tree.h"
#include "pp.h"
#include <aesrand.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/resource.h>
#include <sys/stat.h>
#include <time.h>
#include <unistd.h>
#include <clt13.h> //����pbc��
#include <gmp.h>

PublicKey *setUp(int serParam, int attrNumber, int denth)
{
    // Group //denth:Ⱥ��ĸ���.���Ĳ�����һ initalize
    // serparam����ȫ����:�ˣ���ʼ��/����Ҫ�Ķ���,attrNumber:ϵͳ���Ը���.denth:��·������
    ulong default_flags = CLT_FLAG_NONE | CLT_FLAG_VERBOSE;
    int begin, end;  //���忪ʼ�ͽ�����־λ
    begin = clock(); //��ʼ��ʱ
    int kappa = denth;
    int lambda = 10;
    int pows[kappa], top_level[kappa]; // use for Mulitilinear Map
    clt_state_t *sk;                   // Mulitilinear Map sercert key
    clt_pp_t *pp;                      // Mulitilinear Map Public key
    aes_randstate_t rng;
    PublicKey *publicKey = new PublicKey(attrNumber);

    aes_randinit(rng);
    for (int k = 0; k < kappa; k++)
    {
        top_level[k] = kappa;
    }

    clt_params_t params = {
        .lambda = (size_t)lambda,
        .kappa = (size_t)kappa,
        .nzs = (size_t)kappa,
        .pows = top_level,
    };
    sk = clt_state_new(&params, NULL, 0, default_flags, rng);
    pp = clt_pp_new(sk);

    srand((unsigned)time(NULL));

    // initalie element  �� ��ʼ��gk������pk->encodingOfa��
    // ��ʼ��h1��hn��pk->attribute[i]��
    for (int i = 0; i < attrNumber; i++)
    {
        mpz_init(publicKey->attribute[i]);
    }
    // random master key a; //������ɰ���������Կ�ǰ���/gk-1�������������ǰ���
    mpz_init_set_ui(publicKey->MSK, (rand() % 100) + 1);
    clt_encode(publicKey->encodingOfa, sk, 1, &publicKey->MSK, top_level); //����gk�İ�����
    gmp_printf("The public gk^a=");                                        //���һ��gk^a
    clt_elem_print(publicKey->encodingOfa);
    gmp_printf("\n");

    for (int k = 0; k < kappa; k++)
    {
        pows[k] = 1;
    }
    mpz_t temp;
    mpz_init(temp);
    for (int i = 0; i < attrNumber; i++)
    {
        mpz_set_ui(temp, (rand() % 100) + 1);
        clt_encode((clt_elem_t *)publicKey->attribute[i], sk, 1, &temp, pows);
        gmp_printf("The public h%d=%Zd\n", i,
                   publicKey->attribute[i]); // ��������������h1......hn
    }
    aes_randclear(rng);
    mpz_clear(temp);

    publicKey->attrNumber = attrNumber;
    publicKey->pp = pp;
    publicKey->sk = sk;
    publicKey->top_level = kappa;
    end = clock();                             //������ʱ
    printf("setup time is %d\n", end - begin); //��Ϊʱ�䣬��λ����
    printf("*********setUp() "
           "Complete!!!***************************************************\n");

    return publicKey;
}

CT *encrypt(PublicKey *publicKey, int *att, int message)
{
    CT *ct = new CT(publicKey->attrNumber);

    // initalize elements
    for (int k = 0; k < publicKey->attrNumber; k++)
    {
        mpz_inits(ct->ci[k], NULL);
    }
    int pows_one[publicKey->top_level];
    for (int i = 0; i < publicKey->top_level; i++)
    {
        pows_one[i] = 1;
    }

    mpz_t result, temp1;
    mpz_inits(result, temp1, ct->gs, ct->CM, NULL);
    int s = (rand() % 100) + 1; //���������s ����Zp
    mpz_set_ui(temp1, s);
    clt_encode((clt_elem_t *)ct->gs, publicKey->sk, 1, &temp1, pows_one); // g^s
    clt_elem_mul_ui((clt_elem_t *)result, publicKey->pp, publicKey->encodingOfa,
                    s); //(gk����)s�η�
    mpz_t codeM, mmm;
    mpz_inits(codeM, NULL);
    mpz_init_set_ui(mmm, message);
    int top_level[publicKey->top_level];
    for (int i = 0; i < publicKey->top_level; i++)
    {
        top_level[i] = publicKey->top_level;
    }
    clt_encode((clt_elem_t *)codeM, publicKey->sk, 1, &mmm,
               top_level); //���������M
    clt_elem_add((clt_elem_t *)ct->CM, publicKey->pp, (clt_elem_t *)result,
                 (clt_elem_t *)codeM); // message*(gk����)s�η�
                                       //������ѭ�����ǵõ�V i����s��Ci=hi��s��
    for (int i = 0; i < publicKey->attrNumber; i++)
    {
        if (att[i] == 1)
        {
            clt_elem_mul_ui((clt_elem_t *)ct->ci[i], publicKey->pp,
                            (clt_elem_t *)publicKey->attribute[i],
                            s); // h1��hn��s�����ŵ�ci[]����
        }
    }
    //��������������Ǽ��ܵ�����ȫ�����ݣ�cm���ģ�g��s�����Լ�c[i]=hi��s��
    gmp_printf("Cm=%Zd\n", ct->CM);
    gmp_printf("gs=%Zd\n", ct->gs);
    for (int i = 0; i < publicKey->attrNumber; i++)
    {
        gmp_printf("c[%d]=%Zd\n", i, ct->ci[i]);
    }

    ct->attrNumber = publicKey->attrNumber;

    printf("*********Encrypt() "
           "Complete********************************************\n");

    return ct;
}

int depth(Node *curr)
{
    int dep = 1;
    while (curr->leftsons != NULL)
    {
        dep++;
        curr = curr->leftsons;
    }
    return dep;
}

ssk *keyGen(Tree *tree, PublicKey *publicKey)
{
    ssk *gssk = new ssk(tree->nodeNumb);
    int nodenumber = tree->nodeNumb;

    for (int i = 0; i < gssk->nodeNumber * 4; i++)
    {
        mpz_init(
            gssk->skUnion[i]); //��ʼ���洢����ڵ㣬���ţ����ŵ� �ؼ����kw������
    }
    long rs[nodenumber];
    for (int i = 0; i < nodenumber; i++)
    {
        long s = (rand() % 10) + 1;
        rs[i] = s; // random value of every node �������r1....rn+q
    }
    //������������r1....rn+q
    /*for (int i = 0; i <nodenumber; i++) {
          gmp_printf("rs[%d]=%Zd\n", i, rs[i]);
  }*/
    int pows[publicKey->top_level];
    for (int k = 0; k < publicKey->top_level; k++)
    {
        pows[k] = publicKey->top_level - 1;
    }
    mpz_t rnq, temp1, temp2;
    mpz_inits(rnq, gssk->kh, temp1, temp2, NULL);
    mpz_set_ui(rnq, rs[0]);
    clt_encode((clt_elem_t *)temp1, publicKey->sk, 1, &rnq,
               pows);                                                         // temp1=gk-1^rnq
    clt_encode((clt_elem_t *)temp2, publicKey->sk, 1, &publicKey->MSK, pows); // temp2=gk-1^a
    clt_elem_sub((clt_elem_t *)gssk->kh, publicKey->pp, (clt_elem_t *)temp2,
                 (clt_elem_t *)temp1); // Kh=temp1/temp2    �õ�ͷ����ԿKh
    mpz_clears(rnq, temp1, temp2, NULL);

    Node *quence[nodenumber]; // The container visit the tree
    quence[0] = tree->root;
    int index = 0; // the index of the visited node
    int had = 1;
    Node *pz = NULL;
    int powsOne[publicKey->top_level];
    int powsdepth[publicKey->top_level];
    for (int i = 0; i < publicKey->top_level; i++)
    {
        powsOne[i] = 1;
    }
    mpz_t aw;
    mpz_init(aw);
    mpz_t bw;
    mpz_init(bw);
    int storeplace = 0;
    do
    {
        pz = quence[index++];

        int awint = (rand() % 1000) + 1;
        int bwint = (rand() % 1000) + 1;

        mpz_set_ui(aw, awint);
        mpz_set_ui(bw, bwint);
        int dep = depth(pz);
        for (int i = 0; i < publicKey->top_level; i++)
        {
            powsdepth[i] = dep;
        }
        int place = pz->index - 1; // root->index=1;
        long kaz = rs[place];
        mpz_t rw;
        mpz_init_set_ui(rw, kaz);
        if (pz->Nodetype == 1)
        { // and ���������ؼ����
            gssk->skStartIndex[place] =
                storeplace; // describe the sercert key element start place
                            // �浱ǰ�ڵ�λ�ã�ÿ��һ���ؼ������skUnition,storeplace++
            clt_encode((clt_elem_t *)gssk->skUnion[storeplace++], publicKey->sk, 1,
                       &aw, powsOne); // g^aw  ���һ
            clt_encode((clt_elem_t *)gssk->skUnion[storeplace++], publicKey->sk, 1,
                       &bw, powsOne); // g^bw  �����

            mpz_t codeaw, codebw, coderw;
            mpz_inits(codeaw, codebw, coderw, NULL);
            clt_encode((clt_elem_t *)codeaw, publicKey->sk, 1, &aw,
                       powsdepth); // gj^aw
            clt_encode((clt_elem_t *)codebw, publicKey->sk, 1, &bw,
                       powsdepth); // gj^bw
            clt_encode((clt_elem_t *)coderw, publicKey->sk, 1, &rw,
                       powsdepth); // gj^rw
            clt_elem_mul_ui((clt_elem_t *)codeaw, publicKey->pp, (clt_elem_t *)codeaw,
                            rs[pz->leftsons->index - 1]); // gj^(aw*r1)
            clt_elem_mul_ui((clt_elem_t *)codebw, publicKey->pp, (clt_elem_t *)codebw,
                            rs[pz->rightson->index - 1]);
            clt_elem_sub((clt_elem_t *)coderw, publicKey->pp, (clt_elem_t *)coderw,
                         (clt_elem_t *)codeaw); // gj^(bw*r2)
            clt_elem_sub((clt_elem_t *)gssk->skUnion[storeplace++], publicKey->pp,
                         (clt_elem_t *)coderw,
                         (clt_elem_t *)codebw); // gj^(rw-aw*r1-bw*r2) �����
            mpz_clears(codeaw, codebw, coderw, NULL);
        }
        if (pz->Nodetype == 2)
        { // or  �����ĸ��ؼ����
            gssk->skStartIndex[place] = storeplace;
            clt_encode((clt_elem_t *)gssk->skUnion[storeplace++], publicKey->sk, 1,
                       &aw, powsOne); // g^aw  ���һ
            clt_encode((clt_elem_t *)gssk->skUnion[storeplace++], publicKey->sk, 1,
                       &bw, powsOne); // g^bw  �����
            mpz_t codeaw, codebw, coderw;
            mpz_inits(codeaw, codebw, coderw, NULL);
            clt_encode((clt_elem_t *)codeaw, publicKey->sk, 1, &aw,
                       powsdepth); // gj^aw
            clt_encode((clt_elem_t *)codebw, publicKey->sk, 1, &bw,
                       powsdepth); // gj^bw
            clt_encode((clt_elem_t *)coderw, publicKey->sk, 1, &rw,
                       powsdepth); // gj^rw
            clt_elem_mul_ui((clt_elem_t *)codeaw, publicKey->pp, (clt_elem_t *)codeaw,
                            rs[pz->leftsons->index - 1]); // gj^(aw*r1)
            clt_elem_mul_ui((clt_elem_t *)codebw, publicKey->pp, (clt_elem_t *)codebw,
                            rs[pz->rightson->index - 1]); // gj^(bw*r2)
            clt_elem_sub((clt_elem_t *)gssk->skUnion[storeplace++], publicKey->pp,
                         (clt_elem_t *)coderw,
                         (clt_elem_t *)codeaw); // gj^(rw-aw*r1) �����
            clt_elem_sub((clt_elem_t *)gssk->skUnion[storeplace++], publicKey->pp,
                         (clt_elem_t *)coderw,
                         (clt_elem_t *)codebw); // gj^(rw-bw*r2) �����
            mpz_clears(codeaw, codebw, coderw, NULL);
        }
        if (pz->Nodetype >= 3)
        { // attr  ���뵼��2���ؼ����
            gssk->skStartIndex[place] = storeplace;
            int attributeindex = pz->Nodetype - 3;
            int randzw = (rand() % 100) + 1;
            mpz_t temp, temp2, temp3;
            mpz_inits(temp, temp2, NULL);
            mpz_init_set_ui(temp3, randzw);

            clt_elem_mul_ui((clt_elem_t *)temp, publicKey->pp,
                            (clt_elem_t *)publicKey->attribute[attributeindex],
                            randzw); // temp=hw^zw
            clt_encode((clt_elem_t *)temp2, publicKey->sk, 1, &rw,
                       powsOne); // temp2=g^rw
            clt_elem_add((clt_elem_t *)gssk->skUnion[storeplace++], publicKey->pp,
                         (clt_elem_t *)temp2,
                         (clt_elem_t *)temp); // g^(rw)*hw^zw  ���һ
            clt_encode((clt_elem_t *)gssk->skUnion[storeplace++], publicKey->sk, 1,
                       &temp3, powsOne); // g^(-zw)    �����
        }
        if (pz->leftsons != NULL)
        {
            quence[had++] = pz->leftsons;
        }
        if (pz->rightson != NULL)
        {
            quence[had++] = pz->rightson;
        }
    } while (index != had);
    //���ÿ���ڵ�洢�Ĺؼ���Կ
    for (int i = 0; i < storeplace; i++)
    {
        gmp_printf("skUnion[%d]=%Zd\n", i, gssk->skUnion[i]);
    }
    printf("*********KeyGen() "
           "Complete********************************************\n");

    return gssk;
}

int evaluate(mpz_t ele, Node *p, ssk *ssk, CT *ct,
             PublicKey *publicKey)
{
    if (p->Nodetype >= 3)
    { //���뵼��
        int attrIndex = p->Nodetype - 3;
        int isZero = mpz_cmp_d(ct->ci[attrIndex], 0); //������������
        if (isZero != 0)
        {
            int index = p->index - 1;
            int skStartIndex = ssk->skStartIndex[index];
            mpz_t kw1, kw2;
            mpz_inits(kw1, kw2, NULL);
            clt_elem_mul((clt_elem_t *)kw1, publicKey->pp,
                         (clt_elem_t *)ssk->skUnion[skStartIndex++],
                         (clt_elem_t *)ct->gs); // e(k(w,1),g^s)
            clt_elem_mul((clt_elem_t *)kw2, publicKey->pp,
                         (clt_elem_t *)ssk->skUnion[skStartIndex],
                         (clt_elem_t *)ct->ci[attrIndex]); // e(k(w,2),hw^s)
            clt_elem_sub((clt_elem_t *)ele, publicKey->pp, (clt_elem_t *)kw1,
                         (clt_elem_t *)kw2); // ele=e(k(w,1),g^s)*e(k(w,2),hw^s)
            return 1;
        }
        else
        {
            return 0;
        }
    }
    if (p->Nodetype == 1)
    { // and

        int index = p->index - 1;
        int skStartIndex = ssk->skStartIndex[index];
        mpz_t kw1, kw2, kw3, aw, bw, temp;
        mpz_inits(kw1, kw2, kw3, aw, bw, temp, NULL);
        int validleft = evaluate(aw, p->leftsons, ssk, ct, publicKey);
        int validright = evaluate(bw, p->rightson, ssk, ct, publicKey);

        if (validleft == 1 && validright == 1)
        {
            clt_elem_mul((clt_elem_t *)kw1, publicKey->pp,
                         (clt_elem_t *)ssk->skUnion[skStartIndex++],
                         (clt_elem_t *)aw); // e(k(w,1),Eaw)
            clt_elem_mul((clt_elem_t *)kw2, publicKey->pp,
                         (clt_elem_t *)ssk->skUnion[skStartIndex++],
                         (clt_elem_t *)bw); // e(k(w,2),Ebw)
            clt_elem_mul((clt_elem_t *)kw3, publicKey->pp,
                         (clt_elem_t *)ssk->skUnion[skStartIndex],
                         (clt_elem_t *)ct->gs); // e(k(w,3),g^s)
            clt_elem_add((clt_elem_t *)temp, publicKey->pp, (clt_elem_t *)kw1,
                         (clt_elem_t *)kw2);
            clt_elem_add(
                (clt_elem_t *)ele, publicKey->pp, (clt_elem_t *)temp,
                (clt_elem_t *)
                    kw3); // ele=e(k(w,1),Eaw)*e(k(w,2),Ebw)*e(k(w,3),g^s)  ���õ�Ew
            return 1;
        }
        else
        {
            return 0;
        }
    }
    if (p->Nodetype == 2)
    { // or
        int index = p->index - 1;
        int skStartIndex = ssk->skStartIndex[index];
        mpz_t kw1, kw2, aw, bw;
        mpz_inits(kw1, kw2, aw, bw, NULL);
        int validleft = evaluate(aw, p->leftsons, ssk, ct, publicKey);
        if (validleft == 1)
        {
            clt_elem_mul((clt_elem_t *)kw1, publicKey->pp,
                         (clt_elem_t *)ssk->skUnion[skStartIndex],
                         (clt_elem_t *)aw); // e(k(w,1),Eaw)
            clt_elem_mul((clt_elem_t *)kw2, publicKey->pp,
                         (clt_elem_t *)ssk->skUnion[skStartIndex + 2],
                         (clt_elem_t *)ct->gs); // e(k(w,3),g^s)
            clt_elem_add((clt_elem_t *)ele, publicKey->pp, (clt_elem_t *)kw1,
                         (clt_elem_t *)kw2); // e(k(w,1),Eaw)*e(k(w,3),g^s)
            return 1;
        }
        int validright = evaluate(bw, p->rightson, ssk, ct, publicKey);
        if (validright == 1)
        {
            clt_elem_mul((clt_elem_t *)kw1, publicKey->pp,
                         (clt_elem_t *)ssk->skUnion[skStartIndex + 1],
                         (clt_elem_t *)bw); // e(k(w,2),Ebw)
            clt_elem_mul((clt_elem_t *)kw2, publicKey->pp,
                         (clt_elem_t *)ssk->skUnion[skStartIndex + 3],
                         (clt_elem_t *)ct->gs); // e(k(w,4),g^s)
            clt_elem_add((clt_elem_t *)ele, publicKey->pp, (clt_elem_t *)kw1,
                         (clt_elem_t *)kw2); // e(k(w,2),Ebw)*e(k(w,4),g^s)
            return 1;
        }
        return 0;
    }
}

bool transform(Tree *tree, ssk *ssk, CT *ct, PublicKey *publicKey)
{
    mpz_t E, rnqs;

    mpz_inits(E, rnqs, NULL);

    int aa = evaluate(rnqs, tree->root, ssk, ct, publicKey); // rnqs=gk^rnqs
    if (aa == 0)
    {
        return false;
    }

    clt_elem_mul((clt_elem_t *)E, publicKey->pp, (clt_elem_t *)ssk->kh, (clt_elem_t *)ct->gs); // e'=e(KH,g^s)
    clt_elem_add((clt_elem_t *)E, publicKey->pp, (clt_elem_t *)E, (clt_elem_t *)rnqs);         // E=e(KH,g^s)*(gk^rnqs)=gk^(as-rnqs)*gk^rnqs=gk^as
    clt_elem_sub(ct->result, publicKey->pp, (clt_elem_t *)ct->CM, (clt_elem_t *)E);            // result=CM/E=CM/gk^as

    printf("CT*=");
    clt_elem_print(ct->result);
    printf("\n");

    printf("*********Transform() "
           "Complete********************************************\n");

    return true;
}

bool decrypt(int &decryptMessage, ssk *ssk, CT *ct, PublicKey *publicKey)
{
    int top_level[publicKey->top_level]; // top_level=3
    mpz_t ten, codeTen;
    int testNum = 0;
    //const int tryNum = 500;

    mpz_inits(codeTen, ten, NULL);
    for (int i = 0; i < publicKey->top_level; i++)
    {
        top_level[i] = publicKey->top_level;
    }

    do
    {
        mpz_set_ui(ten, testNum);
        clt_encode((clt_elem_t *)codeTen, publicKey->sk, 1, &ten,
                   top_level); // g^0
        clt_elem_sub((clt_elem_t *)codeTen, publicKey->pp, (clt_elem_t *)codeTen,
                     ct->result); // g^0/result
        if (clt_is_zero((clt_elem_t *)codeTen, publicKey->pp))
        {
            decryptMessage = testNum;

            gmp_printf("codeTen=%Zd\n", codeTen);

            printf("*********Decrypt() "
                   "Complete********************************************\n");

            return true;
        }
        else
        {
            testNum++;
        }
        //} while (testNum < tryNum);
    } while (true);

    return false;
}

Tree *buildTree()
{
    Node *a0 = new Node();
    Node *a1 = new Node();
    Node *root = new Node();
    a0->setType(3); // attribute
    a0->index = 2;
    a1->setType(4); // attribute
    a1->index = 3;
    root->setType(1); // and

    //	root->setType(2);//or

    root->index = 1;
    root->setleftsons(a0);
    root->setrightson(a1);
    a0->setParent(root);
    a1->setParent(root);

    Tree *tree = new Tree();
    tree->root = root;
    tree->nodeNumb = 3;

    return tree;
}

bool inputMessage(int &message)
{
    printf("please input message:");
    if (scanf("%d", &message) <= 0)
    {
        printf("You did not enter any number.\n");
        return false;
    }
    return true;
}
