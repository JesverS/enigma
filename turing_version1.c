// Simulation bombe de Turing
#include "turing.h"


#define ALPHABET_SIZE 26 

typedef struct {
    char mapping[ALPHABET_SIZE]; // mapping[0] est pour 'A', mapping[1] pour 'B', etc.
    int nb_inversion; // Nombre d'inversions trouvées
} DeducedPlugboard;

// Initialise le plugboard déduit à un état neutre
void init_deduced_plugboard(DeducedPlugboard *dpb) {
    for (int i = 0; i < ALPHABET_SIZE; i++) {
        dpb->mapping[i] = '?'; // '?' signifie non connecté/non déduit
    }
}

// Fonction pour ajouter une déduction au plugboard
// Retourne true si une contradiction est trouvée, false sinon.
bool add_plugboard_deduction(DeducedPlugboard *dpb, char a, char b) {
    if(dpb->nb_inversion > 4) {
        return true; // Trop d'inversions, on ne peut plus déduire
    }
    if (a == b) return false;

    int ia = a - 'A';
    int ib = b - 'A';

    char ma = dpb->mapping[ia];
    char mb = dpb->mapping[ib];

    if ((ma != '?' && ma != b) || (mb != '?' && mb != a)) {
        dpb->mapping[ia] = '*';
        dpb->mapping[ib] = '*';
        return true;
    }

    dpb->mapping[ia] = b;
    dpb->mapping[ib] = a;
    dpb->nb_inversion++;
    return false;
}

char plugboard_apply(DeducedPlugboard* dpb, char c) {
    if (c < 'A' || c > 'Z') return c;
    char mapped = dpb->mapping[c - 'A'];
    return (mapped == '?' || mapped == '*') ? c : mapped;
}

bool verifier_contradiction(DeducedPlugboard* dpb, const char* crib_cipher, const char* crib_clear, int* nums_rotors, char* lettres_associes, char lettre_reflector, int taille, int len) {
    // Vérifie si les déductions du plugboard sont contradictoires avec le crible
    //printf("Vérification de la contradiction pour le crible...\n");
    RotorsReflector* rf = init_rotorsreflector(nums_rotors, lettres_associes, lettre_reflector, taille);
    if(rf == NULL) {
        return true; // Échec de l'initialisation des rotors
    }

    for (int i = 0; i < len; i++) {
        char p = crib_clear[i];
        char c = crib_cipher[i];

        char c_mapped = plugboard_apply(dpb, c);
        char c_result = resultat_rf(c_mapped, rf);
        c_mapped = plugboard_apply(dpb, c_result);

        if (p != c_mapped) {
            liberer_rf(rf);
            return false;
        }
    }
    liberer_rf(rf);
    return true; // Pas de contradiction
}
bool process_crib(const char *crib_clear, const char *crib_cipher, int crib_length,
                   DeducedPlugboard *dpb, int* nums_rotors, char* lettres_associes, char lettre_reflector, int taille, int idx, char* next_pos) {
    if (idx > 3 && verifier_contradiction(dpb, crib_cipher, crib_clear, nums_rotors, lettres_associes, lettre_reflector, taille, idx) == false) {
            return true; // Contradiction trouvée
    }

    if (idx >= crib_length) {
        printf("---Drop valide trouver---\n");
        // Fin du crible, pas de contradiction trouvée
        //Affichage du plugboard
        printf("Plugboard déduit (implications) :\n");
        for (int i = 0; i < ALPHABET_SIZE; i++) {
            if (dpb->mapping[i] != '?' && i < (dpb->mapping[i] - 'A')) {
                printf("  %c-%c\n", (char)('A' + i), dpb->mapping[i]);
            }
        }
        printf("Positions initiales rotors (Ring Settings) : %c%c%c\n",
                lettres_associes[0], lettres_associes[1], lettres_associes[2]);
        printf("Ordre des rotors (Indices 0-4) : %d %d %d\n",
                nums_rotors[0], nums_rotors[1], nums_rotors[2]); // Affiche 1, 2, 3...
        printf("Réflecteur : %c\n", lettre_reflector);
        
        return false;
    }


    RotorsReflector* rf1 = init_rotorsreflector(nums_rotors, next_pos, lettre_reflector, taille);
    RotorsReflector* rf2 = init_rotorsreflector(nums_rotors, next_pos, lettre_reflector, taille);
    if (rf1 == NULL || rf2 == NULL) {
        if (rf1) liberer_rf(rf1);
        if (rf2) liberer_rf(rf2);
        return true; // Échec
    }
    
    char p = crib_clear[idx];
    char c = crib_cipher[idx];

    char p_mapped = plugboard_apply(dpb, p);
    char p_result = resultat_rf(p_mapped, rf1);
    char p_expected = plugboard_apply(dpb, p_result);

    char c_mapped = plugboard_apply(dpb, c);
    char c_result = resultat_rf(c_mapped, rf2);
    char c_expected = plugboard_apply(dpb, c_result);

 
    char positions_suivantes[4];
    for (int i = 0; i < 3; i++) {
        positions_suivantes[i] = 'A' + rf1->l_r[i]->pos;
    }
    positions_suivantes[3] = '\0';

    liberer_rf(rf1);
    liberer_rf(rf2);

    // Si les sorties sont identiques, pas de déduction à faire, on avance
    if (p == c_expected && c == p_expected) {
        return process_crib(crib_clear, crib_cipher, crib_length, dpb, nums_rotors, lettres_associes, lettre_reflector, taille, idx + 1, positions_suivantes);
    }

    // Sinon, explorer les deux branches
    bool contradiction1 = false, contradiction2 = false;
    
    // Branche 1 : p <-> c_expected
    if (p != c_expected) {
        DeducedPlugboard dpb1 = *dpb;
        if (add_plugboard_deduction(&dpb1, p, c_expected)) {
            contradiction1 = true;
        } else {
            contradiction1 = process_crib(crib_clear, crib_cipher, crib_length, &dpb1, nums_rotors, lettres_associes, lettre_reflector, taille, idx + 1, positions_suivantes);
        }
    } else {
        contradiction1 = process_crib(crib_clear, crib_cipher, crib_length, dpb, nums_rotors, lettres_associes, lettre_reflector, taille, idx + 1, positions_suivantes);
    }

    // Branche 2 : c <-> p_expected
    if (c != p_expected) {
        DeducedPlugboard dpb2 = *dpb;
        if (add_plugboard_deduction(&dpb2, c, p_expected)) {
            contradiction2 = true;
        } else {
            contradiction2 = process_crib(crib_clear, crib_cipher, crib_length, &dpb2, nums_rotors, lettres_associes, lettre_reflector, taille, idx + 1, positions_suivantes);
        }
    } else {
        contradiction2 = process_crib(crib_clear, crib_cipher, crib_length, dpb, nums_rotors, lettres_associes, lettre_reflector, taille, idx + 1, positions_suivantes);
    }

    // Retourne true seulement si les deux branches mènent à une contradiction
    return contradiction1 && contradiction2;
}

void find_enigma_settings(const char *crib_clear, const char *crib_cipher) {
    int crib_length = strlen(crib_clear);
    if (crib_length == 0 || crib_length != strlen(crib_cipher)) {
        fprintf(stderr, "Erreur: La crib est invalide.\n");
        return;
    }

    printf("Début de la recherche des réglages Enigma (Bomba).\n");
    printf("Crib: Clair=\"%s\", Chiffré=\"%s\"\n", crib_clear, crib_cipher);

    DeducedPlugboard dpb_candidate;
    init_deduced_plugboard(&dpb_candidate);
    int drop_count = 0;

    char initial_rotor_positions[4] = "AAA";
    initial_rotor_positions[3] = '\0'; // Position des rotors (Ring Settings) initialisée à 'A'
    int tab[] = {1,1,1};
	char reflector = 'A';
    // Boucle pour l'ordre des rotors (permutation de 3 rotors parmi les disponibles)
    for(int rotor3 = 1; rotor3<=5;rotor3++){ // Rotor 3
        tab[2] = rotor3;
        for(int rotor2 = 1; rotor2<=5;rotor2++){ // Rotor 2
            tab[1] = rotor2;
            for(int rotor1 = 1; rotor1<=5;rotor1++){ // Rotor 1
                tab[0] = rotor1;
                // Itérer sur toutes les positions de départ des rotors (Ring Settings)
                for (int pos1 = 'A' ; pos1 <= 'Z';pos1++) { // Position du rotor 3
                    initial_rotor_positions[2] = pos1;
                    for (int pos2 = 'A' ; pos2 <= 'Z';pos2++) { // Position du rotor 2
                        initial_rotor_positions[1] = pos2;
                        for (int pos3 = 'A' ; pos3 <= 'Z';pos3++) { // Position du rotor 1
                            initial_rotor_positions[0] = pos3;
                           for(int reflect=0; reflect<3;reflect++){ // Itérer sur les réflecteurs
		                        reflector = 'A'+reflect;
                                
                                if (!process_crib(crib_clear, crib_cipher, crib_length, &dpb_candidate,
                                                  tab, initial_rotor_positions,
                                                  reflector, 3, 0, initial_rotor_positions)) {
                                    // Pas de contradiction, c'est un "drop" (solution candidate)
                                    drop_count++;
                                    printf("\n--- DROP %d FOUND ---\n", drop_count);
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    printf("\nRecherche terminée. %d drops trouvés.\n", drop_count);
}


// Fonction principale pour tester
int main() {
    // Exemple de crib (celle que nous avons utilisée dans l'explication)

    // Texte claire
    char * crib_clear =  "OPERATIONCOM";

    // Texte chiffré
    char * crib_cipher = "TDYIMGEFSHEE";
    find_enigma_settings(crib_clear, crib_cipher);

    return 0;
}
