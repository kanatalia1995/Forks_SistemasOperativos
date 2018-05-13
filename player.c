#include <stdlib.h>

typedef struct 
{
	int id;
	int name;
	char team;
	key_t teamKey;
	int state;
} Player;

Player newPlayer(int pId,int pName ,char pTeam,int pState,key_t pTeamKey)
{
	Player player;
	player.id = pId;
	player.name =  pName;
	player.team = pTeam;
	player.state = pState;
	player.teamKey  = pTeamKey;
	return player;
}

void printPlayer(Player pPlayer)
{
	printf("%s: %d,%s : %d ,%s: %c\n","Id: ", pPlayer.id,"- Player",pPlayer.name ," - Team: ", pPlayer.team);
}



