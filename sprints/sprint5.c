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
int countEntier = 0, countClients = 0, countSpecialites = 0, countTravailleurs = 0, countCommandes = 0, countTache = 0, countExecution = 0, cle_progression = 0, cle_passe = 0, countAffectation = 0, count_travailleur_avec_heure_restant = 0, cle_tache_affectation = 0, countTravailleurSansRedondance = 0, countFacture = 0, break_end = 0;;
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
	int cout;
}Clients;

typedef struct {
	Mot Commande;
	Mot Client_Commande;
	int prix_total; //sprint5 dans facture
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

typedef struct {
	Mot commande_fac; //le nom du commande
	Mot specialite_fac; // la specialite du commande
	int price; // le prix du commande pour chaque specialite
}FACTURE;
// Instructions --------------------------------------------------------------- 
// developpe <mot: Specialite> <entier: cout horaire>
void traite_developpe(SPES* developpe) {
	Mot nom_specialite;			// this is a char type
	get_id(nom_specialite);
	int cout_horaire = get_int();
	int checkredondance = 0; // pour vérifier si on répete la spécialité
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
		printf("La spécialité %s existe déjà!!\n", nom_specialite);
	}
	else if (cout_horaire <= maxNombre)
	{
		printf("vous ne pouvez pas avoir plus de %d Specialites.\n", maxSpecialites);
	}
	else if (cout_horaire > maxNombre)
	{
		printf("vous ne puvez pas saisir un numéro supérieur à %d\n", maxNombre);

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
	int checkSpecialite = 0;	// cette valeur sera 1 si la spécialité qu'on demande existe
	int checkRedondance_nomETspec = 0;	// cette valeur sera 1 si on saisi un nom existant déjà dans le tableau

	for (int i = 0; i < countTravailleurs; i++) { //parcour le tableau des noms des travailleurs pour savoir si le nom qu'on a saisi existe déjà.	nom et meme spécialité
		if ((strcmp(travailleur[i].NOM, nom_travailleur) == 0) && (strcmp(travailleur[i].Specialite_TRAV, nom_specialite) == 0)) {
			checkRedondance_nomETspec = 1;  // =1 si le nom existe
			printf("le travailleur %s a déjà été déclaré pour la même spécialité\n", nom_travailleur);
		}
	}
	//printf("le nom <%s> déjà existe. On ne peut pas avoir deux travailleur de même nom\n",nom_travailleur);
	if ((countTravailleurs < maxTravailleurs) && (checkRedondance_nomETspec == 0))// ajouter le nom et la spécialité dans les tableaux
	{
		if (countSpecialites == 0) {  // si on ne déclare aucune spécialité dans le programme
			printf("il n'y a aucune spécialité déclarée, traitement imposible\n");
		}
		else if (countSpecialites > 0) { // on a déclaré des specialités avant et on va vérifier si la spécialité qu'on domande existe déjà??
			for (int i = 0; i < countSpecialites; i++) { // parcourir le tableu de spécialité
				if (strcmp(developpe[i].Specialite, nom_specialite) == 0) checkSpecialite = 1; //=1 alors la spécialité existé déjà
			}
			if (checkSpecialite == 1) {			// la spécialité a déjà été déclaré
				//printf(MSG_EMBAUCHE, nom_travailleur, nom_specialite);	masquer pour le sprint 2
				strcpy(travailleur[countTravailleurs].NOM, nom_travailleur);
				strcpy(travailleur[countTravailleurs].Specialite_TRAV, nom_specialite);

				if (countTravailleurs == 0) { travailleur[countTravailleurs].tags_competences[get_specialite_ordre] = VRAI; } // utiliser la méthode de sous-tableau, pour resoudre le problème de commande (travailleurs tous)
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
				printf("Specialité <%s> n'a pas été déclaré, traitement impossible\n", nom_specialite);
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

// commande <mot: la commande > <mot: nom du client effectuant la commande> / affectuer une commande par un client déjà existe
void traite_commande(Commandes* Commande, Clients* Client) {
	Mot nom_commande;
	Mot nom_client;
	get_id(nom_commande);
	get_id(nom_client);
	int checkRedondance_client = 0;

	for (int i = 0; i < countClients; i++) { //parcour le tableau des Client. pour la redondance
		if (strcmp(Client[i].Client, nom_client) == 0) {
			checkRedondance_client = 1;  // =1 si le client déjà existe
		}
	}

	int checkRedondance_commande = 0; // deux clients ne pouvez pas commendez la meme chose
	for (int i = 0; i < countCommandes; i++) {
		if (strcmp(Commande[i].Commande, nom_commande) == 0) {
			checkRedondance_commande = 1;  // =1 si la commande déjà existe
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

	for (int i = 0; i < countCommandes; i++) { // vérifier si le nom de commande déjà existe
		if (strcmp(commande[i].Commande, nom_commande) == 0) {
			checkCommande_exist = 1;
		}
	}
	for (int i = 0; i < countSpecialites; i++) { // vérifier si le nom de spécialité déjè existe
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
					printf("ce commande est déjà existe pour la meme specialite\n");
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
		printf("La Commande \"%s\" n'existe pas dans la base de donnée\n", nom_commande);
	}
	else if (checkSpecialite_exist == 0 && checkCommande_exist == 1) { //specialite n'existe pas		comande existe
		printf("la specialite \"%s\" n'existe pas dans la base de donnée\n", nom_specialite);
	}
	else	// ni la commande ni la spaecialite n'existent
	{
		printf("ni la commande \"%s\" ,ni la specialite \"%s\" existent dans la basse de donnée\n", nom_commande, nom_specialite);
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
		for (int i = 0; i < countTache; i++) { // vérifier si le nom de commande déjà existe dans les taches
			if (((strcmp(tache[i].Commande_TACHE, nom_command) == 0) && (nombre_heure_avancement <= tache[i].nombreHeureTache) && (strcmp(tache[i].Specialite_TACHE, nom_specialite) == 0))) {
				checkTache_existe = 1;
				get_indice = i;
			}
		}
		if (checkTache_existe == 1) { //= 1 alors la tache existe
			execution[get_indice].nombreHeureTache_avancement += nombre_heure_avancement;// (ancin value += nouveux value)= la somme d'heures;

			if (execution[get_indice].nombreHeureTache_avancement <= tache[get_indice].nombreHeureTache) {
				for (int i = 0; i < countAffectation; i++) {
					if (strcmp(affectationAUTO[i].nomCommande, nom_command) == 0 && strcmp(affectationAUTO[i].nomSpecialite, nom_specialite) == 0) {
						affectationAUTO[i].nombre_heure_commande_avancement = execution[get_indice].nombreHeureTache_avancement;// ici je relie affectation avec execution
					}
				}
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
						if (travailleur_sans_redondance[i].tags_competences[s] == VRAI); // cette condition est obligatoire pour ne pas verser l'etat car dans le tableau principal il y a une redondence de nom de trav, et les redondance possède l'etat (FAUX)
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
		TRAVAILLEUR t[maxCommandes]; // ce tab est important de le creer ici pour le vider à chaque fois. il faut changer le nom parcontre.
		int count_trav_de_meme_spe = 0; // nombre d'élément dans t
		for (int j = 0; j < countTravailleurSansRedondance; j++) { // choisir les travailleurs ayant les même specialites et les insérer dans un nouveau tab
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
		//printf("\tla valeur minimal est : %d à l'indice %d\n", valeur_min, n);
		// il fait faire des conditions ici
		//1 il faut savoir si il y a plusieur valeur min. 

		int travailleur_sans_tache = 0;
		int plusieur_valeur_min = 0;
		int indice_valeurs_min[maxCommandes]; // si il y a de probleme il faut l'initialiser
		for (int i = 0; i < count_trav_de_meme_spe; i++) {
			if (t[i].heure_restant == 0) {		 // si aucun travailleur de meme specialite n'a de tache encore, alors on choisie par ordre de specialité
				travailleur_sans_tache++;
			}
			if (t[i].heure_restant == valeur_min) { //si il y a plusieur valeur min on choisi dans l'ordre de la specialite 
				plusieur_valeur_min++;
				indice_valeurs_min[i] = 1;
			}
		}
		if (travailleur_sans_tache == count_trav_de_meme_spe) { // le nombre d'heur de tous les travs est 0.  faut étudier la condition où (travailleur_sans_tache < count_trav_de_meme_spe)
			//printf("\t\t travailleur sans taches = %d\n", travailleur_sans_tache);
			for (int i = 0; i < countCommandes; i++) {
				if (strcmp(tache[*indiceTache].Specialite_TACHE, travailleur[i].Specialite_TRAV) == 0) {
					strcpy(affectationAUTO[*indiceTache].nomTravailleur, travailleur[i].NOM);
					strcpy(affectationAUTO[*indiceTache].nomSpecialite, travailleur[i].Specialite_TRAV);
					strcpy(affectationAUTO[*indiceTache].nomCommande, tache[*indiceTache].Commande_TACHE);
					affectationAUTO[*indiceTache].nombre_heure_commande = tache[*indiceTache].nombreHeureTache;
					travailleur[i].heure_restant = affectationAUTO[*indiceTache].nombre_heure_commande;
					countAffectation++;
					break; // break est obligatoi sinon on selection le dernière travailleur de cette specialite.avec break on choisie que le première
				}
			}
		}
		if (plusieur_valeur_min == 1) { // le cas le plus général. on choisi forcement le travailleur qui a le moins d'heure.
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

		for (int i = 0; i < countTravailleurSansRedondance; i++) { // je pense qu'il faut initialiser cete valeur à 0. voir la première boucle de l'affectation
			travailleur_sans_redondance[i].heure_restant = 0;
		}
	}

}

// afficher les travailleurs et leurs specialites----------------------------
void traite_travailleurs(TRAVAILLEUR* travailleur, SPES* developpe, TRAVAILLEUR* travailleurs_tous) {
	Mot nom_specialite;
	get_id(nom_specialite);
	int n = 0;

	for (int i = 0; i < countSpecialites; i++) {	// bien classer les specialiter dans l'ordre de leur declaration puis pour chaque spe les travailleurs sont classer par l'ordre de leurs arriver à l'entreprise
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
			if (strcmp(travailleurs_tous[i].Specialite_TRAV, nom_specialite) == 0) { //parcourir le tableau de specialite pour savoir quel travailleur correspond à quel specialite
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
		for (int i = 0; i < countClients; i++) { // vérifier si le client existe
			if (strcmp(Client[i].Client, nom_client) == 0) {
				client_existe = 1;
			}
		}

		if (client_existe == 1) { // le client existe

			for (int i = 0; i < countCommandes; i++) { // vérifier si le client possède une commande
				if (strcmp(Commande[i].Client_Commande, nom_client) == 0) {
					client_pssede_commande = 1;
				}
			}
			saut_ligne = 0;
			if (client_pssede_commande == 1) {// le client possède une commande
				for (int i = 0; i < countCommandes; i++) {
					if (strcmp(Commande[i].Client_Commande, nom_client) == 0) {
						if (saut_ligne == 0) { printf("le client %s a commande : %s", nom_client, Commande[i].Client_Commande); saut_virgule = 1; }
						else { printf(", %s", Commande[i].Client_Commande); }
					}
				}printf("\n");

			}
			else // le client ne possède pas une commande
			{
				printf("le client %s a commande : \n", nom_client);
			}
		}
		else //le client n'existe pas
		{
			printf("le client %s n'a pas été déclaré précédament\n", nom_client);
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
void traite_charge(AFFECTATION* affectationAUTO, Commandes* commande, SPES* developpe, TRAVAILLEUR* travailleur) {
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
						if (affectationAUTO[i].nombre_heure_commande - affectationAUTO[i].nombre_heure_commande_avancement == 0) {// ajouter cette condition pour ne pas afficher les commandes terminer, sprint5
							continue;
						}
						else
						{
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

void traite_facture(FACTURE* facture, AFFECTATION* affectationAUTO, SPES* developpe, Commandes* commande, Clients* client) {
	int redondance = 0;
	int indicecommande = 0;
	for (int a = 0; a < countAffectation; a++) {  // boucle parcourir le strcut affectation
		redondance = 0;
		if ((affectationAUTO[a].nombre_heure_commande - affectationAUTO[a].nombre_heure_commande_avancement <= 0) && (affectationAUTO[a].nombre_heure_commande != 0)) { // j'ai ajoté la deuxème condition pour car j'ai initialiser cette valeur à0

			if (countFacture == 0) { // pour insérer la première commande
				indicecommande = a;
				strcpy(facture[countFacture].commande_fac, affectationAUTO[a].nomCommande);
				strcpy(facture[countFacture].specialite_fac, affectationAUTO[a].nomSpecialite);
				for (int s = 0; s < countSpecialites; s++) {
					if (strcmp(affectationAUTO[a].nomSpecialite, developpe[s].Specialite) == 0) {
						facture[countFacture].price = developpe[s].PricePerHour * affectationAUTO[a].nombre_heure_commande;
						countFacture++;
					}
				}
			}
			else if (countFacture > 0)
			{
				for (int f = 0; f < countFacture; f++) {

					if ((strcmp(affectationAUTO[a].nomCommande, facture[f].commande_fac) == 0) && (strcmp(affectationAUTO[a].nomSpecialite, facture[f].specialite_fac) == 0)) {
						redondance = 1; // pour ne pas répéter le même commende avec la même specialite
					}
				}
				if (redondance == 0)
				{
					indicecommande = a;
					strcpy(facture[countFacture].commande_fac, affectationAUTO[a].nomCommande);
					strcpy(facture[countFacture].specialite_fac, affectationAUTO[a].nomSpecialite);
					for (int s = 0; s < countSpecialites; s++) {
						if (strcmp(affectationAUTO[a].nomSpecialite, developpe[s].Specialite) == 0) {
							facture[countFacture].price = developpe[s].PricePerHour * affectationAUTO[a].nombre_heure_commande;
							countFacture++;
						}
					}
				}
			}
		}
	}


	int commande_in_affectation = 0;
	for (int ca = 0; ca < countAffectation; ca++) {
		if (strcmp(affectationAUTO[indicecommande].nomCommande, affectationAUTO[ca].nomCommande) == 0) {
			commande_in_affectation++;
		}
	}
	int commande_in_facture = 0;
	for (int cf = 0; cf < countFacture; cf++) {
		if (strcmp(affectationAUTO[indicecommande].nomCommande, facture[cf].commande_fac) == 0) {
			commande_in_facture++;
		}
	}
	int virgule = 0;
	if (commande_in_affectation == commande_in_facture) {
		virgule = 0;
		for (int cf = 0; cf < countFacture; cf++) {
			if (strcmp(affectationAUTO[indicecommande].nomCommande, facture[cf].commande_fac) == 0) {
				if (virgule == 0) {
					printf("facturation %s : %s:%d", affectationAUTO[indicecommande].nomCommande, facture[cf].specialite_fac, facture[cf].price);
					virgule = 1;
				}
				else
				{
					printf(", %s:%d", facture[cf].specialite_fac, facture[cf].price);
				}
			}
		}printf("\n");

	}
	//printf("%d %d \n", countAffectation, countFacture);
	// toutes les commandes sont sterminer si countFacture == countAffectation && toutes les commandes sont present dans affecation
	if (countAffectation == countFacture + 1) {
		for (int c = 0; c < countCommandes; c++) {
			for (int i = 0; i < countFacture; i++) {
				if (strcmp(commande[c].Commande, facture[i].commande_fac) == 0) {
					commande[c].prix_total += facture[i].price;	//faire la somme de chaque commande, et insèrer la somme dans commande.prix_total
				}
			}
		}

		for (int i = 0; i < countClients; i++) {
			for (int j = 0; j < countCommandes; j++) {
				if (strcmp(client[i].Client, commande[j].Client_Commande) == 0) {
					client[i].cout += commande[j].prix_total;
				}
			}
		}
		int virgule = 0;
		for (int h = 0; h < countClients; h++) {
			if (virgule == 0) {
				printf("facturations : %s:%d", client[h].Client, client[h].cout);
				virgule = 1;
			}
			else
			{
				printf(", %s:%d", client[h].Client, client[h].cout);
			}
		}
		printf("\n");
		break_end = 1;
	}
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
	TACHES tache[maxCommandes]; //Chaque commande peut avoir au maximum une tâche par spécialité
	EXECUTION execution[maxCommandes];
	AFFECTATION affectationAUTO[maxCommandes];

	FACTURE facture[maxCommandes];

	int indiceSpecialite; //utiliser ces variables dans la fonction affectation
	int indiceTache;
	int indiceAffectation;
	int nombre_heure_restant[maxCommandes]; // utiliser ce liste dans la fonction affectation pour sauvgarder tous les nombre d'heur, puis faut choisis le minimum
	for (int i = 0; i < maxClients; i++) {
		client[i].cout = 0;
	}

	for (int i = 0; i < maxCommandes; i++) {
		execution[i].nombreHeureTache_avancement = 0; // initialser le tableau à zero pour pouvoir fare l'opération dans la fonction (traite_progression)
		nombre_heure_restant[i] = maxNombre + 1; // initialiser ce tableau à cette valeur pour pouvoir choisir la valeur minimal facilement
		
		facture[i].price = 0;
		Commande[i].prix_total = 0;
		affectationAUTO[i].nombre_heure_commande = 0; //initialiser le tab d'affectation à 0 pour pouvoir faire du calcul
		affectationAUTO[i].nombre_heure_commande_avancement = 0;
	}

	for (int i = 0; i < maxTravailleurs; i++) {//initialiser le sous tableux de chaque travailleurs à faux(0) -> il n'ont pas de specialites encore
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
		if (break_end == 1) {
			break;
		}
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
			traite_facture(&facture, &affectationAUTO, &developpe, &Commande, &client);
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

		//Afficher les données-------------------------------------------------------------------------
		if (strcmp(buffer, "affect") == 0) {
			for (int i = 0; i < countTache; i++) {
				printf("\t%s %s %s %d %d\n", affectationAUTO[i].nomTravailleur, affectationAUTO[i].nomCommande, affectationAUTO[i].nomSpecialite, affectationAUTO[i].nombre_heure_commande, affectationAUTO[i].nombre_heure_commande_avancement);
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
		if (strcmp(buffer, "fact") == 0) {
			for (int i = 0; i < countTache; i++) {
				printf("\t%s\t\t%s\t\t\t%d\n ", facture[i].commande_fac, facture[i].specialite_fac, facture[i].price);
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
