#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include "Encryption.h"
#include "util_shm.h"

int main()
{
    bool *needGen = GetNeedGenPtr();

    while (true)
    {
        while (!(*needGen))
        {
            sleep(1);
        }

        SystemParam *systemParam = GetSystemParamPtr();
        PublicKey *publicKey = setUp(systemParam);
        Gk *gk = GetGk();
        ssk *sk = keyGen(gk, publicKey); //真正的私钥

        printf("start SendPublicKey\n");
        SendPublicKey(publicKey);
        printf("start SendSsk\n");
        SendSsk(sk);
        printf("send over\n\n\n");

        *needGen = false;
    }

    FreeNeedGen();
    FreePublicKey();
    FreeSsk();

    return 0;
}
