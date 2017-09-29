#include <stdio.h>
#include <time.h>
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

#define CHUNKSIZE 1024
#define MAXTHREADS 64

long NumThreads, ChunkSize;
int ThreadParam[MAXTHREADS];
pthread_t	ThreadHandle[MAXTHREADS];
pthread_attr_t	ThreadAttr;
pthread_mutex_t lock;
unsigned long long Primes[8192];
int PrimesIndex[8192];
unsigned long long array[8192];
int size;

void* (*MTFindPrimesFunc) (void *arg);

void *MTFindPrimes(void* threadID){
	
	int i = 0, j, k;
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
	int cs; //start of the chunk
	int ce; //end of the chunk

	for (i = ts; i < te; i = i + ChunkSize){ //go through each chunk
		cs = i;
		//printf("ts = %d te = %d\n", ts, te);
		//printf("cs = %d", cs);
		if ((te - ts + 1) % ChunkSize != 0){ //number of elements thread is assigned is not divisible by chunk size.  
			if ((te - cs) < ChunkSize){//last chunk will be smaller than chunk size.
				ce = te;
			}
			else{
				ce = cs + ChunkSize - 1;
			}
		}
		//printf("ce = %d\n", ce);
		//printf("i = %d", i);
		int NumberOfPrimes = 0;
		for (k = cs; k <= ce; k++){ //go through each element
			//start of algorithm
			isprime = 1;
			//printf("testing k == %d", k);
			if (array[k] % 2 == 0){ //is array[i] even? if so enter here
			//printf("array[%d] is even\n", i);
			isprime = 0;
			//break;
			}
			else { //else enter here
				//printf("testing odd number\n");
				for (j = 3; j<(unsigned long long)(sqrt(array[k]) + 1); j = j + 2){ //is it divisible?
					if (array[k] % j == 0){
						
						//printf("array[%d] is divisible by %d\n", i, j);
						isprime = 0;
						break;
					}
				}
			}
			if (isprime == 1){
			pthread_mutex_lock(&lock);
			//printf("i = %d    NumberOfPrimes == %d    ", k, NumberOfPrimes);
			//printf("%llu is prime\n", array[k]);
			Primes[NumberOfPrimes + cs] = array[k];
			PrimesIndex[NumberOfPrimes + cs] = k;
			//printf("PrimesIndex[%d] = %d\n",NumberOfPrimes + cs,k);

			//printf("Primes[%d] == %llu\n", NumberOfPrimes + cs, array[k]);
			//printf("Will write %d: %llu\n",PrimesIndex[NumberOfPrimes + cs], Primes[NumberOfPrimes + cs]);
			NumberOfPrimes++;
			pthread_mutex_unlock(&lock);
			}
			//end of algorithm
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
		case 5 : NumThreads = atoi(argv[3]);
		ChunkSize = atoi(argv[4]);
		printf("CASE = 5");
		break;
		default : printf("\n\nUsage: ./primePC infile.txt outfile.txt numThreads ChunkSize\nNOTE: ChunkSize must be a factor of 2.\n");
		printf("\n\nExample: ./primePC numbers.txt primes.txt 4 1024\n\n");
		return 0;
	}
	
	if (NumThreads > 64 || NumThreads < 1){
		printf("The number of threads is : %l", NumThreads);
		printf("The number of threads requested is not valid.  It can support anywhere from 1 to %u threads.\n", MAXTHREADS);
		exit(EXIT_FAILURE);
	}
	else if (ChunkSize > 1024 || ChunkSize < 1){
		printf("The chunk size is : %l", ChunkSize);
		printf("The chunk size requested is not valid.  It can support anywhere from 1 to %u threads.\n", MAXTHREADS);
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
		//adjustments for uneven partitioning
		if (size % NumThreads != 0){
			printf("Cannot divide task evently among threads.  Each thread will receive %d .  The last thread will receive less. \n", size/NumThreads + 1);
		}
		if ((ChunkSize * NumThreads) > size){
			if (size % NumThreads != 0){
				ChunkSize = size / NumThreads + 1;
			}
			else{
				ChunkSize = size / NumThreads;
			}
			printf("Chunk size too big, corrected to %d", ChunkSize);
				
		}
		
		if (NumThreads >= 1){
			pthread_mutex_init(&lock, NULL);
			if (NumThreads == 1){
				printf("Executing the serial version\n");
			}
			else{
				printf("Executing the multithreaded version with %li threads and %li sized chunks.\n", NumThreads, ChunkSize);
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
				printf("Joined Thread # %d", ThreadParam[i]);
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

