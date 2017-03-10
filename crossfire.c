#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>

#define MAX_PLAYERS 6
#define MAX_NAME_LEN 30
#define MAX_SLOTS 20
 
typedef struct{//declare player structure
    char name[MAX_NAME_LEN];
    char type[MAX_NAME_LEN];
    char slotType[MAX_NAME_LEN];
    int slotIndex;
    int lifePoints;
    struct capabilities{//nested struct for capabilities
        int smartness;
        int strength;
        int magicSkills;
        int luck;
        int dexterity;
    }Capabilities; //where capital C for capabilities helps imply struct being referenced
}Player; //where capital P for player helps imply struct being referenced
 
typedef struct{
    char type[MAX_NAME_LEN];
    int playerIndex; //if the slot is occupied this gives the index of the player occupying it
    bool occupied;
}Slot;//where capital S for slot helps imply struct being referenced
 
typedef struct{
    bool canMoveRight;
    bool canMoveLeft;
    bool twoClosest;
    int closestSlotIndex1;
    int closestSlotIndex2;
}Options;//where capital O for options helps imply struct being referenced
 
//prototypes
int randRange(int x, int y);
int getPlayers(Player * players, const char ** playerType);
void setCapabilities(Player * players, const char ** playerType, int numPlayers);
int getSlotNumber(int numPlayers);
void displayPlayerStats(Player * players, int numPlayers); //for testing purposes
void setSlots(Slot * slots,  const char ** slotType, int numSlots);
void displaySlots(Slot * slots, int numSlots, Player * players); // for testing purposes
void assignPlayers(Player * players, Slot *slots, int numPlayers, int slotNumber);
void makeMoves(Player * players, Slot * slots, int numPlayers, int slotNumber, const char ** slotType);
void possibleMoves(Player * players, Slot * slots, int numPlayers, int slotNumber, Options *options, int index);
void attackPlayer(Player *attacker, Player *attacked);
void movePlayer(Player *player, Slot *slots, const char ** slotType, int moveDirection);
 
int main(void){
    int i, slotNumber, numPlayers;
    const char *playerType[]= {"Elf", "Human", "Ogre", "Wizard"};
    const char *slotType[]= {"Level Ground", "Hill", "City"};
   
    Player players[MAX_PLAYERS];//array of players
    Slot slots[MAX_SLOTS];//array of slots
   
    numPlayers = getPlayers(players, playerType);
    setCapabilities(players, playerType, numPlayers);
    slotNumber = getSlotNumber(numPlayers);
    setSlots(slots, slotType, slotNumber);
    assignPlayers(players, slots, numPlayers, slotNumber);
    makeMoves(players, slots, numPlayers, slotNumber, slotType);
	displayPlayerStats(players, numPlayers);
    return 0;
}
 
int randRange(int min, int max){
    return (rand()%((max+1)-min))+min; //returns number from within range inclusive
}
 
int getPlayers(Player * players, const char ** playerType){
    int i, numPlayers, typeChoice;
    char playerName[MAX_NAME_LEN]; //takes in a player name
    //first get the number of players
    printf("Please enter the number of players (MAX 6):");
    do{
        scanf("%d", &numPlayers);
        if(numPlayers>MAX_PLAYERS || numPlayers<2){// validate input
            printf("\nPlease enter a valid number of players:");
        }
    }while(numPlayers>MAX_PLAYERS || numPlayers<2); //repeat until valid input registered
   
    //allocate player names and types and store in struct array
    for(i=0;i<numPlayers;i++){
        printf("\nWhat type is player %d?", (i+1));
        //display options for type
        printf("\n%-10s%d\n%-10s%d\n%-10s%d\n%-10s%d",
        playerType[0], 1,
        playerType[1], 2,
        playerType[2], 3,
        playerType[3], 4);
        //repeat until a valid input has been registered
        do {
            printf("\nPlayer type:");
            scanf("%d", &typeChoice);
           if((typeChoice<1) || (typeChoice>4)){
               printf("\n---Please enter a number between 1 and 4---");
           }
        }while((typeChoice<1) || (typeChoice>4));
        
        //now that a type has been identified, get the corresponding player name
        printf("\nWhat is player %d's name?\nName:", (i+1));
        scanf(" %[^\n]s", playerName);
       
        //now that the type and name have been identified, allocate to the struct array
        strcpy(players[i].name, playerName);
        strcpy(players[i].type, playerType[typeChoice-1]);      //See initialisation of the type array, it stores the types, "typeChoice-1" is the position of the type chosen by the user in that array
        players[i].slotIndex = -1; //default initialisation
    }
      return numPlayers;   //returns the number of players  to be used later in main
}
 
void setCapabilities(Player * players, const char ** playerType, int numPlayers){
    int i;
    int sum;
	srand(time(NULL));
    //loop through all players and assign capabilities based on criteria
    for(i=0;i<numPlayers;i++){
        /****CAPABILITIES FOR ELF****/
        if(strcmp(players[i].type, playerType[0])==0){
            players[i].lifePoints = 100;
            players[i].Capabilities.luck = randRange(60, 100); //between 60-100
            players[i].Capabilities.smartness = randRange(70, 100); //between 70-10
            players[i].Capabilities.strength = randRange(1, 50); //between 1-50
            players[i].Capabilities.magicSkills = randRange((50+1), (80-1)); //between but not inclusive 50-80
            players[i].Capabilities.dexterity = randRange(1, 100); //between 1-100
        }
        /****CAPABILITIES FOR HUMAN****/
        if(strcmp(players[i].type, playerType[1])==0){
            players[i].lifePoints = 100;
            //the sum of the human's capabilities must not exceed 300
            do{
            sum = 0;
            sum += players[i].Capabilities.luck = randRange(1, 100); //between 1-100
            sum +=players[i].Capabilities.smartness = randRange(1, 100); //between 1-100
            sum +=players[i].Capabilities.strength = randRange(1, 100); //between 1-100
            sum +=players[i].Capabilities.magicSkills = randRange(1, 100); //between 1-100
            sum +=players[i].Capabilities.dexterity = randRange(1, 100); //between 1-100
            }while(sum>=300);
        }
        /****CAPABILITIES FOR OGRE****/
        if(strcmp(players[i].type, playerType[2])==0){
            players[i].lifePoints = 100;
            do{ //repeats until the sum of luck and smartness is less than 50
                sum =0; 
                sum += players[i].Capabilities.luck = randRange(0, 50);; //between 0-50
                sum += players[i].Capabilities.smartness = randRange(0, 50);; //between 0-50
            }while(sum>50);
            players[i].Capabilities.strength = randRange(80, 100); //between 80-10
            players[i].Capabilities.magicSkills = 0; //set to 0
            players[i].Capabilities.dexterity = randRange(80, 100);; //between 80-100
        } 
        /****CAPABILITIES FOR WIZARD****/
        if(strcmp(players[i].type, playerType[3])==0){
            players[i].lifePoints = 100;
            players[i].Capabilities.luck = randRange(50, 100);//between 50-100
            players[i].Capabilities.smartness = randRange(90, 100); //between 90-10
            players[i].Capabilities.strength = randRange(1, 20); //between 1-20
            players[i].Capabilities.magicSkills = randRange(80, 100); //between 80-100
            players[i].Capabilities.dexterity = randRange(1, 100); //between 1-100
        }
    }
}
 
void displayPlayerStats(Player *players, int numPlayers){
    int i;
    for(i=0;i<numPlayers;i++){
        printf("\n\n%-10s%-10s Life=%d, Luck=%d, Strength=%d, Smartness=%d, Magic=%d, Dexterity=%d, slot:%d",
        players[i].name, players[i].type,
        players[i].lifePoints,
        players[i].Capabilities.luck,
        players[i].Capabilities.strength,
        players[i].Capabilities.smartness,
        players[i].Capabilities.magicSkills,
        players[i].Capabilities.dexterity,
        players[i].slotIndex+1);
    }
}
 
int getSlotNumber(int numPlayers){
    int slotNum;
    printf("\nPlease enter the number of slots (MAX 20):");
    do{
        scanf("%d", &slotNum);
        if(slotNum>20 || slotNum<numPlayers){
            printf("\nPlease enter valid slot number:");
        }
    }while(slotNum>20 || slotNum<numPlayers);
    return slotNum;
}
 
void setSlots(Slot *slots,  const char ** slotType, int numSlots){
    int i;
    int j;
    for(i=0;i<numSlots;i++){
        slots[i].occupied = false;
        slots[i].playerIndex = -1; // -1 is default initialisation
        strcpy(slots[i].type, slotType[randRange(0, 2)]);//sets slot to a random type from the array containing slot type names
    }
}
 
void displaySlots(Slot *slots, int numSlots, Player *players){
    int i;
    printf("\n*****SLOTS******\n");
    for(i=0;i<numSlots;i++){
        if(slots[i].occupied){
            printf("\nSlot:%-4d%-15sPlayer:%-15sOccupied:TRUE", (i+1), slots[i].type, players[slots[i].playerIndex].name);
        }
        else{
            printf("\nSlot:%-4d%-15sPlayer:%-15sOccupied:0", (i+1), slots[i].type, "");
        }
    }
}
 
void assignPlayers(Player *players, Slot *slots, int numPlayers, int slotNumber){
    int i, j;
    for(i=0;i<numPlayers;i++){
        do{
            j = randRange(0, (slotNumber-1));
            if(!slots[j].occupied){ //if the slot is not occupied then it can be allocated
                slots[j].playerIndex = i;
                players[i].slotIndex = j; //set the position of the player to index of slot it is in
                slots[j].occupied = true;
                strcmp(players[i].slotType, slots[j].type); //gives the current slot type of a player
                break;
            }
        }while(slots[j].occupied);
    }
}
 
void possibleMoves(Player *players, Slot *slots, int numPlayers, int slotNumber, Options *options, int index){
    int i = index;
    int searchLeft, searchRight; //we say that leftmost is the begininng of the array(index 0) and rightmost is end of the array      
    if(players[i].slotIndex==0){   //if player is on the first slot
		options->twoClosest = false;
		options->canMoveLeft = false; //player can't move left because they are on edge slot
		searchRight = 1;
        while(!slots[players[i].slotIndex+searchRight].occupied){
        searchRight++;            
        }
		if(searchRight>1){options->canMoveRight = true;}
			else{options->canMoveRight = false;}
        options->closestSlotIndex1=players[slots[players[i].slotIndex+searchRight].playerIndex].slotIndex;
    }
    else if(players[i].slotIndex==slotNumber-1){//if player is on the last slot             
        options->twoClosest = false;
		searchLeft = 1;
		options->canMoveRight = false;
        while(!slots[players[i].slotIndex-searchLeft].occupied){
        searchLeft++;             
        }
		if(searchLeft>1){options->canMoveLeft = true;}
			else{options->canMoveLeft = false;}
        options->closestSlotIndex1=players[slots[players[i].slotIndex-searchLeft].playerIndex].slotIndex;
    }
    else if((players[i].slotIndex>0) && (players[i].slotIndex<(slotNumber-1))){             
        searchLeft = 1;
        searchRight = 1;
		if(!slots[players[i].slotIndex+searchRight].occupied){options->canMoveRight = true;}
			else{options->canMoveRight = false;}
		if(!slots[players[i].slotIndex-searchLeft].occupied){options->canMoveLeft = true;}
			else{options->canMoveLeft = false;}
        while(!slots[players[i].slotIndex+searchRight].occupied && !slots[players[i].slotIndex-searchLeft].occupied){
            if((players[i].slotIndex+searchRight)<slotNumber-1){ //if the search gets to the end of the array it stops
                searchRight++;
            }
            if((players[i].slotIndex-searchLeft)>0){ //if the search gets to the end of the array it stops
                searchLeft++;
            }            
        }
        if(slots[players[i].slotIndex+searchRight].occupied && slots[players[i].slotIndex-searchLeft].occupied){
            options->twoClosest = true;
            options->closestSlotIndex1 = players[slots[players[i].slotIndex+searchRight].playerIndex].slotIndex;
            options->closestSlotIndex2 = players[slots[players[i].slotIndex-searchLeft].playerIndex].slotIndex;           
        }
        else if(slots[players[i].slotIndex+searchRight].occupied){
			options->twoClosest = false;
            options->closestSlotIndex1 = players[slots[players[i].slotIndex+searchRight].playerIndex].slotIndex;               
        }
        else if(slots[players[i].slotIndex-searchLeft].occupied){
			options->twoClosest = false;
            options->closestSlotIndex1 = players[slots[players[i].slotIndex-searchLeft].playerIndex].slotIndex;        
        }
    }
 
}  
 
void makeMoves(Player * players, Slot * slots, int numPlayers, int slotNumber, const char ** slotType){
    int i, choice, moveNum;
    Options options;
    int moveDirection; //-1 default, 0 move right, 1 move left
    for(i=0;i<numPlayers;i++){
        displaySlots(slots, slotNumber, players);
        moveDirection=-1;//-1 default, 0 means left, 1 means right
        int moveNum=1;     //reset move number (used to determine the number for the option displayed to the user              
        printf("\n\nWhat move would player %s like to make? (Enter Corresponding number)", players[i].name);
        possibleMoves(players, slots, numPlayers, slotNumber, &options, i);
   
        if(options.twoClosest){
            printf("\n%-10s%-15s%d\n", "Attack", players[slots[options.closestSlotIndex1].playerIndex].name, moveNum++);
            printf("%-10s%-15s%d\n", "Attack", players[slots[options.closestSlotIndex2].playerIndex].name, moveNum++);
            if(options.canMoveLeft){
                printf("%-25s%d\n", "Move left", moveNum++);             
            }
            if(options.canMoveRight){            
                printf("%-25s%d\n", "Move Right", moveNum++);
            }
           do{
                scanf("%d", &choice);
				if(choice<1 || choice>moveNum){
					printf("PLEASE ENTER A VALID INPUT:");
				}
            }while(choice<1 || choice>moveNum);              
           
            if(choice==1){
                attackPlayer(&players[i], &players[slots[options.closestSlotIndex1].playerIndex]);         
            }
            else if(choice==2){
                attackPlayer(&players[i], &players[slots[options.closestSlotIndex2].playerIndex]);               
            }
            else if(options.canMoveLeft && choice ==3){              
                moveDirection= 1;//-1 default, 0 move right, 1 move left
                movePlayer(&players[i], slots, slotType, moveDirection);
            }
            else if((!options.canMoveLeft) && options.canMoveRight && choice ==3){            
                moveDirection= 0;//-1 default, 0 move right, 1 move left
                movePlayer(&players[i], slots, slotType, moveDirection);
            }
            else if(choice==4){              
                moveDirection= 0;//-1 default, 0 move right, 1 move left
                movePlayer(&players[i], slots, slotType, moveDirection);
            }
        }            
        else {               
            printf("\n%-10s%-15s%d\n", "Attack", players[slots[options.closestSlotIndex1].playerIndex].name, moveNum++);
            if(options.canMoveLeft){
                printf("%-25s%d\n", "Move left", moveNum++);            
            }
            if(options.canMoveRight){
                printf("%-25s%d\n", "Move Right", moveNum++);             
            }
            do{ //Get a valid choice from the user
				scanf("%d", &choice); 
				if(choice<0 || choice > moveNum){printf("PLEASE ENTER A VALID INPUT:");} 
			}while(choice<1 || choice > moveNum);
				
            if(choice==1){
                attackPlayer(&players[i], &players[slots[options.closestSlotIndex1].playerIndex]);           
            }
            else if(options.canMoveLeft && choice==2){              
            moveDirection= 1;//-1 default, 0 move right, 1 move left
            movePlayer(&players[i], slots, slotType, moveDirection);
            }
            else if((!options.canMoveLeft) && options.canMoveRight && choice==2){        
            moveDirection= 0;//-1 default, 0 move right, 1 move left
            movePlayer(&players[i], slots, slotType, moveDirection);
            }
            else if(choice==3){         
            moveDirection= 0;//-1 default, 0 move right, 1 move left
            movePlayer(&players[i], slots, slotType, moveDirection);
            }
        }
    }
}
 
void attackPlayer(Player *attacker, Player *attacked){             
    if(attacked->Capabilities.strength<=70){               
        attacked->lifePoints = attacked->lifePoints - 0.5*attacked->Capabilities.strength;
    }
    else if(attacked->Capabilities.strength>70){      
        attacker->lifePoints = attacker->lifePoints - 0.3*attacked->Capabilities.strength;
    }
}
 
void movePlayer(Player *player, Slot * slots, const char ** slotType, int moveDirection){         
    if(moveDirection==0){ //MOVE RIGHT
        slots[player->slotIndex+1].playerIndex = slots[player->slotIndex].playerIndex;
        slots[player->slotIndex+1].occupied = true;
        slots[player->slotIndex].playerIndex = -1;//becomes empty so change to default
        slots[player->slotIndex].occupied = false; // becomes empty so no longer occupied
        player->slotIndex +=1;
    }
    else if(moveDirection==1){ //MOVE LEFT           
        slots[player->slotIndex-1].playerIndex = slots[player->slotIndex].playerIndex ;
        slots[player->slotIndex-1].occupied = true;
        slots[player->slotIndex].playerIndex = -1;//becomes empty so change to default
        slots[player->slotIndex].occupied = false;
        player->slotIndex -=1;
    }
    if(strcmp(slots[player->slotIndex].type, slotType[1])==0){//IF MOVED TO HILL SLOT
        if(player->Capabilities.dexterity<50){            
            player->Capabilities.strength -= 10;
        }
        else if(player->Capabilities.dexterity>=60){              
            player->Capabilities.strength +=10;
        }
    }
    else if(strcmp(slots[player->slotIndex].type, slotType[2])==0){//IF MOVED TO CITY SLOT       
        if(player->Capabilities.smartness>60){              
            player->Capabilities.magicSkills +=10;
        }
    }             
}
