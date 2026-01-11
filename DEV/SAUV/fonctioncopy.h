#ifndef FONCTIONCOPY_H
#define FONCTIONCOPY_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// --- COULEURS ET STYLE (ANSI) ---
#define RESET   "\033[0m"
#define RED     "\033[1;31m"    // Pour le Joker
#define GREEN   "\033[1;32m"    // Pour les messages de succès
#define YELLOW  "\033[1;33m"    // Pour la sélection
#define BLUE    "\033[1;34m"    // Pour le titre / menu
#define CYAN    "\033[1;36m"    // Pour les coordonnées
#define BOLD    "\033[1m"

// --- CONSTANTES ---
#define JOKER 0
#define CARTE_RETIREE -1
#define MAX_PSEUDO 50
#define MAX_JOUEURS 2

// --- STRUCTURES ---

typedef struct {
    int position;   
    int valeur;     
    int estValide;  
} CarteConnue;

typedef struct {
    CarteConnue *memoire; 
    int capaciteMax;      
} MemoireBot;

typedef struct {
    char pseudo[MAX_PSEUDO];
    int score;
    int estBot; 
} Joueur;

typedef struct {
    int L;               
    int C;               
    int R;               
    int *T;              
    int *P;              
    Joueur joueurs[MAX_JOUEURS]; 
    int joueurActuel;    
    MemoireBot bot;      
} Jeu;

// --- PROTOTYPES ---

void preparer_jeu(Jeu *jeu, int L, int C);
void initialiser_plateau(Jeu *jeu);
void liberer_jeu(Jeu *jeu); 

// Utilitaires conversion
void convertir_k_en_ij(int k, int C, int *i, int *j);
int convertir_ij_en_k(int i, int j, int C);

// Affichage amélioré
void afficher_banniere();
void afficher_plateau(Jeu *jeu, int posRévélée1, int posRévélée2);

// Logique Jeu
void gestion_joker(Jeu *jeu, int positionJoker);
int verifier_paire(Jeu *jeu, int p1, int p2);

// Saisie en Ligne/Colonne
int saisir_position_coordonnees(Jeu *jeu);

// IA
void bot_memoriser(MemoireBot *mem, int position, int valeur);
void bot_oublier_paire(MemoireBot *mem, int val);
void bot_jouer_tour(Jeu *jeu, MemoireBot *mem, int *pos1, int *pos2);
int est_dans_P(Jeu *jeu, int pos);

#endif // FONCTIONCOPY_H
