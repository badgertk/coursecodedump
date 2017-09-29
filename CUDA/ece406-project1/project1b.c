#include <stdio.h>
#include <time.h>
#include <stdint.h>
#include <stdlib.h>
#include <math.h>
#include <pthread.h>

/* 
ASSUMING: 	array of numbers in numbers.txt is unsorted (doesn't need to be)
			there is no existing list of primes to compare divisibility against in the first place

TODO LIST:  
			
FIXED: 		serial version works
			can find primes 
			can read numbers from numbers.txt
			makefile works
			algorithm as optimized as possible (?)
			debug arg formatting
			race conditions
			 */


FILE *fs;  //declare the file pointer file source (numbers.txt)
FILE *ft;  //declare the file pointer file target (primes.txt)

#define MAXTHREADS 64

long 			NumThreads;
int				ThreadParam[MAXTHREADS];
pthread_t		ThreadHandle[MAXTHREADS];
pthread_attr_t	ThreadAttr;
pthread_mutex_t lock;

unsigned long long Primes[8192];
int PrimesIndex[8192];
unsigned long long array[8192];
int size; 

//void FindPrimes(int i);
void* (*MTFindPrimesFunc) (void *arg);

void *MTFindPrimes(void* threadID){
	int i = 0, j;
	int NumberOfPrimes = 0;
	int isprime = 0;
	int tid = *((int *) threadID); //Thread ID
	int ts; //start of thread
	int te; //end of thread
	
	if (size % NumThreads != 0){
		ts = tid * (size/NumThreads + 1);
		if (tid == (NumThreads - 1)){ //last thread will be assigned fewer elements
			te = size - 1;
		}
		else {
			te = ts + size/NumThreads;
		}
	}
	else {
		ts = tid * size/NumThreads;
		te = ts + size/NumThreads;
	}
	
	for (i = ts; i <= te; i++){
		//printf("i = %d", i);
		isprime = 1;
		if (array[i] % 2 == 0){ //is array[i] even? if so enter here
			isprime = 0;
		}
		else { //else enter here
			//printf("testing odd number\n");
			for (j = 3; j<(unsigned long long)(sqrt(array[i]) + 1); j = j + 2){ //is it divisible?
				if (array[i] % j == 0){
					isprime = 0;
					break;
				}
			}
		}
		if (isprime == 1){
	
			pthread_mutex_lock(&lock);
			//printf("i = %d    NumberOfPrimes == %d    ", i, NumberOfPrimes);
			//printf("%llu is prime\n", array[i]);
			Primes[NumberOfPrimes + ts] = array[i];
			PrimesIndex[NumberOfPrimes + ts] = i;
			//printf("PrimesIndex[%d] = %d\n",NumberOfPrimes + ts,i);

			//printf("Primes[%d] == %llu\n", NumberOfPrimes + ts, array[i]);
			//printf("Will write %d: %llu\n",PrimesIndex[NumberOfPrimes + ts], Primes[NumberOfPrimes + ts]);
			NumberOfPrimes++;
			pthread_mutex_unlock(&lock);

		}
	}
 
	pthread_exit(0);
}

int main(int argc, char** argv){
	int 	ThreadErr, i = 0, j = 0, isprime, NumberOfPrimes;
	double	timer;
	clock_t start,stop;
	start = clock();

	switch(argc){
		case 3 : NumThreads = 4; //if number of threads is not specified, default to 4
		printf("CASE = 3");
		break;
		case 4 : NumThreads = atoi(argv[3]); 
		printf("CASE = 4");
		break;
		default : printf("\n\nUsage: ./primeP infile.txt outfile.txt numThreads");
		printf("\n\nExample: ./primeP numbers.txt primes.txt 4\n\n");
		return 0;
	}
	
	if (NumThreads > 64 || NumThreads < 1){
		printf("The number of threads is : %l", NumThreads);
		printf("The number of threads requested is not valid.  It can support anywhere from 1 to %u threads.\n", MAXTHREADS);
		exit(EXIT_FAILURE);
	}
	else{ //actually execute
		 //read the file
 		fs = fopen (argv[1], "r"); //open the file for reading
		if (fs == NULL){
			printf("Cannot open file fs!\n");
			return 0;
		}
		
   		while (fscanf(fs,"%llu", &array[i]) > 0){ 
			i++;
			size++;
		if (fscanf(fs,"%llu", &array[i]) > 0){;
			i++;
			size++;
		}
		else{
			break;
		}

		}
		
		printf("size = %d", size);
		fclose(fs); //close the file 
	
	ft = fopen (argv[2],"w"); //open the file for writing
	if (ft == NULL){
		printf("Cannot open file ft!\n");
		//exit(1);
		return 0;
	}
		//end of read file snippet
		//adjustments for uneven partitioning
		if (size % NumThreads != 0){
			printf("Cannot divide task evently among threads.  Each thread will receive %d .  The last thread will receive less. \n", size/NumThreads + 1);
		}

		if (NumThreads >= 1){
			pthread_mutex_init(&lock, NULL);
			if (NumThreads == 1){
				printf("Executing the serial version\n");
			}
			else{
				printf("Executing the multithreaded version with %li threads.\n", NumThreads);
			}
			pthread_attr_init(&ThreadAttr);
			pthread_attr_setdetachstate(&ThreadAttr, PTHREAD_CREATE_JOINABLE);
			for(i=0; i<NumThreads; i++){
				ThreadParam[i] = i;
				ThreadErr = pthread_create(&ThreadHandle[i], &ThreadAttr, MTFindPrimes, (void *)&ThreadParam[i]);
				if(ThreadErr != 0){
					printf("\nThread Creation Error %d. Exiting abruptly... \n",ThreadErr);
					exit(EXIT_FAILURE);
				}
			}

			
			for(i=0; i<NumThreads; i++){
				printf("Joined Thread # %d\n", ThreadParam[i]);
				pthread_join(ThreadHandle[i], NULL);
			}
		
			printf("FINISHED EXECUTION\n");
			pthread_attr_destroy(&ThreadAttr);
			pthread_mutex_destroy(&lock);

		}
	}

	stop = clock();
	timer = ((double)(stop - start))/((double) CLOCKS_PER_SEC);
	printf("\n timer = %f\n", timer);
	//write to file
	if (NumThreads >= 1){
		NumberOfPrimes = 0;
			for (i = 0; i < size; i++){
				if (Primes[i] != 0){
					fprintf(ft,"%d: %llu\n",PrimesIndex[i] + 1, Primes[i]); 
					NumberOfPrimes++;
				}
			}

		printf("NumberOfPrimes = %d\n", NumberOfPrimes);
	}

	fclose(ft); //close the file
	
	//return 0;
	return (EXIT_SUCCESS);
}

