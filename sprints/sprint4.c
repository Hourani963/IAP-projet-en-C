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
#define MSG_TRAVAILLEURS "la specialite %s peut etre prise en charge par : "

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
#define maxTravailleurs 5
#define maxClients 100
#define maxCommandes 500

typedef char Mot[LGMOT + 1];


// Variables globales
int countEntier = 0, countClients = 0, countSpecialites = 0, countTravailleurs = 0, countCommandes = 0, countTache = 0, countExecution = 0, cle_progression = 0, cle_passe = 0, countAffectation = 0, count_travailleur_avec_heure_restant = 0, cle_tache_affectation = 0, countTravailleurSansRedondance = 0;
Mot needit;
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


typedef struct {	// developpe structure
	Mot Specialite;
	int PricePerHour;
}SPES;

typedef struct {	// Embauche structure
	Mot NOM;
	Mot Specialite_TRAV;
	Booleen tags_competences[maxSpecialites];
	int nombre_tache_pour_chaque_travailleur;
	int heure_restant;

}TRAVAILLEUR;

typedef struct {
	Mot nomTravailleur;
	Mot nomCommande;
	Mot nomSpecialite;
	int nombre_heure_commande;
	int nombre_heure_commande_avancement;
}AFFECTATION;

typedef struct {
	Mot Client;
}Clients;

typedef struct {
	Mot Commande;
	Mot Client_Commande;
}Commandes;

typedef struct {
	Mot Commande_TACHE;
	Mot Specialite_TACHE;
	int nombreHeureTache;
}TACHES;

typedef struct {
	Mot Commande_exec;
	Mot Specialite_exec;
	int nombreHeureTache_avancement;
}EXECUTION;
// Instructions --------------------------------------------------------------- 
// developpe <mot: Specialite> <entier: cout horaire>
void traite_developpe(SPES* developpe) {
	Mot nom_specialite;			// this is a char type
	get_id(nom_specialite);
	int cout_horaire = get_int();
	int checkredondance = 0; // pour v??rifier si on r??pete la sp??cialit??
	for (int i = 0; i < countSpecialites; i++) {	// parc
		if (strcmp(developpe[i].Specialite, nom_specialite) == 0) checkredondance = 1;

	}
	if ((countSpecialites < maxSpecialites) && (cout_horaire <= maxNombre) && (checkredondance == 0)) {
		//printf(MSG_DEVELOPPE, nom_specialite, cout_horaire);		masquer pour le sprint 2

		strcpy(developpe[countSpecialites].Specialite, nom_specialite);
		developpe[countSpecialites].PricePerHour = cout_horaire;
		countSpecialites++;
	}
	else if (cout_horaire <= maxNombre && checkredondance == 1)
	{
		printf("La sp??cialit?? %s existe d??j??!!\n", nom_specialite);
	}
	else if (cout_horaire <= maxNombre)
	{
		printf("vous ne pouvez pas avoir plus de %d Specialites.\n", maxSpecialites);
	}
	else if (cout_horaire > maxNombre)
	{
		printf("vous ne puvez pas saisir un num??ro sup??rieur ?? %d\n", maxNombre);

	}
}
// embauche <mot: nom de travailleur> <mot: specialite> 
void traite_embauche(TRAVAILLEUR* travailleur, SPES* developpe, TRAVAILLEUR* travailleurSansRedondance) {
	Mot nom_travailleur;
	Mot nom_specialite;
	get_id(nom_travailleur);
	get_id(nom_specialite);

	int new = 0;
	if (countTravailleurSansRedondance == 0) { // ici je met que les nom des travailleurs sans redondance des noms
		strcpy(travailleurSansRedondance[0].NOM, nom_travailleur);
		countTravailleurSansRedondance++;
	}
	else
	{
		for (int i = 0; i < countTravailleurSansRedondance; i++) {
			if (strcmp(travailleurSansRedondance[i].NOM, nom_travailleur) == 0) {
				new = 1;
			}
		}
		if (new == 0) {

			strcpy(travailleurSansRedondance[countTravailleurSansRedondance].NOM, nom_travailleur);
			countTravailleurSansRedondance++;
		}
	}


	int get_specialite_ordre = 0;
	for (int i = 0; i < countSpecialites; i++) {
		if (strcmp(developpe[i].Specialite, nom_specialite) == 0) get_specialite_ordre = i;
	}
	int checkSpecialite = 0;	// cette valeur sera 1 si la sp??cialit?? qu'on demande existe
	int checkRedondance_nomETspec = 0;	// cette valeur sera 1 si on saisi un nom existant d??j?? dans le tableau

	for (int i = 0; i < countTravailleurs; i++) { //parcour le tableau des noms des travailleurs pour savoir si le nom qu'on a saisi existe d??j??.	nom et meme sp??cialit??
		if ((strcmp(travailleur[i].NOM, nom_travailleur) == 0) && (strcmp(travailleur[i].Specialite_TRAV, nom_specialite) == 0)) {
			checkRedondance_nomETspec = 1;  // =1 si le nom existe
			printf("le travailleur %s a d??j?? ??t?? d??clar?? pour la m??me sp??cialit??\n", nom_travailleur);
		}
	}
	//printf("le nom <%s> d??j?? existe. On ne peut pas avoir deux travailleur de m??me nom\n",nom_travailleur);
	if ((countTravailleurs < maxTravailleurs) && (checkRedondance_nomETspec == 0))// ajouter le nom et la sp??cialit?? dans les tableaux
	{
		if (countSpecialites == 0) {  // si on ne d??clare aucune sp??cialit?? dans le programme
			printf("il n'y a aucune sp??cialit?? d??clar??e, traitement imposible\n");
		}
		else if (countSpecialites > 0) { // on a d??clar?? des specialit??s avant et on va v??rifier si la sp??cialit?? qu'on domande existe d??j????
			for (int i = 0; i < countSpecialites; i++) { // parcourir le tableu de sp??cialit??
				if (strcmp(developpe[i].Specialite, nom_specialite) == 0) checkSpecialite = 1; //=1 alors la sp??cialit?? exist?? d??j??
			}
			if (checkSpecialite == 1) {			// la sp??cialit?? a d??j?? ??t?? d??clar??
				//printf(MSG_EMBAUCHE, nom_travailleur, nom_specialite);	masquer pour le sprint 2
				strcpy(travailleur[countTravailleurs].NOM, nom_travailleur);
				strcpy(travailleur[countTravailleurs].Specialite_TRAV, nom_specialite);

				if (countTravailleurs == 0) { travailleur[countTravailleurs].tags_competences[get_specialite_ordre] = VRAI; } // utiliser la m??thode de sous-tableau, pour resoudre le probl??me de commande (travailleurs tous)
				else if (countTravailleurs != 0)
				{
					int check = 0;
					for (int s = 0; s < countTravailleurs; s++) {
						if (strcmp(travailleur[s].NOM, nom_travailleur) == 0) {
							travailleur[s].tags_competences[get_specialite_ordre] = VRAI;
							check = 1;
							break;
						}
					}if (check == 0) {
						travailleur[countTravailleurs].tags_competences[get_specialite_ordre] = VRAI;
					}
				}
				countTravailleurs++;
			}
			else {
				printf("Specialit?? <%s> n'a pas ??t?? d??clar??, traitement impossible\n", nom_specialite);
			}
		}
	}
	else if (countTravailleurs > maxTravailleurs)
	{
		printf("vous ne pouvez pas avoir plus de %d travailleurs.\n", maxTravailleurs);
	}
}

// demarche <mot: Client>
void traite_demarche(Clients* Client) {
	Mot nom_client;
	get_id(nom_client);
	int checkRedondance = 0;
	for (int i = 0; i < countClients; i++) {
		if (strcmp(Client[i].Client, nom_client) == 0) {
			checkRedondance = 1;  // =1 si le nom existe
			printf("the name <%s> is already exist !!\n", nom_client);
		}
	}
	if (countClients < maxClients && checkRedondance == 0) {
		//printf(MSG_DEMARCHE, nom_client);	masquer pour le sprint 2
		strcpy(Client[countClients].Client, nom_client);
		countClients++;
	}
	else if (countClients >= maxClients)
	{
		printf("vous ne pouvez pas avoir plus de %d Clients", maxClients);
	}
}

// commande <mot: la commande > <mot: nom du client effectuant la commande> / affectuer une commande par un client d??j?? existe
void traite_commande(Commandes* Commande, Clients* Client) {
	Mot nom_commande;
	Mot nom_client;
	get_id(nom_commande);
	get_id(nom_client);
	int checkRedondance_client = 0;

	for (int i = 0; i < countClients; i++) { //parcour le tableau des Client. pour la redondance
		if (strcmp(Client[i].Client, nom_client) == 0) {
			checkRedondance_client = 1;  // =1 si le client d??j?? existe
		}
	}

	int checkRedondance_commande = 0; // deux clients ne pouvez pas commendez la meme chose
	for (int i = 0; i < countCommandes; i++) {
		if (strcmp(Commande[i].Commande, nom_commande) == 0) {
			checkRedondance_commande = 1;  // =1 si la commande d??j?? existe
		}
	}

	if (countCommandes < maxCommandes && checkRedondance_client == 1 && checkRedondance_commande == 0) {
		strcpy(Commande[countCommandes].Commande, nom_commande);
		strcpy(Commande[countCommandes].Client_Commande, nom_client);
		//printf(MSG_COMMANDE, nom_commande, nom_client);
		countCommandes++;
	}
	else if (countCommandes >= maxCommandes)
	{
		printf("vous ne pouvez pas avoir plus de %d Commandes\n", maxCommandes);
	}
	else if (checkRedondance_client == 0)// si le client n'existe pas
	{
		printf("Il n'y a pas un client sous le nom de \"%s\" \n", nom_client);
	}
	else if (checkRedondance_commande == 1) {
		printf("vous ne pouvez pas avoir deux commande de meme nom \"%s\"\n", nom_commande);
	}
}

// tache -------------------------------
void traite_tache(TACHES* tache, Commandes* commande, SPES* developpe, AFFECTATION* affectationAUTO, EXECUTION* execution, TRAVAILLEUR* travailleurs_meme_specialite, TRAVAILLEUR* travailleur, int* indiceSpecialite, int* indiceTache) {
	Mot nom_commande;
	Mot nom_specialite;
	get_id(nom_commande);
	get_id(nom_specialite);
	int nombre_heures = get_int();
	int checkCommande_exist = 0;
	int checkSpecialite_exist = 0;
	int redondance_commande = 0;

	for (int i = 0; i < countCommandes; i++) { // v??rifier si le nom de commande d??j?? existe
		if (strcmp(commande[i].Commande, nom_commande) == 0) {
			checkCommande_exist = 1;
		}
	}
	for (int i = 0; i < countSpecialites; i++) { // v??rifier si le nom de sp??cialit?? d??j?? existe
		if (strcmp(developpe[i].Specialite, nom_specialite) == 0) {
			checkSpecialite_exist = 1;
			*indiceSpecialite = i;

		}
	}
	if ((checkCommande_exist == 1) && (checkSpecialite_exist == 1)) { // la commande existe et la specialite existe
		if (countTache == 0) {
			//printf(MSG_TACHE, nom_commande, nom_specialite, nombre_heures);
			strcpy(tache[countTache].Commande_TACHE, nom_commande);
			strcpy(tache[countTache].Specialite_TACHE, nom_specialite);
			tache[countTache].nombreHeureTache = nombre_heures;
			*indiceTache = countTache;
			cle_tache_affectation = 1; // on peut entrer dans la fonction affectation
			countTache++;
		}
		else
		{
			for (int i = 0; i < countTache; i++) { // pour ne pas avoir une redondance des taches
				if ((strcmp(tache[i].Commande_TACHE, nom_commande) == 0) && (strcmp(tache[i].Specialite_TACHE, nom_specialite) == 0)) {
					printf("ce commande est d??j?? existe pour la meme specialite\n");
					redondance_commande = 1;
				}
			}
			if (redondance_commande == 0)
			{
				//printf(MSG_TACHE, nom_commande, nom_specialite, nombre_heures);
				strcpy(tache[countTache].Commande_TACHE, nom_commande);
				strcpy(tache[countTache].Specialite_TACHE, nom_specialite);
				tache[countTache].nombreHeureTache = nombre_heures;
				*indiceTache = countTache;
				cle_tache_affectation = 1; // on peut entrer dans la fonction affectation  **************************************
				countTache++;
			}
		}
	}
	else if (checkCommande_exist == 0 && checkSpecialite_exist == 1) { // specialite existe		commande n'existe pas
		printf("La Commande \"%s\" n'existe pas dans la base de donn??e\n", nom_commande);
	}
	else if (checkSpecialite_exist == 0 && checkCommande_exist == 1) { //specialite n'existe pas		comande existe
		printf("la specialite \"%s\" n'existe pas dans la base de donn??e\n", nom_specialite);
	}
	else	// ni la commande ni la spaecialite n'existent
	{
		printf("ni la commande \"%s\" ,ni la specialite \"%s\" existent dans la basse de donn??e\n", nom_commande, nom_specialite);
	}
}

// progression--------------------------
void traite_progression(int* indiceTache, TRAVAILLEUR* travailleur_sans_redondance, TRAVAILLEUR* travailleur, EXECUTION* execution, TACHES* tache, AFFECTATION* affectationAUTO, int* indiceAffectation) {// remaruqe dans cette fonction je garde l'indice de chaque tache dans le tab execution

	if ((cle_progression == 1) && (cle_passe == 0)) {
		Mot nom_command;
		Mot nom_specialite;
		get_id(nom_command);
		get_id(nom_specialite);
		int nombre_heure_avancement = get_int();

		int checkTache_existe = 0;
		int get_indice = 0;
		for (int i = 0; i < countTache; i++) { // v??rifier si le nom de commande d??j?? existe dans les taches
			if (((strcmp(tache[i].Commande_TACHE, nom_command) == 0) && (nombre_heure_avancement < tache[i].nombreHeureTache) && (strcmp(tache[i].Specialite_TACHE, nom_specialite) == 0))) {
				checkTache_existe = 1;
				get_indice = i;
			}
		}
		if (checkTache_existe == 1) { //= 1 alors la tache existe
			execution[get_indice].nombreHeureTache_avancement += nombre_heure_avancement;// (ancin value += nouveux value)= la somme d'heures;

			if (execution[get_indice].nombreHeureTache_avancement < tache[get_indice].nombreHeureTache) {
				for (int i = 0; i < countAffectation; i++) {
					if (strcmp(affectationAUTO[i].nomCommande, nom_command) == 0 && strcmp(affectationAUTO[i].nomSpecialite, nom_specialite) == 0) {
						affectationAUTO[i].nombre_heure_commande_avancement = execution[get_indice].nombreHeureTache_avancement;// ici je relie affectation avec execution
					}
				}
				//affectationAUTO[*indiceTache].nombre_heure_commande_avancement = execution[get_indice].nombreHeureTache_avancement;// ici je relie affectation avec execution

																																   //for (int i = 0; i < countTravailleurs; i++) {// recopier la valeur de nombre d'heaur dans le tab principal des travailleurs
					//if (strcmp(affectationAUTO[*indiceTache].nomTravailleur, travailleur[i].NOM) == 0 && strcmp(affectationAUTO[*indiceTache].nomSpecialite, travailleur[i].Specialite_TRAV) == 0) {
						//travailleur[i].heure_restant = affectationAUTO[*indiceTache].nombre_heure_commande - affectationAUTO[*indiceTache].nombre_heure_commande_avancement; // mise ?? jour de la valeur de nombre d'heure restant pour chaque travailleur
					//}
				//}

				/*for (int i = 0; i < countTravailleurSansRedondance; i++) {//**********
					for (int j = 0; j < countAffectation; j++) {
						if (strcmp(affectationAUTO[j].nomCommande, nom_command)==0 && strcmp(affectationAUTO[j].nomSpecialite,nom_specialite)==0 && strcmp(travailleur_sans_redondance[i].NOM,affectationAUTO[j].nomTravailleur)==0) {
							travailleur_sans_redondance[i].heure_restant -= affectationAUTO[*indiceAffectation].nombre_heure_commande_avancement;
						}
					}
				}*/

				//printf(MSG_PROGRESSION, nom_command, nom_specialite, nombre_heure_avancement);
				strcpy(execution[get_indice].Commande_exec, nom_command);
				strcpy(execution[get_indice].Specialite_exec, nom_specialite);
			}
			else {
				printf("tache terminer, (avancement) %d >= %d (nombre d'heure) \n", execution[get_indice].nombreHeureTache_avancement, tache[get_indice].nombreHeureTache);
			}
		}
		if ((nombre_heure_avancement <= maxNombre) && (checkTache_existe == 0)) {
			printf("la tache pour nom de commande %s et la specialite %s n'existe pas, execution impossible!!\n", nom_command, nom_specialite);
		}
	}
	else if (cle_progression == 1 && cle_passe == 1)
	{
		//printf(MSG_PROGRESSION_PASSE);
		cle_progression = 0;
		cle_passe = 0;
	}
	else if (cle_progression == 0 && cle_passe == 1)
	{
		printf("le mot cle passe est utiliser uniquement qu'avec le mot cle progression\n");
		cle_passe = 0;
	}
}

// AUTO affectation---------------------------
void traite_affectaion(TRAVAILLEUR* travailleur_sans_redondance, int* indiceSpecialite, int* indiceTache, int* indiceAffectation, TACHES* tache, Commandes* commande, SPES* developpe, AFFECTATION* affectationAUTO, EXECUTION* execution, TRAVAILLEUR* travailleurs_meme_specialite, TRAVAILLEUR* travailleur) {
	if (cle_tache_affectation == 1) {
		for (int i = 0; i < countTravailleurSansRedondance; i++) { // ici je met la somme des heure de travaille de chaque travailleur avec les tags_competence de chaque travailleur. tab tous les travailleur
			for (int j = 0; j < countTravailleurs; j++) {
				if (strcmp(travailleur_sans_redondance[i].NOM, travailleur[j].NOM) == 0) { // calculer la somme d'heure de chaque travailleur dans toute les spes
					travailleur_sans_redondance[i].heure_restant += travailleur[j].heure_restant;

					for (int s = 0; s < countSpecialites; s++) {
						if (travailleur_sans_redondance[i].tags_competences[s] == VRAI); // cette condition est obligatoire pour ne pas verser l'etat car dans le tableau principal il y a une redondence de nom de trav, et les redondance poss??de l'etat (FAUX)
						else {// copier le tag_competence
							travailleur_sans_redondance[i].tags_competences[s] = travailleur[j].tags_competences[s];
						}
					}
				}
			}
		}
		/*for (int i = 0; i < countTravailleurSansRedondance; i++) {
			printf("\t nom:(%s)  heure_reste:(%d)\n", travailleur_sans_redondance[i].NOM, travailleur_sans_redondance[i].heure_restant);
		}*/
		TRAVAILLEUR t[maxCommandes]; // ce tab est important de le creer ici pour le vider ?? chaque fois. il faut changer le nom parcontre.
		int count_trav_de_meme_spe = 0; // nombre d'??l??ment dans t
		for (int j = 0; j < countTravailleurSansRedondance; j++) { // choisir les travailleurs ayant les m??me specialites et les ins??rer dans un nouveau tab
			if (travailleur[j].tags_competences[*indiceSpecialite] == VRAI) {
				strcpy(t[count_trav_de_meme_spe].NOM, travailleur_sans_redondance[j].NOM);
				strcpy(t[count_trav_de_meme_spe].Specialite_TRAV, developpe[*indiceSpecialite].Specialite);
				//travailleurs_meme_specialite[count_trav_de_meme_spe].nombre_tache_pour_chaque_travailleur += travailleur[j].nombre_tache_pour_chaque_travailleur;
				t[count_trav_de_meme_spe].heure_restant = travailleur_sans_redondance[j].heure_restant;
				count_trav_de_meme_spe++;
			}
		}
		//printf("\n");
		/*for (int i = 0; i < countTravailleurSansRedondance; i++) {
			printf("\t nom:(%s) spe:(%s) heure_reste:(%d)\n", t[i].NOM, t[i].Specialite_TRAV, t[i].heure_restant);
		}*/

		int n = 0;
		int valeur_min;
		for (int i = 0; i < count_trav_de_meme_spe; i++) {// choisir la valeur minimal de (travailleurs_meme_specialite)
			if (t[n].heure_restant > t[i].heure_restant) {
				n = i;
			}
		}
		valeur_min = t[n].heure_restant;
		//printf("\tla valeur minimal est : %d ?? l'indice %d\n", valeur_min, n);
		// il fait faire des conditions ici
		//1 il faut savoir si il y a plusieur valeur min. 

		int travailleur_sans_tache = 0;
		int plusieur_valeur_min = 0;
		int indice_valeurs_min[maxCommandes]; // si il y a de probleme il faut l'initialiser
		for (int i = 0; i < count_trav_de_meme_spe; i++) {
			if (t[i].heure_restant == 0) {		 // si aucun travailleur de meme specialite n'a de tache encore, alors on choisie par ordre de specialit??
				travailleur_sans_tache++;
			}
			if (t[i].heure_restant == valeur_min) { //si il y a plusieur valeur min on choisi dans l'ordre de la specialite 
				plusieur_valeur_min++;
				indice_valeurs_min[i] = 1;
			}
		}
		if (travailleur_sans_tache == count_trav_de_meme_spe) { // le nombre d'heur de tous les travs est 0.  faut ??tudier la condition o?? (travailleur_sans_tache < count_trav_de_meme_spe)
			//printf("\t\t travailleur sans taches = %d\n", travailleur_sans_tache);
			for (int i = 0; i < countCommandes; i++) {
				if (strcmp(tache[*indiceTache].Specialite_TACHE, travailleur[i].Specialite_TRAV) == 0) {
					strcpy(affectationAUTO[*indiceTache].nomTravailleur, travailleur[i].NOM);
					strcpy(affectationAUTO[*indiceTache].nomSpecialite, travailleur[i].Specialite_TRAV);
					strcpy(affectationAUTO[*indiceTache].nomCommande, tache[*indiceTache].Commande_TACHE);
					affectationAUTO[*indiceTache].nombre_heure_commande = tache[*indiceTache].nombreHeureTache;
					travailleur[i].heure_restant = affectationAUTO[*indiceTache].nombre_heure_commande;
					countAffectation++;
					break; // break est obligatoi sinon on selection le derni??re travailleur de cette specialite.avec break on choisie que le premi??re
				}
			}
		}
		if (plusieur_valeur_min == 1) { // le cas le plus g??n??ral. on choisi forcement le travailleur qui a le moins d'heure.
			strcpy(affectationAUTO[*indiceTache].nomTravailleur, t[n].NOM); // comme il a un seul trav avec valeur min alors la selue indice de cette valeur est n
			strcpy(affectationAUTO[*indiceTache].nomSpecialite, t[n].Specialite_TRAV);
			strcpy(affectationAUTO[*indiceTache].nomCommande, tache[*indiceTache].Commande_TACHE);
			affectationAUTO[*indiceTache].nombre_heure_commande = tache[*indiceTache].nombreHeureTache;
			countAffectation++;
			for (int i = 0; i < countTravailleurs; i++) { // recopier la valeur (heure_restant)  dans le tab travailleur
				if (strcmp(travailleur[i].NOM, affectationAUTO[*indiceTache].nomTravailleur) == 0 && strcmp(travailleur[i].Specialite_TRAV, affectationAUTO[*indiceTache].nomSpecialite) == 0) {
					travailleur[i].heure_restant = affectationAUTO[*indiceTache].nombre_heure_commande;
				}
			}
		}

		//printf("\n");
		/*for (int i = 0; i < countTache; i++) {
			printf("\t nom:(%s) spe:(%s) commande:(%s) heure_reste:(%d) avancement:(%d) \n", affectationAUTO[i].nomTravailleur, affectationAUTO[i].nomSpecialite, affectationAUTO[i].nomCommande, affectationAUTO[i].nombre_heure_commande, affectationAUTO[i].nombre_heure_commande_avancement);
		}*/

		for (int i = 0; i < countTravailleurSansRedondance; i++) { // je pense qu'il faut initialiser cete valeur ?? 0. voir la premi??re boucle de l'affectation
			travailleur_sans_redondance[i].heure_restant = 0;
		}
	}

}

// afficher les travailleurs et leurs specialites----------------------------
void traite_travailleurs(TRAVAILLEUR* travailleur, SPES* developpe, TRAVAILLEUR* travailleurs_tous) {
	Mot nom_specialite;
	get_id(nom_specialite);
	int n = 0;

	for (int i = 0; i < countSpecialites; i++) {	// bien classer les specialiter dans l'ordre de leur declaration puis pour chaque spe les travailleurs sont classer par l'ordre de leurs arriver ?? l'entreprise
		for (int j = 0; j < countTravailleurs; j++) {
			if (travailleur[j].tags_competences[i] == VRAI) {
				strcpy(travailleurs_tous[n].NOM, travailleur[j].NOM);
				strcpy(travailleurs_tous[n].Specialite_TRAV, developpe[i].Specialite);
				n++;
			}
		}
	}
	if (strcmp(nom_specialite, "tous") == 0) {// aficher dans l'ordre des specialites puis dans l'ordre des travailleurs
		int bien_afficher;
		for (int i = 0; i < countSpecialites; i++) {
			bien_afficher = 0;
			for (int j = 0; j < countTravailleurs; j++) {
				if (strcmp(travailleurs_tous[j].Specialite_TRAV, travailleur[i].Specialite_TRAV) == 0) {
					if (bien_afficher == 0) {
						printf("la specialite %s peut etre prise en charge par : ", developpe[i].Specialite);
						printf("%s", travailleurs_tous[j].NOM);
						bien_afficher = 1;
					}
					else
					{
						printf(", %s", travailleurs_tous[j].NOM);
					}
				}
			}printf("\n");
			//printf("\t%s %s \n", travailleurs_tous[i].Specialite_TRAV, travailleurs_tous[i].NOM); 
		}
	}
	else {

		int check_virgule = 0;  // ce variable est juste pour eviter l'errore d'affichage de virgule
		for (int i = 0; i < countTravailleurs; i++) {
			if (strcmp(travailleurs_tous[i].Specialite_TRAV, nom_specialite) == 0) { //parcourir le tableau de specialite pour savoir quel travailleur correspond ?? quel specialite
				if (check_virgule == 0) {
					printf(MSG_TRAVAILLEURS, nom_specialite);
					printf("%s", travailleurs_tous[i].NOM);
				}
				else printf(", %s", travailleurs_tous[i].NOM);
				check_virgule++;
			}
		}
		printf("\n");
	}
}

// Afficher les Clients et leurs commandes----------------------------------
void traite_client(Commandes* Commande, Clients* Client) {
	Mot nom_client;
	get_id(nom_client);
	int saut_ligne;
	int client_sans_commande;
	int virgule;
	if (strcmp(nom_client, "tous") == 0) {
		if (countCommandes == 0) {
			for (int i = 0; i < countClients; i++) {
				printf("le client %s a commande : \n", Client[i].Client);
			}
		}
		for (int i = 0; i < countClients; i++) {
			client_sans_commande = 0;
			virgule = 0;
			saut_ligne = 0;
			for (int j = 0; j < countCommandes; j++) {


				if ((strcmp(Client[i].Client, Commande[j].Client_Commande) == 0)) {
					//printf("\t%s %s\n ", Commande[j].Commande, Commande[j].Client_Commande);
					if (virgule == 0) {
						printf("le client %s a commande : %s", Client[i].Client, Commande[j].Commande);
						client_sans_commande = 1;
						virgule = 1;
					}
					else {
						printf(", %s", Commande[j].Commande);
						saut_ligne = 1;
					}
				}
			}
			if (saut_ligne == 1) printf("\n");
			if ((client_sans_commande == 0) && (countCommandes > 0))
			{
				printf("le client %s a commande : \n", Client[i].Client);
			}

			/*if (saut_ligne == 0)
			{
				printf("le client %s a commande : ", Client[i].Client);
			}*/
		}
	}
	else
	{
		int client_existe = 0;
		int client_pssede_commande = 0;
		int saut_virgule = 0;
		for (int i = 0; i < countClients; i++) { // v??rifier si le client existe
			if (strcmp(Client[i].Client, nom_client) == 0) {
				client_existe = 1;
			}
		}

		if (client_existe == 1) { // le client existe

			for (int i = 0; i < countCommandes; i++) { // v??rifier si le client poss??de une commande
				if (strcmp(Commande[i].Client_Commande, nom_client) == 0) {
					client_pssede_commande = 1;
				}
			}
			saut_ligne = 0;
			if (client_pssede_commande == 1) {// le client poss??de une commande
				for (int i = 0; i < countCommandes; i++) {
					if (strcmp(Commande[i].Client_Commande, nom_client) == 0) {
						if (saut_ligne == 0) { printf("le client %s a commande : %s", nom_client, Commande[i].Client_Commande); saut_virgule = 1; }
						else { printf(", %s", Commande[i].Client_Commande); }
					}
				}printf("\n");

			}
			else // le client ne poss??de pas une commande
			{
				printf("le client %s a commande : \n", nom_client);
			}
		}
		else //le client n'existe pas
		{
			printf("le client %s n'a pas ??t?? d??clar?? pr??c??dament\n", nom_client);
		}
	}
}

// supervision ------------------------
void traite_supervision(EXECUTION* execution, TACHES* tache, Commandes* commande, SPES* developpe, Clients* client) {
	/*for (int i = 0; i < countTache; i++) {
		printf("\t%s %s %d\n ", execution[i].Commande_exec, execution[i].Specialite_exec, execution[i].nombreHeureTache_avancement);
	}*/


	int virgule_correction;
	int commande_in_tach;
	int indice = 0;
	if (countTache == 0) {
		for (int i = 0; i < countCommandes; i++) {
			printf("etat des taches pour %s : \n", commande[i].Commande);
		}
	}
	else
	{
		for (int i = 0; i < countCommandes; i++) {
			commande_in_tach = 0;

			virgule_correction = 0;
			for (int spe = 0; spe < countSpecialites; spe++) {
				for (int j = 0; j < countTache; j++) {
					indice = i;
					if ((strcmp(commande[i].Commande, tache[j].Commande_TACHE) == 0) && (strcmp(developpe[spe].Specialite, tache[j].Specialite_TACHE) == 0)) {

						if (virgule_correction == 0) {
							printf("etat des taches pour %s : %s:%d/%d", tache[j].Commande_TACHE, tache[j].Specialite_TACHE, execution[j].nombreHeureTache_avancement, tache[j].nombreHeureTache);
							virgule_correction = 1;
							commande_in_tach = 1;
						}
						else
						{
							printf(", %s:%d/%d", tache[j].Specialite_TACHE, execution[j].nombreHeureTache_avancement, tache[j].nombreHeureTache);
						}
					}
				}
			}
			if (virgule_correction == 1) printf("\n");

			if (commande_in_tach == 0) {
				printf("etat des taches pour %s : \n", commande[i].Commande);
			}
		}
	}
}

// charge ---------------------------
void traite_charge(AFFECTATION* affectationAUTO, Commandes* commande, SPES* developpe, TRAVAILLEUR* travailleur) { // ordre commande puis ordre specialite
	Mot nom_travailleur;
	get_id(nom_travailleur);
	int vergule = 0;
	int travailleur_sans_charge = 1;
	int com = 0;
	for (int c = 0; c < countCommandes; c++) {
		for (int i = 0; i < countAffectation; i++) {
			if (strcmp(commande[c].Commande, affectationAUTO[i].nomCommande) == 0) {
				com = i;
				for (int s = 0; s < countSpecialites; s++) {
					if (strcmp(developpe[s].Specialite, affectationAUTO[com].nomSpecialite) == 0 && strcmp(affectationAUTO[com].nomTravailleur, nom_travailleur) == 0) {

						if (vergule == 0) {
							printf("charge de travail pour %s : %s/%s/%dheure(s)", affectationAUTO[i].nomTravailleur, affectationAUTO[i].nomCommande, affectationAUTO[i].nomSpecialite, affectationAUTO[i].nombre_heure_commande - affectationAUTO[i].nombre_heure_commande_avancement);
							vergule = 1;
							travailleur_sans_charge = 0;
						}
						else
						{
							printf(", %s/%s/%dheure(s)", affectationAUTO[i].nomCommande, affectationAUTO[i].nomSpecialite, affectationAUTO[i].nombre_heure_commande - affectationAUTO[i].nombre_heure_commande_avancement);
						}
					}
				}
			}

		}
	}
	if (travailleur_sans_charge == 1) {
		printf("charge de travail pour %s : ", nom_travailleur);
	}
	printf("\n");
}

// interruption ------------------------ 
void traite_interruption() {
	printf(MSG_INTERRUPTION);
}

//Boucle principale ---------------------------------------------------------- 
int main(int argc, char* argv[]) {
	SPES developpe[maxSpecialites];
	TRAVAILLEUR travailleur[maxTravailleurs];
	TRAVAILLEUR travailleurs_tous[maxTravailleurs];
	TRAVAILLEUR travailleur_meme_specialite[maxTravailleurs]; // pour l'affectation des taches

	TRAVAILLEUR travailleur_sans_redondance[maxTravailleurs];
	Clients client[maxClients];
	Commandes Commande[maxCommandes];
	TACHES tache[maxCommandes]; //Chaque commande peut avoir au maximum une t??che par sp??cialit??
	EXECUTION execution[maxCommandes];
	AFFECTATION affectationAUTO[maxCommandes];


	int indiceSpecialite; //utiliser ces variables dans la fonction affectation
	int indiceTache;
	int indiceAffectation;
	int nombre_heure_restant[maxCommandes]; // utiliser ce liste dans la fonction affectation pour sauvgarder tous les nombre d'heur, puis faut choisis le minimum


	for (int i = 0; i < maxCommandes; i++) {
		execution[i].nombreHeureTache_avancement = 0; // initialser le tableau ?? zero pour pouvoir fare l'op??ration dans la fonction (traite_progression)
		nombre_heure_restant[i] = maxNombre + 1; // initialiser ce tableau ?? cette valeur pour pouvoir choisir la valeur minimal facilement



		affectationAUTO[i].nombre_heure_commande = 0; //initialiser le tab d'affectation ?? 0 pour pouvoir faire du calcul
		affectationAUTO[i].nombre_heure_commande_avancement = 0;
	}

	for (int i = 0; i < maxTravailleurs; i++) {//initialiser le sous tableux de chaque travailleurs ?? faux(0) -> il n'ont pas de specialites encore
		travailleur[i].heure_restant = 0;
		travailleur[i].nombre_tache_pour_chaque_travailleur = 0;
		travailleur_sans_redondance[i].heure_restant = 0;
		travailleur_sans_redondance[i].nombre_tache_pour_chaque_travailleur = 0;
		travailleur_meme_specialite[i].nombre_tache_pour_chaque_travailleur = 0;
		for (int j = 0; j < maxSpecialites; j++) {
			travailleur[i].tags_competences[j] = FAUX;
			travailleur_sans_redondance[i].tags_competences[j] = FAUX;
		}
	}

	if (argc >= 2 && strcmp("echo", argv[1]) == 0) {
		EchoActif = VRAI;
	}
	Mot buffer;
	while (VRAI) {
		get_id(buffer);
		if (strcmp(buffer, "developpe") == 0) {
			cle_progression = 0;
			traite_developpe(&developpe, &travailleur);
			continue;
		}
		if (strcmp(buffer, "embauche") == 0) {
			cle_progression = 0;
			traite_embauche(&travailleur, &developpe, &travailleur_sans_redondance);
			continue;
		}
		if (strcmp(buffer, "demarche") == 0) {
			cle_progression = 0;
			traite_demarche(&client);
			continue;
		}
		if (strcmp(buffer, "commande") == 0) {
			cle_progression = 0;
			traite_commande(&Commande, &client);
			continue;
		}
		if (strcmp(buffer, "tache") == 0) {
			cle_progression = 0;
			traite_tache(&tache, &Commande, &developpe, &affectationAUTO, &execution, &travailleur_meme_specialite, &travailleur, &indiceSpecialite, &indiceTache);
			traite_affectaion(&travailleur_sans_redondance, &indiceSpecialite, &indiceTache, &indiceAffectation, &tache, &Commande, &developpe, &affectationAUTO, &execution, &travailleur_meme_specialite, &travailleur);
			continue;
		}
		if (strcmp(buffer, "progression") == 0) {
			cle_progression = 1;
			traite_progression(&indiceTache, &travailleur_sans_redondance, &travailleur, &execution, &tache, &affectationAUTO, &indiceAffectation);
			continue;
		}
		if (strcmp(buffer, "specialites") == 0) { //***************************
			cle_progression = 0;
			printf("specialites traitees : ");
			for (int i = 0; i < countSpecialites; i++) {
				printf("%s/%d", developpe[i].Specialite, developpe[i].PricePerHour);
				if (i < countSpecialites - 1) printf(", ");
			}
			printf("\n");
			continue;
		}
		if (strcmp(buffer, "travailleurs") == 0) {
			cle_progression = 0;
			traite_travailleurs(&travailleur, &developpe, &travailleurs_tous);
			continue;
		}
		if (strcmp(buffer, "client") == 0) {
			cle_progression = 0;
			traite_client(&Commande, &client);
			continue;
		}
		if (strcmp(buffer, "supervision") == 0) {
			cle_progression = 0;
			traite_supervision(&execution, &tache, &Commande, &developpe, &client);
			continue;
		}
		if (strcmp(buffer, "charge") == 0) {
			cle_progression = 0;
			traite_charge(&affectationAUTO, &Commande, &developpe, &developpe);
			continue;
		}
		if (strcmp(buffer, "lol") == 0) { //aficher le sous tableau de chaque travailleurs
			for (int i = 0; i < countTravailleurs; i++) {
				for (int j = 0; j < countSpecialites; j++) {
					printf("%d ", travailleur[i].tags_competences[j]);
				}
				printf("\n");
			}
			continue;
		}
		if (strcmp(buffer, "loll") == 0) { //aficher le sous tableau de chaque travailleurs
			for (int i = 0; i < countTravailleurSansRedondance; i++) {
				for (int j = 0; j < countSpecialites; j++) {
					printf("%d ", travailleur_sans_redondance[i].tags_competences[j]);
				}
				printf("\n");
			}
			continue;
		}

		//Afficher les donn??es-------------------------------------------------------------------------
		if (strcmp(buffer, "affect") == 0) {
			for (int i = 0; i < countTache; i++) {
				printf("\t%s %s %s %d %d\n", affectationAUTO[i].nomTravailleur, affectationAUTO[i].nomSpecialite, affectationAUTO[i].nomCommande, affectationAUTO[i].nombre_heure_commande, affectationAUTO[i].nombre_heure_commande_avancement);
			}

			continue;
		}
		if (strcmp(buffer, "printdemarche") == 0) {
			for (int i = 0; i < countClients; i++) {
				printf("\t%s\n ", client[i].Client);
			}
			continue;
		}
		if (strcmp(buffer, "ptravailleur") == 0) {
			for (int i = 0; i < countTravailleurs; i++) {
				printf("\t%d %s %s %d\n ", travailleur[i].heure_restant, travailleur[i].NOM, travailleur[i].Specialite_TRAV, travailleur[i].nombre_tache_pour_chaque_travailleur);
			}
			continue;
		}
		if (strcmp(buffer, "ptravailleursr") == 0) {
			for (int i = 0; i < countTravailleurSansRedondance; i++) {
				printf("\t%d %s %d\n ", travailleur_sans_redondance[i].heure_restant, travailleur_sans_redondance[i].NOM, travailleur_sans_redondance[i].nombre_tache_pour_chaque_travailleur);
			}
			continue;
		}
		if (strcmp(buffer, "printtache") == 0) {
			for (int i = 0; i < countTache; i++) {
				printf("\t%s %s %d\n ", tache[i].Commande_TACHE, tache[i].Specialite_TACHE, tache[i].nombreHeureTache);
			}
			continue;
		}
		if (strcmp(buffer, "printexecution") == 0) {
			for (int i = 0; i < countTache; i++) {
				printf("\t%s %s %d\n ", execution[i].Commande_exec, execution[i].Specialite_exec, execution[i].nombreHeureTache_avancement);
			}
			continue;
		}
		//------------------------------------------------------------
		if (strcmp(buffer, "clear") == 0) {
			cle_progression = 0;
			//system("cls");
			for (int i = 0; i < countAffectation; i++) {
				printf("\t%d %s\n", travailleur_meme_specialite[i].heure_restant, travailleur_meme_specialite[i].NOM, travailleur_meme_specialite[i].Specialite_TRAV);
			}
			continue;
		}
		if (strcmp(buffer, "interruption") == 0) {
			traite_interruption();
			break;
		}
		if (strcmp(buffer, "passe") == 0) {
			cle_passe = 1;
			traite_progression(&indiceTache, &travailleur_sans_redondance, &travailleur, &execution, &tache, &affectationAUTO, &indiceAffectation);
		}
		else printf("!!! instruction inconnue >%s< !!!\n", buffer);
	}
	return 0;
}
