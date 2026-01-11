/**
 * @file memoryx.h
 * @brief MÉMORYX - SAE 1.02 - IUT Metz - VERSION FINALE
 * @description Header principal avec toutes les structures et prototypes
 */

#ifndef MEMORYX_H
#define MEMORYX_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/* ==================== COULEURS ANSI ==================== */
#define RESET   "\033[0m"
#define RED     "\033[1;31m"
#define GREEN   "\033[1;32m"
#define YELLOW  "\033[1;33m"
#define BLUE    "\033[1;34m"
#define CYAN    "\033[1;36m"
#define MAGENTA "\033[1;35m"
#define BOLD    "\033[1m"

/* ==================== CONSTANTES ==================== */
#define JOKER 0
#define CARTE_RETIREE -1
#define MAX_PSEUDO 50
#define MAX_JOUEURS 2

/* ==================== STRUCTURES ==================== */

/**
 * @brief Structure pour la mémoire du bot (carte vue)
 */
typedef struct {
    int position;   // Position de la carte sur le plateau
    int valeur;     // Valeur de la carte
    int estValide;  // 1 si l'info est valide, 0 sinon
} CarteConnue;

/**
 * @brief Structure pour la mémoire complète du bot
 */
typedef struct {
    CarteConnue *memoire;  // Tableau dynamique des cartes mémorisées
    int capaciteMax;       // Taille max du tableau
} MemoireBot;

/**
 * @brief Structure d'un joueur
 */
typedef struct {
    char pseudo[MAX_PSEUDO];
    int score;
    int estBot;  // 1 si bot, 0 si humain
} Joueur;

/**
 * @brief Structure principale du jeu
 */
typedef struct {
    int L;                          // Nombre de lignes
    int C;                          // Nombre de colonnes
    int R;                          // Taille logique de P (cartes restantes)
    int *T;                         // Table des cartes (taille L*C)
    int *P;                         // Table des positions restantes
    Joueur joueurs[MAX_JOUEURS];    // Les 2 joueurs
    int joueurActuel;               // Indice du joueur actuel (0 ou 1)
    int nbJoueurs;                  // 1 pour solitaire, 2 pour duel
    MemoireBot bot;                 // Mémoire du bot
    int coups;                      // Compteur de coups (solitaire)
    int tour;                       // Numéro du tour
    time_t tempsDebut;              // Temps de début (solitaire)
} Jeu;

/* ==================== PROTOTYPES ==================== */

/* --- Initialisation et libération --- */
void preparer_jeu(Jeu *jeu, int L, int C);
void initialiser_plateau(Jeu *jeu);
void liberer_jeu(Jeu *jeu);

/* --- Conversion coordonnées (Section 3 du sujet) --- */
void convertir_k_en_ij(int k, int C, int *i, int *j);
int convertir_ij_en_k(int i, int j, int C);

/* --- Affichage --- */
void afficher_banniere(void);
void afficher_plateau(Jeu *jeu, int pos1, int pos2);
void afficher_plateau_triche(Jeu *jeu);
void afficher_scores(Jeu *jeu, int modeSolitaire);
void afficher_fin_partie(Jeu *jeu, int modeSolitaire);

/* --- Logique du jeu (Sections 5 et 6) --- */
void gestion_joker(Jeu *jeu, int positionJoker);
int verifier_paire(Jeu *jeu, int p1, int p2);
int est_dans_P(Jeu *jeu, int pos);

/* --- Saisie utilisateur --- */
int saisir_position(Jeu *jeu);
void vider_buffer(void);

/* --- Intelligence Artificielle (Section 8) --- */
void bot_initialiser(MemoireBot *mem, int capacite);
void bot_memoriser(MemoireBot *mem, int position, int valeur);
void bot_oublier_carte(MemoireBot *mem, int position);
void bot_oublier_paire(MemoireBot *mem, int valeur);
int bot_chercher_paire(MemoireBot *mem, Jeu *jeu, int *p1, int *p2);
int bot_chercher_valeur(MemoireBot *mem, Jeu *jeu, int valeur, int exclure);
void bot_jouer_tour(Jeu *jeu, int *pos1, int *pos2);

/* --- Utilitaires --- */
void attendre(int secondes);
void effacer_ecran(void);

#endif /* MEMORYX_H */
