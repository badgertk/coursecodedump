#include <stdio.h>
#include <time.h>

#define CONS 12
int main(int argc, char** argv){
	
	double time;
	int i;
	int one,two,three,four,five,six,seven,eight,nine,ten;
	clock_t start,stop;
	start = clock();
	
	for (i = 0; i < 10000; i++){
		one = CONS * 1;
		two = CONS * 2;
		three = CONS * 3;
		four = CONS * 4;
		five = CONS * 5;
		six = CONS * 6;
		seven = CONS * 7;
		eight = CONS * 8;
		nine = CONS * 9;
		ten = CONS * 10;
	}
	
	printf("one = %d\n", one);
	printf("two = %d\n", two);
	printf("three = %d\n", three);
	printf("four = %d\n", four);
	printf("five = %d\n", five);
	printf("six = %d\n", six);
	printf("seven = %d\n", seven);
	printf("eight = %d\n", eight);
	printf("nine = %d\n", nine);
	printf("ten = %d\n", ten);
	
	stop = clock();
	time = ((double)(stop - start))/((double) CLOCKS_PER_SEC);
	printf("time = %f\n", time);
	getchar();
	return 0;
}