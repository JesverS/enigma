#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <fcntl.h>
#include <assert.h>

typedef struct _rotor{
	int pos; //position de l'élément dans la chaine de charactère qui est en position 0 du rotor.
	char* value; //tableau des charactères du rotor.
	char notch; //charactère permettant de savoir quand le rotor a fait un tour complet.
	char turnover; //valeur visible quand le charactère turnover est en position 0 de la liste. NON UTILISE ACTUELLEMENT
} Rotor;

//char reflector[

typedef struct _RotorsReflector{
	Rotor** l_r; //tableau des rotors (3 avec Enigma1)
	char* reflector; //le réflecteur
	int taille;
} RotorsReflector;

Rotor* init_rotor(int num_rotor, char premiere_lettre);
void liberer_rf(RotorsReflector* rf);
RotorsReflector* init_rotorsreflector(int* nums_rotors, char* lettres_associes, char lettre_reflector, int taille);
Rotor* init_rotor(int num_rotor, char premiere_lettre);
int trouver_pos(char* AlphabetM, char l);
char resultat_rf(char c, RotorsReflector* rf);
RotorsReflector* modifier_rf(RotorsReflector* rf,int* nums_rotors, char* lettres_associes, char lettre_reflector,int taille);
Rotor* modifier_rotor(Rotor* r,int num_rotor, char lettre);
