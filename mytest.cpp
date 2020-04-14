#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include "Encryption.h"

int main()
{
    SystemParam systemParam = {
        .serParam = 10,
        .attrNumber = 5,
        .attr = {1, 1, 0, 0, 0},
    };

    Tree *tree = buildTree();

    PublicKey *publicKey = setUp(&systemParam);

    ssk *sk = keyGen(tree, publicKey); //真正的私钥

    int message;
    if (!inputMessage(message))
    {
        return 0;
    }

    tree = buildTree();

    CT *ct = encrypt(publicKey, &systemParam, message);

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
