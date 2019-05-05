#include "procedural.h"



extern Bool debug;

extern int HEIGHT;
extern int WIDTH;

extern int MS;
extern int MAXWALKER;

extern int SPAWNRATE;
extern int DISPAWNRATE;
extern int CHANGEDIR;

extern int nbWalker;
extern int nbIte;
extern int nbSkip;



void startGeneration(Case **map){
	if(map == NULL){
		errorSeg(__LINE__,"map null");
		return;
	}
	Walker * walker;
	walker=newWalker(WIDTH/2,HEIGHT/2,DOWN);

	clearMap(map);

	setWalkerToMap(walker,map);

	for(int i=0;i<nbIte/nbSkip;i++){
		if(nbWalker<=0){
			break;
		}
		system("clear");
		dispMap(map);
		usleep(MS);
		for(int j=0;j<nbSkip;j++){
			update(map);
		}
	}

	system("clear");
	dispMap(map);

	killWalkers(map);

	fillWall(map,TRUE,nbSkip*3);
	system("clear");
	usleep(200000);

	deleteSingleWall(map,TRUE);
	system("clear");
	dispMap(map);
	usleep(200000);

	encloseMap(map);
	system("clear");
	dispMap(map);
	usleep(200000);

	keyDetec();


}

void config(void){
	char select[10];

	system("clear");
	printf(CYA "Config\n" RESET);
	printf("\n");

	dispConfig();
	printf("─────────────────────────────────────────────────\n");
	printf("\n\n");

	printf("Change config (yes/no) : ");
	scanf("%s",select);
	if(strcmp(select,"yes")==0){
		changeConfig();
		printf("─────────────────────────────────────────────────\n");
		printf("\n\n");
		
		dispConfig();	
	}

	keyDetec();

}


void dispMapMenu(Case **map){
	if(map == NULL){
		errorSeg(__LINE__,"map null");
		return;
	}
	system("clear");
	printf(CYA "Save to file\n" RESET);
	printf("\n");
	
	dispMap(map);
	keyDetec();


}

void saveToFile(Case **map){
	if(map == NULL){
		errorSeg(__LINE__,"map null");
		return;
	}
	char filename[60];

	system("clear");
	printf(CYA "Save to file\n" RESET);
	printf("\n");

	printf("Filename : ");

	int isValid=scanf("%s%*c",filename);
	while(isValid==0){
		printf("Invalid filename\n");
		isValid=scanf("%s%*c",filename);
	}

	saveMapToFile(filename,map);
	keyDetec();
}


void multipleGen(Case **map){
	if(map == NULL){
		errorSeg(__LINE__,"map null");
		return;
	}
	Walker * walker;
	char c;
	int iteration=0;

	printf("Q to exit\n");
	printf("S to save to folder \"Map\"\n");
	
	c = keyDetec();

	while(c!='q'){
		system("clear");
		printf("Map %d\n",iteration);
		clearMap(map);
		walker=newWalker(WIDTH/2,HEIGHT/2,DOWN);
		setWalkerToMap(walker,map);

		for(int i=0;i<nbIte;i++){
			if(nbWalker<=0){
				break;
			}

			update(map);
			
		}

		killWalkers(map);

		fillWall(map,FALSE,nbSkip);
		deleteSingleWall(map,FALSE);
		encloseMap(map);

		dispMap(map);
		iteration++;

		if(c=='s'){
			char fileConstruct[100];
			sprintf(fileConstruct,"Map%03d.dat",iteration);
			saveMapToFile(fileConstruct,map);
		}

		c = keyDetec();

	}
}


void dispCredit(void){
	system("clear");
	printf(RESET);


	printf(CYA "Credit\n" RESET);
	printf("╭───────────────────────────────────────────────╮\n");
	printf("│                                               │\n");
	printf("│              Procedural generation            │\n");
	printf("│                                               │\n");
	printf("│                 Maxime POULAIN                │\n");
	printf("│                                               │\n");
	printf("│    Procedural generation of map REV_1         │\n");
	printf("│                                               │\n");
	printf("│    2019                                       │\n");
	printf("│                                               │\n");
	printf("│                                               │\n");
	printf("│                                               │\n");
	printf("│                                               │\n");
	printf("╰───────────────────────────────────────────────╯\n");
	printf("\n");

	keyDetec();

}



void dispMenu(void){

	system("clear");
	printf(RESET);


	printf(CYA "Main Menu\n" RESET);
	printf("╭───────────────────────────────────────────────╮\n");
	printf("│                                               │\n");
	printf("│              Procedural generation            │\n");
	printf("│                                               │\n");
	printf("│   1   Start generation                        │\n");
	printf("│   2   Config                                  │\n");
	printf("│   3   Display map                             │\n");
	printf("│   4   Save to file                            │\n");
	printf("│   5   Multiple generation                     │\n");
	printf("│   6   Credit                                  │\n");
	printf("│                                               │\n");
	printf("│   9   Exit                                    │\n");
	printf("│                                               │\n");
	printf("╰───────────────────────────────────────────────╯\n");
	printf("\n");
}

char choixMenu(void){
	char choix;
	dispMenu();
	printf("Choix : ");
	choix=(char)keyDetec();
	while((choix<'1'|| choix>'6') && choix!='9'){
		printf(RED " Error\n" RESET);
		usleep(ERROR_SLEEP);
		dispMenu();
		printf("Choix : ");
		choix=(char)keyDetec();
	}
	return choix;
}



int keyDetec(void){
	int c;   
	static struct termios oldt, newt;
	tcgetattr( STDIN_FILENO, &oldt);
	newt = oldt;
	newt.c_lflag &= ~(ICANON);          
	tcsetattr( STDIN_FILENO, TCSANOW, &newt);
	c=getchar();
	tcsetattr( STDIN_FILENO, TCSANOW, &oldt);
	return c;
}




void dispConfig(void){

	printf("Debug enable : \t\t\t\t");
	if(debug==FALSE){
		printf("False");
	}
	else{
		printf("True");
	}
	printf("\n");


	printf("Size map : \t\t\t\t%dx%d \n",HEIGHT,WIDTH);

	printf("Fps : \t\t\t\t\t%.2f\n",1.0/(MS/1000000.0));

	printf("Max number of walker : \t\t\t%d\n",MAXWALKER);
	printf("Spawn rate of walker : \t\t\t%d%%\n",SPAWNRATE);
	printf("Dispawn rate of walker : \t\t%d%%\n",DISPAWNRATE);
	printf("Change direction rate of walker : \t%d%%\n",CHANGEDIR);

	printf("Number of iteration : \t\t\t%d\n",nbIte);
	printf("Number of iteration skipped : \t\t%d\n",nbSkip);

}


void changeConfig(void){

	char debugChange[10];
	int sizeChange;
	int fpsChange;
	int maxChange;
	int spawnChange;
	int dispawnChange;
	int directionChange;
	int iterationChange;
	int skippedChange;


	printf("Debug enable : (true/false)\t\t");
	scanf("%s%*c",debugChange);
	if(strcmp(debugChange,"true")==0){
		debug=TRUE;
	}
	else{
		debug=FALSE;
	}
	printf("\n");


	printf("Size map : \t\t\t\t");
	scanf("%d%*c",&sizeChange);
	if(sizeChange<1){
		sizeChange=10;
	}
	printf("\n");


	printf("Fps : \t\t\t\t\t");
	scanf("%d%*c",&fpsChange);
	if(fpsChange<1){
		fpsChange=30;
	}
	printf("\n");

	printf("Max number of walker : \t\t\t");
	scanf("%d%*c",&maxChange);
	if(maxChange<1){
		maxChange=20;
	}
	printf("\n");


	printf("Spawn rate of walker : \t\t\t");
	scanf("%d%*c",&spawnChange);
	if(spawnChange<0 || spawnChange>100){
		spawnChange=15;
	}
	printf("\n");

	printf("Dispawn rate of walker : \t\t");
	scanf("%d%*c",&dispawnChange);
	if(dispawnChange<0 || dispawnChange>100){
		dispawnChange=15;
	}
	printf("\n");

	printf("Change direction rate of walker : \t");
	scanf("%d%*c",&directionChange);
	if(directionChange<0 || directionChange>100){
		directionChange=15;
	}
	printf("\n");



	printf("Number of iteration : \t\t\t");
	scanf("%d%*c",&iterationChange);
	if(iterationChange<0){
		iterationChange=15;
	}
	printf("\n");

	printf("Number of iteration skipped : \t\t");
	scanf("%d%*c",&skippedChange);
	if(skippedChange<0){
		skippedChange=15;
	}
	printf("\n");




	HEIGHT=sizeChange;
	WIDTH=sizeChange;
	MS=(1.0/fpsChange)*1000000.0;
	MAXWALKER=maxChange;
	SPAWNRATE=spawnChange;
	DISPAWNRATE=dispawnChange;
	DISPAWNRATE=directionChange;
	nbIte=iterationChange;
	nbSkip=skippedChange;


}