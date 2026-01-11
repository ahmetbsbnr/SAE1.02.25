/**
 * @file fonctions.h
 * @brief MÉMORYX - SAE 1.02 - IUT Metz
 */

#ifndef FONCTIONS_H
#define FONCTIONS_H

#include <time.h>

#define JOKER 0
#define CARTE_RETIREE -1
#define MAX_CARTES 441
#define MAX_PSEUDO 50

#define BLEU     "\033[34m"
#define ROUGE    "\033[31m"
#define VERT     "\033[32m"
#define JAUNE    "\033[33m"
#define RESET    "\033[0m"

typedef struct {
    char pseudo[MAX_PSEUDO];
    int score;
    int estBot;
} Joueur;

typedef struct {
    int position;
    int valeur;
} CarteMemoire;

typedef struct {
    CarteMemoire cartes[MAX_CARTES];
    int nbCartes;
} MemoireBot;

typedef struct {
    int T[MAX_CARTES];
    int P[MAX_CARTES];
    int L, C;
    int R;
    int coups;
    int tour;
    time_t tempsDebut;
    Joueur joueurs[2];
    int nbJoueurs;
    int joueurActuel;
    MemoireBot memoire;
} Partie;

void initialiserAleatoire(void);
int aleatoire(int min, int max);
void melangerTableau(int t[], int taille);
int dimensionsValides(int L, int C);

void initialiserPlateau(Partie *p, int L, int C);
void afficherTableauRetourne(Partie *p);
void afficherPlateau(Partie *p, int pos1, int pos2);
int positionValide(Partie *p, int pos);
void permuterJoker(Partie *p, int posJoker);
void retirerPaire(Partie *p, int pos1, int pos2);
int partieTerminee(Partie *p);

void configurerJoueurs(Partie *p, int modeDuel);
void joueurSuivant(Partie *p);
void afficherScores(Partie *p);
void afficherGagnant(Partie *p);
int saisirPosition(Partie *p);

void initialiserMemoire(MemoireBot *m);
void memoriserCarte(MemoireBot *m, int pos, int val);
void oublierCarte(MemoireBot *m, int pos);
int chercherPaire(MemoireBot *m, int *p1, int *p2);
int chercherValeur(MemoireBot *m, int val, int exclue);
int tourBot(Partie *p);

#endif
