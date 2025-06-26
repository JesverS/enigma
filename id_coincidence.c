#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <fcntl.h>
#include <assert.h>
#include "enigma_rotor.c"
#include <math.h>
#include "enigma.h"

typedef struct {
    Inversion* inv;  
    double ic;
} InversionIC;

double indice_coincidence(char* messageDC) {
    int i, j, sumC;
    int tailleMessage = strlen(messageDC);
    double sumT = 0.0;
    char c;

    for (i = 0; i < 26; i++) {
        c = 'A' + i;
        sumC = 0;
        for (j = 0; j < tailleMessage; j++) {
            if (messageDC[j] == c) {
                sumC++;
            }
        }
        sumT += sumC * (sumC - 1);
    }

    if (tailleMessage <= 1) {
        return 0.0; // éviter division par zéro
    }

    return fabs((sumT / (tailleMessage * (tailleMessage - 1))) - 0.0746);
}
char* chiffrement_retour(const char *message, RotorsReflector *rotors_reflector, Inversion *inv){
    // Chiffrement
    char* new_message = (char*) malloc(sizeof(char)*strlen(message)+1);
    int i = 0;
    while (message[i] != '\0')
    {
        // On verifie si la lettre est une lettre de l'alphabet
        if (message[i] < 'A' || message[i] > 'Z')
        {
            printf("La lettre %c n'est pas une lettre de l'alphabet\n", message[i]);
            return NULL;
        }
        Inversion *tmp = inv;
        // Etape 1 : On effectue un premier chiffrement avec le tableau de connexion (inversion manuelle )
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

        // Etape 2 : On effectue un deuxieme chiffrement avec les rotors et le reflecteur
        new_message[i] = resultat_rf(new_message[i], rotors_reflector);
        // Etape 3 : On effectue un troisieme chiffrement avec le tableau de connexion (inversion manuelle )
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
    return new_message;
}
char* messagedechiffre(RotorsReflector* rf,char* messageC, int taillemessage){
	char* res = strdup(messageC);
	for(int i=0;i<taillemessage;i++){
		res[i] = resultat_rf(messageC[i], rf);
	}
	return res;
}
int compareICDesc(const void* a, const void* b) {
    InversionIC* ia = *(InversionIC**)a;
    InversionIC* ib = *(InversionIC**)b;
    if (ia->ic > ib->ic) return 1;
    else if (ia->ic < ib->ic) return -1;
    else return 0;
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


Inversion* idc_inversions(RotorsReflector* rf, const char* message){
    const char* alphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    InversionIC** tabMax = (InversionIC**) malloc(sizeof(Inversion)*325);
    int tabMaxCount = 0;
    RotorsReflector* test = copie_rf(rf);
    double ic;
    for (int k = 0; k < 26 - 1; k++) {
        char lettre1 = alphabet[k];

        for (int j = k + 1; j < 26; j++) {
            char lettre2 = alphabet[j];
            Inversion* inv = malloc(sizeof(Inversion));
            inv->p_lettre = lettre1;
            inv->s_lettre = lettre2;
	    inv->suiv = NULL;
            char* messageD = chiffrement_retour(message,test,inv);
            test = copie_rf(rf);
            ic = indice_coincidence(messageD);
            
            
                 
            free(messageD);
            // Sauvegarde dans tableau temporaire
            tabMax[tabMaxCount] = malloc(sizeof(InversionIC));
            tabMax[tabMaxCount]->inv = inv;
            tabMax[tabMaxCount]->ic = ic;
            tabMaxCount++;

        }
    }

    // Tri décroissant selon l'IC
    qsort(tabMax, tabMaxCount, sizeof(InversionIC*), compareICDesc);

    // Construction de la liste chainée des 6 meilleures paires
    Inversion* best_inv = NULL;
    for (int i = 0; i < 6; i++) {
        Inversion* nouv = malloc(sizeof(Inversion));
        nouv->p_lettre = tabMax[i]->inv->p_lettre;
        nouv->s_lettre = tabMax[i]->inv->s_lettre;
        nouv->suiv = best_inv;
        best_inv = nouv;
    }
    printf("%f\n",tabMax[0]->ic);
    liberer_rf(test);
    return best_inv;
}
RotorsReflector* idc_rotors(char* messageC){
	int tab[] = {1,1,1};
	char* pos = strdup("AAA");
	char reflector = 'A';
	unsigned long taillemess = strlen(messageC);
	char buffer[taillemess];
	RotorsReflector* best_rf = init_rotorsreflector(tab, pos,reflector,3);
	RotorsReflector* rf = init_rotorsreflector(tab, pos,reflector,3);
	double best_idc = 1;
	double idc;
	for(int reflect=0; reflect<3;reflect++){
		reflector = 'A'+reflect;
		for(int rotor3 = 1; rotor3<=5;rotor3++){
			tab[2] = rotor3;
			for(int rotor2 = 1; rotor2<=5;rotor2++){
				tab[1] = rotor2;
				for(int rotor1 = 1; rotor1<=5;rotor1++){
					tab[0] = rotor1;
					for(int pos3 = 'A'+1; pos3<'A'+28;pos3++){
						for(int pos2 = 'A' +1 ; pos2 <'A'+28;pos2++){
							for(int pos1 = 'A'+1 ; pos1<'A'+28;pos1++){
								rf = modifier_rf(rf,tab,pos,reflector,3);
								strcpy(buffer,messagedechiffre(rf,messageC,taillemess));
								rf = modifier_rf(rf,tab,pos,reflector,3);
								idc = indice_coincidence(buffer);
								if(idc < best_idc){
									best_idc = idc;
									best_rf = modifier_rf(best_rf,tab,pos,reflector,3);
								}
								pos[0] = pos1;
							}
							pos[1] = pos2;
						}			
						pos[2] = pos3;
					}
				}
			}
		}
	}
	printf("%f\n",best_idc);
	return best_rf;
}
Inversion* creer_liste_inversions() {
    // Définis ici les 6 paires que tu veux permuter
  char paires[6][2] = {
    {'M', 'S'},
    {'E', 'Y'},
    {'A', 'Q'},
    {'C', 'H'},
    {'G', 'W'},
    {'K', 'Z'}
};
    Inversion* head = NULL;

    for (int i = 5; i >= 0; i--) {
        Inversion* nouv = malloc(sizeof(Inversion));
        nouv->p_lettre = paires[i][0];
        nouv->s_lettre = paires[i][1];
        nouv->suiv = head;
        head = nouv;
    }

    return head;
}

void afficher_inversions(Inversion* inv) {
    while (inv) {
        printf("%c <-> %c\n", inv->p_lettre, inv->s_lettre);
        inv = inv->suiv;
    }
}
int main(){
	int tab[] = {3, 1, 4};
	RotorsReflector* rf1 = init_rotorsreflector(tab,"GMI", 'C',3);
	char* message = strdup("LESCHEFSQUIDEPUISDEDENOMBREUSESANNEESSONTALATEDESTARMEESFRANCAISESONTFORMEUNGOUVERNEMENTCEGOUVERNEMENTALLEGUANTLADEFAITEDENOSARMEESSESTMISENRAPPORTECENNEMIPOURCESSERLECOMBATCERTESNOUSAVONSETENOUSSOMMESSUBMERGESPARLAFORCEMECANIQUETERRESTREETAERIENNEDENNEMIINFINIMENTPLUSQUELEURNOMBRECESONTLESCHARSELESAVIONSLATACTIQUEDESALLEMANDSQUINOUSFONTRECULERCESONTLESCHARSELESAVIONSLATACTIQUEDESALLEMANDSQUIONSURPRISNOSCHEFS");
	printf("%s\n",message);
 	Inversion* inv_effectuees =creer_liste_inversions();
	affiche_rotor(rf1);
	message = chiffrement_retour(message,rf1,inv_effectuees);
	//printf("message chiffré : %s\n",message);
	rf1 = init_rotorsreflector(tab,"GMI", 'C',3);
	//message = chiffrement_retour(message,rf1,NULL);
	//printf("message chiffré : %s\n",message);
	//RotorsReflector* rf =idc_rotors(message);
	//affiche_rotor(rf);
	afficherInversionLettre(inv_effectuees);
	Inversion* inversions_algo = idc_inversions(rf1, message);
	inversions_algo = inversions_algo->suiv;
	inversions_algo = inversions_algo->suiv;
	afficherInversionLettre(inversions_algo);
	printf("%s\n",chiffrement_retour(message,rf1,inversions_algo));
	free(message);
	
	return 0;
}
