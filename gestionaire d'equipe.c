#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include <ctype.h>
#include <windows.h>

// Codes de couleurs pour Windows
#define COLOR_RESET   7
#define COLOR_RED     4
#define COLOR_GREEN   2
#define COLOR_YELLOW  6
#define COLOR_BLUE    1
#define COLOR_MAGENTA 5
#define COLOR_CYAN    3
#define COLOR_WHITE   7
#define COLOR_GRAY    8

// Structure pour representer un joueur
typedef struct {
	int id;
	char nom[50];
	char prenom[50];
	int numeroMaillot;
	char poste[20];
	int jourNaissance;
	int moisNaissance;
	int anneeNaissance;
	int buts;
	char dateInscription[20];
	char statut[20];
} Joueur;

// Variables globales
Joueur equipe[30];
int nombreJoueurs = 0;
int prochainId = 1;

// Fonctions utilitaires
void setColor(int color);
void afficherTitre();
void afficherMenu();
void ajouterJoueur();
void ajouterPlusieursJoueurs();
void ajouterDesJoueurs();
void afficherJoueurs();
void trierParNom();
void trierParAge();
void afficherParPoste();
void modifierJoueur();
void supprimerJoueur();
void rechercherJoueur();
void afficherStatistiques();
int calculerAge(int jour, int mois, int annee);
int estAnneeBissextile(int annee);
int validerDate(int jour, int mois, int annee);
int numeroMaillotExiste(int numero);
void clearScreen();

int main() {
	int choix;
	do {
		clearScreen();
		afficherTitre();
		if (nombreJoueurs == 0) {
			time_t maintenant = time(NULL);
			struct tm* tm_info = localtime(&maintenant);

			Joueur joueursInit[15] = {
				{0, "Benali",   "Mohamed",   1,  "gardien",   12,  3, 1990,  0,  "", "titulaire"},
				{0, "ElAmrani", "Ahmed",     2,  "defenseur",  5, 11, 1989,  2,  "", "titulaire"},
				{0, "Haddad",   "Youssef",   3,  "defenseur", 21,  6, 1993,  1,  "", "titulaire"},
				{0, "Mansouri", "Omar",      4,  "defenseur", 9,  8, 1995,  0,  "", "remplacant"},
				{0, "Bensaid",  "Ali",       5,  "milieu",    2,  2, 1992,  4,  "", "titulaire"},
				{0, "Rahmani",  "Yassine",   6,  "milieu",   18,  7, 1996,  3,  "", "titulaire"},
				{0, "Fassi",    "Anas",      7,  "milieu",   27,  1, 1998,  5,  "", "remplacant"},
				{0, "Chafiq",   "Hamza",     8,  "milieu",   14, 12, 1994,  2,  "", "titulaire"},
				{0, "Idrissi",  "Soufiane",  9,  "attaquant", 7,  5, 1997, 10,  "", "titulaire"},
				{0, "Kharbouch","Hicham",   10,  "attaquant",25,  4, 1991,  8,  "", "titulaire"},
				{0, "Belkacem", "Amine",    11,  "attaquant",30, 10, 1990,  6,  "", "remplacant"},
				{0, "Nasser",   "Reda",     12,  "defenseur",16,  9, 1999,  1,  "", "remplacant"},
				{0, "Ziani",    "Karim",    13,  "milieu",   3,  3, 1993,  7,  "", "titulaire"},
				{0, "Talbi",    "Rachid",   14,  "defenseur",22,  2, 1995,  0,  "", "remplacant"},
				{0, "Lamrani",  "Oussama",  15,  "attaquant",11,  1, 1998, 12,  "", "titulaire"}
			};

			for (int i = 0; i < 15 && nombreJoueurs < 30; i++) {
				Joueur j = joueursInit[i];
				j.id = prochainId++;
				sprintf(j.dateInscription, "%02d/%02d/%04d", tm_info->tm_mday, tm_info->tm_mon + 1, tm_info->tm_year + 1900);
				if (numeroMaillotExiste(j.numeroMaillot)) {
					int num = 1;
					while (num <= 99 && numeroMaillotExiste(num)) num++;
					j.numeroMaillot = num;
				}
				equipe[nombreJoueurs++] = j;
			}
		}
		afficherMenu();
		scanf("%d", &choix);

		switch(choix) {
			case 1:
				ajouterDesJoueurs();
				break;
			case 2:
				afficherJoueurs();
				break;
			case 3:
				modifierJoueur();
				break;
			case 4:
				supprimerJoueur();
				break;
			case 5:
				rechercherJoueur();
				break;
			case 6:
				afficherStatistiques();
				break;
			case 7:
				printf("Merci d'avoir utilise le gestionnaire d'equipe de football!\n");
				break;
			default:
				printf("Choix invalide. Veuillez reessayer.\n");
		}

		if (choix != 7) {
			printf("\nPress Enter to continue...");
			getchar();
			getchar();
		}

	} while (choix != 7);

	return 0;
}

// Fonction pour changer la couleur du texte
void setColor(int color) {
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, color);
}

// Fonction pour effacer l'ecran
void clearScreen() {
	system("cls");
}
void afficherTitre() {
	setColor(COLOR_CYAN);
	printf("==========================================\n");
	setColor(COLOR_YELLOW);
	printf("     GESTIONNAIRE D'EQUIPE DE FOOTBALL\n");
	setColor(COLOR_CYAN);
	printf("==========================================\n");
	setColor(COLOR_WHITE);
}
void afficherMenu() {
	setColor(COLOR_BLUE);
	printf("===========================================\n");
	setColor(COLOR_YELLOW);
	printf("\n=== GESTIONNAIRE D'EQUIPE DE FOOTBALL ===\n");
	setColor(COLOR_WHITE);
	printf("1. Ajouter des joueurs\n");
	printf("2. Afficher les joueurs\n");
	printf("3. Modifier un joueur\n");
	printf("4. Supprimer un joueur\n");
	printf("5. Rechercher un joueur\n");
	printf("6. Statistiques\n");
	setColor(COLOR_RED);
	printf("7. Quitter\n");
	setColor(COLOR_BLUE);
	printf("==========================================\n");
	setColor(COLOR_CYAN);
	printf("Votre choix: ");
	setColor(COLOR_WHITE);
}

void ajouterJoueur() {
	if (nombreJoueurs >= 30) {
		setColor(COLOR_RED);
		printf("Erreur: L'equipe est pleine (30 joueurs maximum).\n");
		setColor(COLOR_WHITE);
		return;
	}

	clearScreen();
	Joueur nouveauJoueur;
	nouveauJoueur.id = prochainId++;
	afficherTitre();

	setColor(COLOR_GREEN);
	printf("\n=== AJOUT D'UN NOUVEAU JOUEUR ===\n");
	setColor(COLOR_WHITE);

// Saisie du nom
	do {
		int invalide = 0;
		printf("Nom: ");
		scanf("%49s", nouveauJoueur.nom);
		if (strlen(nouveauJoueur.nom) == 0) {
			setColor(COLOR_RED);
			printf("Erreur: Le nom ne peut pas etre vide.\n");
			setColor(COLOR_WHITE);
			invalide = 1;
		}
//Controller le saisie du nom
		 else {
			for (size_t i = 0; i < strlen(nouveauJoueur.nom); i++) {
				if (isdigit((unsigned char)nouveauJoueur.nom[i])) {
					invalide = 1;
					break;
				}
			}
			if (invalide) {
				setColor(COLOR_RED);
				printf("Erreur: Le nom ne doit pas contenir de chiffres.\n");
				setColor(COLOR_WHITE);
			}
		}
	} while (strlen(nouveauJoueur.nom) == 0 || ({int inv=0; for (size_t i=0; i<strlen(nouveauJoueur.nom); i++) {
		if (isdigit((unsigned char)nouveauJoueur.nom[i])) {
				inv=1;
				break;
			}
		} inv;}));

// Saisie du prenom
	do {
		int invalide = 0;
		printf("Prenom: ");
		scanf("%49s", nouveauJoueur.prenom);
		if (strlen(nouveauJoueur.prenom) == 0) {
			printf("Erreur: Le prenom ne peut pas etre vide.\n");
			invalide = 1;
		}
//Controller le saisie du nom
		 else {
			for (size_t i = 0; i < strlen(nouveauJoueur.prenom); i++) {
				if (isdigit((unsigned char)nouveauJoueur.prenom[i])) {
					invalide = 1;
					break;
				}
			}
			if (invalide) {
				printf("Erreur: Le prenom ne doit pas contenir de chiffres.\n");
			}
		}
	} while (strlen(nouveauJoueur.prenom) == 0 || ({int inv=0; for (size_t i=0; i<strlen(nouveauJoueur.prenom); i++) {
		if (isdigit((unsigned char)nouveauJoueur.prenom[i])) {
				inv=1;
				break;
			}
		} inv;}));

// Saisie du numero de maillot
	do {
		printf("Numero de maillot: ");
		scanf("%d", &nouveauJoueur.numeroMaillot);
		if (nouveauJoueur.numeroMaillot <= 0) {
			printf("Erreur: Le numero de maillot doit etre positif.\n");
		} else if (numeroMaillotExiste(nouveauJoueur.numeroMaillot)) {
			printf("Erreur: Ce numero de maillot existe deja.\n");
		}
	} while (nouveauJoueur.numeroMaillot <= 0 || numeroMaillotExiste(nouveauJoueur.numeroMaillot));

// Saisie du poste
	do {
		printf("Poste (gardien/defenseur/milieu/attaquant): ");
		scanf("%s", nouveauJoueur.poste);
		if (strcmp(nouveauJoueur.poste, "gardien") != 0 &&
		        strcmp(nouveauJoueur.poste, "defenseur") != 0 &&
		        strcmp(nouveauJoueur.poste, "milieu") != 0 &&
		        strcmp(nouveauJoueur.poste, "attaquant") != 0) {
			printf("Erreur: Poste invalide. Utilisez: gardien, defenseur, milieu, ou attaquant.\n");
		}
	} while (strcmp(nouveauJoueur.poste, "gardien") != 0 &&
	         strcmp(nouveauJoueur.poste, "defenseur") != 0 &&
	         strcmp(nouveauJoueur.poste, "milieu") != 0 &&
	         strcmp(nouveauJoueur.poste, "attaquant") != 0);

// Saisie de la date de naissance
	do {
		printf("Date de naissance (dd/mm/yyyy): ");
		scanf("%d/%d/%d", &nouveauJoueur.jourNaissance, &nouveauJoueur.moisNaissance, &nouveauJoueur.anneeNaissance);
		if (!validerDate(nouveauJoueur.jourNaissance, nouveauJoueur.moisNaissance, nouveauJoueur.anneeNaissance)) {
			int age = calculerAge(nouveauJoueur.jourNaissance, nouveauJoueur.moisNaissance, nouveauJoueur.anneeNaissance);
			if (age < 17) {
				printf("Erreur: L'age doit etre d'au moins 17 ans. Age calcule: %d ans.\n", age);
			} else if (age > 40) {
				printf("Erreur: L'age doit etre d'au maximum 40 ans. Age calcule: %d ans.\n", age);
			} else {
				printf("Erreur: Date invalide. Veuillez saisir une date correcte.\n");
			}
		}
	} while (!validerDate(nouveauJoueur.jourNaissance, nouveauJoueur.moisNaissance, nouveauJoueur.anneeNaissance));

// Saisie du nombre de buts
	do {
		printf("Nombre de buts marques: ");
		scanf("%d", &nouveauJoueur.buts);
		if (nouveauJoueur.buts < 0) {
			printf("Erreur: Le nombre de buts ne peut pas etre negatif.\n");
		}
	} while (nouveauJoueur.buts < 0);

// Saisie du statut
	do {
		printf("Statut (titulaire/remplacant): ");
		scanf("%s", nouveauJoueur.statut);
		if (strcmp(nouveauJoueur.statut, "titulaire") != 0 &&
		        strcmp(nouveauJoueur.statut, "remplacant") != 0) {
			printf("Erreur: Statut invalide. Utilisez: titulaire ou remplacant.\n");
		}
	} while (strcmp(nouveauJoueur.statut, "titulaire") != 0 &&
	         strcmp(nouveauJoueur.statut, "remplacant") != 0);

// Date d'inscription
	time_t maintenant = time(NULL);
	struct tm* tm_info = localtime(&maintenant);
	sprintf(nouveauJoueur.dateInscription, "%02d/%02d/%04d",
	        tm_info->tm_mday, tm_info->tm_mon + 1, tm_info->tm_year + 1900);

// Ajouter le joueur a l'equipe
	equipe[nombreJoueurs] = nouveauJoueur;
	nombreJoueurs++;

	setColor(COLOR_GREEN);
	printf("\nJoueur ajoute avec succes!\n");
	setColor(COLOR_WHITE);
	printf("ID: %d\nNom: %s %s\nAge: %d ans\n",
	       nouveauJoueur.id, nouveauJoueur.nom, nouveauJoueur.prenom,
	       calculerAge(nouveauJoueur.jourNaissance, nouveauJoueur.moisNaissance, nouveauJoueur.anneeNaissance));

// Verifier si c'est une star (10+ buts)
	if (nouveauJoueur.buts >= 10) {
		setColor(COLOR_MAGENTA);
		printf("Felicitations! %s %s est maintenant une STAR de l'equipe!\n",
		       nouveauJoueur.prenom, nouveauJoueur.nom);
		setColor(COLOR_WHITE);
	}
}
void ajouterPlusieursJoueurs() {
	afficherTitre();
	int nombre;
	printf("\nCombien de joueurs voulez vous ajouter : ");
	scanf("%d", &nombre);

	if (nombre <= 0) {
		printf("Erreur: Le nombre doit etre positif.\n");
		return;
	}

	for (int i = 0; i < nombre; i++) {
		printf("\n--- Joueur %d/%d ---\n", i + 1, nombre);
		ajouterJoueur();
	}

	printf("\n%d joueurs ajoutes avec succes!\n", nombre);
}

void ajouterDesJoueurs() {
	int choix;
	clearScreen();
	printf("\n=== AJOUTER DES JOUEURS ===\n");
	printf("1. Ajouter un joueur\n");
	printf("2. Ajouter plusieurs joueurs\n");
	printf("3. annule l ajout\n");
	printf("Votre choix: ");
	scanf("%d", &choix);

	switch(choix) {
		case 1:
			ajouterJoueur();
			break;
		case 2:
			ajouterPlusieursJoueurs();
			break;
		case 3:
			printf("return");
			break;
		default:
			printf("Choix invalide.\n");
	}
}

void afficherJoueurs() {
	clearScreen();
	afficherTitre();
	if (nombreJoueurs == 0) {
		printf("\nAucun joueur dans l'equipe.\n");
		return;
	}

	int choix;
	printf("\n=== AFFICHAGE DES JOUEURS ===\n");
	printf("1. Tous les joueurs\n");
	printf("2. Trier par nom\n");
	printf("3. Trier par age\n");
	printf("4. Afficher par poste\n");
	printf("Votre choix: ");
	scanf("%d", &choix);

	switch(choix) {
		case 1: {
// Afficher tous les joueurs triés par ID
			setColor(COLOR_CYAN);
			printf("\n=== TOUS LES JOUEURS (TRIES PAR ID) ===\n");
			setColor(COLOR_YELLOW);
			printf("ID  | Nom            | Prenom         | Maillot | Poste      | Age  | Buts  | Statut      | Inscrit le\n");
			setColor(COLOR_GRAY);
			printf("----|----------------|----------------|---------|------------|------|-------|-------------|----------\n");
			setColor(COLOR_WHITE);

// Créer un tableau d'indices tries par ID
			int indices[nombreJoueurs];
			for (int i = 0; i < nombreJoueurs; i++) {
				indices[i] = i;
			}

// Tri par selection base sur l'ID
			for (int i = 0; i < nombreJoueurs - 1; i++) {
				int minIndex = i;
				for (int j = i + 1; j < nombreJoueurs; j++) {
					if (equipe[indices[j]].id < equipe[indices[minIndex]].id) {
						minIndex = j;
					}
				}
				if (minIndex != i) {
					int temp = indices[i];
					indices[i] = indices[minIndex];
					indices[minIndex] = temp;
				}
			}

// Afficher les joueurs tries par ID
			for (int i = 0; i < nombreJoueurs; i++) {
				int index = indices[i];
				printf("%3d | %-14s | %-14s | %7d | %-10s | %3d | %4d | %-10s | %s\n",
				       equipe[index].id, equipe[index].nom, equipe[index].prenom, equipe[index].numeroMaillot,
				       equipe[index].poste, calculerAge(equipe[index].jourNaissance, equipe[index].moisNaissance, equipe[index].anneeNaissance),
				       equipe[index].buts, equipe[index].statut, equipe[index].dateInscription);
			}
			break;
		}
		case 2:
			trierParNom();
			break;
		case 3:
			trierParAge();
			break;
		case 4:
			afficherParPoste();
			break;
		default:
			printf("Choix invalide.\n");
	}
}

void trierParNom() {
	clearScreen();
	afficherTitre();
// Tri par selection base sur le nom
	for (int i = 0; i < nombreJoueurs - 1; i++) {
		int minIndex = i;
		for (int j = i + 1; j < nombreJoueurs; j++) {
			if (strcmp(equipe[j].nom, equipe[minIndex].nom) < 0) {
				minIndex = j;
			}
		}
		if (minIndex != i) {
			Joueur temp = equipe[i];
			equipe[i] = equipe[minIndex];
			equipe[minIndex] = temp;
		}
	}

	printf("\n=== JOUEURS TRIES PAR NOM ===\n");
	printf("ID  | Nom            | Prenom         | Maillot | Poste      | Age | Buts\n");
	printf("----|----------------|----------------|---------|------------|-----|------\n");
	for (int i = 0; i < nombreJoueurs; i++) {
		printf("%3d | %-14s | %-14s | %7d | %-10s | %3d | %4d\n",
		       equipe[i].id, equipe[i].nom, equipe[i].prenom, equipe[i].numeroMaillot,
		       equipe[i].poste, calculerAge(equipe[i].jourNaissance, equipe[i].moisNaissance, equipe[i].anneeNaissance),
		       equipe[i].buts);
	}
}

void trierParAge() {
	clearScreen();
	afficherTitre();
// Tri par selection base sur l'age
	for (int i = 0; i < nombreJoueurs - 1; i++) {
		int minIndex = i;
		for (int j = i + 1; j < nombreJoueurs; j++) {
			int ageJ = calculerAge(equipe[j].jourNaissance, equipe[j].moisNaissance, equipe[j].anneeNaissance);
			int ageMin = calculerAge(equipe[minIndex].jourNaissance, equipe[minIndex].moisNaissance, equipe[minIndex].anneeNaissance);
			if (ageJ < ageMin) {
				minIndex = j;
			}
		}
		if (minIndex != i) {
			Joueur temp = equipe[i];
			equipe[i] = equipe[minIndex];
			equipe[minIndex] = temp;
		}
	}

	printf("\n=== JOUEURS TRIES PAR AGE ===\n");
	printf("ID  | Nom            | Prenom         | Maillot | Poste      | Age | Buts\n");
	printf("----|----------------|----------------|---------|------------|-----|------\n");
	for (int i = 0; i < nombreJoueurs; i++) {
		printf("%3d | %-14s | %-14s | %7d | %-10s | %3d | %4d\n",
		       equipe[i].id, equipe[i].nom, equipe[i].prenom, equipe[i].numeroMaillot,
		       equipe[i].poste, calculerAge(equipe[i].jourNaissance, equipe[i].moisNaissance, equipe[i].anneeNaissance),
		       equipe[i].buts);
	}
}

void afficherParPoste() {
	clearScreen();
	afficherTitre();
	char posteRecherche[20];
	printf("Poste a afficher (gardien/defenseur/milieu/attaquant): ");
	scanf("%s", posteRecherche);

	printf("\n=== JOUEURS AU POSTE: %s ===\n", posteRecherche);
	printf("ID  | Nom            | Prenom         | Maillot | Age | Buts | Statut\n");
	printf("----|----------------|----------------|---------|-----|------|--------\n");
	int trouve = 0;
	for (int i = 0; i < nombreJoueurs; i++) {
		if (strcmp(equipe[i].poste, posteRecherche) == 0) {
			printf("%3d | %-14s | %-14s | %7d | %3d | %4d | %s\n",
			       equipe[i].id, equipe[i].nom, equipe[i].prenom, equipe[i].numeroMaillot,
			       calculerAge(equipe[i].jourNaissance, equipe[i].moisNaissance, equipe[i].anneeNaissance),
			       equipe[i].buts, equipe[i].statut);
			trouve = 1;
		}
	}

	if (!trouve) {
		printf("Aucun joueur trouve au poste %s.\n", posteRecherche);
	}
}
// Modification de joueur par Id
void modifierJoueur() {
	clearScreen();
	afficherTitre();
	if (nombreJoueurs == 0) {
		printf("\nAucun joueur dans l'equipe.\n");
		return;
	}

	int id;
	printf("\nID du joueur a modifier: ");
	scanf("%d", &id);

	int index = -1;
	for (int i = 0; i < nombreJoueurs; i++) {
		if (equipe[i].id == id) {
			index = i;
			break;
		}
	}

	if (index == -1) {
		printf("Joueur avec l'ID %d non trouve.\n", id);
		return;
	}

	int choix;
	printf("\n=== MODIFICATION DU JOUEUR %s %s ===\n", equipe[index].nom, equipe[index].prenom);
	printf("1. Modifier le poste\n");
	printf("2. Modifier l'age (date de naissance)\n");
	printf("3. Modifier le nombre de buts\n");
	printf("4. annule la modification\n");
	printf("Votre choix: ");
	scanf("%d", &choix);

	switch(choix) {
		case 1:
			do {
				printf("Nouveau poste (gardien/defenseur/milieu/attaquant): ");
				scanf("%s", equipe[index].poste);
				if (strcmp(equipe[index].poste, "gardien") != 0 &&
				        strcmp(equipe[index].poste, "defenseur") != 0 &&
				        strcmp(equipe[index].poste, "milieu") != 0 &&
				        strcmp(equipe[index].poste, "attaquant") != 0) {
					printf("Erreur: Poste invalide.\n");
				}
			} while (strcmp(equipe[index].poste, "gardien") != 0 &&
			         strcmp(equipe[index].poste, "defenseur") != 0 &&
			         strcmp(equipe[index].poste, "milieu") != 0 &&
			         strcmp(equipe[index].poste, "attaquant") != 0);
			printf("Poste modifié avec succes!\n");
			break;

		case 2:
			do {
				printf("Nouvelle date de naissance (dd/mm/yyyy): ");
				scanf("%d/%d/%d", &equipe[index].jourNaissance, &equipe[index].moisNaissance, &equipe[index].anneeNaissance);
				if (!validerDate(equipe[index].jourNaissance, equipe[index].moisNaissance, equipe[index].anneeNaissance)) {
					int age = calculerAge(equipe[index].jourNaissance, equipe[index].moisNaissance, equipe[index].anneeNaissance);
					if (age < 17) {
						printf("Erreur: L'age doit etre d'au moins 17 ans: \n", age);
					} else if (age > 40) {
						printf("Erreur: L'age doit etre d'au maximum 40 ans: \n", age);
					} else {
						printf("Erreur: Date invalide.\n");
					}
				}
			} while (!validerDate(equipe[index].jourNaissance, equipe[index].moisNaissance, equipe[index].anneeNaissance));
			printf("Age modifie avec succes! Nouvel age: %d ans\n",
			       calculerAge(equipe[index].jourNaissance, equipe[index].moisNaissance, equipe[index].anneeNaissance));
			break;

		case 3:
			do {
				printf("Nouveau nombre de buts: ");
				scanf("%d", &equipe[index].buts);
				if (equipe[index].buts < 0) {
					printf("Erreur: Le nombre de buts ne peut pas etre negatif.\n");
				}
			} while (equipe[index].buts < 0);
			printf("Nombre de buts modifie avec succes!\n");
// Verifier si c'est maintenant une star
			if (equipe[index].buts >= 10) {
				printf("Felicitations! %s %s est maintenant une STAR de l'equipe!\n",
				       equipe[index].prenom, equipe[index].nom);
			}
			break;
		case 4:
			printf("Exiting modification.\n");
			break;
		default:
			printf("Choix invalide.\n");
	}
}
// Suppression d'un joueur par 
void supprimerJoueur() {
	clearScreen();
	afficherTitre();
	if (nombreJoueurs == 0) {
		printf("\nAucun joueur dans l'equipe.\n");
		return;
	}

	int id;
	printf("\nID du joueur a supprimer: ");
	scanf("%d", &id);

	int index = -1;
	for (int i = 0; i < nombreJoueurs; i++) {
		if (equipe[i].id == id) {
			index = i;
			break;
		}
	}

	if (index == -1) {
		printf("Joueur avec l'ID %d non trouve.\n", id);
		return;
	}

	printf("Etes-vous sur de vouloir supprimer %s %s? (o/n): ",
	       equipe[index].nom, equipe[index].prenom);
	char confirmation;
	scanf(" %c", &confirmation);
	if (confirmation == 'o' || confirmation == 'O') {
		for (int i = index; i < nombreJoueurs - 1; i++) {
			equipe[i] = equipe[i + 1];
		}
		nombreJoueurs--;
		printf("[SUCCES]Joueur supprime avec succes!\n");
	} else {
		printf("Suppression annulee.\n");
	}

}
//Rechercher un joueur d'apres le nom ou ID	
void rechercherJoueur() {
	clearScreen();
	afficherTitre();
	if (nombreJoueurs == 0) {
		printf("\nAucun joueur dans l'equipe.\n");
		return;
	}

	int choix;
	printf("\n=== RECHERCHE DE JOUEUR ===\n");
	printf("1. Rechercher par ID\n");
	printf("2. Rechercher par nom\n");
	printf("Votre choix: ");
	scanf("%d", &choix);

	switch(choix) {
		case 1: {
			int id;
			printf("ID du joueur: ");
			scanf("%d", &id);

			int trouve = 0;
			for (int i = 0; i < nombreJoueurs; i++) {
				if (equipe[i].id == id) {
					printf("\n=== JOUEUR TROUVE ===\n");
					printf("ID: %d | %s %s | Maillot: %d | Poste: %s | Age: %d ans | Buts: %d | Statut: %s | Inscrit le: %s\n",
					       equipe[i].id, equipe[i].nom, equipe[i].prenom, equipe[i].numeroMaillot,
					       equipe[i].poste, calculerAge(equipe[i].jourNaissance, equipe[i].moisNaissance, equipe[i].anneeNaissance),
					       equipe[i].buts, equipe[i].statut, equipe[i].dateInscription);
					trouve = 1;
					break;
				}
			}
			if (!trouve) {
				printf("Aucun joueur trouve avec l'ID %d.\n", id);
			}
			break;
		}

		case 2: {
			char nomRecherche[50];
			printf("Nom du joueur: ");
			scanf("%s", nomRecherche);

			int trouve = 0;
			for (int i = 0; i < nombreJoueurs; i++) {
				if (strcmp(equipe[i].nom, nomRecherche) == 0) {
					printf("\n=== JOUEUR TROUVE ===\n");
					printf("ID: %d | %s %s | Maillot: %d | Poste: %s | Age: %d ans | Buts: %d | Statut: %s\n",
					       equipe[i].id, equipe[i].nom, equipe[i].prenom, equipe[i].numeroMaillot,
					       equipe[i].poste, calculerAge(equipe[i].jourNaissance, equipe[i].moisNaissance, equipe[i].anneeNaissance),
					       equipe[i].buts, equipe[i].statut);
					trouve = 1;
				}
			}
			if (!trouve) {
				printf("Aucun joueur trouve avec le nom %s.\n", nomRecherche);
			}
			break;
		}

		default:
			printf("Choix invalide.\n");
	}
}
// Les statistiques
void afficherStatistiques() {
	clearScreen();
	afficherTitre();
	if (nombreJoueurs == 0) {
		printf("\nAucun joueur dans l'equipe.\n");
		return;
	}

	setColor(COLOR_CYAN);
	printf("\n=== STATISTIQUES DE L'EQUIPE ===\n");
	setColor(COLOR_WHITE);

// Nombre total de joueurs
	printf("Nombre total de joueurs: %d\n", nombreJoueurs);

// Age moyen
	int sommeAges = 0;
	for (int i = 0; i < nombreJoueurs; i++) {
		sommeAges += calculerAge(equipe[i].jourNaissance, equipe[i].moisNaissance, equipe[i].anneeNaissance);
	}
	double ageMoyen = (double)sommeAges / nombreJoueurs;
	printf("Age moyen des joueurs: %.1f ans\n", ageMoyen);

// Joueurs ayant marque plus de X buts
	int seuilButs;
	printf("Afficher les joueurs ayant marque plus de combien de buts : ");
	scanf("%d", &seuilButs);

	printf("\nJoueurs ayant marque plus de %d buts:\n", seuilButs);
	int trouve = 0;
	for (int i = 0; i < nombreJoueurs; i++) {
		if (equipe[i].buts > seuilButs) {
			printf("- %s %s: %d buts\n", equipe[i].nom, equipe[i].prenom, equipe[i].buts);
			trouve = 1;
		}
	}
	if (!trouve) {
		printf("Aucun joueur n'a marque plus de %d buts.\n", seuilButs);
	}

// Meilleur buteur
	int maxButs = -1;
	int indexMeilleurButeur = -1;
	for (int i = 0; i < nombreJoueurs; i++) {
		if (equipe[i].buts > maxButs) {
			maxButs = equipe[i].buts;
			indexMeilleurButeur = i;
		}
	}

	if (indexMeilleurButeur != -1) {
		setColor(COLOR_MAGENTA);
		printf("\nMeilleur buteur: %s %s avec %d buts\n",
		       equipe[indexMeilleurButeur].nom, equipe[indexMeilleurButeur].prenom, maxButs);
		setColor(COLOR_WHITE);
	}

// Joueur le plus jeune et le plus age
	int ageMin = 999, ageMax = -1;
	int indexPlusJeune = -1, indexPlusAge = -1;

	for (int i = 0; i < nombreJoueurs; i++) {
		int age = calculerAge(equipe[i].jourNaissance, equipe[i].moisNaissance, equipe[i].anneeNaissance);
		if (age < ageMin) {
			ageMin = age;
			indexPlusJeune = i;
		}
		if (age > ageMax) {
			ageMax = age;
			indexPlusAge = i;
		}
	}

	if (indexPlusJeune != -1) {
		setColor(COLOR_GREEN);
		printf("Joueur le plus jeune: %s %s (%d ans)\n",
		       equipe[indexPlusJeune].nom, equipe[indexPlusJeune].prenom, ageMin);
		setColor(COLOR_WHITE);
	}

	if (indexPlusAge != -1) {
		setColor(COLOR_BLUE);
		printf("Joueur le plus age: %s %s (%d ans)\n",
		       equipe[indexPlusAge].nom, equipe[indexPlusAge].prenom, ageMax);
		setColor(COLOR_WHITE);
	}
}
// Fonction pour calculer l'age base sur la date de naissance
int calculerAge(int jour, int mois, int annee) {
	time_t maintenant = time(NULL);
	struct tm* tm_info = localtime(&maintenant);

	int age = tm_info->tm_year + 1900 - annee;

// Ajuster si l'anniversaire n'est pas encore passe cette annee
//	if (tm_info->tm_mon + 1 < mois ||
//	        (tm_info->tm_mon + 1 == mois && tm_info->tm_mday < jour)) {
//		age--;
//	}

	return age;
}

// Fonction pour verifier si une annee est bissextile
int estAnneeBissextile(int annee) {
	return (annee % 4 == 0 && annee % 100 != 0) || (annee % 400 == 0);
}

// Fonction pour valider une date
int validerDate(int jour, int mois, int annee) {
	if (annee < 1900 || annee > 2024) return 0;
	if (mois < 1 || mois > 12) return 0;
	if (jour < 1) return 0;

	int joursParMois[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
// Fevrier en annee bissextile
	if (estAnneeBissextile(annee)) {
		joursParMois[1] = 29;
	}
	if (jour > joursParMois[mois - 1]) return 0;
	
// Vérifier que l'age est entre 17 et 40 ans
	int age = calculerAge(jour, mois, annee);
	if (age < 17 || age > 40) return 0;

	return 1;
}
// Fonction pour verifier si un numero de maillot existe deja
int numeroMaillotExiste(int numero) {
	for (int i = 0; i < nombreJoueurs; i++) {
		if (equipe[i].numeroMaillot == numero) {
			return 1;
		}
	}
	return 0;
}