#include "enigma_rotor.h"

// === Données statiques ===
const char* ROTOR_VALUES[5] = {
    "EKMFLGDQVZNTOWYHXUSPAIBRCJ", // rotor1
    "AJDKSIRUXBLHWTMCQGZNPYFVOE", // rotor2
    "BDFHJLCPRTXVZNYEIWGAKMUSQO", // rotor3
    "ESOVPZJAYQUIRHXLNFTGKDCMWB", // rotor4
    "VZBRGITYUPSDNHLXAWMJQOFECK"  // rotor5
};

const char ROTOR_NOTCH[5]    = {'Y', 'M', 'D', 'R', 'H'};
const char ROTOR_TURNOVER[5] = {'Q', 'E', 'V', 'J', 'Z'};

const char* REFLECTORS[3] = {
    "EJMZALYXVBWFCRQUONTSPIKHGD", // UKW-A
    "YRUHQSLDPXNGOKMIEBFZCWVJAT", // UKW-B
    "FVPJIAOYEDRZXWGCTKUQSBNMHL"  // UKW-C
};


// === Fonctions principales ===

Rotor* init_rotor(int num_rotor, char lettre) {
    if (num_rotor < 1 || num_rotor > 5) return NULL;

    Rotor* new_rotor = (Rotor*) malloc(sizeof(Rotor));
    assert(new_rotor);

    new_rotor->value = strdup(ROTOR_VALUES[num_rotor - 1]);
    new_rotor->notch = ROTOR_NOTCH[num_rotor - 1];
    new_rotor->turnover = ROTOR_TURNOVER[num_rotor - 1];
    new_rotor->pos = ((int) lettre - 'A') % 26;

    return new_rotor;
}

RotorsReflector* init_rotorsreflector(int* nums_rotors, char* lettres_associes, char lettre_reflector, int taille) {
    if (lettre_reflector < 'A' || lettre_reflector > 'C') return NULL;

    RotorsReflector* rf = (RotorsReflector*) malloc(sizeof(RotorsReflector));
    rf->taille = taille;
    rf->l_r = (Rotor**) malloc(taille * sizeof(Rotor*));
    rf->reflector = strdup(REFLECTORS[lettre_reflector - 'A']);

    for (int i = 0; i < taille; i++) {
        if (nums_rotors[i] < 1 || nums_rotors[i] > 5 ||
            lettres_associes[i] < 'A' || lettres_associes[i] > 'Z') {
            liberer_rf(rf);
            return NULL;
        }
        rf->l_r[i] = init_rotor(nums_rotors[i], lettres_associes[i]);
    }

    return rf;
}

int trouver_lettre(char* alphabet, char l, int pos) {
    for (int i = 0; i < 26; i++) {
        if (alphabet[(i + pos) % 26] == l) return i;
    }
    return 0;
}

char resultat_rf(char c, RotorsReflector* rf) {
    int position_lettre;
    char v;
    Rotor** l_r = rf->l_r;

    // Avancement du rotor
    l_r[0]->pos = (l_r[0]->pos + 1) % 26;
    int i = 0;
    while ((int) l_r[i]->pos == (int)(l_r[i]->turnover - 'A' + 1) && i < 2) {
        l_r[i + 1]->pos = (l_r[i + 1]->pos + 1) % 26;
        i++;
    }

    // Passage direct
    for (i = 0; i < rf->taille; i++) {
        position_lettre = ((int)(c - 'A') + l_r[i]->pos) % 26;
        c = l_r[i]->value[position_lettre];
        c = (c - l_r[i]->pos - 'A' + 52) % 26 + 'A';
    }

    // Réflecteur
    c = rf->reflector[(int)(c - 'A')];

    // Retour arrière
    for (i = rf->taille - 1; i >= 0; i--) {
        v = (c + l_r[i]->pos - 'A') % 26 + 'A';
        position_lettre = trouver_lettre(l_r[i]->value, v, 0);
        c = 'A' + (position_lettre - l_r[i]->pos + 26) % 26;
    }

    return c;
}

void liberer_rf(RotorsReflector* rf) {
    if (!rf) return;
    if (rf->l_r) {
        for (int i = 0; i < rf->taille; i++) {
            if (rf->l_r[i]) {
                free(rf->l_r[i]->value);
                free(rf->l_r[i]);
            }
        }
        free(rf->l_r);
    }
    free(rf->reflector);
    free(rf);
}

Rotor* modifier_rotor(Rotor* r, int num_rotor, char lettre) {
    if (!r || num_rotor < 1 || num_rotor > 5) return NULL;
    free(r->value);
    r->value = strdup(ROTOR_VALUES[num_rotor - 1]);
    r->notch = ROTOR_NOTCH[num_rotor - 1];
    r->turnover = ROTOR_TURNOVER[num_rotor - 1];
    r->pos = ((int) lettre - 'A') % 26;
    return r;
}

RotorsReflector* modifier_rf(RotorsReflector* rf, int* nums_rotors, char* lettres_associes, char lettre_reflector, int taille) {
    if (!rf || lettre_reflector < 'A' || lettre_reflector > 'C') return NULL;

    free(rf->reflector);
    rf->reflector = strdup(REFLECTORS[lettre_reflector - 'A']);
    rf->taille = taille;

    for (int i = 0; i < taille; i++) {
        rf->l_r[i] = modifier_rotor(rf->l_r[i], nums_rotors[i], lettres_associes[i]);
    }

    return rf;
}

void affiche_rotor(RotorsReflector* rf) {
    printf("=== Configuration des rotors ===\n");
    for (int i = 0; i < rf->taille; i++) {
        Rotor* r = rf->l_r[i];
        printf("Rotor %d :\n", i + 1);
        printf("  Valeur   : %s\n", r->value);
        printf("  Position : %d (%c)\n", r->pos, 'A' + r->pos);
        printf("  Turnover : %c\n", r->turnover);
    }

    printf("\n=== Réflecteur ===\n");
    printf("  Alphabet du réflecteur : %s\n", rf->reflector);
    printf("===============================\n");
}