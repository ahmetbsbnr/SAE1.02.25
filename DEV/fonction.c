#include "fonction.h"

void afficher_banniere() {
    printf(BLUE);
    printf(" __  __  ______  __  __   ____   _____  __   __ __   __\n");
    printf("|  \\/  ||  ____||  \\/  | / __ \\ |  __ \\ \\ \\ / / \\ \\ / /\n");
    printf("| \\  / || |__   | \\  / || |  | || |__) | \\ V /   \\ V / \n");
    printf("| |\\/| ||  __|  | |\\/| || |  | ||  _  /   | |     > <  \n");
    printf("| |  | || |____ | |  | || |__| || | \\ \\   | |    / . \\ \n");
    printf("|_|  |_||______||_|  |_| \\____/ |_|  \\_\\  |_|   /_/ \\_\\\n");
    printf(RESET);
    printf("             SAE 1.2 - IUT DE METZ\n\n");
}

void preparer_jeu(Jeu *jeu, int L, int C) {
    if (L % 2 == 0 || C % 2 == 0) {
        printf("Erreur : L et C doivent etre impairs.\n");
        exit(EXIT_FAILURE);
    }
    if (L == 1 && C == 1) {
        printf("Erreur : L=C=1 interdit.\n");
        exit(EXIT_FAILURE);
    }

    jeu->L = L;
    jeu->C = C;
    jeu->R = L * C;
    
    jeu->T = (int *)malloc(jeu->L * jeu->C * sizeof(int));
    jeu->P = (int *)malloc(jeu->L * jeu->C * sizeof(int));
    
    jeu->bot.capaciteMax = jeu->L * jeu->C;
    jeu->bot.memoire = (CarteConnue *)malloc(jeu->bot.capaciteMax * sizeof(CarteConnue));

    if (jeu->T == NULL || jeu->P == NULL || jeu->bot.memoire == NULL) {
        printf("Erreur d'allocation memoire.\n");
        exit(EXIT_FAILURE);
    }

    for(int i=0; i < jeu->bot.capaciteMax; i++) {
        jeu->bot.memoire[i].estValide = 0;
        jeu->bot.memoire[i].position = i;
    }
}

void initialiser_plateau(Jeu *jeu) {
    int taille_totale = jeu->L * jeu->C;

    for (int i = 0; i < taille_totale; i++) {
        jeu->P[i] = i;
    }

    for (int i = 0; i < taille_totale - 1; i++) {
        int j = i + rand() % (taille_totale - i);
        int temp = jeu->P[i];
        jeu->P[i] = jeu->P[j];
        jeu->P[j] = temp;
    }

    jeu->T[jeu->P[0]] = 0;

    int valeur_carte = 1;
    for (int i = 1; i < taille_totale; i += 2) {
        jeu->T[jeu->P[i]] = valeur_carte;
        jeu->T[jeu->P[i+1]] = valeur_carte;
        valeur_carte++;
    }
}

void liberer_jeu(Jeu *jeu) {
    if (jeu->T) free(jeu->T);
    if (jeu->P) free(jeu->P);
    if (jeu->bot.memoire) free(jeu->bot.memoire);
}

void convertir_k_en_ij(int k, int C, int *i, int *j) {
    *i = k / C;
    *j = k % C;
}

int convertir_ij_en_k(int i, int j, int C) {
    return i * C + j;
}

void afficher_plateau(Jeu *jeu, int posRévélée1, int posRévélée2) {
    printf("\n");
    
    printf("      "); 
    for(int j=0; j < jeu->C; j++) {
        printf(CYAN "%2d " RESET, j);
    }
    printf("\n");

    printf("     +");
    for(int j=0; j < jeu->C; j++) printf("---");
    printf("+\n");

    for (int i = 0; i < jeu->L; i++) {
        printf(CYAN "  %2d " RESET "|", i); 

        for (int j = 0; j < jeu->C; j++) {
            int k = convertir_ij_en_k(i, j, jeu->C);
            
            if (jeu->T[k] == CARTE_RETIREE) {
                printf("   "); 
            } 
            else if (k == posRévélée1 || k == posRévélée2) {
                if (jeu->T[k] == JOKER) {
                    printf(RED " %d " RESET, jeu->T[k]);
                } else {
                    printf(YELLOW " %2d" RESET, jeu->T[k]);
                }
            } 
            else {
                printf(" . ");
            }
        }
        printf("|\n");
    }
    
    printf("     +");
    for(int j=0; j < jeu->C; j++) printf("---");
    printf("+\n");
}

void afficher_plateau_triche(Jeu *jeu) {
    printf("\n");
    printf(RED "      [TRICHE]" RESET "\n");
    
    printf("      "); 
    for(int j=0; j < jeu->C; j++) {
        printf(CYAN "%2d " RESET, j);
    }
    printf("\n");

    printf("     +");
    for(int j=0; j < jeu->C; j++) printf("---");
    printf("+\n");

    for (int i = 0; i < jeu->L; i++) {
        printf(CYAN "  %2d " RESET "|", i); 

        for (int j = 0; j < jeu->C; j++) {
            int k = convertir_ij_en_k(i, j, jeu->C);
            
            if (jeu->T[k] == CARTE_RETIREE) {
                printf("   "); 
            } 
            else if (jeu->T[k] == JOKER) {
                printf(RED " %d " RESET, jeu->T[k]);
            } 
            else {
                printf(YELLOW " %2d" RESET, jeu->T[k]);
            }
        }
        printf("|\n");
    }
    
    printf("     +");
    for(int j=0; j < jeu->C; j++) printf("---");
    printf("+\n");
}

void gestion_joker(Jeu *jeu, int positionJoker) {
    int index_aleatoire_dans_P = rand() % jeu->R;
    int r = jeu->P[index_aleatoire_dans_P];

    int temp = jeu->T[positionJoker];
    jeu->T[positionJoker] = jeu->T[r];
    jeu->T[r] = temp;

    printf(">>> JOKER ! Passage de tour et permutation effectuee.\n");
}

int verifier_paire(Jeu *jeu, int p1, int p2) {
    if (jeu->T[p1] == jeu->T[p2]) {
        jeu->T[p1] = -1;
        jeu->T[p2] = -1;

        int i = 0;
        while(i < jeu->R) {
            if (jeu->P[i] == p1 || jeu->P[i] == p2) {
                jeu->P[i] = jeu->P[jeu->R - 1];
                jeu->R--;
            } else {
                i++;
            }
        }
        return 1;
    }
    return 0;
}

int saisir_position_coordonnees(Jeu *jeu) {
    int i, j, k;
    int valide = 0;
    while (!valide) {
        printf("Entrez " CYAN "LIGNE" RESET " et " CYAN "COLONNE" RESET " (ex: 0 2) : ");
        if (scanf("%d %d", &i, &j) != 2) {
             while(getchar() != '\n'); 
             continue;
        }

        if (i < 0 || i >= jeu->L || j < 0 || j >= jeu->C) {
            printf(RED "Coordonnees hors du plateau !\n" RESET);
            continue;
        }

        k = convertir_ij_en_k(i, j, jeu->C);

        if (jeu->T[k] == CARTE_RETIREE) {
             printf(RED "Cette carte a deja ete retiree.\n" RESET);
        } else {
            valide = 1;
        }
    }
    return k;
}

void bot_memoriser(MemoireBot *mem, int position, int valeur) {
    if (position >= 0 && position < mem->capaciteMax) {
        mem->memoire[position].position = position;
        mem->memoire[position].valeur = valeur;
        mem->memoire[position].estValide = 1; 
    }
}

void bot_oublier_paire(MemoireBot *mem, int val) {
    for (int i = 0; i < mem->capaciteMax; i++) {
        if (mem->memoire[i].valeur == val) {
            mem->memoire[i].estValide = 0;
        }
    }
}

int est_dans_P(Jeu *jeu, int pos) {
    for(int i=0; i < jeu->R; i++) {
        if(jeu->P[i] == pos) return 1;
    }
    return 0;
}

void bot_jouer_tour(Jeu *jeu, MemoireBot *mem, int *pos1, int *pos2) {
    printf(">> Le Bot reflechit...\n");

    for (int i = 0; i < mem->capaciteMax; i++) {
        if (mem->memoire[i].estValide && est_dans_P(jeu, i)) {
            for (int j = i + 1; j < mem->capaciteMax; j++) {
                if (mem->memoire[j].estValide && est_dans_P(jeu, j)) {
                    if (mem->memoire[i].valeur == mem->memoire[j].valeur) {
                        *pos1 = i;
                        *pos2 = j;
                        return;
                    }
                }
            }
        }
    }

    int idx1 = rand() % jeu->R;
    int idx2;
    do {
        idx2 = rand() % jeu->R;
    } while (idx1 == idx2);

    *pos1 = jeu->P[idx1];
    *pos2 = jeu->P[idx2];
}
