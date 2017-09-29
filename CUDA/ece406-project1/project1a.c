#include<stdio.h>
#include <time.h>
#include <stdint.h>
#include <stdlib.h>
#include <math.h>

/* 
ASSUMING: 	array of numbers in numbers.txt is unsorted (doesn't need to be)
			there is no existing list of primes to compare divisibility against in the first place

TODO LIST:  
			
FIXED: 		can find primes 
			can read numbers from numbers.txt
			makefile works
			algorithm as optimized as possible (?)
			debug arg formatting 
			 */


FILE *fs;  //declare the file pointer file source (numbers.txt)
FILE *ft;  //declare the file pointer file target (primes.txt)
	
	
int main(int argc, char** argv){
	
	if(argc != 3)
	{
		printf("\n\nUsage: ./prime infile.txt outfile.txt");
		printf("\n\nExample: ./prime numbers.txt primes.txt\n\n");
		return 0;
	}
	//char c;
	double timer;

	int i = 0, j, isprime, size = 0, k = 0;
	int NumberOfPrimes = 0; //hard code SIZE of array???
	unsigned long long array[8192];
	unsigned long long Primes[8192];
	int PrimesIndex[8192];
		clock_t start,stop;
	
	start = clock();
	
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

	
	printf("size = %d\n", size);
	fclose(fs); //close the file 
	
	ft = fopen (argv[2],"w"); //open the file for writing
	if (ft == NULL){
		printf("Cannot open file ft!\n");
		//exit(1);
		return 0;
	}
	
	for (i = 0; i < size; i++){
		isprime = 1;

		if (array[i] % 2 == 0){ //is array[i] even? if so enter here
			//printf("is even\n");
			isprime = 0;
			//break;
		}
		else { //else enter here
			//printf("testing odd number\n");
			for (j = 3; j<(unsigned long long)(sqrt(array[i]) + 1); j = j + 2){ //is it divisible?

				if (array[i] % j == 0){
					//printf("is divisible by %d\n", j);
					isprime = 0;
					break;
				}
			}
		}

		

		//below here, errors errors everywhere
		
		if (isprime == 1){
            //printf("i = %d    NumberOfPrimes == %d    ", i, NumberOfPrimes);

			//printf("%llu is prime\n", array[i]);	
			Primes[NumberOfPrimes] = array[i];  //look here	
			PrimesIndex[NumberOfPrimes] = i; //look here

			//printf("PrimesIndex[%d] = %d\n",NumberOfPrimes,i);

			//printf("Primes[%d] == %llu\n", NumberOfPrimes, array[i]);
			//printf("Will write %d: %llu\n",PrimesIndex[NumberOfPrimes], Primes[NumberOfPrimes]);
			NumberOfPrimes++; //possible error here
		}	
	}
	printf("%d", NumberOfPrimes);
	
    stop = clock();
	timer = ((double)(stop - start))/((double) CLOCKS_PER_SEC);
	printf("Time: %f\n", timer);

	
	for (i = 0; i < NumberOfPrimes; i++){
		//printf("i == %d            ", i);
		//printf("Writing into text file %d: %llu\n",PrimesIndex[i] + 1, Primes[i]);
		//printf("The array of primeIndex is %d \n", PrimesIndex[i]);
		//printf("The array of primes is %llu\n", Primes[i]);
		fprintf(ft,"%d: %llu\n",PrimesIndex[i] + 1, Primes[i]); //error appears here
	}
	fclose(ft); //close the file

	
	return 0;
}
