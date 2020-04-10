#include <sys/shm.h>
#include <libexplain/shmctl.h>
#include "Node.h"
#include "Tree.h"
#include "pp.h"
#include "util_shm.h"

template <typename T>
T *GetShmMem(key_t key)
{
    void *shm = NULL; //分配的共享内存的原始首地址
    int shmid;        //共享内存标识符

    //创建共享内存
    shmid = shmget((key_t)key, sizeof(T), 0666 | IPC_CREAT);
    if (shmid == -1)
    {
        printf("shmget failed\n");
        exit(EXIT_FAILURE);
    }

    //将共享内存连接到当前进程的地址空间
    shm = shmat(shmid, 0, 0);
    if (shm == (void *)-1)
    {
        printf("shmat failed\n");
        exit(EXIT_FAILURE);
    }
    printf("\nMemory attached at %p\n", shm);

    return (T *)shm;
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

Tree *GetTree()
{
    static Tree *shm_tree = GetShmMem<Tree>(KEY_TREE);
    return shm_tree;
}

void SendTree(const Tree *tree)
{
    Tree *shm_tree = GetTree();
    *shm_tree = *tree;
}

void FreeTree()
{
    Tree *shm_tree = GetTree();
    FreeShmMem(KEY_TREE, (void *)shm_tree);
}

PublicKey *GetPublicKey()
{
    static PublicKey *shm_publicKey = GetShmMem<PublicKey>(KEY_PUBLICKEY);
    return shm_publicKey;
}

void SendPublicKey(const PublicKey *publicKey)
{
    PublicKey *shm_publicKey = GetPublicKey();
    *shm_publicKey = *publicKey;
}

void FreePublicKey()
{
    PublicKey *shm_publicKey = GetPublicKey();
    FreeShmMem(KEY_PUBLICKEY, (void *)shm_publicKey);
}

ssk *GetSsk()
{
    static ssk *shm_sk = GetShmMem<ssk>(KEY_SSK);
    return shm_sk;
}

void SendSsk(const ssk *sk)
{
    ssk *shm_sk = GetSsk();
    *shm_sk = *sk;
}

void FreeSsk()
{
    ssk *shm_sk = GetSsk();
    FreeShmMem(KEY_SSK, (void *)shm_sk);
}
