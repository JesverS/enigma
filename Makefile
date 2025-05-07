all: enigma bruteforce

enigma: enigma.o enigma_rotor.o
	gcc -std=c11 -Wall -Wextra -o enigma enigma.o enigma_rotor.o

bruteforce: bute_force.o enigma_rotor.o
	gcc -std=c11 -Wall -Wextra -o bruteforce bute_force.o enigma_rotor.o

enigma.o: enigma.c
	gcc -std=c11 -Wall -Wextra -c enigma.c -o enigma.o

enigma_rotor.o: enigma_rotor.c
	gcc -std=c11 -Wall -Wextra -c enigma_rotor.c -o enigma_rotor.o

bute_force.o: bute_force.c
	gcc -std=c11 -Wall -Wextra -c bute_force.c -o bute_force.o

clean:
	rm -f enigma.o enigma_rotor.o bute_force.o enigma bruteforce

run-enigma: enigma
	./enigma

run-bruteforce: bruteforce
	./bruteforce

.PHONY: all clean run-enigma run-bruteforce
