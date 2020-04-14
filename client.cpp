#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include "Encryption.h"
#include "util_shm.h"

int main()
{
    SystemParam systemParam = {
        .serParam = 10,
        .attrNumber = 5,
        .attr = {1, 1, 0, 0, 0},
    };

    printf("Send systemParam start:\n");
    SendSystemParam(&systemParam);
    printf("Send systemParam over\n\n");

    printf("Send request start:\n");
    bool *needGen = GetNeedGenPtr();
    *needGen = true;
    while (*needGen)
    {
        sleep(1);
    }
    printf("Send request over\n\n");

    printf("GetPublicKey start:\n");
    PublicKey *publicKey = GetPublicKey();
    Gk *gk = GetGk();
    gmp_printf("The publicKey encodingOfa=");
    clt_elem_print(publicKey->encodingOfa);
    gmp_printf("\n");
    for (int i = 0; i < systemParam.attrNumber; i++)
    {
        gmp_printf("The publicKey attribute[%d]=%Zd\n", i, publicKey->GetAttribute(i));
    }
    printf("GetPublicKey over\n\n");

    printf("GetSsk start:\n");
    ssk *sk = GetSsk();
    //输出每个节点存储的关键密钥
    for (int i = 0; i < 7; i++)
    {
        gmp_printf("The sk skUnion[%d]=%Zd\n", i, sk->skUnion[i]);
    }
    printf("GetSsk over\n\n");

    int message;
    if (!inputMessage(message))
    {
        return 0;
    }

    CT *ct = encrypt(publicKey, &systemParam, message);

    int decryptMessage = 0;
    if (decrypt(decryptMessage, gk, sk, ct, publicKey))
    {
        printf("\nThe Message is %d\n", decryptMessage);
    }
    else
    {
        printf("\nCan not decrypt the Message!!\n");
    }

    return 0;
}
