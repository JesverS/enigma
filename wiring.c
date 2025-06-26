#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "wiring.h"
#include "enigma_rotor.h"

RotorsReflector* initialisationRotor(){
    int choix;
    char buffer[256];
    // Creation des deux tableaux contenant les informations des retors
    int retors[3];
    char retors_start_possition[26];
    char reflector;

    printf("==============================================\n");
    printf("   Bienvenue sur le menu d'initialisation\n");
    printf("          de la machine Enigma\n");
    printf("==============================================\n\n");
    
    printf("Pour initialiser la machine, vous devez configurer plusieurs paramètres.\n\n");
    
    printf("1. Sélection des rotors :\n");
    printf("   - Vous devez choisir trois rotors parmi les cinq disponibles.\n");
    printf("   - Les rotors sont numérotés de 1 à 5.\n");
    printf("   - Vous sélectionnerez d'abord un rotor, puis vous indiquerez sa position de départ.\n");
    printf("   - La position de départ est une lettre majuscule entre A et Z.\n\n");
    
    printf("2. Sélection du réflecteur :\n");
    printf("   - Vous devez choisir un réflecteur parmi les trois disponibles : A, B ou C.\n\n");
    
    printf("Merci de suivre les instructions et d'entrer les valeurs demandées lorsque cela vous sera indiqué.\n\n");

    for (int i = 0; i < 3; i++)
    {
        printf("Selectionner le retor %d : \n", i+1);
        fgets(buffer, 256, stdin);
        choix = atoi(buffer);
        retors[i] = choix;

        printf("Selectionner la lettre de depart du retor %d\n", i+1);
        fgets(buffer, 256, stdin);

        choix = buffer[0];
        retors_start_possition[i] = choix;
    }
    
    printf("Selectionner le seflector à utiliser\n");
    fgets(buffer, 256, stdin);
    choix = buffer[0];
    reflector = choix;

    RotorsReflector *rotors_reflector = init_rotorsreflector(retors, retors_start_possition, reflector, 3);
    return rotors_reflector;
}

Inversion* ajouterInversionLettre(Inversion *inv){
    char buffer[256];
    char lettre1;
    char lettre2;

    printf("==============================================\n");
    printf("   Bienvenue sur le menu d'ajout d'inversion\n");
    printf("          de la machine Enigma\n");
    printf("==============================================\n\n");

    printf("Veuillez entrer la première lettre à inverser : ");
    fgets(buffer, 256, stdin);
    while (buffer[0] < 'A' || buffer[0] > 'Z')
    {
        printf("La lettre %c n'est pas une lettre majuscule de l'alphabet\n", buffer[0]);
        printf("Veuillez entrer la première lettre à inverser : ");
        fgets(buffer, 256, stdin);
    }
    
    lettre1 = buffer[0];

    printf("Veuillez entrer la deuxième lettre à inverser : ");
    fgets(buffer, 256, stdin);
    while (buffer[0] < 'A' || buffer[0] > 'Z')
    {
        printf("La lettre %c n'est pas une lettre majuscule de l'alphabet\n", buffer[0]);
        printf("Veuillez entrer la deuxième lettre à inverser : ");
        fgets(buffer, 256, stdin);
    }
    
    lettre2 = buffer[0];

    Inversion* new_inv  = (Inversion*)malloc(sizeof(Inversion));
    new_inv->p_lettre = lettre1;
    new_inv->s_lettre = lettre2;
    new_inv->suiv = inv;
    
    printf("L'inversion de lettre %c et %c a etait ajouter\n", lettre1, lettre2);
    return new_inv;
}

Inversion* supprimerInversionLettre(Inversion *inv){
    char buffer[256];
    char lettre1;
    char lettre2;

    if(inv == NULL){
        printf("Il n'y a aucune inversion à supprimer\n");
        return NULL;
    }

    printf("==============================================\n");
    printf("   Bienvenue sur le menu d'de suppression d'inversion\n");
    printf("                de la machine Enigma\n");
    printf("==============================================\n\n");

    printf("Veuillez entrer la première lettre à qui etait inversée : ");
    fgets(buffer, 256, stdin);
    while (buffer[0] < 'A' || buffer[0] > 'Z')
    {
        printf("La lettre %c n'est pas une lettre majuscule de l'alphabet\n", buffer[0]);
        printf("Veuillez entrer la première lettre à qui etait inversée : ");
        fgets(buffer, 256, stdin);
    }
    
    lettre1 = buffer[0];

    printf("Veuillez entrer la deuxième lettre à qui etait inversée : ");
    fgets(buffer, 256, stdin);
    while (buffer[0] < 'A' || buffer[0] > 'Z')
    {
        printf("La lettre %c n'est pas une lettre majuscule de l'alphabet\n", buffer[0]);
        printf("Veuillez entrer la deuxième lettre à qui etait inversée : ");
        fgets(buffer, 256, stdin);
    }

    lettre2 = buffer[0];

    Inversion* tmp = inv;
    if((tmp->p_lettre == lettre1 && tmp->s_lettre == lettre2) || (tmp->p_lettre == lettre2 && tmp->s_lettre == lettre1)){
        inv = tmp->suiv;
        free(tmp);
        printf("L'inversion de lettre %c et %c a etait supprimer\n", lettre1, lettre2);
        return inv;
    }
    while (tmp->suiv)
    {
        if((tmp->suiv->p_lettre == lettre1 && tmp->suiv->s_lettre == lettre2) || (tmp->suiv->p_lettre == lettre2 && tmp->suiv->s_lettre == lettre1)){
            Inversion* tmp2 = tmp->suiv;
            tmp->suiv = tmp->suiv->suiv;
            free(tmp2);
            printf("L'inversion de lettre %c et %c a etait supprimer\n", lettre1, lettre2);
            return inv;
        }
        tmp = tmp->suiv;
    }

    printf("Aucune inversion n'a etait trouver\n");
    return inv;
}

void chiffrement(char *message, RotorsReflector *rotors_reflector, Inversion *inv){
    // Chiffrement
    char new_message[strlen(message)+1];
    int i = 0;
    while (message[i] != '\0')
    {
        // On verifie si la lettre est une lettre de l'alphabet
        if (message[i] < 'A' || message[i] > 'Z')
        {
            printf("La lettre %c n'est pas une lettre de l'alphabet\n", message[i]);
            return;
        }
        Inversion *tmp = inv;
        // Etape 1 : On effectu un premier chiffrement avec le tableau de connexion (inversion manuelle )
        while (tmp)
        {
            if(message[i] == tmp->p_lettre){
                new_message[i] = tmp->s_lettre;
                break;
            }else if (message[i] == tmp->s_lettre)
            {
                new_message[i] = tmp->p_lettre;
                break;
            }
            tmp = tmp->suiv;
        }
        if(!tmp){ // Aucune inversion n'a etait trouver 
            new_message[i] = message[i];
        }

        // Etape 2 : On effectu un deuxieme chiffrement avec les rotors et le reflecteur
        new_message[i] = resultat_rf(new_message[i], rotors_reflector);
        // Etape 3 : On effectu un troisieme chiffrement avec le tableau de connexion (inversion manuelle )
        tmp = inv;
        while (tmp)
        {
            if(new_message[i] == tmp->p_lettre){
                new_message[i] = tmp->s_lettre;
                break;
            }else if (new_message[i] == tmp->s_lettre)
            {
                new_message[i] = tmp->p_lettre;
                break;
            }
            tmp = tmp->suiv;
        }
        i++;
    }
    new_message[i] = '\0'; // Ajout du caractère nul de fin de chaîne
    printf("\n\nLe nouveau mot est :  %s \n", new_message);
}

void afficherInversionLettre(Inversion *inv){
    Inversion *tmp = inv;
    if(tmp == NULL){
        printf("Il n'y a aucune inversion de lettre manuel\n");
        return;
    }
    printf("==============================================\n");
    printf("   Liste des inversions de lettre manuel\n");
    printf("==============================================\n\n");
    while (tmp)
    {
        printf("Lettre 1 : %c, Lettre 2 : %c\n", tmp->p_lettre, tmp->s_lettre);
        tmp = tmp->suiv;
    }
}

void libererInversions(Inversion *inv) {
    Inversion *tmp;
    while (inv != NULL) {
        tmp = inv;
        inv = inv->suiv;
        free(tmp);
    }
}

void menuMachineReglable(){
    RotorsReflector* rotors_reflector = initialisationRotor();
    Inversion *inv = NULL;
    int choix;
    char buffer[256];

    do{
        printf("==============================================\n");
        printf("   Bienvenue sur le menu de la machine Enigma\n");
        printf("0 - Pour quitter le menu\n");
        printf("1 - Pour faire un chiffrement \n");      
        printf("2 - Pour faire un dechiffrement \n");
        printf("3 - Pour ajouter une inversion de lettre manuel supplementaire \n");
        printf("4 - Pour supprimer une inversion de lettre manuel supplementaire \n");
        printf("5 - Pour afficher les inversions de lettres manuel supplementaire \n");
        printf("6 - Pour refaire l'initialisation de la machine \n");
        printf("==============================================\n\n");   
        if(fgets(buffer, 256, stdin) != NULL){
            choix = atoi(buffer);
            switch(choix){
                case 0:
                    printf("Fermeture du menu!\n");
                    break;
                case 1:
                    // Option 1 :
                    printf("Entrez le message a crypté  (Comme sur la machine orriginal il ne faut pas mettre d'espace): \n");
                    char message[256];
                    fgets(message, 256, stdin);
                    message[strlen(message)-1] = '\0';
                    chiffrement(message, rotors_reflector, inv);
                    break;
                    
                case 2:
                    // Option 2 : 
                    // Ajoutez ici la fonction de déchiffrement
                    break;

                case 3:
                    // Option 3 :
                    inv = ajouterInversionLettre(inv);
                    break;
                case 4:
                    // Option 4 :
                    inv = supprimerInversionLettre(inv);
                    break;
                case 5:
                    afficherInversionLettre(inv);
                    break; 
                case 6:
                    rotors_reflector = initialisationRotor();
                    break;
                
                default:
                    printf("Cela ne correspond pas à une option.\n");
            }
        }
    } while(choix != 0);

    liberer_rf(rotors_reflector);
    libererInversions(inv);
}

RotorsReflector *copyRotorsReflector(RotorsReflector *rotors_reflector){
    RotorsReflector *new_rotors_reflector = (RotorsReflector*)malloc(sizeof(RotorsReflector));
    new_rotors_reflector->l_r = (Rotor**)malloc(3 * sizeof(Rotor*));
    new_rotors_reflector->reflector = strdup(rotors_reflector->reflector);
    new_rotors_reflector->taille = rotors_reflector->taille;

    for (int i = 0; i < 3; i++)
    {
        new_rotors_reflector->l_r[i] = (Rotor*)malloc(sizeof(Rotor));
        new_rotors_reflector->l_r[i]->pos = rotors_reflector->l_r[i]->pos;
        new_rotors_reflector->l_r[i]->value = strdup(rotors_reflector->l_r[i]->value);
        new_rotors_reflector->l_r[i]->notch = rotors_reflector->l_r[i]->notch;
        new_rotors_reflector->l_r[i]->turnover = rotors_reflector->l_r[i]->turnover;
    }
    return new_rotors_reflector;
}

RotorsReflector* init_rotorsreflector_aleatoire(){
    int retors[3];
    char retors_start_possition[26];
    char reflector;

    for (int i = 0; i < 3; i++)
    {
        retors[i] = rand() % 5 + 1;
        retors_start_possition[i] = rand() % 26 + 'A';
        printf("Rotor %d : %d, %c\n", i+1, retors[i], retors_start_possition[i]);
    }
    
    reflector = rand() % 3 + 'A';
    printf("Reflector : %c\n", reflector);

    RotorsReflector *rotors_reflector = init_rotorsreflector(retors, retors_start_possition, reflector, 3);
    return rotors_reflector;
}

Inversion *init_inversion_aleatoire(){
    Inversion *inv = NULL;
    int nb_inversion = rand() % 10; // Nombre d'inversions aléatoires entre 0 et 10
    char listlettre[26] = {0}; // Tableau pour suivre les lettres déjà utilisées
    printf("Il y a %d inversions de lettres aléatoires\n", nb_inversion);
    for (int i = 0; i < nb_inversion; i++) {
        Inversion *new_inv = (Inversion*)malloc(sizeof(Inversion));
        char lettre1, lettre2;

        // Générer la première lettre unique
        do {
            lettre1 = rand() % 26 + 'A';
        } while (listlettre[lettre1 - 'A']);
        listlettre[lettre1 - 'A'] = 1;

        // Générer la deuxième lettre unique
        do {
            lettre2 = rand() % 26 + 'A';
        } while (listlettre[lettre2 - 'A']);
        listlettre[lettre2 - 'A'] = 1;

        new_inv->p_lettre = lettre1;
        new_inv->s_lettre = lettre2;
        new_inv->suiv = inv;
        inv = new_inv;
        printf("Inversion %d : %c, %c\n", i+1, lettre1, lettre2);
    }

    return inv;
}

// Machine Enigma avec configuration aléatoire
void menuMachineAleatoire() {
    RotorsReflector* rotors_reflector = init_rotorsreflector_aleatoire();
    Inversion *inv = init_inversion_aleatoire();

    RotorsReflector* tmp_rotors_reflector = copyRotorsReflector(rotors_reflector);
   // Inversion *tmp_inv = //copyInversion(inv);
    int choix;
    char buffer[256];
    printf("==============================================\n");
    printf("La machine Enigma a été configurée aléatoirement.\n");
    printf("==============================================\n\n");

    do{
        printf("==============================================\n");
        printf("   Bienvenue sur le menu de la machine Enigma configurée aléatoirement \n");
        printf("0 - Pour quitter le menu\n");
        printf("1 - Pour faire un chiffrement/Déchifrement \n");
        printf("2 - Pour refaire la configuration aléatoire de la machine \n");
        printf("==============================================\n\n");   
        if(fgets(buffer, 256, stdin) != NULL){
            choix = atoi(buffer);
            switch(choix){
                case 0:
                    printf("Fermeture du menu!\n");
                    break;
                case 1:
                    // Option 1 :
                    printf("Entrez le message a crypté  (Comme sur la machine orriginal il ne faut pas mettre d'espace): \n");
                    char message[256];
                    fgets(message, 256, stdin);
                    message[strlen(message)-1] = '\0';
                    chiffrement(message, tmp_rotors_reflector, inv);
                    liberer_rf(tmp_rotors_reflector);
                    tmp_rotors_reflector = copyRotorsReflector(rotors_reflector);
                    break;
                    
                case 2:
                    rotors_reflector = init_rotorsreflector_aleatoire();
                    inv = init_inversion_aleatoire();
                
                    tmp_rotors_reflector = copyRotorsReflector(rotors_reflector);
                    break;
                default:
                    printf("Cela ne correspond pas à une option.\n");
            }
        }
    } while(choix != 0);
    liberer_rf(tmp_rotors_reflector);
    liberer_rf(rotors_reflector);
    libererInversions(inv);
}


// Fonction pour afficher le menu de sélection de la machine
void menu_selection_machine() {
    int choix;
    char buffer[256];

    printf("==============================================\n");
    printf("   Bienvenue sur le menu de sélection de la machine Enigma\n");
    printf("0 - Pour configurer les paramètres de la machine\n");
    printf("1 - Pour utiliser une configuration aléatoire\n");
    printf("==============================================\n\n");

    fgets(buffer, 256, stdin);
    choix = atoi(buffer);

    if (choix == 0) {
        menuMachineReglable();
    } else if (choix == 1) {
        menuMachineAleatoire();
    } else {
        printf("Choix invalide. Utilisation de la configuration aléatoire par défaut.\n");
        menuMachineAleatoire();
    }
}

int main(){
    srand(time(NULL));
    menu_selection_machine();
    return 0;
}