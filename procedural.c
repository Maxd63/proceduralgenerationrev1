#include "procedural.h"

int nbWalker=0;
Bool debug=FALSE;
int nbCasePath=0;

int HEIGHT=80;
int WIDTH=80;

int MS=33333;
int MAXWALKER=20;

int SPAWNRATE=10;
int DISPAWNRATE=23;
int CHANGEDIR=40;
/*OUT OF 100*/


int nbIte=500;
int nbSkip = 3;
/*map isn't global -> security first*/



void dispMap(Case **map){

	if(map==NULL){
		errorSeg(__LINE__,"map");
		return;
	}

	for(int i=0;i<HEIGHT;i++){
		for(int j=0;j<WIDTH;j++){


			if(map[i][j].walker!=NULL){
				printf(BLU "██" RESET);
				continue;
			}

			else if(map[i][j].block==WALL){
				printf(YEL "██" RESET);
			}
			else if(map[i][j].block==PATH){
				printf(RED "██" RESET);
			}
			else{
				printf("  ");
			}
		}
		printf("\n");
	}
}


void clearMap(Case **map){
	if(map==NULL){
		errorSeg(__LINE__,"map");
		return;
	}
	for(int i=0;i<HEIGHT;i++){
		for(int j=0;j<WIDTH;j++){
			map[i][j].walker=NULL;
			map[i][j].block=EMPTY;

		}
	}
}



Case** newMap(void){
	Case ** map;

	map = (Case**)malloc(HEIGHT * sizeof(Case*));
	if(map == NULL){
		errorMalloc(__LINE__);
		exit(1);
	}
	for(int i=0;i<HEIGHT;i++){
		map[i]=(Case*)malloc(WIDTH*sizeof(Case));
		if(map[i]==NULL){
			errorMalloc(__LINE__);
			exit(1);
		}
		for(int j=0;j<WIDTH;j++){
			map[i][j].block = EMPTY;
			map[i][j].walker = NULL;

		}
	}

	return map;

}

void unNewMap(Case**map){
	if(map==NULL){
		return;
	}
	for(int i=0;i<HEIGHT;i++){
		if(map[i]==NULL){
			continue;
		}
		free(map[i]);
	}
	free(map);
}


Walker* newWalker(int x, int y, Direction dir){
	if(x<0 || y<0 || x> WIDTH || y>HEIGHT){
		errorCommon(__LINE__,"coord invalid");
		return NULL;
	}


	Walker * walker;
	walker=(Walker*)malloc(sizeof(Walker));
	if(walker == NULL){
		errorMalloc(__LINE__);
		exit(1);
	}

	walker->direction = dir;
	walker->coord.X = x;
	walker->coord.Y = y;
	return walker;
}

void unNewWalker(Walker *walker){
	if(walker == NULL){
		return;
	}
	free(walker);	
}


Bool setWalkerToMap(Walker *walker, Case **map){
	if(walker == NULL){
		errorSeg(__LINE__,"walker null");
		return FALSE;
	}
	if(map == NULL){
		errorSeg(__LINE__,"map null");
		return FALSE;
	}
	if(isWalkerOut(walker)){
		errorCommon(__LINE__,"walker out of bounds");
		unsetWalkerToMap(walker,map);
		return FALSE;
	}

	map[walker->coord.Y][walker->coord.X].walker = walker;
	nbWalker++;

	return TRUE;
}

void unsetWalkerToMap(Walker *walker,Case ** map){
	if(walker == NULL){
		errorSeg(__LINE__,"walker null");
		return;
	}
	if(map == NULL){
		errorSeg(__LINE__,"map null");
		return;
	}	
	if(isWalkerOut(walker)){
		errorCommon(__LINE__,"walker out of bounds");
		return;
	}
	map[walker->coord.Y][walker->coord.X].walker = NULL;
	nbWalker--;

}

Bool isWalkerOut(Walker * walker){
	if(walker == NULL){
		return FALSE;
	}

	if(walker->coord.X < 0 || walker->coord.Y < 0 || walker->coord.X > WIDTH-1 || walker->coord.Y > HEIGHT-1){
		return TRUE;
	}
	return FALSE;
}

Bool copyMap(Case**from, Case**to){
	if(from == NULL || to == NULL){
		errorSeg(__LINE__,"map null");
		return FALSE;
	}

	for(int i=0;i<HEIGHT;i++){
		for(int j=0;j<WIDTH;j++){
			to[i][j].block=from[i][j].block;
			to[i][j].walker=from[i][j].walker;
		}
	}
	return TRUE;

}


void killWalkers(Case **map){
	if(map == NULL){
		errorSeg(__LINE__,"map null");
		return;
	}
	for(int i=0;i<HEIGHT;i++){
		for(int j=0;j<WIDTH;j++){
			if(map[i][j].walker!=NULL){
				unsetWalkerToMap(map[i][j].walker,map);
				unNewWalker(map[i][j].walker);
			}
		}
	}
}











void update(Case ** map){
	if(map == NULL){
		errorSeg(__LINE__,"map null");
		return;
	}

	Bool isDispawn = FALSE;

	Case **aux = newMap();
	copyMap(map,aux);


	for(int i=0;i<HEIGHT;i++){
		for(int j=0;j<WIDTH;j++){
			
			if(aux[i][j].walker!=NULL){
				updateWalker(map[i][j].walker,map);
				dispawnWalker(map[i][j].walker,map,&isDispawn);
			}	
			
		}
	}
	spawnWalker(map);
	unNewMap(aux);

}

void updateWalker(Walker *walker, Case **map){
	if(walker == NULL){
		errorSeg(__LINE__,"walker null");
		return;
	}
	if(map == NULL){
		errorSeg(__LINE__,"map null");
		return;
	}	

	if(!isWalkerOut(walker)){
		setBlockToMap(PATH,walker->coord.Y,walker->coord.X,map);
		nbCasePath++;
		unsetWalkerToMap(walker,map);
	}


	switch(walker->direction){
		case DOWN:
		(walker->coord.Y)++;
		break;
		case LEFT:
		(walker->coord.X)--;
		break;
		case UP:
		(walker->coord.Y)--;
		break;
		case RIGHT:
		(walker->coord.X)++;
		break;
	}

	if(!isWalkerOut(walker)){
		setWalkerToMap(walker,map);
	}

	changeDirection(walker);
}



void changeDirection(Walker *walker){
	if(walker == null){
		errorSeg(__LINE__,"map null");
		return;
	}
	int myRandom = rand()%100;

	if(myRandom<CHANGEDIR){
		switch(rand()%4){
			case 0: walker->direction = DOWN;
			break;
			case 1: walker->direction = LEFT;
			break;
			case 2: walker->direction = UP;
			break;
			case 3: walker->direction = RIGHT;
			break;
		}
	}

}

void spawnWalker(Case ** map){
	if(map==NULL){
		errorSeg(__LINE__,"map null");
		return;
	}
	if(nbWalker>MAXWALKER){
		return;
	}

	if(rand()%100<SPAWNRATE){
		int toSpawn = rand()%nbCasePath;
		int actualCase=0;

		for(int i=0;i<HEIGHT;i++){
			for(int j=0;j<WIDTH;j++){
				if(toSpawn==actualCase){
					Walker *walker = newWalker(j,i,rand()%4);
					setWalkerToMap(walker,map);
					return;
				}

				if(map[i][j].block==PATH){
					actualCase++;
				}
			}
		}


	}
}

void dispawnWalker(Walker *walker, Case **map,Bool *isDispawn){

	if(nbWalker<=1){
		return;
	}

	if(walker == NULL){
		errorSeg(__LINE__,"walker null");
		return;
	}
	if(map == NULL){
		errorSeg(__LINE__,"map null");
		return;
	}
	if(isDispawn == NULL){
		errorSeg(__LINE__,"isDispawn Bool null");
		return;
	}



	if(*isDispawn==TRUE){
		return;
	}

	if(rand()%100<DISPAWNRATE){
		unsetWalkerToMap(walker,map);
		free(walker);
		*isDispawn=TRUE;
	}
	else{
		*isDispawn=FALSE;
	}
}








void saveMapToFile(char filename[],Case **map){
	if(filename == NULL){
		errorSeg(__LINE__,"filename null");
		return;
	}
	if(map == NULL){
		errorSeg(__LINE__,"map null");
		return;
	}	


	FILE * flot;

	char realFileName[100];
	sprintf(realFileName,"Map/%s",filename);

	flot = fopen(realFileName,"w");
	if(flot==NULL){
		errorCommon(__LINE__,"fopen error (w)");
	}
	printf("MAP : %s\n",filename);
	printf("Size : %dx%d\n\n",HEIGHT,WIDTH);

	for(int i=0;i<HEIGHT;i++){
		for(int j=0;j<WIDTH;j++){
			fprintf(flot, "%d ",map[i][j].block);
		}
		fprintf(flot, "\n");
	}

	fclose(flot);
}





Bool isCoordOut(int x, int y){
	if(x<0 || x>WIDTH-1 || y<0 || y>HEIGHT-1){
		return TRUE;
	}
	return FALSE;
}



Bool setBlockToMap(Block block,int x, int y, Case **map){
	if(x<0 ||x>WIDTH-1 ||y<0 ||y>HEIGHT-1){
		return FALSE;
	}
	if(map == null){
		return FALSE;
	}
	map[x][y].block=block;
	return TRUE;
}

Bool setWallToNeighborhood(Case **map,int x,int y){
	if(map == NULL){
		errorSeg(__LINE__,"map null");
		return FALSE;
	}	

	if(isCoordOut(x,y)){
		errorCommon(__LINE__,"invalid coord");
		return FALSE;
	}

	if(map[x][y].block != EMPTY){
		return FALSE;
	}


	//up case
	if(!isCoordOut(x,y-1)){
		if(map[x][y-1].block==PATH){
			setBlockToMap(WALL,x,y,map);
			return TRUE;
		}
	}

	//right case
	if(!isCoordOut(x+1,y)){
		if(map[x+1][y].block==PATH){
			setBlockToMap(WALL,x,y,map);
			return TRUE;
		}
	}

	//down case
	if(!isCoordOut(x,y+1)){
		if(map[x][y+1].block==PATH){
			setBlockToMap(WALL,x,y,map);	
			return TRUE;
		}
	}

	//left case
	if(!isCoordOut(x-1,y)){
		if(map[x-1][y].block==PATH){
			setBlockToMap(WALL,x,y,map);
			return TRUE;
		}
	}
	return FALSE;

}

void fillWall(Case **map,Bool showConstruct,int skip){
	if(map == NULL){
		errorSeg(__LINE__,"map null");
		return;
	}

	int auxSkip=0;

	for(int i=0;i<HEIGHT;i++){
		for(int j=0;j<WIDTH;j++){
			if(setWallToNeighborhood(map,j,i)){
				auxSkip++;
				if(showConstruct==TRUE && auxSkip%skip==0){
					dispMap(map);
					usleep(MS);
					system("clear");
				}
			}
		}
	}
}






Bool deleteWallToNeighborhood(Case **map,int x,int y){
	if(map == NULL){
		errorSeg(__LINE__,"map null");
		return FALSE;
	}	

	if(isCoordOut(x,y)){
		errorCommon(__LINE__,"invalid coord");
		return FALSE;
	}

	if(map[x][y].block != WALL){
		return FALSE;
	}

	Bool toDelete = TRUE;
	//up case
	if(!isCoordOut(x,y-1)){
		if(map[x][y-1].block!=PATH){
			toDelete=FALSE;
		}
	}

	//right case
	if(!isCoordOut(x+1,y)){
		if(map[x+1][y].block!=PATH){
			toDelete=FALSE;
		}
	}

	//down case
	if(!isCoordOut(x,y+1)){
		if(map[x][y+1].block!=PATH){
			toDelete=FALSE;
		}
	}

	//left case
	if(!isCoordOut(x-1,y)){
		if(map[x-1][y].block!=PATH){
			toDelete=FALSE;
		}
	}

	if(toDelete){
		setBlockToMap(PATH,x,y,map);
	}
	return toDelete;
	
}


void deleteSingleWall(Case **map,Bool showConstruct){
	if(map == NULL){
		errorSeg(__LINE__,"map null");
		return;
	}

	for(int i=0;i<HEIGHT;i++){
		for(int j=0;j<WIDTH;j++){
			if(deleteWallToNeighborhood(map,j,i)){
				if(showConstruct){
					dispMap(map);
					usleep(MS);
					system("clear");
				}
			}
		}
	}
}





void encloseMap(Case **map){
	if(map == NULL){
		errorSeg(__LINE__,"map null");
		return;
	}

	for(int i=0;i<HEIGHT;i++){
		if(map[i][0].block==PATH){
			setBlockToMap(WALL,i,0,map);
		}
		if(map[i][WIDTH-1].block==PATH){
			setBlockToMap(WALL,i,WIDTH-1,map);
		}
	}
	for(int j=0;j<WIDTH;j++){
		if(map[0][j].block==PATH){
			setBlockToMap(WALL,0,j,map);
		}
		if(map[HEIGHT-1][j].block==PATH){
			setBlockToMap(WALL,HEIGHT-1,j,map);
		}
	}

}






