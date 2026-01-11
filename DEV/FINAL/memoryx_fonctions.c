/**
 * @file memoryx_fonctions.c
 * @brief MÉMORYX - SAE 1.02 - IUT Metz - VERSION FINALE
 * @description Implémentation de toutes les fonctions du jeu
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
   PARTIE A : INITIALISATION ET MÉMOIRE
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
   PARTIE B : CONVERSION COORDONNÉES (Section 3)
   ============================================================ */

void convertir_k_en_ij(int k, int C, int *i, int *j) {
    *i = k / C;
    *j = k % C;
}

int convertir_ij_en_k(int i, int j, int C) {
    return i * C + j;
}

/* ============================================================
   PARTIE C : AFFICHAGE
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
                printf("    ");
            }
            else if (k == pos1 || k == pos2) {
                if (jeu->T[k] == JOKER) {
                    printf(RED " JK " RESET);
                } else {
                    printf(YELLOW "%3d " RESET, jeu->T[k]);
                }
            }
            else {
                /* Carte cachée : affiche la position */
                printf("%3d ", k);
            }
        }
        printf("|\n");
    }

    printf("     +");
    for (j = 0; j < jeu->C; j++) printf("----");
    printf("+\n");
}

void afficher_plateau_triche(Jeu *jeu) {
    int i, j, k;

    printf("\n");
    printf(RED "      *** MODE TRICHE ***\n" RESET);

    printf("      ");
    for (j = 0; j < jeu->C; j++) {
        printf(CYAN "%3d " RESET, j);
    }
    printf("\n");

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

void afficher_scores(Jeu *jeu, int modeSolitaire) {
    int n = (jeu->L * jeu->C - 1) / 2;
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

/* ============================================================
   PARTIE D : LOGIQUE DU JEU (Sections 5 et 6)
   ============================================================ */

int est_dans_P(Jeu *jeu, int pos) {
    int i;
    for (i = 0; i < jeu->R; i++) {
        if (jeu->P[i] == pos) return 1;
    }
    return 0;
}

void gestion_joker(Jeu *jeu, int positionJoker) {
    int index_aleatoire = rand() % jeu->R;
    int position_aleatoire = jeu->P[index_aleatoire];
    int temp;

    temp = jeu->T[positionJoker];
    jeu->T[positionJoker] = jeu->T[position_aleatoire];
    jeu->T[position_aleatoire] = temp;

    printf(MAGENTA ">>> Le Joker a ete deplace ! <<<\n" RESET);

    bot_oublier_carte(&jeu->bot, positionJoker);
    bot_oublier_carte(&jeu->bot, position_aleatoire);
}

int verifier_paire(Jeu *jeu, int p1, int p2) {
    int i;

    if (jeu->T[p1] == jeu->T[p2]) {
        jeu->T[p1] = CARTE_RETIREE;
        jeu->T[p2] = CARTE_RETIREE;

        i = 0;
        while (i < jeu->R) {
            if (jeu->P[i] == p1 || jeu->P[i] == p2) {
                jeu->P[i] = jeu->P[jeu->R - 1];
                jeu->R--;
            } else {
                i++;
            }
        }

        bot_oublier_carte(&jeu->bot, p1);
        bot_oublier_carte(&jeu->bot, p2);

        return 1;
    }
    return 0;
}

/* ============================================================
   PARTIE E : SAISIE UTILISATEUR
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

        if (ligne < 0 || ligne >= jeu->L || col < 0 || col >= jeu->C) {
            printf(RED "Coordonnees hors du plateau !\n" RESET);
            continue;
        }

        k = convertir_ij_en_k(ligne, col, jeu->C);

        if (jeu->T[k] == CARTE_RETIREE) {
            printf(RED "Cette carte a deja ete retiree.\n" RESET);
            continue;
        }

        valide = 1;
    }
    return k;
}

/* ============================================================
   PARTIE F : INTELLIGENCE ARTIFICIELLE (Section 8)
   ============================================================ */

void bot_initialiser(MemoireBot *mem, int capacite) {
    int i;
    mem->capaciteMax = capacite;
    mem->memoire = (CarteConnue *)malloc(capacite * sizeof(CarteConnue));

    if (mem->memoire == NULL) {
        printf(RED "Erreur d'allocation memoire pour le bot.\n" RESET);
        exit(EXIT_FAILURE);
    }

    for (i = 0; i < capacite; i++) {
        mem->memoire[i].position = i;
        mem->memoire[i].valeur = -1;
        mem->memoire[i].estValide = 0;
    }
}

void bot_memoriser(MemoireBot *mem, int position, int valeur) {
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

void bot_jouer_tour(Jeu *jeu, int *pos1, int *pos2) {
    int idx1, idx2;
    int valeur1;

    if (bot_chercher_paire(&jeu->bot, jeu, pos1, pos2)) {
        printf(YELLOW "Bot: J'ai trouve une paire en memoire !\n" RESET);
        return;
    }

    idx1 = rand() % jeu->R;
    *pos1 = jeu->P[idx1];

    valeur1 = jeu->T[*pos1];

    if (valeur1 == JOKER) {
        *pos2 = -1;
        return;
    }

    *pos2 = bot_chercher_valeur(&jeu->bot, jeu, valeur1, *pos1);

    if (*pos2 != -1) {
        printf(YELLOW "Bot: Je connais la paire de cette carte !\n" RESET);
        return;
    }

    do {
        idx2 = rand() % jeu->R;
        *pos2 = jeu->P[idx2];
    } while (*pos1 == *pos2);
}
