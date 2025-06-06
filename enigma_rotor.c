#include "enigma_rotor.h"



Rotor* init_rotor(int num_rotor, char lettre){
	char buff[40]; //buffer de la taille d'une ligne dans le fichier rotor_ukw_enigma1.txt
	char buff2[28]; //buffer pour stocker l'alphabet réarranger
	char nom_rotor[8];
	/*bout de code permettant d'ouvrir le fichier contenant les rotors, de déplacer l'offset a la ligne voulue, et de lire cette ligne. */
	int f = open("rotor_ukw_enigma1.txt",O_RDONLY);
	if(num_rotor !=1){
		lseek(f,((num_rotor-1)*38),SEEK_SET);
	}
	else{
		lseek(f,0,SEEK_SET);
	}
	read(f, buff, sizeof(buff) - 1);  
    	buff[sizeof(buff) - 1] = '\0'; 

	
	//p'tit malloc.
	Rotor* new_rotor = (Rotor*) malloc(sizeof(Rotor));
	assert(new_rotor);
	//assignation des valeurs au nouveau rotor.
	sscanf(buff, "%s %s %c %c",nom_rotor,buff2,&(new_rotor->notch),&(new_rotor->turnover));
	new_rotor->value = strdup(buff2);
	close(f);
	
	new_rotor->pos = ((int) lettre-'A')%26;
	return new_rotor;
}

int trouver_pos(char* AlphabetM, char l){
	int i;
	for(i=0; i<strlen(AlphabetM);i++){
		if(AlphabetM[i] ==l){
			return i;
		}
	}
	return -1;
}


RotorsReflector* init_rotorsreflector(int* nums_rotors, char* lettres_associes, char lettre_reflector,int taille){
	int i;
	int len =3;
	RotorsReflector* rf = (RotorsReflector*) malloc(sizeof(RotorsReflector));
	char buff[40]; 
	char buff_alphabet[28];
	char nom_ukw[5];
	int f = open("rotor_ukw_enigma1.txt",O_RDONLY);
	int deplacement_offset = 5*38 + ((int) (lettre_reflector - 'A'))*33;
	
	lseek(f,deplacement_offset,SEEK_SET);
	read(f, buff, sizeof(buff) - 1);  
   	buff[sizeof(buff) - 1] = '\0';
	sscanf(buff,"%s %s",nom_ukw,buff_alphabet);
	
	rf->l_r = (Rotor**) malloc(taille * sizeof(Rotor*)); 
	rf->reflector = strdup(buff_alphabet);
	rf->taille = taille;
	close(f);

	for(i=0; i<len; i++){
		if (nums_rotors[i] < 1 || nums_rotors[i] > 5) {
            printf("Veuillez choisir un rotor entre 1 et 5\n");
            liberer_rf(rf);  
            return NULL;
        }

    // Vérification que lettres_associes contient des lettres valides
   		if (lettres_associes[i] < 'A' || lettres_associes[i] > 'Z') {
            printf("La lettre tappé doit etre en majuscule et comprise en A et Z");
            liberer_rf(rf);  
            return NULL;
        }

		rf->l_r[i] = init_rotor(*(nums_rotors+i), *(lettres_associes +i));
	}
	
	return rf;
}


int trouver_lettre(char* AlphabetM, char l,int pos){
	int i;
	for(i=0; i<strlen(AlphabetM);i++){
		if(i==26){
			break;
		}
		if(AlphabetM[(i+pos)%26] ==l){
			return i;
		}
	}
	return 0;
}

int trouver_lettre_bis(char* AlphabetM, char l,int pos){
	int i;
	for(i=26; i>0;i--){
		if(AlphabetM[(pos+i)%26] ==l){
			printf("paf");
			return 26-i;
		}
	}
	return -1;
}

char resultat_rf(char c, RotorsReflector* rf){
	int position_lettre,i;
	char v;
	Rotor** l_r = rf->l_r;
	l_r[0]->pos = (l_r[0]->pos +1)%26;
	i = 0;
	while((int) l_r[i]->pos  == (int) (l_r[i]->turnover - 'A'+1) && i<2){
		l_r[i+1]->pos = (l_r[i+1]->pos +1)%26;
		i++;
	}
	
	for(i=0; i<rf->taille;i++){
	//	printf("%c -> ", c);
		position_lettre = ((int)(c-'A') + l_r[i]->pos ) % 26;
        	c = l_r[i]->value[position_lettre];
        	c =(c - l_r[i]->pos -'A'+52)%26 +'A';
 
	}
	//printf("%c -> ", c);
	position_lettre =( (int) (c -'A') )%26;
	c = rf->reflector[position_lettre];
	
	for (i = rf->taille - 1; i >= 0; i--) {
	//	printf("r %c -> ", c);
		v = (c + l_r[i]->pos -'A')%26 +'A';
       		position_lettre = trouver_lettre(l_r[i]->value ,v,0);
       		c = 'A' + (position_lettre - l_r[i]->pos + 26)%26 ; 
 
   }
	
	return c;
}

void liberer_rf(RotorsReflector* rf) {
    if (rf == NULL) {
        return; 
    }

    if (rf->l_r != NULL) {
        for (int i = 0; i < 3; i++) {  
            if (rf->l_r[i] != NULL) {
                free(rf->l_r[i]->value);  
                free(rf->l_r[i]); 
            }
        }
        free(rf->l_r); 
    }
    free(rf->reflector);
    free(rf);
}
Rotor* modifier_rotor(Rotor* r,int num_rotor, char lettre){
	char buff[40]; //buffer de la taille d'une ligne dans le fichier rotor_ukw_enigma1.txt
	char buff2[28]; //buffer pour stocker l'alphabet réarranger
	char nom_rotor[8];
	/*bout de code permettant d'ouvrir le fichier contenant les rotors, de déplacer l'offset a la ligne voulue, et de lire cette ligne. */
	int f = open("rotor_ukw_enigma1.txt",O_RDONLY);
	if(num_rotor !=1){
		lseek(f,((num_rotor-1)*38),SEEK_SET);
	}
	else{
		lseek(f,0,SEEK_SET);
	}
	read(f, buff, sizeof(buff) - 1);  
    	buff[sizeof(buff) - 1] = '\0'; 
//assignation des valeurs au nouveau rotor.
	sscanf(buff, "%s %s %c %c",nom_rotor,buff2,&(r->notch),&(r->turnover));
	strcpy(r->value,buff2);
	close(f);
	
	r->pos = ((int) lettre-'A')%26;
	return r;
}
RotorsReflector* modifier_rf(RotorsReflector* rf,int* nums_rotors, char* lettres_associes, char lettre_reflector,int taille){
	int i;
	int len =3;
	char buff[40]; 
	char buff_alphabet[28];
	char nom_ukw[5];
	int f = open("rotor_ukw_enigma1.txt",O_RDONLY);
	int deplacement_offset = 5*38 + ((int) (lettre_reflector - 'A'))*33;
	
	lseek(f,deplacement_offset,SEEK_SET);
	read(f, buff, sizeof(buff) - 1);  
   	buff[sizeof(buff) - 1] = '\0';
	sscanf(buff,"%s %s",nom_ukw,buff_alphabet);
	
	strcpy(rf->reflector,buff_alphabet);
	rf->taille = taille;
	close(f);
	Rotor* r;
	for(i=0; i<len; i++){
		rf->l_r[i] = modifier_rotor(rf->l_r[i], nums_rotors[i] , lettres_associes[i]);
		r = rf->l_r[i];
		printf("%s, %d \n",r->value, r->pos);
	}
	printf("%s \n",rf->reflector);
	return rf;
}
void affiche_rotor(RotorsReflector* rf){
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

/*
int main(){
	//init_rotor(3,'A');
	int tab[] = {1,1,1};
	int tab2[] = {3,3,3};
	char c;
	RotorsReflector* rf = init_rotorsreflector(tab,"AAA", 'C',3);
	RotorsReflector* rf2 = init_rotorsreflector(tab2,"AAA", 'C',3);
	c = resultat_rf('O',rf);
	printf("%c \n", c);
	c = resultat_rf('P',rf);
	printf("%c \n", c);
	c = resultat_rf('E',rf);
	printf("%c \n", c);
	c = resultat_rf('R',rf);
	printf("%c \n", c);
	c = resultat_rf('A',rf);
	printf("%c \n", c);
	rf = modifier_rf(rf2,tab,"AAA", 'C',3);
	c = resultat_rf('R',rf);
	printf("%c \n", c);
	c = resultat_rf('H',rf);
	printf("%c \n", c);
	c = resultat_rf('L',rf);
	printf("%c \n", c);
	c = resultat_rf('H',rf);
	printf("%c \n", c);
	c = resultat_rf('O',rf);
	printf("%c \n", c);
	printf("%d \n",rf->l_r[1]->pos);
	liberer_rf(rf);
	return 0;
}*/
