#include <sys/shm.h>
#include <libexplain/shmctl.h>
#include <string>
#include "Node.h"
#include "Tree.h"
#include "pp.h"
#include "util_shm.h"
#include "util_clt.h"

void *GetShmMem_(key_t key, size_t size)
{
    void *shm = NULL; //分配的共享内存的原始首地址
    int shmid;        //共享内存标识符

    //创建共享内存
    shmid = shmget((key_t)key, size, 0666 | IPC_CREAT);
    if (shmid == -1)
    {
        printf("shmget failed. key:%x size:%lu\n", key, size);
        exit(EXIT_FAILURE);
    }

    //将共享内存连接到当前进程的地址空间
    shm = shmat(shmid, 0, 0);
    if (shm == (void *)-1)
    {
        printf("shmat failed\n");
        exit(EXIT_FAILURE);
    }
    printf("Memory attached at %p\n", shm);

    return shm;
}

template <typename T>
T *GetShmMem(key_t key)
{
    return (T *)GetShmMem_(key, sizeof(T));
}

void FreeShmMem(key_t key, void *ptr)
{
    shmid_ds shmbuffer;

    //把共享内存从当前进程中分离
    if (shmdt(ptr) == -1)
    {
        printf("shmdt failed\n");
        exit(EXIT_FAILURE);
    }
    //删除共享内存
    shmctl(key, IPC_RMID, 0);
}

bool *GetNeedGenPtr()
{
    static bool *needGen = GetShmMem<bool>(KEY_NEEDGEN);
    return needGen;
}

void FreeNeedGen()
{
    bool *needGen = GetNeedGenPtr();
    FreeShmMem(KEY_NEEDGEN, (void *)needGen);
}

SystemParam *GetSystemParamPtr()
{
    static SystemParam *systemParam = GetShmMem<SystemParam>(KEY_SYSTEMPARAM);
    return systemParam;
}

void SendSystemParam(const SystemParam *systemParam)
{
    SystemParam *shm_systemParam = GetSystemParamPtr();
    shm_systemParam->serParam = systemParam->serParam;
    shm_systemParam->attrNumber = systemParam->attrNumber;
    for (int i = 0; i < systemParam->attrNumber; ++i)
    {
        shm_systemParam->attr[i] = systemParam->attr[i];
    }
}

void FreeSystemParam()
{
    SystemParam *systemParam = GetSystemParamPtr();
    FreeShmMem(KEY_SYSTEMPARAM, (void *)systemParam);
}

Tree *GetTreePtr(int nodeNumb)
{
    //static Tree *shm_tree = (Tree *)GetShmMem_(KEY_TREE, sizeof(Tree) + sizeof(Node) * nodeNumb);
    static Tree *shm_tree = GetShmMem<Tree>(KEY_TREE);
    return shm_tree;
}

void SendTree(const Tree *tree)
{
    Tree *shm_tree = GetTreePtr(tree->nodeNumb);
    //*shm_tree = *tree;
    shm_tree->nodeNumb = tree->nodeNumb;
    //shm_tree->nodes = tree->nodes;
    //printf("tree nodes size:%d\n", tree->nodes.size());
}

void FreeTree()
{
    Tree *shm_tree = GetTreePtr(0);
    FreeShmMem(KEY_TREE, (void *)shm_tree);
}

FILE *GetPublicKeyFile(std::string command)
{
    FILE *file;

    file = fopen("publicKey.dat", command.c_str());
    if (file == NULL)
    {
        perror("Error opening file");
    }

    return file;
}

PublicKey *GetPublicKeyPtr()
{
    static PublicKey *shm_publicKey = GetShmMem<PublicKey>(KEY_PUBLICKEY);
    return shm_publicKey;
}

PublicKey *GetPublicKey()
{
    PublicKey *shm_publicKey = GetPublicKeyPtr();
    FILE *file = GetPublicKeyFile("rb+");

    printf("GetPublicKey shm_publicKey->top_level:%d\n", shm_publicKey->top_level);
    printf("GetPublicKey shm_publicKey->attrNumber:%d\n", shm_publicKey->attrNumber);

    shm_publicKey->pp = util_clt_pp_restore(file);
    printf("GetPublicKey util_clt_pp_restore over\n");

    shm_publicKey->sk = util_clt_state_restore(file);
    printf("GetPublicKey util_clt_state_restore over\n");

    shm_publicKey->MSK = clt_elem_new();
    util_clt_elem_restore(file, shm_publicKey->MSK);
    printf("GetPublicKey shm_publicKey->MSK over\n");

    for (int i = 0; i < shm_publicKey->attrNumber; ++i)
    {
        mpz_init(shm_publicKey->attributes[i].t);
        if (mpz_inp_raw(shm_publicKey->attributes[i].t, file) == 0)
        {
            fprintf(stderr, "attributes mpz_inp_raw failed!\n");
            exit(1);
        }
    }
    printf("GetPublicKey shm_publicKey->attributes over\n");

    shm_publicKey->encodingOfa = clt_elem_new();
    util_clt_elem_restore(file, shm_publicKey->encodingOfa);
    printf("GetPublicKey shm_publicKey->encodingOfa over\n");

    rewind(file);
    fclose(file);

    return shm_publicKey;
}

void SendPublicKey(const PublicKey *publicKey)
{
    //printf("SendPublicKey enter\n");

    PublicKey *shm_publicKey = GetPublicKeyPtr();
    FILE *file = GetPublicKeyFile("wb+");

    //*shm_publicKey = *publicKey;
    shm_publicKey->top_level = publicKey->top_level;
    shm_publicKey->attrNumber = publicKey->attrNumber;

    util_clt_pp_store(file, publicKey->pp);
    util_clt_state_store(file, publicKey->sk);
    util_clt_elem_store(file, publicKey->MSK);
    /*if (mpz_out_raw(file, publicKey->MSK) == 0)
    {
        fprintf(stderr, "mpz_out_raw failed!\n");
        exit(1);
    }*/
    //printf("SendPublicKey attributes begin\n");
    for (int i = 0; i < publicKey->attrNumber; ++i)
    {
        if (mpz_out_raw(file, ((PublicKey *)publicKey)->attributes[i].t) == 0)
        {
            fprintf(stderr, "attributes mpz_out_raw failed!\n");
            exit(1);
        }
    }
    //printf("SendPublicKey attributes end\n");
    util_clt_elem_store(file, publicKey->encodingOfa);

    rewind(file);
    fclose(file);

    //printf("SendPublicKey end\n");
}

void FreePublicKey()
{
    PublicKey *shm_publicKey = GetPublicKeyPtr();
    FreeShmMem(KEY_PUBLICKEY, (void *)shm_publicKey);
}

FILE *GetSskFile(std::string command)
{
    FILE *file;

    file = fopen("ssk.dat", command.c_str());
    if (file == NULL)
    {
        perror("Error opening file");
    }

    return file;
}

ssk *GetSskPtr()
{
    static ssk *shm_sk = GetShmMem<ssk>(KEY_SSK);
    return shm_sk;
}

ssk *GetSsk()
{
    ssk *shm_sk = GetSskPtr();
    FILE *file = GetSskFile("rb+");

    printf("GetSsk shm_sk->nodeNumber:%d\n", shm_sk->nodeNumber);

    mpz_init(shm_sk->kh);
    if (mpz_inp_raw(shm_sk->kh, file) == 0)
    {
        fprintf(stderr, "kh mpz_inp_raw failed!\n");
        exit(1);
    }
    printf("GetSsk shm_sk->kh over\n");

    for (int i = 0; i < shm_sk->nodeNumber * 4; ++i)
    {
        mpz_init(shm_sk->skUnion[i]);
        if (mpz_inp_raw(shm_sk->skUnion[i], file) == 0)
        {
            fprintf(stderr, "skUnion[%d] mpz_inp_raw failed!\n", i);
            exit(1);
        }
    }
    printf("GetSsk shm_sk->skUnion over\n");

    printf("GetSsk shm_sk->skStartIndex over\n");

    rewind(file);
    fclose(file);

    return shm_sk;
}

void SendSsk(const ssk *sk)
{
    ssk *shm_sk = GetSskPtr();
    FILE *file = GetSskFile("wb+");

    shm_sk->nodeNumber = sk->nodeNumber;

    if (mpz_out_raw(file, sk->kh) == 0)
    {
        fprintf(stderr, "kh mpz_out_raw failed!\n");
        exit(1);
    }

    for (int i = 0; i < sk->nodeNumber * 4; ++i)
    {
        if (mpz_out_raw(file, sk->skUnion[i]) == 0)
        {
            fprintf(stderr, "skUnion[%d] mpz_out_raw failed!\n", i);
            exit(1);
        }
    }

    for (int i = 0; i < sk->nodeNumber; ++i)
    {
        shm_sk->skStartIndex[i] = sk->skStartIndex[i];
    }

    rewind(file);
    fclose(file);
}

void FreeSsk()
{
    ssk *shm_sk = GetSskPtr();
    FreeShmMem(KEY_SSK, (void *)shm_sk);
}
