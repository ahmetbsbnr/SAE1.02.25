/**
 * @file memoryx.h
 * @brief MÉMORYX - SAE 1.02 - IUT Metz - VERSION FINALE
 * @description Header principal avec structures et inclusion des modules
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
    int position;   /* Position de la carte sur le plateau */
    int valeur;     /* Valeur de la carte */
    int estValide;  /* 1 si l'info est valide, 0 sinon */
} CarteConnue;

/**
 * @brief Structure pour la mémoire complète du bot
 */
typedef struct {
    CarteConnue *memoire;  /* Tableau dynamique des cartes mémorisées */
    int capaciteMax;       /* Taille max du tableau */
} MemoireBot;

/**
 * @brief Structure d'un joueur
 */
typedef struct {
    char pseudo[MAX_PSEUDO];
    int score;
    int estBot;  /* 1 si bot, 0 si humain */
} Joueur;

/**
 * @brief Structure principale du jeu
 */
typedef struct {
    int L;                          /* Nombre de lignes */
    int C;                          /* Nombre de colonnes */
    int R;                          /* Taille logique de P (cartes restantes) */
    int *T;                         /* Table des cartes (taille L*C) */
    int *P;                         /* Table des positions restantes */
    Joueur joueurs[MAX_JOUEURS];    /* Les 2 joueurs */
    int joueurActuel;               /* Indice du joueur actuel (0 ou 1) */
    int nbJoueurs;                  /* 1 pour solitaire, 2 pour duel */
    MemoireBot bot;                 /* Mémoire du bot */
    int coups;                      /* Compteur de coups (solitaire) */
    int tour;                       /* Numéro du tour */
    time_t tempsDebut;              /* Temps de début (solitaire) */
} Jeu;

/* ==================== INCLUSION DES MODULES ==================== */
#include "tableau.h"
#include "bot.h"
#include "affichage.h"
#include "utils.h"

#endif /* MEMORYX_H */
