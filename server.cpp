#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include "Encryption.h"
#include "util_shm.h"

int main()
{
    int shmid;                //共享内存标识符
    const int attrNumber = 5; // 系统属性个数
    const int serparam = 10;  // 安全参数
    const int depth = 3;      // 电路最大深度

    // can decrypt   系统属性个数==用户的属性个数
    // int encattr[5] = { 1, 0, 0, 0, 0};	//can not decrypt
    int encattr[attrNumber] = {1, 1, 0, 0, 0};

    bool *needGen = GetNeedGenPtr();

    while (true)
    {
        /*while (!(*needGen))
        {
            sleep(1);
        }*/

        Tree *tree = buildTree();
        PublicKey *publicKey = setUp(serparam, attrNumber, depth);
        ssk *sk = keyGen(tree, publicKey); //真正的私钥

        //printf("start SendTree\n");
        //SendTree(tree);
        printf("start SendPublicKey\n");
        SendPublicKey(publicKey);
        printf("start SendSsk\n");
        SendSsk(sk);
        printf("send over\n");

        *needGen = false;

        break;
    }

    FreeNeedGen();
    //FreeTree();
    FreePublicKey();
    FreeSsk();

    return 0;
}
