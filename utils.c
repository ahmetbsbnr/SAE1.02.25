/**
 * @file utils.c
 * @brief MÉMORYX - Fonctions utilitaires
 * @description Saisie utilisateur, attente, effacement écran
 */

#include "memoryx.h"

#ifdef _WIN32
#include <windows.h>
void attendre(int secondes) { Sleep(secondes * 1000); }
void effacer_ecran(void) { system("cls"); }
#else
#include <unistd.h>
void attendre(int secondes) { sleep(secondes); }
void effacer_ecran(void) { system("clear"); }
#endif

/* ============================================================
   SAISIE UTILISATEUR
   ============================================================ */

void vider_buffer(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

int saisir_position(Jeu *jeu) {
    int ligne, col, k;
    int valide = 0;

    while (!valide) {
        printf("Entrez " CYAN "LIGNE COLONNE" RESET " (ex: 0 2) : ");

        if (scanf("%d %d", &ligne, &col) != 2) {
            vider_buffer();
            printf(RED "Entree invalide. Entrez deux nombres.\n" RESET);
            continue;
        }

        /* Vérification des bornes */
        if (ligne < 0 || ligne >= jeu->L || col < 0 || col >= jeu->C) {
            printf(RED "Coordonnees hors du plateau !\n" RESET);
            continue;
        }

        k = convertir_ij_en_k(ligne, col, jeu->C);

        /* Vérification carte non retirée */
        if (jeu->T[k] == CARTE_RETIREE) {
            printf(RED "Cette carte a deja ete retiree.\n" RESET);
            continue;
        }

        valide = 1;
    }
    return k;
}
