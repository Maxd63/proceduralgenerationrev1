#include "procedural.h"

extern Bool debug;

void errorMalloc(int line){
	if(debug==FALSE){
		return;
	}
	fprintf(stderr,RED "Error Malloc Line : %d\n" RESET,line);
}

void errorSeg(int line,char *name){
	if(debug==FALSE){
		return;
	}
	if(name != NULL){
		fprintf(stderr,RED "Seg Fault (%s) Line : %d\n" RESET,name,line);
	}
	else{
		fprintf(stderr,RED "Seg Fault (UNKNOWN) Line : %d\n" RESET,line);
	}
}

void errorCommon(int line,char *name){
	if(debug==FALSE){
		return;
	}
	if(name != NULL){
		fprintf(stderr,RED "\"%s\" Error Line : %d\n" RESET,name,line);
	}
	else{
		fprintf(stderr,RED "Error Line : %d\n" RESET,line);
	}
}