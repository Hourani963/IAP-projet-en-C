#pragma warning(disable:4996)
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum { FAUX = 0, VRAI = 1 } Booleen;
Booleen EchoActif = FAUX;

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
int countEntier = 0, countClients = 0, countSpecialites = 0, countTravailleurs = 0, countCommandes = 0, countTache = 0, countExecution = 0, cle_progression = 0, cle_passe = 0, countAffectation = 0, count_travailleur_avec_heure_restant = 0, cle_tache_affectation = 0, countTravailleurSansRedondance = 0, countFacture = 0, break_end = 0;
int indice_pasee = 0;
int indice_spe_passe;
Mot com_passe;
Mot spe_passe;

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
	Mot nom_specialite;
	get_id(nom_specialite);
	int cout_horaire = get_int();
	int checkredondance = 0; // pour vérifier si on répete la spécialité
	for (int indiceSearch_spe = 0; indiceSearch_spe < countSpecialites; indiceSearch_spe++) {
		if (strcmp(developpe[indiceSearch_spe].Specialite, nom_specialite) == 0) checkredondance = 1; //alors la spécialite déjà existe
	}
	if ((countSpecialites < maxSpecialites) && (cout_horaire <= maxNombre) && (checkredondance == 0)) {
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

	int indice_spe = 0;
	for (int i = 0; i < countSpecialites; i++) {
		if (strcmp(developpe[i].Specialite, nom_specialite) == 0) {
			indice_spe = i;
		}
	}
	int newName_trav = 1;
	if (countTravailleurSansRedondance == 0) { // ici je met que les noms des travailleurs, sans redondance des noms
		strcpy(travailleurSansRedondance[0].NOM, nom_travailleur);
		travailleurSansRedondance[0].tags_competences[indice_spe] = VRAI;
		countTravailleurSansRedondance++;
	}
	else
	{
		for (int indiceSearch_trav = 0; indiceSearch_trav < countTravailleurSansRedondance; indiceSearch_trav++) {
			if (strcmp(travailleurSansRedondance[indiceSearch_trav].NOM, nom_travailleur) == 0) {
				newName_trav = 0; // alors le travailleur existe, on ne l'ajoute pas
			}
		}
		if (newName_trav == 1) {

			strcpy(travailleurSansRedondance[countTravailleurSansRedondance].NOM, nom_travailleur);
			travailleurSansRedondance[countTravailleurSansRedondance].tags_competences[indice_spe] = VRAI;
			countTravailleurSansRedondance++;
		}
	}
	int get_specialite_ordre = 0;
	for (int indiceSearch_spe = 0; indiceSearch_spe < countSpecialites; indiceSearch_spe++) {
		if (strcmp(developpe[indiceSearch_spe].Specialite, nom_specialite) == 0) get_specialite_ordre = indiceSearch_spe;
	}
	int checkSpecialite = 0;	// cette valeur sera 1 si la spécialité qu'on demande existe
	int checkRedondance_nomETspec = 0;	// cette valeur sera 1 si on saisi un nom existant déjà dans le tableau

	for (int indiceSearch_trav = 0; indiceSearch_trav < countTravailleurs; indiceSearch_trav++) { //parcour le tableau des noms des travailleurs pour savoir si le nom qu'on a saisi existe déjà.	nom et meme spécialité
		if ((strcmp(travailleur[indiceSearch_trav].NOM, nom_travailleur) == 0) && (strcmp(travailleur[indiceSearch_trav].Specialite_TRAV, nom_specialite) == 0)) {
			checkRedondance_nomETspec = 1;  // =1 si le nom existe
			printf("le travailleur %s a déjà été déclaré pour la même spécialité\n", nom_travailleur);
		}
	}
	if ((countTravailleurs < maxTravailleurs) && (checkRedondance_nomETspec == 0))// ajouter le nom et la spécialité dans les tableaux
	{
		if (countSpecialites == 0) {  // si on ne déclare aucune spécialité dans le programme
			printf("il n'y a aucune spécialité déclarée, traitement imposible\n");
		}
		else { // on a déclaré des specialités avant et on va vérifier si la spécialité qu'on domande existe déjà??
			for (int indiceSearch_spe = 0; indiceSearch_spe < countSpecialites; indiceSearch_spe++) { // parcourir le tableu de spécialité
				if (strcmp(developpe[indiceSearch_spe].Specialite, nom_specialite) == 0) checkSpecialite = 1; //=1 alors la spécialité existé déjà
			}
			if (checkSpecialite == 1) {			// la spécialité a déjà été déclaré
				strcpy(travailleur[countTravailleurs].NOM, nom_travailleur);
				strcpy(travailleur[countTravailleurs].Specialite_TRAV, nom_specialite);

				if (countTravailleurs == 0) { travailleur[countTravailleurs].tags_competences[get_specialite_ordre] = VRAI; } // utiliser la méthode de sous-tableau, pour resoudre le problème de commande (travailleurs tous)
				else
				{
					int check = 0;
					for (int s = 0; s < countTravailleurs; s++) {// si on déclare une autre compétence pour un travailleur existant, on ajout le tags_competence dans le sous tableau de son premère nom lors de la premère déclaration.
						if (strcmp(travailleur[s].NOM, nom_travailleur) == 0) {
							travailleur[s].tags_competences[get_specialite_ordre] = VRAI;
							check = 1;
							break; // break est important pour ne pas continuer la recherche, on veux que le première nom
						}
					}
					if (check == 0) {
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
	for (int indiceSearch_client = 0; indiceSearch_client < countClients; indiceSearch_client++) {
		if (strcmp(Client[indiceSearch_client].Client, nom_client) == 0) {
			checkRedondance = 1;  // =1 si le nom existe
			printf("Le nom de client <%s> est déjà existe !!\n", nom_client);
		}
	}
	if (countClients < maxClients && checkRedondance == 0) {
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
void traite_tache(TACHES* tache, Commandes* commande, SPES* developpe, int* indiceSpecialite, int* indiceTache) {
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
int get_indice;
Mot spe;
int old_indiceTache;
int old_indice_spe;
void traite_progression(TRAVAILLEUR* travailleur, EXECUTION* execution, TACHES* tache, AFFECTATION* affectationAUTO, SPES* developpe) {// remaruqe dans cette fonction je garde l'indice de chaque tache dans le tab execution

	if ((cle_progression == 1) && (cle_passe == 0)) {
		Mot nom_command;
		Mot nom_specialite;
		get_id(nom_command);
		get_id(nom_specialite);
		int indiceCOM = 0;
		int nombre_heure_avancement = get_int();

		strcpy(spe, nom_specialite); // sprint 6
		int checkTache_existe = 0;

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
						indiceCOM = i;
						affectationAUTO[i].nombre_heure_commande_avancement = execution[get_indice].nombreHeureTache_avancement;// ici je relie affectation avec execution
					}
				}
				strcpy(execution[get_indice].Commande_exec, nom_command);
				strcpy(execution[get_indice].Specialite_exec, nom_specialite);

				for (int i = 0; i < countAffectation; i++) { // mise à jour de nombreHeureRestant  dans la struct travailleur
					if (strcmp(affectationAUTO[indiceCOM].nomTravailleur, travailleur[i].NOM) == 0 && strcmp(affectationAUTO[indiceCOM].nomSpecialite, travailleur[i].Specialite_TRAV) == 0) {
						travailleur[i].heure_restant -= nombre_heure_avancement;
					}
				}
			}
			else {
				printf("tache terminer, (avancement) %d >= %d (nombre d'heure) \n", execution[get_indice].nombreHeureTache_avancement, tache[get_indice].nombreHeureTache);
			}
		}
		if ((nombre_heure_avancement <= maxNombre) && (checkTache_existe == 0)) {
			printf("la tache pour nom de commande %s et la specialite %s n'existe pas, execution impossible!!\n", nom_command, nom_specialite);
		}
		strcpy(com_passe, nom_command);// j'enregistre les infos de dernière progression pour les utiliser si je trouver le mot pass dans la suite.
		strcpy(spe_passe, nom_specialite);
	}// j'utilise l'indice

	else if (cle_progression == 1 && cle_passe == 1)
	{
		for (int i = 0; i < countSpecialites; i++) {
			if (strcmp(developpe[i].Specialite, spe) == 0) {
				indice_spe_passe = i;
			}
		}
		indice_pasee = get_indice;
		cle_tache_affectation = 1;
	}
	else if (cle_progression == 0 && cle_passe == 1)
	{
		printf("le mot cle passe est utiliser uniquement qu'avec le mot cle progression\n");
		cle_passe = 0;
	}
}

// AUTO affectation---------------------------
void traite_affectaion(TRAVAILLEUR* travailleur_sans_redondance, int* indiceSpecialite, int* indiceTache, TACHES* tache, SPES* developpe, AFFECTATION* affectationAUTO, TRAVAILLEUR* travailleur) {
	if (cle_passe == 0) {
		old_indiceTache = *indiceTache;
		old_indice_spe = *indiceSpecialite;
	}
	for (int i = 0; i < countTravailleurSansRedondance; i++) {
		travailleur_sans_redondance[i].heure_restant = 0;
	}
	if (cle_tache_affectation == 1) {
		if (cle_passe == 1) {
			*indiceTache = get_indice; // l'indice de la progression qui etait écrit avec le mot passe
			*indiceSpecialite = indice_spe_passe; // l'indice de la spe qui etait écrit avec le mot passe
		}
		for (int i = 0; i < countTravailleurSansRedondance; i++) { // ici je met la somme des heures de travaille de chaque travailleur avec les tags_competence de chaque travailleur. tab tous les travailleur
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
		TRAVAILLEUR travailleur_meme_specialite[maxCommandes]; // ce tab est important de le creer ici pour le vider à chaque fois..
		int count_trav_de_meme_spe = 0; // nombre d'élément dans t
		for (int j = 0; j < countTravailleurSansRedondance; j++) { // choisir les travailleurs ayant les même specialites et les insérer dans un nouveau tab
			if (travailleur_sans_redondance[j].tags_competences[*indiceSpecialite] == VRAI) {
				strcpy(travailleur_meme_specialite[count_trav_de_meme_spe].NOM, travailleur_sans_redondance[j].NOM);
				strcpy(travailleur_meme_specialite[count_trav_de_meme_spe].Specialite_TRAV, developpe[*indiceSpecialite].Specialite);

				travailleur_meme_specialite[count_trav_de_meme_spe].heure_restant = travailleur_sans_redondance[j].heure_restant;
				count_trav_de_meme_spe++;
			}
		}
		/*for (int i = 0; i < count_trav_de_meme_spe; i++) {
			printf("\t%s %s %d\n", travailleur_meme_specialite[i].NOM, travailleur_meme_specialite[i].Specialite_TRAV, travailleur_meme_specialite[i].heure_restant);
		}
		for (int i = 0; i < countTravailleurs; i++) {
			printf("\t\t%s %s %d\n", travailleur[i].NOM, travailleur[i].Specialite_TRAV, travailleur[i].heure_restant);
		}*/

		int valeurMin_indice = 0;
		int valeur_min;
		if (count_trav_de_meme_spe == 1) {
			strcpy(affectationAUTO[*indiceTache].nomTravailleur, travailleur_meme_specialite[0].NOM);
			strcpy(affectationAUTO[*indiceTache].nomCommande, tache[*indiceTache].Commande_TACHE);
			strcpy(affectationAUTO[*indiceTache].nomSpecialite, travailleur_meme_specialite[0].Specialite_TRAV);
			affectationAUTO[*indiceTache].nombre_heure_commande = tache[*indiceTache].nombreHeureTache;
			for (int i = 0; i < countTravailleurs; i++) { // recopier la valeur (heure_restant)  dans le tab travailleur
				if (strcmp(travailleur[i].NOM, affectationAUTO[*indiceTache].nomTravailleur) == 0 && strcmp(travailleur[i].Specialite_TRAV, affectationAUTO[*indiceTache].nomSpecialite) == 0) {
					if (cle_passe == 0)travailleur[i].heure_restant += affectationAUTO[*indiceTache].nombre_heure_commande;
				}
			}
			if (cle_passe == 0)countAffectation++;
		}
		else
		{
			for (int i = 0; i < count_trav_de_meme_spe; i++) {// choisir la valeur minimal de (travailleurs_meme_specialite)
				if (travailleur_meme_specialite[valeurMin_indice].heure_restant > travailleur_meme_specialite[i].heure_restant) {
					valeurMin_indice = i;
				}
			}
			valeur_min = travailleur_meme_specialite[valeurMin_indice].heure_restant;

			int travailleur_sans_tache = 0;
			int plusieur_valeur_min = 0;
			int indice_valeurs_min[maxCommandes]; // si il y a de probleme il faut l'initialiser

			for (int i = 0; i < count_trav_de_meme_spe; i++) {
				if (travailleur_meme_specialite[i].heure_restant == 0) {		 // si aucun travailleur de meme specialite n'a de tache encore, alors on choisie par ordre de specialité
					travailleur_sans_tache++;
				}
				if (travailleur_meme_specialite[i].heure_restant == valeur_min) { //si il y a plusieur valeur min on choisi dans l'ordre de la specialite 
					plusieur_valeur_min++;
					indice_valeurs_min[i] = 1;
				}
			}

			if (plusieur_valeur_min >= 1 && count_trav_de_meme_spe > 1) { // le cas le plus générale quand il ya un seul valeur minimal (plusieur_valeur_min == 1) mais dans ce code je vais pas traiter l'ordre des travailleur lors qu'il y en a plusieur de même valuer min , je fais donc (plusieur_valeur_min >= 1) 
				strcpy(affectationAUTO[*indiceTache].nomTravailleur, travailleur_meme_specialite[valeurMin_indice].NOM); // comme il y a un seul trav avec valeur min alors la selue indice de cette valeur est n
				strcpy(affectationAUTO[*indiceTache].nomSpecialite, travailleur_meme_specialite[valeurMin_indice].Specialite_TRAV);
				strcpy(affectationAUTO[*indiceTache].nomCommande, tache[*indiceTache].Commande_TACHE);
				affectationAUTO[*indiceTache].nombre_heure_commande = tache[*indiceTache].nombreHeureTache;
				if (cle_passe == 0) countAffectation++; //pour le sprint 6, si je change le travailleur de tel commande je n'ai pas besoin d'augmenter les tableau

				for (int i = 0; i < countTravailleurs; i++) { // faire la mise à jour dans la struct travailleur
					if ((strcmp(affectationAUTO[*indiceTache].nomTravailleur, travailleur[i].NOM) == 0) && (strcmp(affectationAUTO[*indiceTache].nomSpecialite, travailleur[i].Specialite_TRAV) == 0)) {
						if (cle_passe == 0)travailleur[i].heure_restant += affectationAUTO[*indiceTache].nombre_heure_commande;
					}
				}
			}

			if (cle_passe == 1) { // suprimer toutes les valeur pour recalculer l'affictation sera à quel travailleur
				for (int i = 0; i < countTravailleurs; i++) {
					travailleur[i].heure_restant = 0;
				}
			}
			if (cle_passe == 1) { // recalculer les heures de chaque travailleur
				for (int i = 0; i < countTravailleurs; i++) {
					for (int j = 0; j < countAffectation; j++) {
						if (strcmp(travailleur[i].NOM, affectationAUTO[j].nomTravailleur) == 0 && strcmp(travailleur[i].Specialite_TRAV, affectationAUTO[j].nomSpecialite) == 0) {
							travailleur[i].heure_restant += affectationAUTO[j].nombre_heure_commande - affectationAUTO[j].nombre_heure_commande_avancement;
						}
					}
				}
			}
		}
	}
}

// afficher les travailleurs et leurs specialites----------------------------
void traite_travailleurs(TRAVAILLEUR* travailleur, SPES* developpe, TRAVAILLEUR* travailleurs_tous) {
	Mot nom_specialite;
	get_id(nom_specialite);
	int saut_ligne = 0;
	if (strcmp(nom_specialite, "tous") == 0) {// aficher dans l'ordre des specialites puis dans l'ordre des travailleurs
		int bien_afficher;
		for (int i = 0; i < countSpecialites; i++) {
			bien_afficher = 0;
			saut_ligne = 0;
			for (int j = 0; j < countTravailleurs; j++) {
				if (travailleur[j].tags_competences[i] == VRAI) {
					if (bien_afficher == 0) {
						printf("la specialite %s peut etre prise en charge par : ", developpe[i].Specialite);
						printf("%s", travailleur[j].NOM);
						bien_afficher = 1;
						saut_ligne = 1;
					}
					else
					{
						printf(", %s", travailleur[j].NOM);
					}
				}
			}
			if (saut_ligne == 1)printf("\n");
		}

	}
	else {
		int indice_spec = 0;
		for (int i = 0; i < countSpecialites; i++) {
			if (strcmp(nom_specialite, developpe[i].Specialite) == 0) {
				indice_spec = i;
			}
		}
		int check_virgule = 0;  // ce variable est juste pour eviter l'errore d'affichage de virgule
		for (int i = 0; i < countTravailleurs; i++) {
			if (travailleur[i].tags_competences[indice_spec] == VRAI) { //parcourir le tableau de specialite pour savoir quel travailleur correspond à quel specialite
				if (check_virgule == 0) {
					printf("la specialite %s peut etre prise en charge par : ", nom_specialite);
					printf("%s", travailleur[i].NOM);
					saut_ligne = 1;
				}
				else printf(", %s", travailleur[i].NOM);
				check_virgule++;
			}
		}
		if (saut_ligne == 1)printf("\n");
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
					if (virgule == 0) {
						printf("le client %s a commande : %s", Client[i].Client, Commande[j].Commande);
						client_sans_commande = 1;
						virgule = 1;
						saut_ligne = 1;
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
		}
	}
	else
	{
		int client_existe = 0;
		int client_pssede_commande = 0;
		int saut_virgule = 0;
		int SL = 0; //saute ligne
		for (int i = 0; i < countClients; i++) { // vérifier si le client existe
			if (strcmp(Client[i].Client, nom_client) == 0) {
				client_existe = 1;
			}
		}
		for (int i = 0; i < countCommandes; i++) { // vérifier si le client possède une commande
			if (strcmp(Commande[i].Client_Commande, nom_client) == 0) {
				client_pssede_commande = 1;
			}
		}
		if (client_existe == 1 && client_pssede_commande == 1) { // le client existe && le client possède une commande

			saut_ligne = 0;

			for (int i = 0; i < countCommandes; i++) {
				if (strcmp(Commande[i].Client_Commande, nom_client) == 0) {
					if (saut_virgule == 0) {
						printf("le client %s a commande : %s", nom_client, Commande[i].Commande);
						saut_virgule = 1;
					}
					else {
						printf(", %s", Commande[i].Commande);
					}
				}
			}
			printf("\n");
		}
		else if (client_existe == 1 && client_pssede_commande == 0) // le client ne possède pas une commande
		{
			printf("le client %s a commande : \n", nom_client);
		}
		else if (client_existe == 0)//le client n'existe pas
		{
			printf("le client %s n'a pas été déclaré précédament\n", nom_client);
		}
	}
}

// supervision ------------------------
void traite_supervision(EXECUTION* execution, TACHES* tache, Commandes* commande, SPES* developpe) {
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
void traite_charge(AFFECTATION* affectationAUTO, Commandes* commande, SPES* developpe) {
	Mot nom_travailleur;
	get_id(nom_travailleur);
	int vergule = 0;
	int travailleur_sans_charge = 1;
	int com = 0;
	int spe = 0;
	for (int c = 0; c < countCommandes; c++) {
		for (int s = 0; s < countSpecialites; s++) {
			for (int t = 0; t < countAffectation; t++) {
				if ((strcmp(affectationAUTO[t].nomSpecialite, developpe[s].Specialite) == 0) && (strcmp(affectationAUTO[t].nomTravailleur, nom_travailleur) == 0) && (affectationAUTO[t].nombre_heure_commande - affectationAUTO[t].nombre_heure_commande_avancement != 0) && (strcmp(affectationAUTO[t].nomCommande, commande[c].Commande) == 0)) {
					if (vergule == 0) {
						printf("charge de travail pour %s : %s/%s/%dheure(s)", affectationAUTO[t].nomTravailleur, affectationAUTO[t].nomCommande, affectationAUTO[t].nomSpecialite, affectationAUTO[t].nombre_heure_commande - affectationAUTO[t].nombre_heure_commande_avancement);
						vergule = 1;
						travailleur_sans_charge = 0;
					}
					else
					{
						printf(", %s/%s/%dheure(s)", affectationAUTO[t].nomCommande, affectationAUTO[t].nomSpecialite, affectationAUTO[t].nombre_heure_commande - affectationAUTO[t].nombre_heure_commande_avancement);
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

void traite_facture(FACTURE* facture, AFFECTATION* affectationAUTO, SPES* developpe, Commandes* commande, Clients* client) {
	int redondance = 0;
	int indicecommande = 0;
	for (int a = 0; a < countAffectation; a++) {  // boucle parcourir la strcut affectation
		redondance = 0;
		if ((affectationAUTO[a].nombre_heure_commande - affectationAUTO[a].nombre_heure_commande_avancement <= 0) && (affectationAUTO[a].nombre_heure_commande != 0)) { // j'ai ajoté la deuxème condition pour car j'ai initialiser cette valeur à0

			if (countFacture == 0) { // pour insérer la première facture
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
			else
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

	if (countAffectation == countFacture) {
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
		travailleur_sans_redondance[i].heure_restant = 0;
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
		if (break_end == 1) {
			break;
		}
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
			traite_tache(&tache, &Commande, &developpe, &indiceSpecialite, &indiceTache);
			traite_affectaion(&travailleur_sans_redondance, &indiceSpecialite, &indiceTache, &tache, &developpe, &affectationAUTO, &travailleur);
			continue;
		}
		if (strcmp(buffer, "progression") == 0) {
			cle_progression = 1;
			traite_progression(&travailleur, &execution, &tache, &affectationAUTO, &developpe);
			traite_facture(&facture, &affectationAUTO, &developpe, &Commande, &client);
			continue;
		}
		if (strcmp(buffer, "specialites") == 0) { //***************************
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
			traite_supervision(&execution, &tache, &Commande, &developpe);
			continue;
		}
		if (strcmp(buffer, "charge") == 0) {
			cle_progression = 0;
			traite_charge(&affectationAUTO, &Commande, &developpe);
			continue;
		}
		if (strcmp(buffer, "clear") == 0) {
			cle_progression = 0;
			system("cls");
			continue;
		}
		//Afficher les données-------------------------------------------------------------------------
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
		//******************************************************************
		if (strcmp(buffer, "interruption") == 0) {
			printf("## fin de programme\n");
			break;
		}
		if (strcmp(buffer, "passe") == 0) {
			cle_passe = 1;

			traite_progression(&travailleur, &execution, &tache, &affectationAUTO, &developpe);
			traite_affectaion(&travailleur_sans_redondance, &indiceSpecialite, &indiceTache, &tache, &developpe, &affectationAUTO, &travailleur);
			traite_facture(&facture, &affectationAUTO, &developpe, &Commande, &client);
			cle_progression = 0;
			cle_passe = 0;
		}
		else printf("!!! instruction inconnue >%s< !!!\n", buffer);
	}
	return 0;
}
