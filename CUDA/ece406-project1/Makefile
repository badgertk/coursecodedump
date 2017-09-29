prime		:	project1a.c
			gcc project1a.c -o prime -lm

primeP		:	project1b.c
			gcc -pthread project1b.c -o primeP -lm

primePC		: 	project1c.c
			gcc -pthread project1c.c -o primePC -lm
      
clean		:
			rm -v prime
			rm -v primeP
			rm -v primePC