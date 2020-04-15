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

    Gk *gk = GetGk();

    PublicKey *publicKey = GlobalSetup(&systemParam);

    ssk *sk = GlobalKeyGen(gk, publicKey); //真正的私钥

    int message;
    printf("please input message:");
    if (scanf("%d", &message) <= 0)
    {
        printf("You did not enter any number.\n");
        return 0;
    }

    gk = GetGk();

    CT *ct = ClientEncryption(publicKey, &systemParam, message);

    int decryptMessage = 0;
    if (ClientDecryption(decryptMessage, gk, sk, ct, publicKey))
    {
        printf("\nThe Message is %d\n", decryptMessage);
    }
    else
    {
        printf("\nCan not decrypt the Message!!\n");
    }

    return 0;
}
