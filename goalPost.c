#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <assert.h>

typedef struct 
{
	int id;
	key_t key;
	char team;
	int state;
	int goals;
} GoalPost ;

GoalPost newGoalPost(int pId, char pTeam,int pState,key_t pKey)
{
	GoalPost goalPost;
	goalPost.id = pId;
	goalPost.team = pTeam;
	goalPost.state = pState;
	goalPost.goals = 0;
	goalPost.key = pKey;
	return goalPost;
}

void printGoalPost(GoalPost* pGoalPost)
{
	printf("%s: %d, %s: %c , %s: %d \n","Id: ", pGoalPost->id, " - Team: ", pGoalPost->team,"- Goals",pGoalPost->goals);
}

void deleteMemoryGoalPost(key_t pKey)
{
	int shmid;
    if ((shmid = shmget(pKey, sizeof(GoalPost), 0777 | IPC_CREAT))  < 0)
    {
        perror("shmget");
        exit(1);
    }
    shmctl(shmid, IPC_RMID, NULL);
}

GoalPost* getMemoryGoalPost(key_t pKey)
{
	int shmid;
    if ((shmid = shmget(pKey, sizeof(GoalPost), 0777 | IPC_CREAT))  < 0)
    {
        perror("shmget");
        exit(1);
    }
    return (GoalPost*)shmat(shmid, NULL, 0);
}


void waitGoalPost(GoalPost* resource)
{
    while(resource->state <= 0){};//wait
    (resource->state)--;
}

void signalGoalPost(GoalPost* resource)
{
    (resource->state)++;
}

void createMemoryGoalPost(GoalPost goalPost){
	int shmid;
	if((shmid = shmget(goalPost.key, sizeof(GoalPost), 0777 | IPC_CREAT))  < 0)
	{
	    perror("shmget");
	    exit(1);
	}

	GoalPost* goalPostPointer = (GoalPost*)shmat(shmid, NULL, 0);
	*goalPostPointer = goalPost;
}