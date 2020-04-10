#include "Node.h"
#include "Tree.h"
#include "pp.h"

PublicKey *setUp(int serParam, int attrNumber, int denth);

CT *encrypt(PublicKey *publicKey, int *att, int message);

int depth(Node *curr);

ssk *keyGen(Tree *tree, PublicKey *publicKey);

int evaluate(mpz_t ele, Node *p, ssk *ssk, CT *ct,
             PublicKey *publicKey);

bool transform(Tree *tree, ssk *ssk, CT *ct, PublicKey *publicKey);

bool decrypt(int &decryptMessage, ssk *ssk, CT *ct, PublicKey *publicKey);

Tree *buildTree();

bool inputMessage(int &message);
