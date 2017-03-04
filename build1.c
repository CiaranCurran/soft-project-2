#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_PLAYERS 6
#define MAX_NAME_LEN 50

//declare player structure
typedef struct{
	char name[50];
	char type[50];
	int lifePoints;
	//nested struct for capabilities
	struct capabilities{
		int smartness;
		int strength;
		int magicSkills;
		int luck;
		int dexterity;
	}Capabilities; //where capital C for capabilities helps imply struct being referenced
}Player; //where capital P for player helps imply struct being referenced

//prototypes
int getPlayers(Player * const players);

int main(void){
	int i;
	//declare array of player structs
	Player players[MAX_PLAYERS];//can take maximum size of players
	
	int numPlayers = getPlayers(players);
	
	/***SHOW THAT getPlayers WORKED CORRECTLY***/
	for(i=0;i<numPlayers;i++){
		printf("\n%-15s%s", players[i].name, players[i].type);
	}
	
	
} 

/****THIS COULD BE CONDENSED TO GET ALL TYPES AND NAMES IN ONE GO ****/
int getPlayers(Player * const wPlayers){ //dont be confused by the "w" infront of the names, it just helps distinguish the array is being used in the function
	 //declare variables
	 int i;
	 int numPlayers;
	 int typeChoice;
	 char playerName[MAX_NAME_LEN];
	//initialise array of pointers to type strings
	char *typeName[]= {"Elf", "Human", "Ogre", "Wizard"}; // the index of the array points to a character string
	 
	 //first get the number of players the user will be working with
	 printf("Please enter the number of players:");
	do{
		scanf("%d", &numPlayers);
		if(numPlayers>MAX_PLAYERS || numPlayers<1){// validate input
			printf("\nPlease enter a valid number of players:");
		}
	}while(numPlayers>MAX_PLAYERS || numPlayers<1); //repeat until valid input registered
	
	 //allocate player names and types and store in struct array
	 for(i=0;i<numPlayers;i++){
		 printf("\nWhat type is player %d?", (i+1));
		 //display options for type
		 printf("\n%-10s%d\n%-10s%d\n%-10s%d\n%-10s%d",
		 typeName[0], 1,
		 typeName[1], 2,
		 typeName[2], 3,
		 typeName[3], 4);
		 //repeat until a valid input has been registered
		 do {
			 printf("\nplayer type:");
			 scanf("%d", &typeChoice);
			if((typeChoice<1) || (typeChoice>4)){
				printf("\n---Please enter a number between 1 and 4---");
			}
		 }while((typeChoice<1) || (typeChoice>4));
		 
		 /****VALIDATION OF INPUT NEEDED HERE******/
		 //now that a type has been identified, get the corresponding player name
		 printf("\nWhat is player %d's name?", (i+1));
		 scanf("%s", playerName);
		 
		 //now that the type and name have been identified, allocate to the struct array
		 strcpy(wPlayers[i].name, playerName);
		 strcpy(wPlayers[i].type, typeName[typeChoice-1]); //See initialisation of the type array, it stores the types, "typeChoice-1" is the position of the type chosen by the user in that array
		 
		  
	 }
	  return numPlayers;   //returns the number of players  to be used later in main
}
