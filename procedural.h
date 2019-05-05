#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <termios.h>





#define RED         "\x1b[31m"
#define GRE         "\x1b[32m"
#define YEL         "\x1b[33m"
#define BLU         "\x1b[34m"
#define MAG         "\x1b[35m"
#define CYA         "\x1b[36m"
#define RESET       "\x1b[0m"

/*USEFUL WHEN USED TO JAVA AND C#*/
#define null NULL
#define false FALSE
#define true TRUE

#define ERROR_SLEEP 500000




typedef enum{FALSE,TRUE}Bool;


typedef enum{EMPTY,PATH,WALL}Block;
typedef enum{DOWN,LEFT,UP,RIGHT}Direction;

typedef struct{
	int X;
	int Y;
}Coord;


typedef struct{
	Direction direction;
	Coord coord;
}Walker;


typedef struct{
	Walker *walker;
	Block block;
}Case;





void dispMap(Case **map);
void clearMap(Case **map);

Case** newMap(void);
void unNewMap(Case**map);

Walker* newWalker(int x, int y, Direction dir);
void unNewWalker(Walker *walker);


Bool setWalkerToMap(Walker *walker, Case **map);
void unsetWalkerToMap(Walker *walker,Case ** map);
Bool isWalkerOut(Walker * walker);
Bool copyMap(Case**from, Case**to);
void killWalkers(Case **map);



void update(Case ** map);
void updateWalker(Walker *walker, Case **map);


void changeDirection(Walker *walker);
void spawnWalker(Case ** map);
void dispawnWalker(Walker *walker, Case **map,Bool *isDispawn);

void saveMapToFile(char filename[],Case **map);



/*WALLS*/
Bool isCoordOut(int x, int y);
Bool setBlockToMap(Block block,int x, int y, Case **map);
Bool setWallToNeighborhood(Case **map,int x,int y);
void fillWall(Case **map,Bool showConstruct,int skip);

Bool deleteWallToNeighborhood(Case **map,int x,int y);
void deleteSingleWall(Case **map,Bool showConstruct);
void encloseMap(Case **map);


/*MENU*/

void startGeneration(Case **map);
void config(void);
void dispMapMenu(Case **map);
void saveToFile(Case **map);
void multipleGen(Case **map);
void dispCredit(void);

void saveToFile(Case **map);
void dispMenu(void);
char choixMenu(void);

int keyDetec(void);

void dispConfig(void);
void changeConfig(void);



/*DEBUG*/
void errorMalloc(int line);
void errorSeg(int line,char *name);
void errorCommon(int line,char *name);


