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
#define maxTravailleurs 50
#define maxClients 100
#define maxCommandes 5

typedef char Mot[LGMOT + 1];


// Variables globales
int countEntier = 0, countClients = 0, countSpecialites = 0, countTravailleurs = 0, countCommandes = 0, countTache = 0, countExecution = 0, cle_progression = 0, cle_passe = 0;

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

}TRAVAILLEUR;


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
void traite_embauche(TRAVAILLEUR* travailleur, SPES* developpe) {
	Mot nom_travailleur;
	Mot nom_specialite;
	get_id(nom_travailleur);
	get_id(nom_specialite);



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
		printf(MSG_COMMANDE, nom_commande, nom_client);
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
void traite_tache(TACHES* tache, Commandes* commande, SPES* developpe) {
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
		}
	}
	if ((checkCommande_exist == 1) && (checkSpecialite_exist == 1)) { // la commande existe et la specialite existe
		if (countTache == 0) {
			printf(MSG_TACHE, nom_commande, nom_specialite, nombre_heures);
			strcpy(tache[countTache].Commande_TACHE, nom_commande);
			strcpy(tache[countTache].Specialite_TACHE, nom_specialite);
			tache[countTache].nombreHeureTache = nombre_heures;
			countTache++;
		}
		else
		{
			/*for (int i = 0; i < countTache; i++) {
				if (strcmp(tache[i].Commande_TACHE, nom_commande) == 0) {
					printf("Chaque commande peut avoir au maximum une tache par specialite\n");
					redondance_commande= 1;
				}
			}*/
			if (redondance_commande == 0)
			{
				printf(MSG_TACHE, nom_commande, nom_specialite, nombre_heures);
				strcpy(tache[countTache].Commande_TACHE, nom_commande);
				strcpy(tache[countTache].Specialite_TACHE, nom_specialite);
				tache[countTache].nombreHeureTache = nombre_heures;
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
void traite_progression(EXECUTION* execution, TACHES* tache) {// remaruqe dans cette fonction je garde l'indice de chaque tache

	if ((cle_progression == 1) && (cle_passe == 0)) {
		Mot nom_command;
		Mot nom_specialite;
		get_id(nom_command);
		get_id(nom_specialite);
		int nombre_heure_avancement = get_int();

		int checkTache_existe = 0;
		int get_indice = 0;
		for (int i = 0; i < countTache; i++) { // vérifier si le nom de commande déjà existe dans les taches
			if (((strcmp(tache[i].Commande_TACHE, nom_command) == 0) && (nombre_heure_avancement < tache[i].nombreHeureTache) && (strcmp(tache[i].Specialite_TACHE, nom_specialite) == 0))) {
				checkTache_existe = 1;
				get_indice = i;
			}
		}
		if (checkTache_existe == 1) { //= 1 alors la tache existe
			execution[get_indice].nombreHeureTache_avancement += nombre_heure_avancement;// (ancin value += nouveux value)= la somme d'heures;
			if (execution[get_indice].nombreHeureTache_avancement < tache[get_indice].nombreHeureTache) {

				printf(MSG_PROGRESSION, nom_command, nom_specialite, nombre_heure_avancement);
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
		printf(MSG_PROGRESSION_PASSE);
		cle_progression = 0;
		cle_passe = 0;
	}
	else if (cle_progression == 0 && cle_passe == 1)
	{
		printf("le mot cle passe est utiliser uniquement qu'avec le mot cle progression\n");
		cle_passe = 0;
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
	if (strcmp(nom_client, "tous") == 0) {
		for (int i = 0; i < countClients; i++) {
			printf("le client %s a commande : \n", Client[i].Client);
		}
		/*for (int i = 0; i < countClients; i++) {
			for (int j = 0; j < countCommandes; j++) {
				if ((strcmp(Client[i].Client, Commande[j].Client_Commande) == 0)) {
					printf("\t%s %s\n ", Commande[j].Commande, Commande[j].Client_Commande);
				}
			}
		}*/
	}
	else
	{
		printf("le client %s a commande : \n", nom_client);
		/*for (int i = 0; i < countCommandes; i++) {
			if (strcmp(Commande[i].Client_Commande, nom_client) == 0) {
				printf("\t%s\n", Commande[i].Commande);
			}
		}*/
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
	SPES developpe[maxSpecialites];
	TRAVAILLEUR travailleur[maxTravailleurs];
	TRAVAILLEUR travailleurs_tous[maxTravailleurs];
	Clients client[maxClients];
	Commandes Commande[maxCommandes];
	TACHES tache[maxCommandes]; //Chaque commande peut avoir au maximum une tâche par spécialité
	EXECUTION execution[maxCommandes];
	for (int i = 0; i < maxCommandes; i++) { // initialser le tableau à zero pour pouvoir fare l'opération dans la fonction (traite_progression)
		execution[i].nombreHeureTache_avancement = 0;
	}

	for (int i = 0; i < maxTravailleurs; i++) {
		for (int j = 0; j < maxSpecialites; j++) {
			travailleur[i].tags_competences[j] = FAUX;
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
			traite_embauche(&travailleur, &developpe);
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
			traite_tache(&tache, &Commande, &developpe);
			continue;
		}
		if (strcmp(buffer, "progression") == 0) {
			cle_progression = 1;
			traite_progression(&execution, &tache);
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
			traite_supervision();
			continue;
		}
		if (strcmp(buffer, "charge") == 0) {
			cle_progression = 0;
			traite_charge();
			continue;
		}
		if (strcmp(buffer, "lol") == 0) {
			for (int i = 0; i < countTravailleurs; i++) {
				for (int j = 0; j < countSpecialites; j++) {
					printf("%d ", travailleur[i].tags_competences[j]);
				}
				printf("\n");
			}
			continue;
		}

		//Afficher les données-------------------------------------------------------------------------

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
		if (strcmp(buffer, "printexecution") == 0) {
			for (int i = 0; i < countTache; i++) {
				printf("\t%s %s %d\n ", execution[i].Commande_exec, execution[i].Specialite_exec, execution[i].nombreHeureTache_avancement);
			}
			continue;
		}
		//------------------------------------------------------------
		if (strcmp(buffer, "clear") == 0) {
			cle_progression = 0;
			system("cls");
			continue;
		}
		if (strcmp(buffer, "interruption") == 0) {
			traite_interruption();
			break;
		}
		if (strcmp(buffer, "passe") == 0) {
			cle_passe = 1;
			traite_progression(&execution, &tache);
		}
		else printf("!!! instruction inconnue >%s< !!!\n", buffer);
	}
	return 0;
}
