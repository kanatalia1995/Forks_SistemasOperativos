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

void deleteMemoryGoalPost(key_t pKey)
{
	GoalPost* deleted = getMemoryGoalPost(pKey);
	shmdt(deleted);

	int shmid;
    if ((shmid = shmget(pKey, sizeof(GoalPost), 0777 | IPC_CREAT))  < 0)
    {
        perror("shmget");
        exit(1);
    }
    shmctl(shmid, IPC_RMID, NULL);
}




int getGoalPost(GoalPost* resource)
{
    if (resource->state <= 0) 
    {
    	return 0 ;//wait
    }else {
    	(resource->state)--;
    	return 1; 
    }
    
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