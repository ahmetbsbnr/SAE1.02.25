#include "fonctioncopy.h"

/* ============================================================
   PARTIE A : MISE EN PLACE ET MÉMOIRE (Sections 3, 4 et 7)
   ============================================================ */

void afficher_banniere() {
    printf(BLUE);
    printf("  __  __    ____  __  __  ___  _____   __  __  \n");
    printf(" |  \\/  | |      |  \\/  |/ _ \\|  __ \\  \\ \\/ /  \n");
    printf(" | \\  / | | __   | \\  / | (_) | |__) |  \\  /   \n");
    printf(" | |\\/| | |      | |\\/| |\\__, |  _  /    ||    \n");
    printf(" |_|  |_|| | ___  |_|  |_|  |_| \\_\\    /_\\    \n");
    printf(RESET);
    printf("        SAE 1.2 - IUT DE METZ\n\n");
}

void preparer_jeu(Jeu *jeu, int L, int C) {
    // Vérification des contraintes L et C impairs [cite: 14]
    if (L % 2 == 0 || C % 2 == 0) {
        printf("Erreur : L et C doivent etre impairs.\n");
        exit(EXIT_FAILURE);
    }
    if (L == 1 && C == 1) { // Cas interdit [cite: 15]
        printf("Erreur : L=C=1 interdit.\n");
        exit(EXIT_FAILURE);
    }

    jeu->L = L;
    jeu->C = C;
    jeu->R = L * C; // R initial = taille totale [cite: 71]
    
    // Allocation dynamique de T et P
    jeu->T = (int *)malloc(jeu->L * jeu->C * sizeof(int));
    jeu->P = (int *)malloc(jeu->L * jeu->C * sizeof(int));
    
    // Allocation mémoire du Bot (taille max = plateau) [cite: 128]
    jeu->bot.capaciteMax = jeu->L * jeu->C;
    jeu->bot.memoire = (CarteConnue *)malloc(jeu->bot.capaciteMax * sizeof(CarteConnue));

    if (jeu->T == NULL || jeu->P == NULL || jeu->bot.memoire == NULL) {
        printf("Erreur d'allocation memoire.\n");
        exit(EXIT_FAILURE);
    }

    // Initialisation mémoire bot à "vide"
    for(int i=0; i < jeu->bot.capaciteMax; i++) {
        jeu->bot.memoire[i].estValide = 0; // Rien vu au début
        jeu->bot.memoire[i].position = i;
    }
}

void initialiser_plateau(Jeu *jeu) {
    int taille_totale = jeu->L * jeu->C;

    // 1. Remplir P tel que P[i] = i [cite: 71]
    for (int i = 0; i < taille_totale; i++) {
        jeu->P[i] = i;
    }

    // 2. Mélange de P (Permutation aléatoire) [cite: 72]
    for (int i = 0; i < taille_totale - 1; i++) {
        int j = i + rand() % (taille_totale - i);
        int temp = jeu->P[i];
        jeu->P[i] = jeu->P[j];
        jeu->P[j] = temp;
    }

    // 3. Remplir T via P [cite: 73]
    // Joker (valeur 0) en P[0]
    jeu->T[jeu->P[0]] = 0;

    // Les paires (1, 2... n)
    int valeur_carte = 1;
    for (int i = 1; i < taille_totale; i += 2) {
        jeu->T[jeu->P[i]] = valeur_carte;     // Première carte de la paire
        jeu->T[jeu->P[i+1]] = valeur_carte;   // Deuxième carte de la paire
        valeur_carte++;
    }
}

void liberer_jeu(Jeu *jeu) {
    if (jeu->T) free(jeu->T);
    if (jeu->P) free(jeu->P);
    if (jeu->bot.memoire) free(jeu->bot.memoire);
}

/* ============================================================
   PARTIE B : OUTILS DE CONVERSION (Section 3)
   ============================================================ */

// Convertit k (index T) en i (ligne) et j (colonne) [cite: 58]
void convertir_k_en_ij(int k, int C, int *i, int *j) {
    *i = k / C; // k div C
    *j = k % C; // k mod C
}

// Convertit i et j en index k unique [cite: 59]
int convertir_ij_en_k(int i, int j, int C) {
    return i * C + j;
}

/* ============================================================
   PARTIE C : LOGIQUE DU JEU (Sections 5 et 6)
   ============================================================ */

void afficher_plateau(Jeu *jeu, int posRévélée1, int posRévélée2) {
    printf("\n");
    
    // En-tête des colonnes (couleur Cyan)
    printf("      "); 
    for(int j=0; j < jeu->C; j++) {
        printf(CYAN "%2d " RESET, j);
    }
    printf("\n");

    // Ligne de séparation
    printf("     +");
    for(int j=0; j < jeu->C; j++) printf("---");
    printf("+\n");

    for (int i = 0; i < jeu->L; i++) {
        // En-tête des lignes (couleur Cyan)
        printf(CYAN "  %2d " RESET "|", i); 

        for (int j = 0; j < jeu->C; j++) {
            int k = convertir_ij_en_k(i, j, jeu->C);
            
            // Carte retirée
            if (jeu->T[k] == CARTE_RETIREE) {
                printf("   "); 
            } 
            // Carte révélée (soit par le joueur, soit déjà trouvée par le passé si on stockait l'état visible)
            // Ici on gère l'affichage temporaire du tour :
            else if (k == posRévélée1 || k == posRévélée2) {
                if (jeu->T[k] == JOKER) {
                    printf(RED " %d " RESET, jeu->T[k]); // Joker en ROUGE
                } else {
                    printf(YELLOW " %2d" RESET, jeu->T[k]); // Valeur en JAUNE
                }
            } 
            // Carte cachée
            else {
                printf(" . "); // Un point ou un symbole pour carte cachée
            }
        }
        printf("|\n");
    }
    
    // Ligne de fin
    printf("     +");
    for(int j=0; j < jeu->C; j++) printf("---");
    printf("+\n");
}

// Algorithme de permutation du Joker 
void gestion_joker(Jeu *jeu, int positionJoker) {
    // 1. Générer aléatoirement r entre 0 et R-1
    int index_aleatoire_dans_P = rand() % jeu->R;
    int r = jeu->P[index_aleatoire_dans_P];

    // 2. Permuter T[positionJoker] et T[r]
    int temp = jeu->T[positionJoker];
    jeu->T[positionJoker] = jeu->T[r];
    jeu->T[r] = temp;

    printf(">>> JOKER ! Passage de tour et permutation effectuee.\n");
    // NOTE : On ne change PAS la table P 
}

int verifier_paire(Jeu *jeu, int p1, int p2) {
    // Si les cartes sont identiques
    if (jeu->T[p1] == jeu->T[p2]) {
        // Mise à jour de T : valeurs à -1 [cite: 100]
        jeu->T[p1] = -1;
        jeu->T[p2] = -1;

        // Mise à jour de P : retirer p1 et p2 
        // Astuce : on remplace la position à supprimer par la dernière de P, puis on réduit R
        int i = 0;
        while(i < jeu->R) {
            if (jeu->P[i] == p1 || jeu->P[i] == p2) {
                jeu->P[i] = jeu->P[jeu->R - 1]; // Remplacer par la dernière
                jeu->R--; // Réduire la taille logique
            } else {
                i++;
            }
        }
        return 1; // Paire trouvée
    }
    return 0; // Pas de paire
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

        // Vérifications de base (dimensions)
        if (i < 0 || i >= jeu->L || j < 0 || j >= jeu->C) {
            printf(RED "Coordonnees hors du plateau !\n" RESET);
            continue;
        }

        // Conversion (Section 3)
        k = convertir_ij_en_k(i, j, jeu->C);

        // Vérification carte retirée
        if (jeu->T[k] == CARTE_RETIREE) {
             printf(RED "Cette carte a deja ete retiree.\n" RESET);
        } else {
            valide = 1;
        }
    }
    return k; // On retourne k pour le reste de la logique interne
}

// Fonction utilitaire pour sécuriser la saisie
int saisir_position_valide(Jeu *jeu) {
    int k;
    int valide = 0;
    while (!valide) {
        printf("Position ? ");
        if (scanf("%d", &k) != 1) { // Vérifie si l'entrée est un nombre
             while(getchar() != '\n'); // Vider buffer
             continue;
        }
        // Vérifie bornes et si la carte n'est pas déjà retirée (-1)
        if (k >= 0 && k < jeu->L * jeu->C && jeu->T[k] != -1) {
            valide = 1;
        } else {
            printf("Position invalide ou carte deja retiree.\n");
        }
    }
    return k;
}

/* ============================================================
   PARTIE D : INTELLIGENCE ARTIFICIELLE (Section 8)
   ============================================================ */

void bot_memoriser(MemoireBot *mem, int position, int valeur) {
    // Mise à jour ou ajout (Correction si Joker a changé la place) [cite: 125]
    if (position >= 0 && position < mem->capaciteMax) {
        mem->memoire[position].position = position;
        mem->memoire[position].valeur = valeur;
        mem->memoire[position].estValide = 1; 
    }
}

void bot_oublier_paire(MemoireBot *mem, int val) {
    // Le bot "oublie" les cartes qui ont cette valeur car elles ne sont plus en jeu
    for (int i = 0; i < mem->capaciteMax; i++) {
        if (mem->memoire[i].valeur == val) {
            mem->memoire[i].estValide = 0;
        }
    }
}

// Helper : vérifie si une position physique est toujours dans P
int est_dans_P(Jeu *jeu, int pos) {
    for(int i=0; i < jeu->R; i++) {
        if(jeu->P[i] == pos) return 1;
    }
    return 0;
}

void bot_jouer_tour(Jeu *jeu, MemoireBot *mem, int *pos1, int *pos2) {
    printf(">> Le Bot reflechit...\n");

    // STRATÉGIE 1 : Chercher une paire en mémoire [cite: 130]
    for (int i = 0; i < mem->capaciteMax; i++) {
        if (mem->memoire[i].estValide && est_dans_P(jeu, i)) {
            for (int j = i + 1; j < mem->capaciteMax; j++) {
                if (mem->memoire[j].estValide && est_dans_P(jeu, j)) {
                    // Si les valeurs correspondent
                    if (mem->memoire[i].valeur == mem->memoire[j].valeur) {
                        *pos1 = i;
                        *pos2 = j;
                        return; // Bot joue la paire
                    }
                }
            }
        }
    }

    // STRATÉGIE 2 : Hasard parmi les cartes restantes (P) [cite: 134]
    // Le bot utilise P pour ne pas taper dans le vide, ce qui est autorisé [cite: 122]
    int idx1 = rand() % jeu->R;
    int idx2;
    do {
        idx2 = rand() % jeu->R;
    } while (idx1 == idx2); // Assurer deux cartes différentes

    *pos1 = jeu->P[idx1];
    *pos2 = jeu->P[idx2];
}
