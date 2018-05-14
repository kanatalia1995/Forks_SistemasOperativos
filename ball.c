#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <assert.h>

typedef struct 
{
	int state;
	int gameFlag;
	key_t key; //memory
} Ball ;

Ball newBall(int pState, key_t pKeyT)
{
	Ball ball;
	ball.state = pState;
	ball.gameFlag = 1;
	ball.key =  pKeyT;
	return ball;
}

void printBall(Ball* pBall)
{
	printf("%s: %d\n", "Ball state", pBall->state);
}

Ball* getMemoryBall(key_t pKey)
{
	int shmid;
    if ((shmid = shmget(pKey, sizeof(Ball), 0777 | IPC_CREAT))  < 0)
    {
        perror("shmget");
        exit(1);
    }
    return (Ball*)shmat(shmid, NULL, 0);
}

void deleteMemoryBall(key_t pKey)
{
	Ball* deleted;
	deleted = getMemoryBall(pKey);
	shmdt(deleted);

	int shmid;
    if ((shmid = shmget(pKey, sizeof(Ball), 0777 | IPC_CREAT))  < 0)
    {
        perror("shmget");
        exit(1);
    }
    shmctl(shmid, IPC_RMID, NULL);
}




void waitBall(Ball* resource)
{
    while(resource->state <= 0){};//wait
    (resource->state)--;
}

void signalBall(Ball* resource)
{
    (resource->state)++;
}

void createMemoryBall(Ball ball){
	int shmid;
	if((shmid = shmget(ball.key, sizeof(Ball), 0777 | IPC_CREAT))  < 0)
	{
	    perror("shmget");
	    exit(1);
	}
	Ball* ballPointer = (Ball*)shmat(shmid, NULL, 0);
	*ballPointer = ball;
}


