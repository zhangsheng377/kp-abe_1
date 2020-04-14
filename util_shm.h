#include <sys/shm.h>
#include "Node.h"
#include "Tree.h"
#include "pp.h"

#ifndef UTIL_SHM_H_
#define UTIL_SHM_H_

static const key_t KEY_NEEDGEN = 1234;
bool *GetNeedGenPtr();
void FreeNeedGen();

static const key_t KEY_SYSTEMPARAM = 1238;
SystemParam *GetSystemParamPtr();
void SendSystemParam(const SystemParam *systemParam);
void FreeSystemParam();

static const key_t KEY_TREE = 12351;
Tree *GetTreePtr(int nodeNumb);
void SendTree(const Tree *tree);
void FreeTree();

static const key_t KEY_PUBLICKEY = 12362;
PublicKey *GetPublicKey();
void SendPublicKey(const PublicKey *publicKey);
void FreePublicKey();

static const key_t KEY_SSK = 12371;
ssk *GetSsk();
void SendSsk(const ssk *sk);
void FreeSsk();

#endif /* UTIL_SHM_H_ */
