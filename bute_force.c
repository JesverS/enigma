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

    int tab[] = {1,1,1};
	char* pos = strdup("AAA");
	char reflector = 'A';
	RotorsReflector* rf = init_rotorsreflector(tab, pos,reflector,3);
	for(int reflect=0; reflect<3;reflect++){
		reflector = 'A'+reflect;
		for(int rotor3 = 1; rotor3<=5;rotor3++){
			tab[2] = rotor3;
			for(int rotor2 = 1; rotor2<=5;rotor2++){
				tab[1] = rotor2;
				for(int rotor1 = 1; rotor1<=5;rotor1++){
					tab[0] = rotor1;
					for(int pos3 = 'A'; pos3<'A'+27;pos3++){
                        pos[2] = pos3;
						for(int pos2 = 'A' ; pos2 <'A'+27;pos2++){
                            pos[1] = pos2;
							for(int pos1 = 'A' ; pos1<'A'+27;pos1++){
                                pos[0] = pos1;
                                rf = modifier_rf(rf,tab,pos,reflector,3);
                                if (rf == NULL) {
                                    printf("Erreur lors de l'initialisation des rotors.\n");
                                    continue;
                                }

                                // Déchiffrer le message avec cette configuration
                                message_dechiffre = enigma_dechiffrer(message_chiffre, rf);

                                rf = modifier_rf(rf,tab,pos,reflector,3);
                               
                                printf("Message chiffré: %s\n", message_chiffre);
                                printf("Message déchiffré: %s\n", message_dechiffre);
                                printf("Rotors: %d %d %d | Positions: %c %c %c | Reflector %c\n", 
                                    rotor1, rotor2, rotor3, pos[0], pos[1], pos[2], reflector);
								rf = modifier_rf(rf,tab,pos,reflector,3);

                                if(pos[0] == 'I' && pos[1] == 'O' && pos[2] == 'P') {
                                    //return;
                                }
                                printf("\n\n");
                                // Vérifie si le résultat est valide
                                if (verification_dictionnaire(message_dechiffre) >= 2) {
                                    printf("Configuration trouvée !\n");
                                    printf("Rotors: %d %d %d | Positions: %c %c %c | Reflector %c\n", 
                                        rotor1, rotor2, rotor3, (pos1), (pos2), (pos3), reflector);
                                    printf("Message chiffré: %s\n", message_chiffre);
                                    printf("Message déchiffré: %s\n", message_dechiffre);
                                    
                                    return;
                                }
                                free(message_dechiffre);
							}
						}			
					}
				}
			}
		}

	}

    printf("Aucune configuration correcte trouvée \n");
}

int main(){
    // Exemple de message chiffré 
    // UNITEOPERATIONSTRATEGIE
    char *message = "QRFSXXZEEFATXPWVBDLWCPZHNBJQKZOPJGRMOMB";
    brute_force_enigma(message);
}

// VDZJDMDLBOYYDNFSYXKAAMHUVLYVONJRGITHIKI
// QRFSXXZEEFATXPWVBDLWCPZHNBJQKZOPJGRMOMB
// HQNNTGHRWZSZFLPCFJHLXWYBRZTAPBLVETVDUGF