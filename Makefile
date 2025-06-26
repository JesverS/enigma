all: enigma bruteforce turing_version1 turing_version2 find_crib

enigma: enigma.o enigma_rotor.o
	gcc -Wall -o enigma enigma.o enigma_rotor.o

bruteforce: bute_force.o enigma_rotor.o
	gcc -Wall -o bruteforce bute_force.o enigma_rotor.o

turing_version1: turing_version1.o enigma_rotor.o
	gcc -Wall -o turing_version1 turing_version1.o enigma_rotor.o

turing_version2: turing_version2.o enigma_rotor.o
	gcc -Wall -O3 -march=native -o turing_version2 turing_version2.o enigma_rotor.o

find_crib: find_crib.c
	gcc -o find_crib find_crib.c

enigma.o: enigma.c enigma.h enigma_rotor.h
	gcc -Wall -c enigma.c -o enigma.o

enigma_rotor.o: enigma_rotor.c enigma_rotor.h
	gcc -Wall -c enigma_rotor.c -o enigma_rotor.o

bute_force.o: bute_force.c brute_force.h enigma_rotor.h
	gcc -Wall -c bute_force.c -o bute_force.o

turing_version1.o: turing_version1.c turing.h enigma_rotor.h
	gcc -Wall -c turing_version1.c -o turing_version1.o

turing_version2.o: turing_version2.c turing.h enigma_rotor.h
	gcc -Wall -c turing_version2.c -o turing_version2.o

clean:
	rm -f enigma.o enigma_rotor.o bute_force.o turing.o enigma bruteforce turing_version1 turing_version2 turing_version2.o

.PHONY: all clean run-enigma run-bruteforce run-turing
