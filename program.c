#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "player.c"
#include "ball.c"
#include "goalPost.c"


key_t key_TeamA = 6000; 
key_t key_TeamB = 6001; 
key_t key_Ball = 6002; 

//Teams declaration
Player amI;



void printTeam(Player* team){
    // printf('size : %d',sizeof(team));
    for (int i = 0;i< 5;i++){
        printPlayer(team[i]);
    }
}

Player* initTeam(char pName,int playerAmount){
    Player team[playerAmount];
    for(int i =0;i<playerAmount;i++){
        pid_t  pid;
        pid = fork();
        if ( pid == 0) {
            amI=  newPlayer(pid,i,pName,1);
            printf("New player registered");
            printPlayer(amI);
            // do the job specific to the child process
            //...
            // don't forget to exit from the child
            exit(0);
        }
        if (pid > 0 ){
            team[i]=  newPlayer(pid,i,pName,1);
        }

    };
    Player* teamPointer = team ;
    printTeam(teamPointer);
    return teamPointer;
    
    
}



int main()
{

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
    
    Player* teamA = initTeam('A',5);
    Player* teamB = initTeam('B',5);

    return 0;
}

