#pragma warning(disable:4996)
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum { FAUX = 0, VRAI = 1 } Booleen;
Booleen EchoActif = FAUX;


// Messages emis par les instructions -----------------------------------------
#define MSG_DEVELOPPE "## nouvelle specialite \"%s\" ; cout horaire \"%d\"\n" 
#define MSG_EMBAUCHE "## nouveau travailleur \"%s\" competent pour la specialite \"%s\"\n"
#define MSG_DEMARCHE "## nouveau client \"%s\"\n"
#define MSG_COMMANDE "## nouvelle commande \"%s\", par client \"%s\"\n"
#define MSG_TACHE "## la commande \"%s\" requiere la specialite \"%s\" (nombre d'heures \"%d\")\n"
#define MSG_PROGRESSION "## pour la commande \"%s\", pour la specialite \"%s\" : \"%d\" heures de plus ont ete realisees\n"
#define MSG_PROGRESSION_PASSE "## une reallocation est requise\n"
#define MSG_SPECIALITES "## consultation des specialites\n"
#define MSG_TRAVAILLEURS "## consultation des travailleurs competents pour la specialite \"%s\"\n"
#define MSG_TRAVAILLEURS_TOUS "## consultation des travailleurs competents pour chaque specialite\n"
#define MSG_CLIENT_NOM "## consultation des commandes effectuees par \"%s\"\n"
#define MSG_CLIENT_TOUS "## consultation des commandes effectuees par chaque client\n"
#define MSG_SUPERVISION "## consultation de l'avancement des commandes\n"
#define MSG_CHARGE "## consultation de la charge de travail de \"%s\"\n"
#define MSG_INTERRUPTION "## fin de programme\n" 

// Lexemes -------------------------------------------------------------------- 
#define maxNombre 10000
#define LGMOT 35
#define NBCHIFFREMAX 5
#define maxSpecialites 10
#define maxTravailleurs 50
#define maxClients 100
#define maxCommandes 500

typedef char Mot[LGMOT + 1];


// Variables globales
int countEntier = 0, countClients = 0, countSpecialites = 0, countTravailleurs = 0, countCommandes = 0, countTache = 0, countExecution = 0, cle_progression=0 , cle_passe=0;

void get_id(Mot id) {
	scanf("%s", id);
	if (EchoActif) printf(">>echo %s\n", id);
}

int get_int() {
	char buffer[NBCHIFFREMAX + 1];
	scanf("%s", buffer);
	if (EchoActif) printf(">>echo %s\n", buffer);
	return atoi(buffer);
}



// Instructions --------------------------------------------------------------- 
// developpe <mot: Specialite> <entier: cout horaire>
void traite_developpe() {
	Mot nom_specialite;			// this is a char type
	get_id(nom_specialite);
	int cout_horaire = get_int();
	int checkredondance = 0; // pour vérifier si on répete la spécialité
	printf(MSG_DEVELOPPE, nom_specialite, cout_horaire);
}
// embauche <mot: nom de travailleur> <mot: specialite> 
void traite_embauche() {
	Mot nom_travailleur;
	Mot nom_specialite;
	get_id(nom_travailleur);
	get_id(nom_specialite);
	
	printf(MSG_EMBAUCHE, nom_travailleur, nom_specialite);
				
}

// demarche <mot: Client>
void traite_demarche() {
	Mot nom_client;
	get_id(nom_client);
	int checkRedondance = 0;
	
	printf(MSG_DEMARCHE, nom_client);
		
}

// commande <mot: la commande > <mot: nom du client effectuant la commande> / affectuer une commande par un client déjà existe
void traite_commande() {
	Mot nom_commande;
	Mot nom_client;
	get_id(nom_commande);
	get_id(nom_client);
	
	printf(MSG_COMMANDE, nom_commande, nom_client);
		
}

// tache -------------------------------
void traite_tache() {
	Mot nom_commande;
	Mot nom_specialite;
	get_id(nom_commande);
	get_id(nom_specialite);
	int nombre_heures = get_int();
	
	printf(MSG_TACHE, nom_commande, nom_specialite, nombre_heures);
			
}

// progression--------------------------
void traite_progression() {// remaruqe dans cette fonction je garde l'indice de chaque tache
	

	if ((cle_progression == 1) && (cle_passe == 0)) {
		Mot nom_command;
		Mot nom_specialite;
		get_id(nom_command);
		get_id(nom_specialite);
		int nombre_heure_avancement = get_int();
		printf(MSG_PROGRESSION, nom_command, nom_specialite, nombre_heure_avancement);
	}
	else if (cle_progression == 1 && cle_passe == 1) {
		printf(MSG_PROGRESSION_PASSE);
		cle_progression = 0;
		cle_passe = 0;
	}
	else if (cle_progression == 0 && cle_passe == 1) {
		printf("le mot cle passe est utiliser uniquement qu'avec le mot cle progression\n");
		cle_passe = 0;
	}
}


// afficher les travailleurs et leurs specialites----------------------------
void traite_travailleurs() {
	Mot nom_specialite;
	get_id(nom_specialite);
	if (strcmp(nom_specialite, "tous") == 0) {
	printf(MSG_TRAVAILLEURS_TOUS);
		
	}
	else {
		printf(MSG_TRAVAILLEURS, nom_specialite);
	}
		
}

// Afficher les Clients et leurs commandes----------------------------------
void traite_client() {
	Mot nom_client;
	get_id(nom_client);
	if (strcmp(nom_client, "tous") == 0) {
		printf(MSG_CLIENT_TOUS);
	}
	else
	{
		printf(MSG_CLIENT_NOM, nom_client);
	}
}

// supervision ------------------------
void traite_supervision() {
	printf(MSG_SUPERVISION);
}

// charge ---------------------------
void traite_charge() {
	Mot nom_client;
	get_id(nom_client);
	printf(MSG_CHARGE, nom_client);
}

// interruption ------------------------ 
void traite_interruption() {
	printf(MSG_INTERRUPTION);
}
//Boucle principale ---------------------------------------------------------- 
int main(int argc, char* argv[]) {
	if (argc >= 2 && strcmp("echo", argv[1]) == 0) {
		EchoActif = VRAI;
	}
	Mot buffer;
	while (VRAI) {
		get_id(buffer);
		if (strcmp(buffer, "developpe") == 0) {
			cle_progression = 0;
			traite_developpe();
			continue;
		}
		if (strcmp(buffer, "embauche") == 0) {
			cle_progression = 0;
			traite_embauche();
			continue;
		}
		if (strcmp(buffer, "demarche") == 0) {
			cle_progression = 0;
			traite_demarche();
			continue;
		}
		if (strcmp(buffer, "commande") == 0) {
			cle_progression = 0;
			traite_commande();
			continue;
		}
		if (strcmp(buffer, "tache") == 0) {
			cle_progression = 0;
			traite_tache();
			continue;
		}
		if (strcmp(buffer, "progression") == 0) {
			cle_progression = 1;
			traite_progression();
			continue;
		}
		if (strcmp(buffer, "travailleurs") == 0) {
			cle_progression = 0;
			traite_travailleurs();
			continue;
		}
		if (strcmp(buffer, "client") == 0) {
			cle_progression = 0;
			traite_client();
			continue;
		}
		if (strcmp(buffer, "specialites") == 0) {
			cle_progression = 0;
			printf(MSG_SPECIALITES);
			continue;
		}
		if (strcmp(buffer, "supervision") == 0) {
			cle_progression = 0;
			traite_supervision();
			continue;
		}
		if (strcmp(buffer, "charge") == 0) {
			cle_progression = 0;
			traite_charge();
			continue;
		}
		if (strcmp(buffer, "clear") == 0) {
			cle_progression = 0;
			system("cls");
			continue;
		}
		if (strcmp(buffer, "interruption") == 0) {
			cle_progression = 0;
			traite_interruption();
			break;
		}
		if (strcmp(buffer, "passe") == 0) {
			cle_passe = 1;
			traite_progression();
		}
		else printf("!!! instruction inconnue >%s< !!!\n", buffer);
	}
	return 0;
}
