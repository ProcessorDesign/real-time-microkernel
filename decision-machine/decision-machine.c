#include <stdio.h>
#include <stdlib.h>

//const int IL_PRZEB=3;
//const int IL_URZ=4;
#define IL_URZ 4
#define IL_PRZEB 3

typedef struct _device {
	int state;
} DEVICE;

typedef struct _przebieg {
	DEVICE tab[IL_URZ];
	int wyklucza[IL_PRZEB];
} PRZEBIEG;

PRZEBIEG tabPrzebiegow[IL_PRZEB];
int wykluczenia[IL_PRZEB]={0};
int utwierdzone[IL_PRZEB]={0};


void init() {
	int i,j,states[IL_URZ];
	FILE *f = fopen("przebiegi.cfg","r");
	for(i=0;i<IL_PRZEB;i++) {
		for(j=0;j<IL_URZ;j++) {
		fscanf(f,"%d",&tabPrzebiegow[i].tab[j]);
	}
	}
	fclose(f);
}


int main(int argc, int argv[]) {

	int quit=0;
	char str[25];

	//wczytac przebiegu z pliku conf
	//utwierdzic przebiegi podane z klawiatury

	init();
	
	while(!quit) {

	printf("%% ");
	scanf("%s",str);

	if(!strcmp(str,"exit")) { 
		printf("exiting....\n");
		quit=1;
	 }

	

	}

	return 0;
}

