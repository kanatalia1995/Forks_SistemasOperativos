#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <assert.h>
#include "process.c"

key_t keyReady = 6000; //Ready
key_t keyProcessState = 6001; //Process States
key_t keyReadyResource = 6002; //Process States resource
key_t keyProcessStateResource = 6003; //Process States resource
key_t keyProcessReadySize = 6004; //Process States 
int maxProcessId = 2000; //Number of process that can be in memory at once

#define BLOCK 1
#define EXECUTING 2
#define MEMORY 3
#define RELEASE 4

char* spyProgram = "./spy";

char* intToString(int pInt)
{
    char* str = malloc(10);
    sprintf(str, "%d", pInt);
    return str;
}

char* floatToString(float pFloat)
{
    char* str = malloc(20);
    sprintf(str, "%7.3f", pFloat);
    return str;
}

int stringToInt(char* pString)
{
    return (int) strtol(pString, (char **)NULL, 10);
}

float stringToFloat(char* pString)
{
    return strtof(pString, (char **)NULL);
}

int getRandomInt(int min, int max) 
{
    int r;
    time_t t;
    const  int range = 1 + max - min;
    const  int buckets = RAND_MAX / range;
    const  int limit = buckets * range;
    srand((unsigned) time(&t));
    do
    {
        r = rand();
    } while (r >= limit);

    return min + (r / buckets);
}

void deleteShareMemory(int shareMemorySize, key_t pKey, int size)
{
	int shmid;
    if ((shmid = shmget(pKey, shareMemorySize * size, 0777 | IPC_CREAT))  < 0)
    {
        perror("shmget");
        exit(1);
    }
    shmctl(shmid, IPC_RMID, NULL);
}

char* getShareMemoryPointer(key_t pKey, int size)
{
	int shmid;
    if ((shmid = shmget(pKey, size, 0777 | IPC_CREAT))  < 0)
    {
        perror("shmget");
        exit(1);
    }
    return (char*)shmat(shmid, NULL, 0);
}

struct tm* getTime() cre
{
	time_t rawtime;
	time(&rawtime);
	return localtime(&rawtime);
}

void printTime(struct tm* timeinfo)
{
	printf("Current local time and date: %s", asctime(timeinfo));
}

void wait(int* resource)
{
    while(*resource <= 0){};
    (*resource)--;
}

void signal(int* resource)
{
    (*resource)++;
}
