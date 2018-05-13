#include <string.h>
#include <stdlib.h>
#include <time.h>



int getRandom(int min, int max) 
{
    return min + rand()% (max + 1 - min );
}

struct tm* getTime() 
{
    time_t rawtime;
    time(&rawtime);
    return localtime(&rawtime);
}

int* getMemoryRandom(key_t pKey)
{
    int shmid;
    if ((shmid = shmget(pKey, sizeof(int), 0777 | IPC_CREAT))  < 0)
    {
        perror("shmget");
        exit(1);
    }
    return (int*)shmat(shmid, NULL, 0);
}

void deleteMemoryRandom(key_t pKey)
{
    int* deleted = getMemoryRandom(pKey);
    shmdt(deleted);

    int shmid;
    if ((shmid = shmget(pKey, sizeof(int), 0777 | IPC_CREAT))  < 0)
    {
        perror("shmget");
        exit(1);
    }
    shmctl(shmid, IPC_RMID, NULL);
}
void createMemoryRandom(int init, key_t key){
    int shmid;
    if((shmid = shmget(key, sizeof(int), 0777 | IPC_CREAT))  < 0)
    {
        perror("shmget");
        exit(1);
    }

    int* rand  = (int*)shmat(shmid, NULL, 0);
    *rand = init;
}

void waitRandom(int* resource)
{
    while(*resource <= 0){};
    (*resource)--;
}

void signalRandom(int* resource)
{
    (*resource)++;
}
