#include <sys/shm.h>
#include "Node.h"
#include "Tree.h"
#include "pp.h"

static const key_t KEY_NEEDGEN = 1234;
bool *GetNeedGenPtr();
void FreeNeedGen();

static const key_t KEY_TREE = 1235;
Tree *GetTree();
void SendTree(const Tree *tree);
void FreeTree();

static const key_t KEY_PUBLICKEY = 1236;
PublicKey *GetPublicKey();
void SendPublicKey(const PublicKey *publicKey);
void FreePublicKey();

static const key_t KEY_SSK = 1237;
ssk *GetSsk();
void SendSsk(const ssk *sk);
void FreeSsk();
