#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_PLAYERS 6
#define MAX_NAME_LEN 50


void displayPlayerStats(Player * const players, int numPlayers); //for testing purposes
void attackPlayer(Player *attacker, Player *attacked);
void movePlayer(Player *player, Slot * const slots, const char ** slotType, int moveDirection);



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
	Slot slots[MAX_SLOTS];
	
	int numPlayers = getPlayers(players);
	slotNumber = getSlotNumber(numPlayers);
   	setSlots(slots, slotType, slotNumber);
	displayPlayerStats(players, numPlayers);
	
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
int getSlotNumber(int numPlayers){
    int slotNum;
    printf("\nPlease enter the number of slots:");
    do{
        scanf("%d", &slotNum);
        if(slotNum>20 || slotNum<numPlayers){
            printf("\nPlease enter valid slot number:");
        }
    }while(slotNum>20 || slotNum<numPlayers);
    return slotNum;
}
void setSlots(Slot * const slots,  const char ** slotType, int numSlots){
    int i;
    int j;
    for(i=0;i<numSlots;i++){
        slots[i].occupied = false;
        slots[i].playerIndex = -1; // -1 is default initialisation
        strcpy(slots[i].type, slotType[randRange(0, 2)]);//sets slot to a random type from the array containing slot type names
    }
}
void displayPlayerStats(Player * const players, int numPlayers){
    int i;
    for(i=0;i<numPlayers;i++){
        printf("\n%-10s%-10s Life=%d, Luck=%d, Strength=%d, Smartness=%d, Magic=%d, Dexterity=%d slot:%d",
               players[i].name, players[i].type,
               players[i].lifePoints,
               players[i].Capabilities.luck,
               players[i].Capabilities.strength,
               players[i].Capabilities.smartness,
               players[i].Capabilities.magicSkills,
               players[i].Capabilities.dexterity,
               players[i].slotIndex);
    }
}

void attackPlayer(Player *attacker, Player *attacked){
    //printf("\nLOCATION NO 2.1\n");
    if(attacked->Capabilities.strength<=70){
        //printf("\nLOCATION NO 2.2\n");
        attacked->lifePoints = attacked->lifePoints - 0.5*attacked->Capabilities.strength;
    }
    else if(attacked->Capabilities.strength>70){
        //printf("\nLOCATION NO 2.3\n");
        attacker->lifePoints = attacker->lifePoints - 0.3*attacked->Capabilities.strength;
    }
}


void movePlayer(Player *player, Slot * const slots, const char ** slotType, int moveDirection){
    //move to the right
    //printf("\nLOCATION NO 3.1\n");
    if(moveDirection==0){
        //printf("\nLOCATION NO 3.3\n");
        slots[player->slotIndex+1].playerIndex = slots[player->slotIndex].playerIndex;
        slots[player->slotIndex+1].occupied = true;
        slots[player->slotIndex].playerIndex = -1;//becomes empty so change to default
        slots[player->slotIndex].occupied = false; // becomes empty so no longer occupied
        player->slotIndex +=1;
    }
    
    else if(moveDirection==1){
        //printf("\nLOCATION NO 3.4\n");
        slots[player->slotIndex-1].playerIndex = slots[player->slotIndex].playerIndex ;
        slots[player->slotIndex-1].occupied = true;
        slots[player->slotIndex].playerIndex = -1;//becomes empty so change to default
        slots[player->slotIndex].occupied = false;
        player->slotIndex -=1;
    }
    
    //if hill
    if(strcmp(slots[player->slotIndex].type, slotType[1])==0){
        //printf("\nLOCATION NO 3.5\n");
        if(player->Capabilities.dexterity<50){
            //printf("\nLOCATION NO 3.6\n");
            player->Capabilities.strength -= 10;
        }
        else if(player->Capabilities.dexterity>=60){
            //printf("\nLOCATION NO 3.7\n");
            player->Capabilities.strength +=10;
        }
    }
    else if(strcmp(slots[player->slotIndex].type, slotType[2])==0){
        //printf("\nLOCATION NO 3.8\n");
        if(player->Capabilities.smartness>60){
            //printf("\nLOCATION NO 3.9\n");
            player->Capabilities.magicSkills +=10;
        }
    }
    //printf("\nLOCATION NO 4.0.1\n");                
}
