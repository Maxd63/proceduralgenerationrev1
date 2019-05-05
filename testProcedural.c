#include "procedural.h"


extern int nbWalker;

extern int HEIGHT;
extern int WIDTH;
extern int MS;


extern int nbIte;
extern int nbSkip;

void proceduralGenerator(void){
	char select;

	Case ** map;

	map=newMap();

	select=choixMenu();
	while(select!='9'){
		switch(select){
			case '1':startGeneration(map);
			break;
			case '2':config();
			break;
			case '3':dispMapMenu(map);
			break;
			case '4':saveToFile(map);
			break;
			case '5':multipleGen(map);
			break;
			case '6':dispCredit();
			break;
			default:
			break;

		}
		select=choixMenu();
	}

	unNewMap(map);
	printf("\n\n");

}












int main(void){
	
	srand(time(0));

	proceduralGenerator();
	
	return 0;
}


