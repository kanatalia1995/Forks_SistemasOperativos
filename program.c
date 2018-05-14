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
#define TIME 5

#define RESET     "\x1B[0m"
#define BOLD      "\x1B[1m"
#define FG_RED    "\x1B[31m"
#define FG_GREEN  "\x1B[32m"
#define FG_YELLOW "\x1B[33m"
#define FG_WHITE  "\x1B[37m"
#define BG_BLACK  "\x1B[40m"


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
	printf("%s", FG_YELLOW);	// Active yellow color
    printf("************************\n");
    printf("******Score table*******\n");
    printf("│  Team A   ║  Team B  │\n");
    printf("     %d           %d     \n", teamB->goals,teamA->goals);
    printf("└───────────┴──────────┘\n");
    printf("%s", RESET);	// Desactive yellow color
    puts("");
}

void PrintFinalScore(){
	printf("%s", BOLD);	// Active BOLD
	printf("%s", FG_YELLOW);	// Active yellow color 
    printf("\n%s\n","----- Final score -----");
    printResult();
    printf("%s", RESET);	// Desactive yellow color and BOLD
}

void PrintNewPlayer(int ppid, Player pPlayer)
{
	printf("%s%s  +  %sppid : %d | ", BOLD, FG_GREEN, RESET, ppid);
	printPlayer(pPlayer);
}


Player *initTeam(char pName, key_t pTeamKey, key_t pOtherTeam){
    static Player team[playerAmount];
    for(int i =0;i<playerAmount;i++){
        pid_t  pid;
        pid = fork();
        if ( pid == 0) {
            amI=  newPlayer(getpid(),i+1,pName,1,pTeamKey);
            int ran;
            // printf("New player registered ppid %d \n",getppid());
            // printPlayer(amI);
            PrintNewPlayer(getppid(), amI);
            while(getMemoryBall(key_Ball)->gameFlag){ // If the game is over this flag doesn't allow to continue.
                waitRandom(getMemoryRandom(key_Random));
            	sleep(1);
            	srand(time(0)+i*getpid());
                ran =  getRandom(5,20);
                printf("Random number %d\n" , ran);
                signalRandom(getMemoryRandom(key_Random));
                sleep(ran);
                waitBall(getMemoryBall(key_Ball));
                for (int i = 0; i<3;i++){
                    if(getGoalPost(getMemoryGoalPost(amI.teamKey)) == 1){
                        GoalPost* goal = getMemoryGoalPost(pOtherTeam);
                        goal->goals = goal->goals+1;
                        printf("\nGooaaal of the Team %c, Player %d ⚽ \n",amI.team, amI.name);
                        puts("");
                        printResult();
                        signalGoalPost(getMemoryGoalPost(pOtherTeam));
                        break;
                    }else{
                        sleep(1);
                    } 
                }
                signalBall(getMemoryBall(key_Ball));
                printf("%s%s Player %d, Team %c%s\n", BG_BLACK, "Ball was returned", amI.name,amI.team, RESET);
                if(getGoalPost(getMemoryGoalPost(amI.team)) == 1){
                    waitRandom(getMemoryRandom(key_Random));
                    srand(time(0)+i*i);
                    ran =  getRandom(2,5);
                    signalRandom(getMemoryRandom(key_Random));
                    sleep(ran);
                    signalGoalPost(getMemoryGoalPost(pOtherTeam));
                    printf("%s Player: %d Team : %c\n","Our goal post is alone!", amI.name,amI.team);
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
        printf("%s  × %s playerId %d\n", FG_RED, RESET, team[i].id);
        kill(team[i].id,SIGTERM);
    }
}


int main()
{
    finishAllMemories();
    srand(time(NULL));                                  // Cambiamos la semilla de la función rand() cada vez que iniciamos el programa.
    puts("");

    GoalPost goalPostA = newGoalPost(1,'A',1,key_TeamA);// Init available
    createMemoryGoalPost(goalPostA);
    printf("PostGoal A has been created: ");
    printGoalPost(&goalPostA);

    GoalPost goalPostB = newGoalPost(2,'B',1,key_TeamB);
    createMemoryGoalPost(goalPostB);
    printf("PostGoal B has been created: ");
    printGoalPost(&goalPostB);
    puts("");

    Ball ball = newBall(1,key_Ball);
    createMemoryBall(ball);
    printf("The Ball is playing!\n");
    printBall(&ball);

    puts("");
    printf("All new players registered: ");
    puts("");
    
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
    printf("\nFinishing all processes: \n");
    killAllProcess(teamA);
    killAllProcess(teamB);

    //Ball* ballState = getMemoryBall(key_Ball);
    //ballState->gameFlag = 0; 

   	PrintFinalScore();
    finishAllMemories();
    return 0;
}

