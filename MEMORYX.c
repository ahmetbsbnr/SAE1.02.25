#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Définition des constantes pour la taille maximale des tableaux
#define MAX_LIGNES 20
#define MAX_COLONNES 20
#define MAX_CARTES (MAX_LIGNES * MAX_COLONNES)

/* --- PROTOTYPES DES FONCTIONS --- */

// Convertit une position unique k (1D) en numéro de ligne i
// Source: Formule i = k div C
int getLigne(int position, int nbColonnes);

// Convertit une position unique k (1D) en numéro de colonne j
// Source: Formule j = k mod C
int getColonne(int position, int nbColonnes);

// Convertit des coordonnées (i, j) en position unique k
// Source: Formule k = i * C + j
int getPosition(int ligne, int colonne, int nbColonnes);

// Demande à l'utilisateur de saisir L et C
// Vérifie que L et C sont impairs et que L*C >= 3
void saisirDimensions(int *L, int *C);

// Mélange un tableau d'entiers (Algorithme de Fisher-Yates)
// Utilisé pour mélanger la table P des positions
void melangerTableau(int tab[], int taille);

// Prépare le jeu : remplit P, mélange P, remplit T avec les paires
// Place le Joker (0) et les paires (1, 2...) selon le mélange
void initialiserJeu(int T[], int P[], int L, int C);

// Affiche le plateau de jeu formaté en grille
// Montre la position (si cachée) ou la valeur (si visible)
void afficherPlateau(int T[], int L, int C, int c1, int c2);


/* --- FONCTION PRINCIPALE (MAIN) --- */

int main() {
    // Initialisation du générateur aléatoire
    srand(time(NULL));

    // Déclaration des variables principales
    int L, C;
    int T[MAX_CARTES]; // Table des cartes (valeurs)
    int P[MAX_CARTES]; // Table des positions (mélange)

    printf("=== PROJET MEMORYX - SAE 1.2 ===\n");

    // 1. Configuration de la partie
    saisirDimensions(&L, &C);

    // 2. Initialisation des données (P et T)
    initialiserJeu(T, P, L, C);

    // 3. Affichage initial (toutes les cartes sont cachées : -1, -1)
    printf("\nPlateau initial (Cartes faces cachees) :\n");
    afficherPlateau(T, L, C, -1, -1);

    // Note : Pour le débogage, voici la solution (table T complète)
    // Decommentez la ligne ci-dessous pour voir les réponses
    // printf("\n[DEBUG] Table T : "); for(int i=0; i<L*C; i++) printf("%d ", T[i]); printf("\n");

    printf("\nPrêt a jouer ! (La boucle de jeu sera codee a l'etape suivante)\n");
    
    return 0;
}


/* --- IMPLEMENTATION DES FONCTIONS --- */

int getLigne(int position, int nbColonnes) {
    return position / nbColonnes;
}

int getColonne(int position, int nbColonnes) {
    return position % nbColonnes;
}

int getPosition(int ligne, int colonne, int nbColonnes) {
    return (ligne * nbColonnes) + colonne;
}

void saisirDimensions(int *L, int *C) {
    do {
        printf("Saisir L (lignes) et C (colonnes) impairs (ex: 3 5) : ");
        scanf("%d %d", L, C);
        // Source : L et C doivent être impairs, cas 1x1 interdit
    } while (*L % 2 == 0 || *C % 2 == 0 || (*L * *C) < 3);
}

void melangerTableau(int tab[], int taille) {
    for (int i = 0; i < taille; i++) {
        // Echange l'élément i avec un élément aléatoire
        int j = rand() % taille;
        int temp = tab[i];
        tab[i] = tab[j];
        tab[j] = temp;
    }
}

void initialiserJeu(int T[], int P[], int L, int C) {
    int nbCartes = L * C;
    
    // 1. Remplir P avec 0, 1, 2... puis mélanger
    for (int i = 0; i < nbCartes; i++) P[i] = i;
    melangerTableau(P, nbCartes);

    // 2. Remplir T en suivant l'ordre de P
    // Le Joker (0) est en P[0]
    T[P[0]] = 0; 

    // Les autres cartes sont des paires (1, 1, 2, 2...)
    // On commence à k=1 car k=0 est le Joker
    for (int k = 1; k < nbCartes; k++) {
        T[P[k]] = (k + 1) / 2;
    }
}

void afficherPlateau(int T[], int L, int C, int c1, int c2) {
    for (int i = 0; i < L; i++) {
        for (int j = 0; j < C; j++) {
            int k = getPosition(i, j, C); // Conversion 2D -> 1D
            
            // Si la carte est celle choisie (c1 ou c2), on montre sa valeur T[k]
            if (k == c1 || k == c2) {
                printf("[%2d] ", T[k]);
            } else {
                // Sinon on montre sa position k (face cachée)
                printf(" %2d  ", k);
            }
        }
        printf("\n"); // Saut de ligne après chaque rangée
    }
}