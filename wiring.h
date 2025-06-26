#ifndef WIRING_H
#define WIRING_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

typedef struct inversion {
    char p_lettre;
    char s_lettre;
    struct inversion* suiv;
} Inversion;

#endif