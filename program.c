#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <time.h>
#include "player.c"
#include "ball.c"
#include "goalPost.c"
#include "resource.c"


key_t key_TeamA = 6000; 
key_t key_TeamB = 6001; 
key_t key_Ball = 6003; 
key_t key_Random = 6004; 

#define playerAmount 5
#define TIME 1

//Teams declaration
Player amI;



void printTeam(Player* team){
    // printf('size : %d',sizeof(team));
    for (int i = 0;i< 5;i++){
        printPlayer(team[i]);
    }
}



void printResult(){
    GoalPost* teamA =  getMemoryGoalPost(key_TeamA);
    GoalPost* teamB =  getMemoryGoalPost(key_TeamB);
    printf("******Score table******\n");
    printf("Team A :%d Team B :%d\n\n", teamB->goals,teamA->goals); // the goals have been saved in the goalPost of the other group
}

Player *initTeam(char pName, key_t pTeamKey, key_t pOtherTeam){
    static Player team[playerAmount];
    for(int i =0;i<playerAmount;i++){
        pid_t  pid;
        pid = fork();
        if ( pid == 0) {
            amI=  newPlayer(getpid(),i,pName,1,pTeamKey);
            int ran;
            printf("New player registered ppid %d \n",getppid());
            printPlayer(amI);
            while(getMemoryBall(key_Ball)->gameFlag){ // If the game is over this flag doesn't allow to continue.
                waitRandom(getMemoryRandom(key_Random));
                ran =  getRandom(5,20);
                printf("Random number  %d\n" , ran);
                signalRandom(getMemoryRandom(key_Random));
                sleep(ran);
                waitBall(getMemoryBall(key_Ball));
                for (int i = 0; i<3;i++){
                    if(getGoalPost(getMemoryGoalPost(amI.teamKey)) == 1){
                        GoalPost* goal = getMemoryGoalPost(pOtherTeam);
                        goal->goals = goal->goals+1;
                        printf("Gooaaal Team : %c, Player: %d\n",amI.team, amI.name);
                        printResult();

                        signalGoalPost(getMemoryGoalPost(pOtherTeam));
                        break;
                    }else{
                        sleep(1);
                    } 
                }
                signalBall(getMemoryBall(key_Ball));
                printf("%s Player: %d Team : %d\n","Ball was returned", amI.name,amI.team);
                if(getGoalPost(getMemoryGoalPost(amI.team)) == 1){
                    waitRandom(getMemoryRandom(key_Random));
                    ran =  getRandom(2,5);
                    signalRandom(getMemoryRandom(key_Random));
                    sleep(ran);
                    signalGoalPost(getMemoryGoalPost(pOtherTeam));
                    printf("%s Player: %d Team : %d\n","Our goal post is alone!", amI.name,amI.team);
                }
            }
            exit(0);
        }
        if (pid > 0 ){
            team[i]=  newPlayer(pid,i,pName,1,pTeamKey);
        }

    };
    // Player* teamPointer = team ;
    //printTeam(teamPointer);
    return team;
    
    
}

void finishAllMemories(){
    deleteMemoryBall(key_Ball);
    deleteMemoryGoalPost(key_TeamA);
    deleteMemoryGoalPost(key_TeamB);
    deleteMemoryRandom(key_Random);

}

void killAllProcess(Player* team){
    for(int i = 0; i<5; i++){
        printf("Finishing playerId %d\n",team[i].id);
        kill(team[i].id,SIGTERM);
    }
}


int main()
{
    //finishAllMemories();
    srand(time(NULL));                                  // Cambiamos la semilla de la función rand() cada vez que iniciamos el programa.

    GoalPost goalPostA = newGoalPost(1,'A',1,key_TeamA);// Init available
    createMemoryGoalPost(goalPostA);
    printf("PostGoalA has been created: ");
    printGoalPost(&goalPostA);

    GoalPost goalPostB = newGoalPost(2,'B',1,key_TeamB);
    createMemoryGoalPost(goalPostB);
    printf("PostGoalB has been created: ");
    printGoalPost(&goalPostB);

    Ball ball = newBall(1,key_Ball);
    createMemoryBall(ball);
    printf("The Ball is playing!\n");
    printBall(&ball);
    createMemoryRandom(1,key_Random);
    
    Player* teamA = initTeam('A',key_TeamA,key_TeamB);
    Player* teamB = initTeam('B',key_TeamB,key_TeamA);

    int flag = 1;
    struct tm* time ;
    int minute;
    time = getTime();
    int endTime = (time->tm_min+TIME)%60 ;
    while(flag){
        time = getTime();
        minute = time->tm_min;
        if(endTime == minute){
            flag=0;
        }
    }
    
    //finishing child process
    killAllProcess(teamA);
    killAllProcess(teamB);

    //Ball* ballState = getMemoryBall(key_Ball);
    //ballState->gameFlag = 0; 
    //Final result
    printf("%s\n","-------------Final score-------------\n");
    printResult();
    finishAllMemories();
    return 0;
}

