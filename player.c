#include <stdlib.h>

typedef struct 
{
	int id;
	int name;
	char team;
	int state;
} Player;

Player newPlayer(int pId,int pName ,char pTeam,int pState )
{
	Player player;
	player.id = pId;
	player.name =  pName;
	player.team = pTeam;
	player.state = pState;
	return player;
}

void printPlayer(Player pPlayer)
{
	printf("%s: %d,%s : %d ,%s: %c\n","Id: ", pPlayer.id,"- Player",pPlayer.name ," - Team: ", pPlayer.team);
}



