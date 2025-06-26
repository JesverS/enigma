#ifndef TURING_H
#define TURING_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "enigma_rotor.h"
#include <stdbool.h>

#define MAX_MENU_LEN 100

typedef struct {
    char cipher;
    char plain;
} MenuEntry;

typedef struct {
    int start_index;         // indice dans le texte chiffré où commence le menu
    int length;              // taille = taille du crib
    MenuEntry entries[MAX_MENU_LEN];
} Menu;

int is_valid_crib_position(const char *message, const char *crib, int pos);
int* find_crib_positions(const char *message, const char *crib);

#endif
