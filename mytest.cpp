#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include "Encryption.h"

int main()
{
    const int attrNumber = 5; // 系统属性个数
    const int serparam = 10;  // 安全参数
    const int depth = 3;      // 电路最大深度

    // can decrypt   系统属性个数==用户的属性个数
    // int encattr[5] = { 1, 0, 0, 0, 0};	//can not decrypt
    int encattr[attrNumber] = {1, 1, 0, 0, 0};

    Tree *tree = buildTree();

    PublicKey *publicKey = setUp(serparam, attrNumber, depth);

    ssk *sk = keyGen(tree, publicKey); //真正的私钥

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
