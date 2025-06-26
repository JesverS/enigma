#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <fcntl.h>
#include <assert.h>
#include "enigma_rotor.c"
#include <math.h>


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

    return sumT / (tailleMessage * (tailleMessage - 1));
}

char* messagedechiffre(RotorsReflector* rf,char* messageC, int taillemessage){
	char* res = strdup(messageC);
	for(int i=0;i<taillemessage;i++){
		res[i] = resultat_rf(messageC[i], rf);
	}
	return res;
}
RotorsReflector* crackage(const char* messageC){
	int tab[] = {1,1,1};
	char* pos = strdup("AAA");
	char reflector = 'A';
	unsigned long taillemess = strlen(messageC);
	char buffer[taillemess];
	RotorsReflector* best_rf = init_rotorsreflector(tab, pos,reflector,3);
	RotorsReflector* rf = init_rotorsreflector(tab, pos,reflector,3);
	double best_idc = 0;
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
								if(idc > best_idc){
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
int main(){
	int tab[] = {2,1,2};
	RotorsReflector* rf1 = init_rotorsreflector(tab,"AAB", 'C',3);
	//printf("%lf",indice_coincidence("LA LOUTRE EST SORTIE DU TERRIER"));
	char* message = strdup("ILAVAITDEMEUREDANSCEVILLEGENSIMPLESANSFORTUNEILSETAITVENUAVECUNESACOCHETQUELQUESVIEUXLIVRESLEQUARTIERLECONNUTPEUDEGENSLAPPROCHAIENTILNEPARLAITPRESQUEJAMAISONLESENTAITBONETMALGRELETALONQUILTRAINAITCEQUILAVAITDEVUDANSLESRUESCEQUILAVAITENDUREPEUTETRESSEMBLAITAUCOUPARDONILSINSTALLADANSUNPETITLOGISDANSUNERUEETROITEILALLAITCHAQUESOIRDANSUNEEGLISEASSISSURUNBANCLETEMPSPASSAITSONNOMNAVAITPASETEPRONONCEILSEMBLAITNEPASVOULOIRETRECONNUILREGARDAITLESENFANTSJOUERPARFOISILLEURFAISAITUNPETITGESTELECURERACONTAITQUILAVAITCONFESSEUNHOMMECETTEANNEEETQUECELUILAVAITPLEUREBEAUCOUPPERSONNENESAVAITVUECELAENPERSONNEMAISLEBRUITCOURAITONPARLAITDETEMPSANCIENSDEPECHEDEJUSTICEILNEPORTAITJAMAISDECOULEURLUMINEUSEQUEGRISEDESVIEUXVETEMENTSMEPRESBOUCHESILPARTAITMATINTOTETRENTRAITAVANTLANUITQUELQUEFOISONENTENDAITUNCHANTFAIBLEDERRIERELAPORTE");
	//printf("%lf",indice_coincidence(message));
	message = messagedechiffre(rf1, message, strlen(message));
	RotorsReflector* rf =crackage(message);
	//rf1 = init_rotorsreflector(tab,"AAA", 'C',3);
	//printf("%lf",indice_coincidence(messagedechiffre(rf1,message,strlen(message))));
	//rf1 = init_rotorsreflector(tab,"AAA", 'C',3);
	affiche_rotor(rf);
	printf("%s\n",messagedechiffre(rf,message,strlen(message)));
	return 0;
}

