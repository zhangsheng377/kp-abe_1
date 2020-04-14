#include "Node.h"
#include "Gk.h"
#include "pp.h"

PublicKey *setUp(SystemParam *systemParam);

CT *encrypt(PublicKey *publicKey, SystemParam *systemParam, int message);

ssk *keyGen(Gk *gk, PublicKey *publicKey);

bool decrypt(int &decryptMessage, Gk *gk, ssk *ssk, CT *ct, PublicKey *publicKey);

Gk *GetGk();

bool inputMessage(int &message);
