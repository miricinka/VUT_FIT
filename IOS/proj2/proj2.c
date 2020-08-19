/* ==========================================================================*/
/*                                                                           */
/*   Nazev programu: proj2.c                                                 */
/*   Popis programu: Synchronizacni problem Faneuil Hall Problem             */
/*   Autor:          Mirka Kolarikova                                        */
/*   Datum:          16.4.2020                                               */
/*                                                                           */
/* ==========================================================================*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include <semaphore.h>
#include <unistd.h>
#include <sys/mman.h>
#include <semaphore.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>

/* map and unmap shared variables */
#define MMAP(ptr) {(ptr) = mmap(NULL, sizeof(*(ptr)), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);}
#define UNMAP(ptr) {munmap((ptr), sizeof((ptr)));}

/* semaphores */
sem_t *sem1 = NULL;
sem_t *no_judge_sem = NULL;
sem_t *mutex_sem = NULL;
sem_t *signedin_sem = NULL;
sem_t *confirmed_sem = NULL;
sem_t *poradnik_sem = NULL;

/* shared memory variables */
int* var1 = NULL;
int* var_entered = NULL;
int* var_checked = NULL;
int* var_in_building = NULL;
int* judge_in = NULL;

/* shared memory FILE */
FILE *output;


/* incorrect entry error function */
void error_exit(){
	fprintf(stderr,"Error 1: Incorrect entry.\nHelp: proj2 PI IG JG IT JT.\n");
	exit(1);
}

/* validates command line arguments */
/* returns 0 if ok, 1 in case of error */
int argument_check(int argc, char *argv[]){
	//must be prog2 PI IG JG IT JT
	if(argc != 6){
		return 1;
	}
    //check if passed argument is a positive integer
	for(int i = 1; i < 6; i++){
		char *znaky;
		int num = strtol(argv[i], &znaky, 10);
		if(strlen(znaky)>0){return 1;}
		if(i == 1){ //PI >=1
			if(num < 1){return 1;}
		}
		else{ //IG JG IT JT 
			if(num > 2000 || num < 0){return 1;}
		}
	}

return 0;
}

/* inicializes shared variables and semaphores */
int init(){
	//open output file proj2.out
	output = fopen("proj2.out", "w");
	//init shared variables
	MMAP(var1);
	*var1 = 1;
	MMAP(var_entered);
	*var_entered = 0;
	MMAP(var_checked);
	*var_checked = 0;
	MMAP(var_in_building);
	*var_in_building = 0;
	MMAP(judge_in);
	*judge_in = 0;
	//init semaphores
	if ((sem1 = sem_open("/xkolar76.ios.sem1", O_CREAT | O_EXCL, 0666, 1)) == SEM_FAILED) {
        fprintf(stderr, "Error: Opening sem1 semaphore failed.\n");
        return 1;
    }
    if ((no_judge_sem = sem_open("/xkolar76.ios.no_judge_sem", O_CREAT | O_EXCL, 0666, 1)) == SEM_FAILED) {
        fprintf(stderr, "Error: Opening no_judge semaphore failed.\n");
        perror("open no_judge_sem");
        return 1;
    }
    if ((mutex_sem = sem_open("/xkolar76.ios.mutex_sem", O_CREAT | O_EXCL, 0666, 1)) == SEM_FAILED) {
        fprintf(stderr, "Error: Opening mutex semaphore failed.\n");
        return 1;
    }
    if ((signedin_sem = sem_open("/xkolar76.ios.signedin_sem", O_CREAT | O_EXCL, 0666, 0)) == SEM_FAILED) {
        fprintf(stderr, "Error: Opening signedin_sem semaphore failed.\n");
        return 1;
    }
    if ((confirmed_sem = sem_open("/xkolar76.ios.confirmed_sem", O_CREAT | O_EXCL, 0666, 0)) == SEM_FAILED) {
        fprintf(stderr, "Error: Opening confirmed semaphore failed.\n");
        return 1;
    }
    if ((poradnik_sem = sem_open("/xkolar76.ios.poradnik_sem", O_CREAT | O_EXCL, 0666, 1)) == SEM_FAILED) {
        fprintf(stderr, "Error: Opening poradnik_sem semaphore failed.\n");
        return 1;
    }
 return 0;
}

/* deletes shared variables, closes and unlinks semaphpres */
void clean_all(){
	//clean shared variables
	UNMAP(var1);
	UNMAP(var_entered);
	UNMAP(var_checked);
	UNMAP(var_in_building);
	UNMAP(judge_in);
	//clean semaphores
	sem_close(sem1);
	sem_unlink("/xkolar76.ios.sem1");
	sem_close(no_judge_sem);
	sem_unlink("/xkolar76.ios.no_judge_sem");
	sem_close(mutex_sem);
	sem_unlink("/xkolar76.ios.mutex_sem");
	sem_close(signedin_sem);
	sem_unlink("/xkolar76.ios.signedin_sem");
	sem_close(confirmed_sem);
	sem_unlink("/xkolar76.ios.confirmed_sem");
	sem_close(poradnik_sem);
	sem_unlink("/xkolar76.ios.poradnik_sem");
	//close outputfile
	fclose(output);
}

/* generates PI immigrants */
void immigrant_generator(int PI, int IG, int IT){

	//generates PI of immigrants
	for(int i = 1; i <= PI; i++){
		
		//generates in <0,IG> time
		usleep((rand() % (IG + 1)) * 1000 );

		if (fork() == 0){
			//start
			sem_wait(poradnik_sem);
			setbuf(output, NULL);
			fprintf(output,"%d: IMM %d: starts\n",*var1, i);
			*var1 = *var1 + 1;
			sem_post(poradnik_sem);

			sem_wait(no_judge_sem);
			sem_wait(poradnik_sem);
			*var_in_building = *var_in_building + 1;
			*var_entered = *var_entered + 1;

			//enter
			setbuf(output, NULL);
			fprintf(output,"%d: IMM %d: enters: %d: %d: %d\n",*var1, i, *var_entered, *var_checked, *var_in_building);
			*var1 = *var1 + 1;
			sem_post(poradnik_sem);
			sem_post(no_judge_sem);

			//check
			sem_wait(mutex_sem);
			sem_wait(poradnik_sem);
			*var_checked = *var_checked + 1;
			setbuf(output, NULL);
			fprintf(output,"%d: IMM %d: checks: %d: %d: %d\n",*var1, i, *var_entered, *var_checked, *var_in_building);
			*var1 = *var1 + 1;
			sem_post(poradnik_sem);

			if(*judge_in == 1 && (*var_checked == *var_entered)){
				sem_post(signedin_sem);
			}else{
				sem_post(mutex_sem);
			}

			//wait until confirmed
			//want certificate
			sem_wait(confirmed_sem);
			sem_wait(poradnik_sem);
			setbuf(output, NULL);
			fprintf(output,"%d: IMM %d: wants certificate: %d: %d: %d\n",*var1, i, *var_entered, *var_checked, *var_in_building);
			*var1 = *var1 + 1;
			sem_post(poradnik_sem);
				
			usleep((rand() % (IT + 1)) * 1000 );

			//get certifikate
			sem_wait(poradnik_sem);
			setbuf(output, NULL);
			fprintf(output,"%d: IMM %d: got certificate: %d: %d: %d\n",*var1, i, *var_entered, *var_checked, *var_in_building);
			*var1 = *var1 + 1;
			sem_post(poradnik_sem);

			//leave building
			sem_wait(no_judge_sem);
			sem_wait(poradnik_sem);
			*var_in_building = *var_in_building -1;
			setbuf(output, NULL);
			fprintf(output,"%d: IMM %d: leaves: %d: %d: %d\n",*var1, i, *var_entered, *var_checked, *var_in_building);
			*var1 = *var1 + 1;
			sem_post(poradnik_sem);
			sem_post(no_judge_sem);

            exit(0);
		}
	}
	for(int i=0;i<PI;i++){
    	wait(NULL);
    }
}

/* JUDGE process */
void judge_process(int judgeTODO, int JG, int JT){
	while(judgeTODO > 0){//enters again until all done

			usleep((rand() % (JG + 1)) * 1000 );

			//wants to enter
			sem_wait(no_judge_sem);
			sem_wait(mutex_sem);
			sem_wait(poradnik_sem);
			setbuf(output, NULL);
			fprintf(output,"%d: JUDGE: wants to enter\n", *var1);
			*var1 = *var1 + 1;
			sem_post(poradnik_sem);

			//enters
			sem_wait(poradnik_sem);
			setbuf(output, NULL);
			fprintf(output,"%d: JUDGE: enters: %d: %d: %d\n",*var1, *var_entered, *var_checked, *var_in_building);
			*var1 = *var1 + 1;
			sem_post(poradnik_sem);
			*judge_in =  1;

			//wait for immigrants
			if(*var_entered > *var_checked){
				//add semaphore
				sem_wait(poradnik_sem);
				setbuf(output, NULL);
				fprintf(output,"%d: JUDGE: waits for IMM: %d: %d: %d\n",*var1, *var_entered, *var_checked, *var_in_building);
				*var1 = *var1 + 1;
				sem_post(poradnik_sem);
				sem_post(mutex_sem);
				sem_wait(signedin_sem);
			}

			//start confirmation
			sem_wait(poradnik_sem);
			setbuf(output, NULL);
			fprintf(output,"%d: JUDGE: starts confirmation: %d: %d: %d\n",*var1, *var_entered, *var_checked, *var_in_building);
			*var1 = *var1 + 1;
			sem_post(poradnik_sem);

			judgeTODO = judgeTODO - *var_checked;

			//sleeps random time <0,JT>
			usleep((rand() % (JT + 1)) * 1000 );

			//end confirmation
			sem_wait(poradnik_sem);
			int checked = *var_checked;
			*var_checked = 0;
			*var_entered = 0;
			setbuf(output, NULL);
			fprintf(output,"%d: JUDGE: ends confirmation: %d: %d: %d\n",*var1, *var_entered, *var_checked, *var_in_building);
			*var1 = *var1 + 1;
			sem_post(poradnik_sem);

			for( int y = 0; y < checked; y++){
				sem_post(confirmed_sem);
			}


			//sleeps random time <0,JT> before leaving
			usleep((rand() % (JT + 1)) * 1000 );


			//leave building
			sem_wait(poradnik_sem);
			setbuf(output, NULL);
			fprintf(output,"%d: JUDGE: leaves: %d: %d: %d\n",*var1, *var_entered, *var_checked, *var_in_building);
			*var1 = *var1 + 1;
			sem_post(poradnik_sem);

			*judge_in =  0;
			sem_post(mutex_sem);
			sem_post(no_judge_sem);
		}
		//finishes
		sem_wait(poradnik_sem);
		setbuf(output, NULL);
		fprintf(output,"%d: JUDGE: finishes\n",*var1);
			*var1 = *var1 + 1;
		sem_post(poradnik_sem);
}

/* main */
int main(int argc, char *argv[]){

	//passed arguments validation
	int not_ok = argument_check(argc, argv);
	if(not_ok != 0){error_exit();}

	if(init() == 1){
		clean_all();
		return -1;
	}

	//randomize random
	srand(time(0));


	//fork immigrant process and judge process
	pid_t p1_immigrant, p2_judge;
	bool a = (p1_immigrant = fork()) && (p2_judge = fork());
	a = a + 1;

	if((p1_immigrant < 0) || (p2_judge < 0)){
		perror("fork"); 
        exit(2);
	}
	if(p1_immigrant == 0){ 
		//code for immigrant process
		immigrant_generator(atoi(argv[1]), atoi(argv[2]), atoi(argv[4]));
		exit(0);
	}else if(p2_judge == 0){ 
		//code for judge process
		judge_process(atoi(argv[1]), (atoi(argv[3])), (atoi(argv[5])));
		exit(0);
		
	}
	//main waits for immigrat generator and judge to be done
	wait(NULL);
	wait(NULL);
	clean_all();
	return 0;
}


