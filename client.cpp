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

    Tree *tree = buildTree();

    *needGen = true;

    while (*needGen)
    {
        sleep(1);
    }

    PublicKey *publicKey = GetPublicKey();
    gmp_printf("The public gk^a=");
    clt_elem_print(publicKey->encodingOfa);
    gmp_printf("\n");
    for (int i = 0; i < attrNumber; i++)
    {
        gmp_printf("The public h%d=%Zd\n", i,
                   publicKey->GetAttribute(i));
    }
    printf("*********GetPublicKey() "
           "Complete********************************************\n");

    ssk *sk = GetSsk();
    //输出每个节点存储的关键密钥
    for (int i = 0; i < 7; i++)
    {
        gmp_printf("skUnion[%d]=%Zd\n", i, sk->skUnion[i]);
    }
    printf("*********GetSsk() "
           "Complete********************************************\n");

    int message;
    if (!inputMessage(message))
    {
        return 0;
    }

    CT *ct = encrypt(publicKey, encattr, message);

    if (!transform(tree, sk, ct, publicKey))
    {
        printf("\nencattr is wrong. Can not decrypt the Message!!\n");
    }

    int decryptMessage = 0;
    if (decrypt(decryptMessage, sk, ct, publicKey))
    {
        printf("\nThe Message is %d\n", decryptMessage);
    }
    else
    {
        printf("\nCan not decrypt the Message!!\n");
    }

    return 0;
}
