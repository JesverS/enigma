#include "brute_force.h"

int verification_dictionnaire(const char *texte) {
    int score = 0;
    
    for (int i = 0; i < taille_dico; i++) {
        if (strstr(texte, dictionnaire[i]) != NULL) {
            score++;
        }
    }

    return score;
}

char* enigma_dechiffrer(const char *message, RotorsReflector* rotors_reflector) {
    // call a enigma 
    char *new_message = (char*)malloc(strlen(message) + 1); // Allocation dynamique
    int i = 0;
    while (message[i] != '\0')
    {
        // On verifie si la lettre est une lettre de l'alphabet
        if (message[i] < 'A' || message[i] > 'Z')
        {
            printf("La lettre %c n'est pas une lettre de l'alphabet\n", message[i]);
            return NULL;
        }
        // Etape 2 : On effectu un chiffrement avec les rotors et le reflecteur
        new_message[i] = resultat_rf(message[i], rotors_reflector);
        i++;
    }
    new_message[i] = '\0';
    return new_message;
}

void brute_force_enigma(const char *message_chiffre) {
    char *message_dechiffre;
    int rotor[3];
    char position[3];
    char c;
    RotorsReflector *rf = NULL;
    for (rotor[0] = 1; rotor[0] <= 5; rotor[0]++) { 
        for (rotor[1] = 1; rotor[1] <= 5; rotor[1]++) {
            for (rotor[2] = 1; rotor[2] <= 5; rotor[2]++) {
                for (position[0] = 'A'; position[0] <= 'Z'; position[0]++) {
                    for (position[1] = 'A'; position[1] <= 'Z'; position[1]++) {
                        for (position[2] = 'A'; position[2] <= 'Z'; position[2]++) {
                            for(c = 'A'; c <= 'C'; c++){
                                
                                // Créer une nouvelle configuration de rotors
                                if(rf == NULL){
                                    rf = init_rotorsreflector(rotor, position, c, 3);
                                }else{
                                   // printf("Reinitialisation des rotors\n");
                                    rf = modifier_rf(rf, rotor, position, c, 3);
                                }
                               
                                if (rf == NULL) {
                                    printf("Erreur lors de l'initialisation des rotors.\n");
                                    continue;
                                }

                                // Déchiffrer le message avec cette configuration
                                message_dechiffre = enigma_dechiffrer(message_chiffre, rf);

                                printf("Rotors: %d %d %d | Positions: %c %c %c | Reflector %c\n", 
                                    rotor[0], rotor[1], rotor[2], position[0], position[1], position[2], c);
                                printf("Message chiffré: %s\n", message_chiffre);
                                printf("Message déchiffré: %s\n", message_dechiffre);
                                
                                printf("\n\n");
                                // Vérifie si le résultat est valide
                                if (verification_dictionnaire(message_dechiffre) >= 2) {
                                    printf("Configuration trouvée !\n");
                                    printf("Rotors: %d %d %d | Positions: %c %c %c | Reflector %c\n", 
                                        rotor[0], rotor[1], rotor[2], position[0], position[1], position[2], c);
                                    printf("Message chiffré: %s\n", message_chiffre);
                                    printf("Message déchiffré: %s\n", message_dechiffre);
                                    
                                    return;
                                }
                                free(message_dechiffre);
                            }
                            return;
                        }
                    }
                }
            }
        }
        printf("Configuration %d %d %d non valide\n", rotor[0], rotor[1], rotor[2]);
    } 
    printf("Aucune configuration correcte trouvée \n");
}

int main(){
    // Exemple de message chiffré 
    // UNITEOPERATIONSTRATEGIE
    char *message = "RHLHOPYJWZHJTFARFNCPKRMAIOPFBIBFBFWQMIP";
    brute_force_enigma(message);
}