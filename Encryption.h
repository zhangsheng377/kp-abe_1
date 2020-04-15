#include "Node.h"
#include "Gk.h"
#include "pp.h"

PublicKey *GlobalSetup(SystemParam *systemParam);

CT *ClientEncryption(PublicKey *publicKey, SystemParam *systemParam, int message);

ssk *GlobalKeyGen(Gk *gk, PublicKey *publicKey);

bool ClientDecryption(int &decryptMessage, Gk *gk, ssk *ssk, CT *ct, PublicKey *publicKey);

Gk *GetGk();
