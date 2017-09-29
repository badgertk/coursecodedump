#include <stdio.h>
#include <time.h>
#include <stdint.h>
#include <stdlib.h>
#include <math.h>
#include <pthread.h>

/* 
ASSUMING: 	array of numbers in numbers.txt is unsorted (doesn't need to be)
			there is no existing list of primes to compare divisibility against in the first place

TODO LIST:  race conditions, mutex lock/unlock doesn't fix segmentation fault(URGENT)
			
FIXED: 		serial version works
			can find primes 
			can read numbers from numbers.txt
			makefile works
			algorithm as optimized as possible (?)
			debug arg formatting
			 */


FILE *fs;  //declare the file pointer file source (numbers.txt)
FILE *ft;  //declare the file pointer file target (primes.txt)

#define MAXTHREADS 64

long 			NumThreads;
int				ThreadParam[MAXTHREADS];
pthread_t		ThreadHandle[MAXTHREADS];
pthread_attr_t	ThreadAttr;
pthread_mutex_t mutex_arrayread;// =  PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex_arraywrite;// =  PTHREAD_MUTEX_INITIALIZER;
int isprime;
volatile unsigned long long Primes[8192];
volatile int PrimesIndex[8192];
volatile unsigned long long array[8192];
int NumberOfPrimes = 0, size; 

int FindPrimes(unsigned long long num);
void* (*MTFindPrimesFunc) (void *arg);

int FindPrimes(unsigned long long num){
	/** 
	insert project1a.c stuff here
	**/
	int i, j;
	
		isprime = 1;
/* 		 printf("Array is: [");
		for (i = 0; i < size; i++){
			printf("%d:%llu ", i, array[i]);
		}
		printf("\\0]\n");  */
		//printf("testing %llu\n",num);
		if (num % 2 == 0){ //is array[i] even? if so enter here
			//printf("is even\n");
			return 0;
			//break;
		}
		else { //else enter here
			//printf("testing odd number\n");
			for (j = 3; j<(unsigned long long)(sqrt(num + 1)); j = j + 2){ //is it divisible?

				if (num % j == 0){
					//printf("is divisible by %d\n", j);
					isprime = 0;
					return 0;
				}
			}
			return 1;
		}

}

void *MTFindPrimes(void* threadID){
	
	int i = 0, j;
	isprime = 0;
	int ts = *((int *) threadID); //Thread ID
	ts *= size/NumThreads;
	int te = ts + size/NumThreads - 1;
	unsigned long long threadArray [size/NumThreads];
	
	//printf("ts - te = %d", ts - te);

/*  	for (i = 0; i < te - ts; i++){
	pthread_mutex_lock(&mutex_arrayread);
		printf("threadArray[%d] = %llu", ts + i, array[ts + i]);
		threadArray[i + ts] = array[ts + i];
	pthread_mutex_unlock(&mutex_arrayread);
	} */

	for (i = ts; i <= te; i++){
		//printf("i = %d", i);
		isprime = 1;
		if (array[i] % 2 == 0){ //is array[i] even? if so enter here
			//printf("array[%d] is even\n", i);
			isprime = 0;
			//break;
		}
		else { //else enter here
			//printf("testing odd number\n");
			for (j = 3; j<(unsigned long long)(sqrt(array[i]) + 1); j = j + 2){ //is it divisible?
				if (array[i] % j == 0){
					
					//printf("array[%d] is divisible by %d\n", i, j);
					isprime = 0;
					break;
				}
			}
		}
		if (isprime == 1){
            //printf("i = %d    NumberOfPrimes == %d    ", i, NumberOfPrimes);
			//pthread_mutex_lock(&mutex_arraywrite);
			//printf("%llu is prime\n", array[i]);	
			Primes[i] = array[i];  //look here	
			PrimesIndex[i] = i; //look here
			//pthread_mutex_unlock(&mutex_arraywrite);
			printf("PrimesIndex[%d] = %d\n",i,i);

			//printf("Primes[%d] == %llu\n", i, array[i]);
			//printf("Will write %d: %llu\n",PrimesIndex[i], Primes[i]);
		}
		else if (isprime == 0){
			//pthread_mutex_lock(&mutex_arraywrite);
			Primes[i] = 0;  //look here	
			PrimesIndex[i] = i; //look here
			//pthread_mutex_unlock(&mutex_arraywrite);
		}
	}
 
	pthread_exit(0);
}

int main(int argc, char** argv){
	int 	ThreadErr, i = 0, j = 0, isprime;
	double	timer;
	clock_t start,stop;
	start = clock();

	switch(argc){
		case 3 : NumThreads = 4; 
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
			//exit(1);
			return 0;
		}
		
   		while (fscanf(fs,"%llu", &array[i]) > 0){ 
			//printf("reading %llu\n", array[i]);
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
		if (NumThreads == 1){
			printf("Executing the serial version\n");
			for (i = 0; i < size; i++){
				isprime = FindPrimes(array[i]);
				if (isprime == 1){
					printf("i = %d    NumberOfPrimes == %d    ", i, NumberOfPrimes);

					printf("%llu is prime\n", array[i]);	
					Primes[NumberOfPrimes] = array[i];  //look here	
					PrimesIndex[NumberOfPrimes] = i; //look here
	
					printf("PrimesIndex[%d] = %d\n",NumberOfPrimes,i);

					printf("Primes[%d] == %llu\n", NumberOfPrimes, array[i]);
					printf("Will write %d: %llu\n",PrimesIndex[NumberOfPrimes], Primes[NumberOfPrimes]);
					NumberOfPrimes++; //possible error here
				}	
			}
			printf("FINISHED EXEC SERIAL VERSION");
		}

		if (NumThreads > 1){
			//pthread_mutex_init(&mutex_arrayread, NULL);
			//pthread_mutex_init(&mutex_arraywrite, NULL);
			printf("Executing the multithreaded version with %li threads.\n", NumThreads);
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
				printf("Joined Thread # %d", ThreadParam[i]);
				pthread_join(ThreadHandle[i], NULL);
			}
		
			//execute multithreaded version
			printf("FINISHED EXEC MULTITHREADED VERSION");
			printf("Array of primes is [");
			for (i = 0; i < size; i++){
				printf("%llu ", Primes[i]);
			}
			printf("\0]");
			pthread_attr_destroy(&ThreadAttr);
			//pthread_mutex_destroy(&mutex_arrayread);
			//pthread_mutex_destroy(&mutex_arraywrite);
		}
	}

	stop = clock();
	timer = ((double)(stop - start))/((double) CLOCKS_PER_SEC);
	printf("\n timer = %f\n", timer);
	//write to file
	if (NumThreads == 1){
		for (i = 0; i < NumberOfPrimes; i++){
			printf("i == %d            ", i);
			printf("Writing into text file %d: %llu\n",PrimesIndex[i] + 1, Primes[i]);
			//printf("The array of primeIndex is %d \n", PrimesIndex[i]);
			//printf("The array of primes is %llu\n", Primes[i]);
			fprintf(ft,"%d: %llu\n",PrimesIndex[i] + 1, Primes[i]);
		}
	}
	if (NumThreads > 1){
		NumberOfPrimes = 0;
		//for (j = 1; j <=NumThreads; j++){
		for (i = 0; i < size; i++){
			if (Primes[i] != 0){
				fprintf(ft,"%d: %llu\n",PrimesIndex[i] + 1, Primes[i]); 
				NumberOfPrimes++;
			}
		}
		//}

		printf("NumberOfPrimes = %d\n", NumberOfPrimes);
	}

	fclose(ft); //close the file
	
	//return 0;
	return (EXIT_SUCCESS);
}

