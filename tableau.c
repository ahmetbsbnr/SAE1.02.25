/**
 * @file tableau.c
 * @brief MÉMORYX - Fonctions de gestion du tableau/plateau
 * @description Initialisation, génération, Joker, paires
 */

#include "memoryx.h"

/* ============================================================
   INITIALISATION ET MÉMOIRE
   ============================================================ */

void preparer_jeu(Jeu *jeu, int L, int C) {
    /* Vérification des contraintes : L et C impairs */
    if (L % 2 == 0 || C % 2 == 0) {
        printf(RED "Erreur : L et C doivent etre impairs.\n" RESET);
        exit(EXIT_FAILURE);
    }
    /* Cas L=C=1 interdit */
    if (L == 1 && C == 1) {
        printf(RED "Erreur : L=C=1 interdit.\n" RESET);
        exit(EXIT_FAILURE);
    }
    /* Produit minimum */
    if (L * C < 3) {
        printf(RED "Erreur : L*C doit etre >= 3.\n" RESET);
        exit(EXIT_FAILURE);
    }

    jeu->L = L;
    jeu->C = C;
    jeu->R = L * C;  /* Taille logique initiale = taille totale */
    jeu->coups = 0;
    jeu->tour = 0;
    jeu->tempsDebut = time(NULL);

    /* Allocation dynamique de T et P */
    jeu->T = (int *)malloc(jeu->L * jeu->C * sizeof(int));
    jeu->P = (int *)malloc(jeu->L * jeu->C * sizeof(int));

    if (jeu->T == NULL || jeu->P == NULL) {
        printf(RED "Erreur d'allocation memoire.\n" RESET);
        exit(EXIT_FAILURE);
    }

    /* Initialisation mémoire du Bot */
    bot_initialiser(&jeu->bot, jeu->L * jeu->C);
}

void initialiser_plateau(Jeu *jeu) {
    int taille_totale = jeu->L * jeu->C;
    int i, j, temp;
    int valeur_carte;

    /* 1. Remplir P tel que P[i] = i */
    for (i = 0; i < taille_totale; i++) {
        jeu->P[i] = i;
    }

    /* 2. Mélange de P avec Fisher-Yates */
    for (i = taille_totale - 1; i > 0; i--) {
        j = rand() % (i + 1);
        temp = jeu->P[i];
        jeu->P[i] = jeu->P[j];
        jeu->P[j] = temp;
    }

    /* 3. Initialiser T à CARTE_RETIREE */
    for (i = 0; i < taille_totale; i++) {
        jeu->T[i] = CARTE_RETIREE;
    }

    /* 4. Placer le Joker (valeur 0) en T[P[0]] */
    jeu->T[jeu->P[0]] = JOKER;

    /* 5. Placer les paires (valeurs 1 à n) */
    valeur_carte = 1;
    for (i = 1; i < taille_totale; i += 2) {
        jeu->T[jeu->P[i]] = valeur_carte;
        jeu->T[jeu->P[i + 1]] = valeur_carte;
        valeur_carte++;
    }

    /* 6. Réinitialiser P pour contenir toutes les positions (0 à R-1) */
    for (i = 0; i < taille_totale; i++) {
        jeu->P[i] = i;
    }
}

void liberer_jeu(Jeu *jeu) {
    if (jeu->T) {
        free(jeu->T);
        jeu->T = NULL;
    }
    if (jeu->P) {
        free(jeu->P);
        jeu->P = NULL;
    }
    if (jeu->bot.memoire) {
        free(jeu->bot.memoire);
        jeu->bot.memoire = NULL;
    }
}

/* ============================================================
   CONVERSION COORDONNÉES (Section 3)
   ============================================================ */

/* Convertit k (index T) en i (ligne) et j (colonne) */
void convertir_k_en_ij(int k, int C, int *i, int *j) {
    *i = k / C;  /* k div C */
    *j = k % C;  /* k mod C */
}

/* Convertit i et j en index k unique */
int convertir_ij_en_k(int i, int j, int C) {
    return i * C + j;
}

/* ============================================================
   LOGIQUE DU JEU (Sections 5 et 6)
   ============================================================ */

/* Vérifie si une position est toujours dans P (cartes restantes) */
int est_dans_P(Jeu *jeu, int pos) {
    int i;
    for (i = 0; i < jeu->R; i++) {
        if (jeu->P[i] == pos) return 1;
    }
    return 0;
}

/**
 * Gestion du Joker (Section 6 du sujet)
 * - Le joueur passe son tour
 * - Tirage r aléatoire dans [0, R-1]
 * - Permutation de T[positionJoker] avec T[P[r]]
 * - P reste INCHANGÉ
 */
void gestion_joker(Jeu *jeu, int positionJoker) {
    int index_aleatoire = rand() % jeu->R;
    int position_aleatoire = jeu->P[index_aleatoire];
    int temp;

    /* Permutation T[positionJoker] <-> T[P[r]] */
    temp = jeu->T[positionJoker];
    jeu->T[positionJoker] = jeu->T[position_aleatoire];
    jeu->T[position_aleatoire] = temp;

    printf(MAGENTA ">>> Le Joker a ete deplace ! <<<\n" RESET);

    /* Le bot doit "oublier" les DEUX positions affectées par la permutation */
    bot_oublier_carte(&jeu->bot, positionJoker);
    bot_oublier_carte(&jeu->bot, position_aleatoire);
}

/**
 * Vérifie si les deux positions forment une paire
 * Si oui : met à jour T (valeurs à -1) et P (retire les positions)
 */
int verifier_paire(Jeu *jeu, int p1, int p2) {
    int i;

    if (jeu->T[p1] == jeu->T[p2]) {
        /* Mettre à jour T : cartes retirées */
        jeu->T[p1] = CARTE_RETIREE;
        jeu->T[p2] = CARTE_RETIREE;

        /* Mettre à jour P : retirer p1 et p2 */
        i = 0;
        while (i < jeu->R) {
            if (jeu->P[i] == p1 || jeu->P[i] == p2) {
                jeu->P[i] = jeu->P[jeu->R - 1];
                jeu->R--;
            } else {
                i++;
            }
        }

        /* Le bot oublie cette paire */
        bot_oublier_carte(&jeu->bot, p1);
        bot_oublier_carte(&jeu->bot, p2);

        return 1;  /* Paire trouvée */
    }
    return 0;  /* Pas de paire */
}
