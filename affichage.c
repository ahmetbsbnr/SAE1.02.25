/**
 * @file affichage.c
 * @brief MÉMORYX - Fonctions d'affichage
 * @description Plateau, scores, bannière, fin de partie
 */

#include "memoryx.h"

/* ============================================================
   BANNIÈRE
   ============================================================ */

void afficher_banniere(void) {
    printf(BLUE);
    printf(" __  __  ______  __  __   ____   _____  __   __ __   __\n");
    printf("|  \\/  ||  ____||  \\/  | / __ \\ |  __ \\ \\ \\ / / \\ \\ / /\n");
    printf("| \\  / || |__   | \\  / || |  | || |__) | \\ V /   \\ V / \n");
    printf("| |\\/| ||  __|  | |\\/| || |  | ||  _  /   | |     > <  \n");
    printf("| |  | || |____ | |  | || |__| || | \\ \\   | |    / . \\ \n");
    printf("|_|  |_||______||_|  |_| \\____/ |_|  \\_\\  |_|   /_/ \\_\\\n");
    printf(RESET);
    printf("              SAE 1.02 - IUT DE METZ\n");
    printf("                 VERSION FINALE\n\n");
}

/* ============================================================
   AFFICHAGE DU PLATEAU
   ============================================================ */

void afficher_plateau(Jeu *jeu, int pos1, int pos2) {
    int i, j, k;

    printf("\n");

    /* En-tête des colonnes */
    printf("      ");
    for (j = 0; j < jeu->C; j++) {
        printf(CYAN "%3d " RESET, j);
    }
    printf("\n");

    /* Ligne de séparation */
    printf("     +");
    for (j = 0; j < jeu->C; j++) printf("----");
    printf("+\n");

    for (i = 0; i < jeu->L; i++) {
        printf(CYAN "  %2d " RESET "|", i);

        for (j = 0; j < jeu->C; j++) {
            k = convertir_ij_en_k(i, j, jeu->C);

            if (jeu->T[k] == CARTE_RETIREE) {
                printf("    ");  /* Carte retirée = vide */
            }
            else if (k == pos1 || k == pos2) {
                /* Carte révélée ce tour */
                if (jeu->T[k] == JOKER) {
                    printf(RED " JK " RESET);
                } else {
                    printf(YELLOW "%3d " RESET, jeu->T[k]);
                }
            }
            else {
                /* Carte cachée : affiche la position (conforme Table 2 du sujet) */
                printf("%3d ", k);
            }
        }
        printf("|\n");
    }

    /* Ligne de fin */
    printf("     +");
    for (j = 0; j < jeu->C; j++) printf("----");
    printf("+\n");
}

void afficher_plateau_triche(Jeu *jeu) {
    int i, j, k;

    printf("\n");
    printf(RED "      *** MODE TRICHE ***\n" RESET);

    /* En-tête des colonnes */
    printf("      ");
    for (j = 0; j < jeu->C; j++) {
        printf(CYAN "%3d " RESET, j);
    }
    printf("\n");

    /* Ligne de séparation */
    printf("     +");
    for (j = 0; j < jeu->C; j++) printf("----");
    printf("+\n");

    for (i = 0; i < jeu->L; i++) {
        printf(CYAN "  %2d " RESET "|", i);

        for (j = 0; j < jeu->C; j++) {
            k = convertir_ij_en_k(i, j, jeu->C);

            if (jeu->T[k] == CARTE_RETIREE) {
                printf("    ");
            }
            else if (jeu->T[k] == JOKER) {
                printf(RED " JK " RESET);
            }
            else {
                printf(YELLOW "%3d " RESET, jeu->T[k]);
            }
        }
        printf("|\n");
    }

    printf("     +");
    for (j = 0; j < jeu->C; j++) printf("----");
    printf("+\n");
}

/* ============================================================
   AFFICHAGE DES SCORES
   ============================================================ */

void afficher_scores(Jeu *jeu, int modeSolitaire) {
    int n = (jeu->L * jeu->C - 1) / 2;  /* Nombre de paires */
    int tempsEcoule;
    int pairesTotales;

    printf("\n--------------------------------------------------\n");

    if (modeSolitaire) {
        tempsEcoule = (int)(time(NULL) - jeu->tempsDebut);
        printf(" Mode Solitaire | %s | Score: %d/%d\n",
               jeu->joueurs[0].pseudo, jeu->joueurs[0].score, n);
        printf(" Coups: %d | Temps: %02d:%02d\n",
               jeu->coups, tempsEcoule / 60, tempsEcoule % 60);
    } else {
        pairesTotales = jeu->joueurs[0].score + jeu->joueurs[1].score;
        printf(" %s: " GREEN "%d" RESET "  VS  %s: " GREEN "%d" RESET " | Paires: %d/%d\n",
               jeu->joueurs[0].pseudo, jeu->joueurs[0].score,
               jeu->joueurs[1].pseudo, jeu->joueurs[1].score,
               pairesTotales, n);
    }

    printf("--------------------------------------------------\n");
}

/* ============================================================
   AFFICHAGE FIN DE PARTIE
   ============================================================ */

void afficher_fin_partie(Jeu *jeu, int modeSolitaire) {
    int n = (jeu->L * jeu->C - 1) / 2;
    int tempsTotal;

    printf("\n");
    printf(BOLD "╔════════════════════════════════════════╗\n");
    printf("║          FIN DE PARTIE !               ║\n");
    printf("╚════════════════════════════════════════╝\n" RESET);
    printf("\n");

    if (modeSolitaire) {
        tempsTotal = (int)(time(NULL) - jeu->tempsDebut);
        printf("Bravo " GREEN "%s" RESET " !\n", jeu->joueurs[0].pseudo);
        printf("Paires trouvees : " YELLOW "%d/%d" RESET "\n", jeu->joueurs[0].score, n);
        printf("Nombre de coups : " YELLOW "%d" RESET "\n", jeu->coups);
        printf("Temps total     : " YELLOW "%02d:%02d" RESET "\n", tempsTotal / 60, tempsTotal % 60);
    } else {
        printf("%s : " GREEN "%d" RESET " paires\n", jeu->joueurs[0].pseudo, jeu->joueurs[0].score);
        printf("%s : " GREEN "%d" RESET " paires\n", jeu->joueurs[1].pseudo, jeu->joueurs[1].score);
        printf("\n");

        if (jeu->joueurs[0].score > jeu->joueurs[1].score) {
            printf(GREEN "*** %s GAGNE ! ***\n" RESET, jeu->joueurs[0].pseudo);
        } else if (jeu->joueurs[1].score > jeu->joueurs[0].score) {
            printf(GREEN "*** %s GAGNE ! ***\n" RESET, jeu->joueurs[1].pseudo);
        } else {
            printf(YELLOW "*** EGALITE ! ***\n" RESET);
        }
    }
    printf("\n");
}
