
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

int is_valid_crib_position(const char *message, const char *crib, int pos) {
    int crib_length = strlen(crib);
    // Vérifier si le message peut contenir le crible à la position donnée
    for (int i = 0; i < crib_length; i++) {
        if (message[pos + i] == crib[i]) {
            return 0; // Le crible ne correspond pas car la lettre à la position pos + i est égale à la lettre du crible 
        }
    }

    return 1; // Position valide
}


int* find_crib_positions(const char *message, const char *crib){
    int message_length = strlen(message);
    int crib_length = strlen(crib);
    int *positions = (int *)malloc((message_length-crib_length) * sizeof(int)); // dans le pire des cas, toutes les positions peuvent être valides
    int count = 0;

    if (positions == NULL) {
        printf("Erreur d'allocation mémoire\n");
        return NULL;
    }

    for (int i = 0; i <= message_length - crib_length; i++) {
        if (is_valid_crib_position(message, crib, i)) {
            positions[count] = i;
            count++;
        }
    }

    // Afficher les positions trouvées
    printf("Positions du crible \"%s\" dans le message :\n", crib);
    for (int i = 0; i < count; i++) {
        printf("Crib potentiel en position %d ", positions[i]);
    }
    printf("\n");

    return positions; // Retourne les positions valides
}

int main() {
    const char *message = "ZWAXSRTMKCMHCRISWEUTHQHN";
    const char *crib = "WETTERBERICHT";

    int *positions = find_crib_positions(message, crib);
    
    if (positions != NULL) {
        free(positions); // Libérer la mémoire allouée
    }

    return 0;
}