/**
 * @file bot.c
 * @brief MÉMORYX - Intelligence Artificielle du Bot
 * @description Mémoire et stratégie du bot (Section 8 du sujet)
 */

#include "memoryx.h"

/* ============================================================
   GESTION DE LA MÉMOIRE DU BOT
   ============================================================ */

void bot_initialiser(MemoireBot *mem, int capacite) {
    int i;
    mem->capaciteMax = capacite;
    mem->memoire = (CarteConnue *)malloc(capacite * sizeof(CarteConnue));

    if (mem->memoire == NULL) {
        printf(RED "Erreur d'allocation memoire pour le bot.\n" RESET);
        exit(EXIT_FAILURE);
    }

    /* Initialiser toutes les entrées comme invalides */
    for (i = 0; i < capacite; i++) {
        mem->memoire[i].position = i;
        mem->memoire[i].valeur = -1;
        mem->memoire[i].estValide = 0;
    }
}

void bot_memoriser(MemoireBot *mem, int position, int valeur) {
    /* Le bot ne mémorise pas le Joker (trop volatile) */
    if (valeur == JOKER) return;

    if (position >= 0 && position < mem->capaciteMax) {
        mem->memoire[position].position = position;
        mem->memoire[position].valeur = valeur;
        mem->memoire[position].estValide = 1;
    }
}

void bot_oublier_carte(MemoireBot *mem, int position) {
    if (position >= 0 && position < mem->capaciteMax) {
        mem->memoire[position].estValide = 0;
    }
}

void bot_oublier_paire(MemoireBot *mem, int valeur) {
    int i;
    for (i = 0; i < mem->capaciteMax; i++) {
        if (mem->memoire[i].valeur == valeur) {
            mem->memoire[i].estValide = 0;
        }
    }
}

/* ============================================================
   STRATÉGIE DU BOT
   ============================================================ */

/**
 * Cherche une paire connue en mémoire
 * Retourne 1 si trouvée, 0 sinon
 */
int bot_chercher_paire(MemoireBot *mem, Jeu *jeu, int *p1, int *p2) {
    int i, j;

    for (i = 0; i < mem->capaciteMax; i++) {
        if (mem->memoire[i].estValide && est_dans_P(jeu, i)) {
            for (j = i + 1; j < mem->capaciteMax; j++) {
                if (mem->memoire[j].estValide && est_dans_P(jeu, j)) {
                    if (mem->memoire[i].valeur == mem->memoire[j].valeur) {
                        *p1 = i;
                        *p2 = j;
                        return 1;
                    }
                }
            }
        }
    }
    return 0;
}

/**
 * Cherche une position connue avec une valeur donnée
 * Retourne la position ou -1 si non trouvée
 */
int bot_chercher_valeur(MemoireBot *mem, Jeu *jeu, int valeur, int exclure) {
    int i;
    for (i = 0; i < mem->capaciteMax; i++) {
        if (mem->memoire[i].estValide &&
            mem->memoire[i].valeur == valeur &&
            i != exclure &&
            est_dans_P(jeu, i)) {
            return i;
        }
    }
    return -1;
}

/**
 * Le bot joue son tour
 * Stratégie :
 * 1. Chercher une paire connue en mémoire
 * 2. Sinon, tirer une carte au hasard
 * 3. Après première carte, chercher si on connaît sa paire
 * 4. Sinon, tirer une seconde carte au hasard
 */
void bot_jouer_tour(Jeu *jeu, int *pos1, int *pos2) {
    int idx1, idx2;
    int valeur1;

    /* Stratégie 1 : Chercher une paire connue */
    if (bot_chercher_paire(&jeu->bot, jeu, pos1, pos2)) {
        printf(YELLOW "Bot: J'ai trouve une paire en memoire !\n" RESET);
        return;
    }

    /* Stratégie 2 : Tirer une première carte au hasard */
    idx1 = rand() % jeu->R;
    *pos1 = jeu->P[idx1];

    /* Récupérer la valeur de la première carte */
    valeur1 = jeu->T[*pos1];

    /* Si c'est le Joker, pas besoin de chercher une deuxième carte */
    if (valeur1 == JOKER) {
        *pos2 = -1;
        return;
    }

    /* Stratégie 3 : Chercher si on connaît la paire de cette carte */
    *pos2 = bot_chercher_valeur(&jeu->bot, jeu, valeur1, *pos1);

    if (*pos2 != -1) {
        printf(YELLOW "Bot: Je connais la paire de cette carte !\n" RESET);
        return;
    }

    /* Stratégie 4 : Tirer une deuxième carte au hasard */
    do {
        idx2 = rand() % jeu->R;
        *pos2 = jeu->P[idx2];
    } while (*pos1 == *pos2);
}
