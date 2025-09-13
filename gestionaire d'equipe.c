#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include <ctype.h>
#include <windows.h>
#include <stdarg.h>

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
int validerDate(int jour, int mois, int annee);
int numeroMaillotExiste(int numero);
void clearScreen();
void printError(const char* format, ...);
int countTitulaires();
int gardienExists(int excludeId);
int isAllDigits(const char* s);
int gardienTitulaireExists();
int validerNom(char* nom, const char* type);
void saisirNom(char* nom, const char* type);
int saisirChoixNumerique(int min, int max, const char* message);
void afficherMenuPoste();
void afficherMenuStatut();
void afficherEnTeteTableau();
void afficherJoueur(int index);
void afficherJoueurLigne(int index);
int saisirEntierPositif(const char* message);
void trierEquipe(int (*comparer)(const void*, const void*));
int comparerParNom(const void* a, const void* b);
int comparerParAge(const void* a, const void* b);
int comparerParId(const void* a, const void* b);
void afficherJoueursParCritere(const char* titre, int (*comparer)(const void*, const void*));
void afficherJoueursParPoste(const char* poste);

int main() {
	int choix;
	do {
		clearScreen();
		afficherTitre();
		if (nombreJoueurs == 0) {
			time_t maintenant = time(NULL);
			struct tm* tm_info = localtime(&maintenant);

			Joueur joueursInit[15] = {
				{0, "Benali",   "Mohamed",   1,  "gardien",   12,  3, 1999,  0,  "", "titulaire"},
				{0, "ElAmrani", "Ahmed",     2,  "defenseur",  5, 11, 2000,  2,  "", "titulaire"},
				{0, "Haddad",   "Youssef",   3,  "defenseur", 21,  6, 1993,  1,  "", "titulaire"},
				{0, "Mansouri", "Omar",      4,  "defenseur", 9,  8, 1995,  0,  "", "remplacant"},
				{0, "Bensaid",  "Ali",       5,  "milieu",    2,  2, 1992,  4,  "", "titulaire"},
				{0, "Rahmani",  "Yassine",   6,  "milieu",   18,  7, 1996,  3,  "", "titulaire"},
				{0, "Fassi",    "Anas",      7,  "milieu",   27,  1, 2006,  5,  "", "remplacant"},
				{0, "Chafiq",   "Hamza",     8,  "milieu",   14, 12, 1994,  2,  "", "titulaire"},
				{0, "Idrissi",  "Soufiane",  9,  "attaquant", 7,  5, 1997, 10,  "", "titulaire"},
				{0, "Kharbouch","Hicham",   10,  "attaquant",25,  4, 1991,  22,  "", "titulaire"},
				{0, "Belkacem", "Amine",    11,  "attaquant",30, 10, 1990,  6,  "", "remplacant"},
				{0, "Nasser",   "Reda",     12,  "defenseur",16,  9, 2003,  1,  "", "remplacant"},
				{0, "Ziani",    "Karim",    13,  "milieu",   3,  3, 1993,  7,  "", "titulaire"},
				{0, "Talbi",    "Rachid",   14,  "defenseur",22,  2, 2001,  0,  "", "remplacant"},
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
				printError("Choix invalide. Veuillez reessayer.\n");
		}

		if (choix != 7) {
			printf("\ntaper Entrer pour continue...");
			getchar();
			getchar();
		}

	} while (choix != 7);

	return 0;
}

// ============================================================================
// FONCTIONS UTILITAIRES DE BASE
// ============================================================================

// Fonction pour changer la couleur du texte dans la console
// Utilise l'API Windows pour modifier les couleurs d'affichage
void setColor(int color) {
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, color);
}

// Fonction pour effacer l'ecran de la console
// Utilise la commande system "cls" pour nettoyer l'affichage
void clearScreen() {
	system("cls");
}
void printError(const char* format, ...) {
	va_list args;
	va_start(args, format);
	setColor(COLOR_RED);
	vprintf(format, args);
	setColor(COLOR_WHITE);
	va_end(args);
}
int countTitulaires() {
	int total = 0;
	for (int i = 0; i < nombreJoueurs; i++) {
		if (strcmp(equipe[i].statut, "titulaire") == 0) {
			total++;
		}
	}
	return total;
}
// excludeId: lors de la modification d'un joueur, ignorer le poste actuel de ce joueur
int gardienExists(int excludeId) {
	for (int i = 0; i < nombreJoueurs; i++) {
		if (excludeId != 0 && equipe[i].id == excludeId) {
			continue;
		}
		if (strcmp(equipe[i].poste, "gardien") == 0) {
			return 1;
		}
	}
	return 0;
}
int isAllDigits(const char* s) {
	if (s == NULL || *s == '\0') return 0;
	for (size_t i = 0; s[i] != '\0'; i++) {
		if (!isdigit((unsigned char)s[i])) return 0;
	}
	return 1;
}
int gardienTitulaireExists() {
	for (int i = 0; i < nombreJoueurs; i++) {
		if (strcmp(equipe[i].poste, "gardien") == 0 && strcmp(equipe[i].statut, "titulaire") == 0) {
			return 1;
		}
	}
	return 0;
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
	setColor(COLOR_GREEN);
	printf("1. "); setColor(COLOR_WHITE); printf("Ajouter des joueurs\n");
	setColor(COLOR_GREEN);
	printf("2. "); setColor(COLOR_WHITE); printf("Afficher les joueurs\n");
	setColor(COLOR_GREEN);
	printf("3. "); setColor(COLOR_WHITE); printf("Modifier un joueur\n");
	setColor(COLOR_GREEN);
	printf("4. "); setColor(COLOR_WHITE); printf("Supprimer un joueur\n");
	setColor(COLOR_GREEN);
	printf("5. "); setColor(COLOR_WHITE); printf("Rechercher un joueur\n");
	setColor(COLOR_GREEN);
	printf("6. "); setColor(COLOR_WHITE); printf("Statistiques\n");
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
		printError("Erreur: L'equipe est pleine!!.\n");
		return;
	}

	clearScreen();
	Joueur nouveauJoueur;
	nouveauJoueur.id = prochainId++;
	afficherTitre();

	setColor(COLOR_GREEN);
	printf("\n=== AJOUT D'UN NOUVEAU JOUEUR ===\n");
	setColor(COLOR_WHITE);

// Saisie du nom et prenom
	saisirNom(nouveauJoueur.nom, "Nom");
	saisirNom(nouveauJoueur.prenom, "Prenom");

// Saisie du numero de maillot
	do {
		printf("Numero de maillot (1-99): ");
		while (scanf("%d", &nouveauJoueur.numeroMaillot) != 1) {
			printError("Erreur: Entrez uniquement des chiffres.\n");
			while (getchar() != '\n');
			printf("Numero de maillot (1-99): ");
		}
		if (nouveauJoueur.numeroMaillot < 1 || nouveauJoueur.numeroMaillot > 99) {
			printError("Erreur: Le numero de maillot doit etre entre 1 et 99.\n");
		} else if (numeroMaillotExiste(nouveauJoueur.numeroMaillot)) {
			printError("Erreur: Ce numero de maillot existe deja.\n");
		}
	} while (nouveauJoueur.numeroMaillot < 1 || nouveauJoueur.numeroMaillot > 99 || numeroMaillotExiste(nouveauJoueur.numeroMaillot));

// Saisie du poste
	afficherMenuPoste();
	int choixPoste = saisirChoixNumerique(1, 4, "Votre choix (1-4): ");
	const char* postes[] = {"", "gardien", "defenseur", "milieu", "attaquant"};
	strcpy(nouveauJoueur.poste, postes[choixPoste]);

// Saisie de la date de naissance
	do {
		printf("Date de naissance (dd/mm/yyyy): ");
		while (scanf("%d/%d/%d", &nouveauJoueur.jourNaissance, &nouveauJoueur.moisNaissance, &nouveauJoueur.anneeNaissance) != 3) {
			printError("Erreur: Entrez uniquement des chiffres au format dd/mm/yyyy.\n");
			while (getchar() != '\n');
			printf("Date de naissance (dd/mm/yyyy): ");
		}
		if (!validerDate(nouveauJoueur.jourNaissance, nouveauJoueur.moisNaissance, nouveauJoueur.anneeNaissance)) {
			int age = calculerAge(nouveauJoueur.jourNaissance, nouveauJoueur.moisNaissance, nouveauJoueur.anneeNaissance);
			if (age < 17) {
				printError("Erreur: L'age doit etre d'au moins 17 ans. Age calcule: %d ans.\n", age);
			} else if (age > 40) {
				printError("Erreur: L'age doit etre d'au maximum 40 ans. Age calcule: %d ans.\n", age);
			} else {
				printError("Erreur: Date invalide. Veuillez saisir une date correcte.\n");
			}
		}
	} while (!validerDate(nouveauJoueur.jourNaissance, nouveauJoueur.moisNaissance, nouveauJoueur.anneeNaissance));

// Saisie du nombre de buts
	nouveauJoueur.buts = saisirEntierPositif("Nombre de buts marques: ");

// Saisie du statut
	afficherMenuStatut();
	int choixStatut = saisirChoixNumerique(1, 2, "Votre choix (1-2): ");
	const char* statuts[] = {"", "titulaire", "remplacant"};
	strcpy(nouveauJoueur.statut, statuts[choixStatut]);

// Date d'inscription
	time_t maintenant = time(NULL);
	struct tm* tm_info = localtime(&maintenant);
	sprintf(nouveauJoueur.dateInscription, "%02d/%02d/%04d",
	        tm_info->tm_mday, tm_info->tm_mon + 1, tm_info->tm_year + 1900);

// Contraintes: max 11 titulaires et exactement 1 gardien au maximum
	if (strcmp(nouveauJoueur.statut, "titulaire") == 0 && countTitulaires() >= 11) {
		setColor(COLOR_YELLOW);
		printf("Deja 11 titulaires. Vous serez ajoute comme remplacant.\n");
		setColor(COLOR_WHITE);
		strcpy(nouveauJoueur.statut, "remplacant");
	}
	if (strcmp(nouveauJoueur.poste, "gardien") == 0) {
		if (gardienTitulaireExists() && strcmp(nouveauJoueur.statut, "titulaire") == 0) {
			setColor(COLOR_YELLOW);
			printf("Un gardien titulaire existe deja. Vous serez ajoute comme remplacant.\n");
			setColor(COLOR_WHITE);
			strcpy(nouveauJoueur.statut, "remplacant");
		} else if (gardienExists(0) && strcmp(nouveauJoueur.statut, "remplacant") == 0) {
// ok: autoriser des gardiens supplementaires comme remplaçants
		} else if (gardienExists(0) && strcmp(nouveauJoueur.statut, "titulaire") != 0) {
// couvert par la première branche; garder pour la clarte
		} else if (gardienExists(0) && strcmp(nouveauJoueur.statut, "titulaire") == 0) {
// securite de secours, bien que la premiere branche devrait attraper le conflit titulaire
			setColor(COLOR_YELLOW);
			printf("Un gardien existe deja. Vous serez ajoute comme remplacant.\n");
			setColor(COLOR_WHITE);
			strcpy(nouveauJoueur.statut, "remplacant");
		}
	}

// Ajouter le joueur a l'equipe
	equipe[nombreJoueurs] = nouveauJoueur;
	nombreJoueurs++;

	setColor(COLOR_GREEN);
	printf("\nJoueur ajoute avec succes!\n");
	setColor(COLOR_WHITE);
	printf("ID: %d\nNom: %s %s\nAge: %d ans\n",
	       nouveauJoueur.id, nouveauJoueur.nom, nouveauJoueur.prenom,
	       calculerAge(nouveauJoueur.jourNaissance, nouveauJoueur.moisNaissance, nouveauJoueur.anneeNaissance));
	printf("Date de naissance: %02d/%02d/%04d\n",
	       nouveauJoueur.jourNaissance, nouveauJoueur.moisNaissance, nouveauJoueur.anneeNaissance);
	printf("Buts: %d\n", nouveauJoueur.buts);

// Verifier si c'est une star (20+ buts)
	if (nouveauJoueur.buts >= 20) {
		setColor(COLOR_MAGENTA);
		printf("Felicitations! %s %s est maintenant une STAR de l'equipe!\n",
		       nouveauJoueur.prenom, nouveauJoueur.nom);
		setColor(COLOR_WHITE);
	}
}
void ajouterPlusieursJoueurs() {
	afficherTitre();
	int nombre = saisirEntierPositif("\nCombien de joueurs voulez vous ajouter : ");
	
	for (int i = 0; i < nombre; i++) {
		printf("\n--- Joueur %d/%d ---\n", i + 1, nombre);
		ajouterJoueur();
	}
	setColor(COLOR_GREEN);
	printf("\n%d joueurs ajoutes avec succes!\n", nombre);
	setColor(COLOR_WHITE);
}

void ajouterDesJoueurs() {
	int choix;
	clearScreen();
	setColor(COLOR_CYAN);
	printf("\n=== AJOUTER DES JOUEURS ===\n");
	setColor(COLOR_GREEN);
	printf("1. "); setColor(COLOR_WHITE); printf("Ajouter un joueur\n");
	setColor(COLOR_GREEN);
	printf("2. "); setColor(COLOR_WHITE); printf("Ajouter plusieurs joueurs\n");
	setColor(COLOR_GREEN);
	printf("3. "); setColor(COLOR_WHITE); printf("annule l ajout\n");
	setColor(COLOR_CYAN);
	printf("Votre choix: ");
	setColor(COLOR_WHITE);
	scanf("%d", &choix);

	switch(choix) {
		case 1:
			ajouterJoueur();
			break;
		case 2:
			ajouterPlusieursJoueurs();
			break;
		case 3:
			printf("retour");
			break;
		default:
			printError("Choix invalide.\n");
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
	setColor(COLOR_CYAN);
	printf("\n=== AFFICHAGE DES JOUEURS ===\n");
	setColor(COLOR_GREEN);
	printf("1. "); setColor(COLOR_WHITE); printf("Tous les joueurs\n");
	setColor(COLOR_GREEN);
	printf("2. "); setColor(COLOR_WHITE); printf("Trier par nom\n");
	setColor(COLOR_GREEN);
	printf("3. "); setColor(COLOR_WHITE); printf("Trier par age\n");
	setColor(COLOR_GREEN);
	printf("4. "); setColor(COLOR_WHITE); printf("Afficher par poste\n");
	setColor(COLOR_CYAN);
	printf("Votre choix: ");
	setColor(COLOR_WHITE);
	scanf("%d", &choix);

	switch(choix) {
		case 1:
			afficherJoueursParCritere("TOUS LES JOUEURS", comparerParId);
			break;
		case 2:
			afficherJoueursParCritere("JOUEURS TRIES PAR NOM", comparerParNom);
			break;
		case 3:
			afficherJoueursParCritere("JOUEURS TRIES PAR AGE", comparerParAge);
			break;
		case 4:
			afficherParPoste();
			break;
		default:
			printError("Choix invalide.\n");
	}
}

//affichage par poste
void afficherParPoste() {
	clearScreen();
	afficherTitre();
	printf("Choisissez le poste a afficher:\n");
	afficherMenuPoste();
	int choixPoste = saisirChoixNumerique(1, 4, "Votre choix (1-4): ");
	const char* postes[] = {"", "gardien", "defenseur", "milieu", "attaquant"};
	afficherJoueursParPoste(postes[choixPoste]);
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
		printError("Joueur avec l'ID %d non trouve.\n", id);
		return;
	}

	int choix;
	setColor(COLOR_CYAN);
	printf("\n=== MODIFICATION DU JOUEUR %s %s ===\n", equipe[index].nom, equipe[index].prenom);
	setColor(COLOR_GREEN);
	printf("1. "); setColor(COLOR_WHITE); printf("Modifier le poste\n");
	setColor(COLOR_GREEN);
	printf("2. "); setColor(COLOR_WHITE); printf("Modifier l'age (date de naissance)\n");
	setColor(COLOR_GREEN);
	printf("3. "); setColor(COLOR_WHITE); printf("Modifier le nombre de buts\n");
	printf("4. "); setColor(COLOR_WHITE); printf("Modifier le statut (titulaire/remplacant)\n");
	setColor(COLOR_CYAN);
	setColor(COLOR_GREEN);
	printf("5. "); setColor(COLOR_WHITE); printf("annule la modification\n");
	setColor(COLOR_GREEN);
	printf("Votre choix: ");
	setColor(COLOR_WHITE);
	scanf("%d", &choix);

	switch(choix) {
		case 1: {
			afficherMenuPoste();
			int choixPoste = saisirChoixNumerique(1, 4, "Votre choix (1-4): ");
			const char* postes[] = {"", "gardien", "defenseur", "milieu", "attaquant"};
			strcpy(equipe[index].poste, postes[choixPoste]);
			if (strcmp(equipe[index].poste, "gardien") == 0 && gardienExists(equipe[index].id)) {
				printError("Erreur: L'equipe ne peut avoir qu'un seul gardien.\n");
				break;
			}
			setColor(COLOR_GREEN);
			printf("Poste modifie avec succes!\n");
			setColor(COLOR_WHITE);
			break;
		}

		case 2:
			do {
				printf("Nouvelle date de naissance (dd/mm/yyyy): ");
				while (scanf("%d/%d/%d", &equipe[index].jourNaissance, &equipe[index].moisNaissance, &equipe[index].anneeNaissance) != 3) {
					printError("Erreur: Entrez uniquement des chiffres au format dd/mm/yyyy.\n");
					while (getchar() != '\n');
					printf("Nouvelle date de naissance (dd/mm/yyyy): ");
				}
				if (!validerDate(equipe[index].jourNaissance, equipe[index].moisNaissance, equipe[index].anneeNaissance)) {
					int age = calculerAge(equipe[index].jourNaissance, equipe[index].moisNaissance, equipe[index].anneeNaissance);
					if (age < 17) {
						printError("Erreur: L'age doit etre d'au moins 17 ans: \n");
					} else if (age > 40) {
						printError("Erreur: L'age doit etre d'au maximum 45 ans: \n");
					} else {
						printError("Erreur: Date invalide.\n");
					}
				}
			} while (!validerDate(equipe[index].jourNaissance, equipe[index].moisNaissance, equipe[index].anneeNaissance));
			setColor(COLOR_GREEN);
			printf("Age modifie avec succes! Nouvel age: %d ans\n",
			       calculerAge(equipe[index].jourNaissance, equipe[index].moisNaissance, equipe[index].anneeNaissance));
			setColor(COLOR_WHITE);
			break;

		case 3:
			equipe[index].buts = saisirEntierPositif("Nouveau nombre de buts: ");
			setColor(COLOR_GREEN);
			printf("Nombre de buts modifie avec succes!\n");
			setColor(COLOR_WHITE);
			if (equipe[index].buts >= 10) {
				printf("Felicitations! %s %s est maintenant une STAR de l'equipe!\n",
				       equipe[index].prenom, equipe[index].nom);
			}
			break;
		case 4:
			printf("Sortie de la modification.\n");
			break;
		case 5: {
			afficherMenuStatut();
			int choixStatut = saisirChoixNumerique(1, 2, "Votre choix (1-2): ");
			const char* statuts[] = {"", "titulaire", "remplacant"};
			if (strcmp(statuts[choixStatut], "titulaire") == 0 && strcmp(equipe[index].statut, "titulaire") != 0 && countTitulaires() >= 11) {
				printError("Erreur: Vous ne pouvez pas avoir plus de 11 titulaires.\n");
				break;
			}
			strcpy(equipe[index].statut, statuts[choixStatut]);
			setColor(COLOR_GREEN);
			printf("Statut modifie avec succes!\n");
			setColor(COLOR_WHITE);
			break;
		}
		default:
			printError("Choix invalide.\n");
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
		printError("Joueur avec l'ID %d non trouve.\n", id);
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
		setColor(COLOR_GREEN);
		printf("[SUCCES]Joueur supprime !\n");
		setColor(COLOR_WHITE);
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
	setColor(COLOR_CYAN);
	printf("\n=== RECHERCHE DE JOUEUR ===\n");
	setColor(COLOR_GREEN);
	printf("1. "); setColor(COLOR_WHITE); printf("Rechercher par ID\n");
	setColor(COLOR_GREEN);
	printf("2. "); setColor(COLOR_WHITE); printf("Rechercher par nom\n");
	setColor(COLOR_CYAN);
	printf("Votre choix: ");
	setColor(COLOR_WHITE);
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
					printf("ID: %d | %s %s | Maillot: %d | Poste: %s | Age: %d ans | Buts: %d | Statut: %s | Naissance: %02d/%02d/%04d | Inscrit le: %s\n",
					       equipe[i].id, equipe[i].nom, equipe[i].prenom, equipe[i].numeroMaillot,
					       equipe[i].poste, calculerAge(equipe[i].jourNaissance, equipe[i].moisNaissance, equipe[i].anneeNaissance),
					       equipe[i].buts, equipe[i].statut,
					       equipe[i].jourNaissance, equipe[i].moisNaissance, equipe[i].anneeNaissance,
					       equipe[i].dateInscription);
					trouve = 1;
					break;
				}
			}
			if (!trouve) {
				printError("Aucun joueur trouve avec l'ID %d.\n", id);
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
					printf("ID: %d | %s %s | Maillot: %d | Poste: %s | Age: %d ans | Buts: %d | Statut: %s | Naissance: %02d/%02d/%04d\n",
					       equipe[i].id, equipe[i].nom, equipe[i].prenom, equipe[i].numeroMaillot,
					       equipe[i].poste, calculerAge(equipe[i].jourNaissance, equipe[i].moisNaissance, equipe[i].anneeNaissance),
					       equipe[i].buts, equipe[i].statut,
					       equipe[i].jourNaissance, equipe[i].moisNaissance, equipe[i].anneeNaissance);
					trouve = 1;
				}
			}
			if (!trouve) {
				printError("Aucun joueur trouve avec le nom %s.\n", nomRecherche);
			}
			break;
		}

		default:
			printError("Choix invalide.\n");
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
	setColor(COLOR_YELLOW);
	printf("Nombre total de joueurs: ");
	setColor(COLOR_WHITE);
	printf("%d\n", nombreJoueurs);

 // Age moyen
 	int sommeAges = 0;
 	for (int i = 0; i < nombreJoueurs; i++) {
 		sommeAges += calculerAge(equipe[i].jourNaissance, equipe[i].moisNaissance, equipe[i].anneeNaissance);
 	}
 	double ageMoyen = (double)sommeAges / nombreJoueurs;
	setColor(COLOR_YELLOW);
	printf("Age moyen des joueurs: ");
	setColor(COLOR_WHITE);
	printf("%.1f ans\n", ageMoyen);

 // Joueurs ayant marque plus de X buts
	int seuilButs = saisirEntierPositif("Afficher les joueurs ayant marque plus de combien de buts : ");

	setColor(COLOR_CYAN);
	printf("\nJoueurs ayant marque plus de ");
	setColor(COLOR_WHITE);
	printf("%d ", seuilButs);
	setColor(COLOR_CYAN);
	printf("buts:\n");
	setColor(COLOR_WHITE);
 	int trouve = 0;
 	for (int i = 0; i < nombreJoueurs; i++) {
 		if (equipe[i].buts > seuilButs) {
			setColor(COLOR_WHITE);
			printf("- %s %s: ", equipe[i].nom, equipe[i].prenom);
			setColor(COLOR_GREEN);
			printf("%d ", equipe[i].buts);
			setColor(COLOR_WHITE);
			printf("buts\n");
 			trouve = 1;
 		}
 	}
 	if (!trouve) {
		setColor(COLOR_GRAY);
		printf("Aucun joueur n'a marque plus de ");
		setColor(COLOR_WHITE);
		printf("%d ", seuilButs);
		setColor(COLOR_GRAY);
		printf("buts.\n");
		setColor(COLOR_WHITE);
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
		printf("\nMeilleur buteur: ");
		setColor(COLOR_WHITE);
		printf("%s %s avec ", equipe[indexMeilleurButeur].nom, equipe[indexMeilleurButeur].prenom);
		setColor(COLOR_GREEN);
		printf("%d ", maxButs);
		setColor(COLOR_WHITE);
		printf("buts\n");
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
		printf("Joueur le plus jeune: ");
		setColor(COLOR_WHITE);
		printf("%s %s (", equipe[indexPlusJeune].nom, equipe[indexPlusJeune].prenom);
		setColor(COLOR_GREEN);
		printf("%d ", ageMin);
		setColor(COLOR_WHITE);
		printf("ans)\n");
 	}

 	if (indexPlusAge != -1) {
 		setColor(COLOR_BLUE);
		printf("Joueur le plus age: ");
		setColor(COLOR_WHITE);
		printf("%s %s (", equipe[indexPlusAge].nom, equipe[indexPlusAge].prenom);
		setColor(COLOR_GREEN);
		printf("%d ", ageMax);
		setColor(COLOR_WHITE);
		printf("ans)\n");
 	}
 }

// Fonction pour calculer l'age base sur la date de naissance
int calculerAge(int jour, int mois, int annee) {
	time_t maintenant = time(NULL);
	struct tm* tm_info = localtime(&maintenant);

	int age = tm_info->tm_year + 1900 - annee;

	return age;
}
// Fonction pour valider une date
int validerDate(int jour, int mois, int annee) {
	if (annee < 1900 || annee > 2024)
	 return 0;
	if (mois < 1 || mois > 12)
	return 0;
	if (jour < 1)
	return 0;
// Verifier que l'age est entre 17 et 40 ans
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

// Fonctions d'aide pour l'optimisation du code
int validerNom(char* nom, const char* type) {
	if (strlen(nom) == 0) {
		printError("Erreur: Le %s ne peut pas etre vide.\n", type);
		return 0;
	}
	for (size_t i = 0; i < strlen(nom); i++) {
		if (isdigit((unsigned char)nom[i])) {
			printError("Erreur: Le %s ne doit pas contenir de chiffres.\n", type);
			return 0;
		}
	}
	return 1;
}

void saisirNom(char* nom, const char* type) {
	do {
		printf("%s: ", type);
		scanf("%49s", nom);
	} while (!validerNom(nom, type));
}

int saisirChoixNumerique(int min, int max, const char* message) {
	int choix = 0;
	do {
		printf("%s", message);
		while (scanf("%d", &choix) != 1) {
			printError("Erreur: Entrez uniquement des chiffres (%d-%d).\n", min, max);
			while (getchar() != '\n');
			printf("%s", message);
		}
		if (choix < min || choix > max) {
			printError("Erreur: Choix invalide. Choisissez entre %d et %d.\n", min, max);
		}
	} while (choix < min || choix > max);
	return choix;
}

void afficherMenuPoste() {
	printf("Choisissez le poste de joueur:\n");
	setColor(COLOR_GREEN); printf("  1. "); setColor(COLOR_WHITE); printf("gardien\n");
	setColor(COLOR_GREEN); printf("  2. "); setColor(COLOR_WHITE); printf("defenseur\n");
	setColor(COLOR_GREEN); printf("  3. "); setColor(COLOR_WHITE); printf("milieu\n");
	setColor(COLOR_GREEN); printf("  4. "); setColor(COLOR_WHITE); printf("attaquant\n");
}

void afficherMenuStatut() {
	printf("Choisissez le statut:\n");
	setColor(COLOR_GREEN); printf("  1. "); setColor(COLOR_WHITE); printf("titulaire\n");
	setColor(COLOR_GREEN); printf("  2. "); setColor(COLOR_WHITE); printf("remplacant\n");
}

// Fonctions d'aide avancées pour une optimisation supplémentaire
void afficherEnTeteTableau() {
	setColor(COLOR_YELLOW);
	printf("ID  | Nom            | Prenom         | Maillot | Poste      | Age  | Buts  | Statut      | Inscrit le\n");
	setColor(COLOR_GRAY);
	printf("----|----------------|----------------|---------|------------|------|-------|-------------|----------\n");
	setColor(COLOR_WHITE);
}

void afficherJoueurLigne(int index) {
	printf("%3d | %-14s | %-14s | %7d | %-10s | %3d | %4d | %-10s | %s\n",
	       equipe[index].id, equipe[index].nom, equipe[index].prenom, equipe[index].numeroMaillot,
	       equipe[index].poste, calculerAge(equipe[index].jourNaissance, equipe[index].moisNaissance, equipe[index].anneeNaissance),
	       equipe[index].buts, equipe[index].statut, equipe[index].dateInscription);
}

int saisirEntierPositif(const char* message) {
	int valeur = -1;
	do {
		char input[32];
		printf("%s", message);
		if (scanf("%31s", input) != 1) {
			while (getchar() != '\n');
			printError("Erreur: Entrez uniquement des chiffres.\n");
			continue;
		}
		if (!isAllDigits(input)) {
			printError("Erreur: Entrez uniquement des chiffres.\n");
			continue;
		}
		valeur = atoi(input);
		if (valeur <= 0) {
			printError("Erreur: La valeur doit etre positive.\n");
		}
	} while (valeur <= 0);
	return valeur;
}

// Fonction de tri générique
void trierEquipe(int (*comparer)(const void*, const void*)) {
	for (int i = 0; i < nombreJoueurs - 1; i++) {
		int minIndex = i;
		for (int j = i + 1; j < nombreJoueurs; j++) {
			if (comparer(&equipe[j], &equipe[minIndex]) < 0) {
				minIndex = j;
			}
		}
		if (minIndex != i) {
			Joueur temp = equipe[i];
			equipe[i] = equipe[minIndex];
			equipe[minIndex] = temp;
		}
	}
}

// Fonctions de comparaison pour le tri
int comparerParNom(const void* a, const void* b) {
	Joueur* joueurA = (Joueur*)a;
	Joueur* joueurB = (Joueur*)b;
	return strcmp(joueurA->nom, joueurB->nom);
}

int comparerParAge(const void* a, const void* b) {
	Joueur* joueurA = (Joueur*)a;
	Joueur* joueurB = (Joueur*)b;
	int ageA = calculerAge(joueurA->jourNaissance, joueurA->moisNaissance, joueurA->anneeNaissance);
	int ageB = calculerAge(joueurB->jourNaissance, joueurB->moisNaissance, joueurB->anneeNaissance);
	return ageA - ageB;
}

int comparerParId(const void* a, const void* b) {
	Joueur* joueurA = (Joueur*)a;
	Joueur* joueurB = (Joueur*)b;
	return joueurA->id - joueurB->id;
}

void afficherJoueursParCritere(const char* titre, int (*comparer)(const void*, const void*)) {
	clearScreen();
	afficherTitre();
	trierEquipe(comparer);
	setColor(COLOR_CYAN);
	printf("\n=== %s ===\n", titre);
	afficherEnTeteTableau();
	for (int i = 0; i < nombreJoueurs; i++) {
		afficherJoueurLigne(i);
	}
}

void afficherJoueursParPoste(const char* poste) {
	setColor(COLOR_CYAN);
	printf("\n=== JOUEURS AU POSTE: %s ===\n", poste);
	setColor(COLOR_YELLOW);
	printf("ID  | Nom            | Prenom         | Maillot | Age | Buts | Statut\n");
	setColor(COLOR_GRAY);
	printf("----|----------------|----------------|---------|-----|------|--------\n");
	setColor(COLOR_WHITE);
	
	int trouve = 0;
	for (int i = 0; i < nombreJoueurs; i++) {
		if (strcmp(equipe[i].poste, poste) == 0) {
			printf("%3d | %-14s | %-14s | %7d | %3d | %4d | %s\n",
			       equipe[i].id, equipe[i].nom, equipe[i].prenom, equipe[i].numeroMaillot,
			       calculerAge(equipe[i].jourNaissance, equipe[i].moisNaissance, equipe[i].anneeNaissance),
			       equipe[i].buts, equipe[i].statut);
			trouve = 1;
		}
	}
	if (!trouve) {
		printError("Aucun joueur trouve au poste %s.\n", poste);
	}
}