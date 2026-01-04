/**
 * @file fonctions.c
 * @brief MÉMORYX - SAE 1.02 - IUT Metz
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include "fonctions.h"

/* ==================== UTILITAIRES ==================== */

void initialiserAleatoire(void) {
    srand(time(NULL));
}

int aleatoire(int min, int max) {
    return min + rand() % (max - min + 1);
}

void melangerTableau(int t[], int taille) {
    int i, j, temp;
    /* Mélange simple : échanger chaque case avec une position aléatoire */
    for (i = 0; i < taille; i++) {
        j = aleatoire(0, taille - 1);
        temp = t[i];
        t[i] = t[j];
        t[j] = temp;
    }
}

int dimensionsValides(int L, int C) {
    return (L % 2 == 1) && (C % 2 == 1) && 
           (L >= 3) && (L <= 21) && 
           (C >= 3) && (C <= 21);
}

/* ==================== PLATEAU ==================== */

void initialiserPlateau(Partie *p, int L, int C) {
    int i, carte;
    
    p->L = L;
    p->C = C;
    p->R = L * C;
    
    /* Initialiser P : P[i] = i */
    for (i = 0; i < p->R; i++) {
        p->P[i] = i;
    }
    
    /* Mélanger P */
    melangerTableau(p->P, p->R);
    
    /* Initialiser T à -1 */
    for (i = 0; i < p->R; i++) {
        p->T[i] = CARTE_RETIREE;
    }
    
    /* Placer le Joker en P[0] */
    p->T[p->P[0]] = JOKER;
    
    /* Placer les paires */
    carte = 1;
    for (i = 1; i < p->R; i += 2) {
        p->T[p->P[i]] = carte;
        p->T[p->P[i + 1]] = carte;
        carte++;
    }
    
    /* Réinitialiser P */
    for (i = 0; i < p->R; i++) {
        p->P[i] = i;
    }
}

void afficherTableauRetourne(Partie *p) {
    int i, j, k;
    
    printf("\n*** TRICHE ***\n");
    printf("     ");
    for (j = 1; j <= p->C; j++) printf(" %3d ", j);
    printf("\n     +");
    for (j = 0; j < p->C; j++) printf("-----");
    printf("+\n");
    
    for (i = 0; i < p->L; i++) {
        printf(" %2d  |", i + 1);
        for (j = 0; j < p->C; j++) {
            k = i * p->C + j;
            if (p->T[k] == CARTE_RETIREE) {
                printf("     ");
            } else if (p->T[k] == JOKER) {
                printf(" JK  ");
            } else {
                printf(" %3d ", p->T[k]);
            }
        }
        printf("|\n");
    }
    
    printf("     +");
    for (j = 0; j < p->C; j++) printf("-----");
    printf("+\n");
}

void afficherPlateau(Partie *p, int pos1, int pos2) {
    int i, j, k;
    
    /* Toujours afficher le tableau retourné */
    afficherTableauRetourne(p);
    
    /* Tableau de jeu */
    printf("\n     ");
    for (j = 1; j <= p->C; j++) printf(" %3d ", j);
    printf("\n     +");
    for (j = 0; j < p->C; j++) printf("-----");
    printf("+\n");
    
    for (i = 0; i < p->L; i++) {
        printf(" %2d  |", i + 1);
        for (j = 0; j < p->C; j++) {
            k = i * p->C + j;
            if (p->T[k] == CARTE_RETIREE) {
                printf("     ");
            } else if (k == pos1 || k == pos2) {
                if (p->T[k] == JOKER) {
                    printf(" %sJK%s  ", BLEU, RESET);
                } else {
                    printf(" %s%3d%s ", BLEU, p->T[k], RESET);
                }
            } else {
                printf("  ?  ");
            }
        }
        printf("|\n");
    }
    
    printf("     +");
    for (j = 0; j < p->C; j++) printf("-----");
    printf("+\n");
}

int positionValide(Partie *p, int pos) {
    return (pos >= 0) && (pos < p->L * p->C) && (p->T[pos] != CARTE_RETIREE);
}

void permuterJoker(Partie *p, int posJoker) {
    int r, posAlea, temp;
    
    r = aleatoire(0, p->R - 1);
    posAlea = p->P[r];
    
    temp = p->T[posJoker];
    p->T[posJoker] = p->T[posAlea];
    p->T[posAlea] = temp;
    
    printf("Le Joker a ete deplace !\n");
}

void retirerPaire(Partie *p, int pos1, int pos2) {
    int i, j = 0;
    int temp[MAX_CARTES];
    
    p->T[pos1] = CARTE_RETIREE;
    p->T[pos2] = CARTE_RETIREE;
    
    for (i = 0; i < p->R; i++) {
        if (p->P[i] != pos1 && p->P[i] != pos2) {
            temp[j++] = p->P[i];
        }
    }
    
    for (i = 0; i < j; i++) {
        p->P[i] = temp[i];
    }
    p->R = j;
}

int partieTerminee(Partie *p) {
    return p->R <= 1;
}

/* ==================== JOUEURS ==================== */

void configurerJoueurs(Partie *p, int modeDuel) {
    int choix;
    
    p->joueurActuel = 0;
    
    if (modeDuel) {
        p->nbJoueurs = 2;
        
        strcpy(p->joueurs[0].pseudo, "Joueur 1");
        p->joueurs[0].score = 0;
        p->joueurs[0].estBot = 0;
        
        printf("Joueur 2 est un bot ? (1=Oui, 0=Non) : ");
        scanf("%d", &choix);
        
        if (choix) {
            strcpy(p->joueurs[1].pseudo, "Bot");
            p->joueurs[1].estBot = 1;
            initialiserMemoire(&p->memoire);
        } else {
            strcpy(p->joueurs[1].pseudo, "Joueur 2");
            p->joueurs[1].estBot = 0;
        }
        p->joueurs[1].score = 0;
    } else {
        /* Mode solitaire : humain ou bot */
        printf("Jouer avec le Bot ? (1=Oui, 0=Non) : ");
        scanf("%d", &choix);
        
        p->nbJoueurs = 1;
        
        if (choix) {
            strcpy(p->joueurs[0].pseudo, "Bot");
            p->joueurs[0].estBot = 1;
            initialiserMemoire(&p->memoire);
        } else {
            strcpy(p->joueurs[0].pseudo, "Joueur 1");
            p->joueurs[0].estBot = 0;
        }
        p->joueurs[0].score = 0;
    }
}

void joueurSuivant(Partie *p) {
    if (p->nbJoueurs > 1) {
        p->joueurActuel = (p->joueurActuel + 1) % 2;
    }
}

void afficherScores(Partie *p) {
    int i;
    int n = (p->L * p->C - 1) / 2;
    int tempsEcoule;
    
    printf("\nPaires: %d/%d | Coups: %d", 
           p->joueurs[0].score + (p->nbJoueurs == 2 ? p->joueurs[1].score : 0), n, p->coups);
    
    /* Afficher le temps écoulé en mode solitaire */
    if (p->nbJoueurs == 1) {
        tempsEcoule = (int)(time(NULL) - p->tempsDebut);
        printf(" | Temps: %02d:%02d", tempsEcoule / 60, tempsEcoule % 60);
    }
    printf("\n");
    
    for (i = 0; i < p->nbJoueurs; i++) {
        printf("%s: %d  ", p->joueurs[i].pseudo, p->joueurs[i].score);
    }
    printf("\n");
}

void afficherGagnant(Partie *p) {
    int n = (p->L * p->C - 1) / 2;
    int tempsTotal;
    
    printf("\n*************************\n");
    printf("*    FIN DE PARTIE !    *\n");
    printf("*************************\n\n");
    
    if (p->nbJoueurs == 1) {
        tempsTotal = (int)(time(NULL) - p->tempsDebut);
        printf("Bravo %s !\n", p->joueurs[0].pseudo);
        printf("Paires trouvees : %d/%d\n", p->joueurs[0].score, n);
        printf("Nombre de coups : %d\n", p->coups);
        printf("Temps total : %02d:%02d\n", tempsTotal / 60, tempsTotal % 60);
    } else {
        printf("%s : %d paires\n", p->joueurs[0].pseudo, p->joueurs[0].score);
        printf("%s : %d paires\n", p->joueurs[1].pseudo, p->joueurs[1].score);
        printf("\n");
        if (p->joueurs[0].score > p->joueurs[1].score) {
            printf("*** %s GAGNE ! ***\n", p->joueurs[0].pseudo);
        } else if (p->joueurs[1].score > p->joueurs[0].score) {
            printf("*** %s GAGNE ! ***\n", p->joueurs[1].pseudo);
        } else {
            printf("*** EGALITE ! ***\n");
        }
    }
}

int saisirPosition(Partie *p) {
    int ligne, col, pos;
    
    while (1) {
        printf("Ligne Colonne : ");
        if (scanf("%d %d", &ligne, &col) != 2) {
            while (getchar() != '\n');
            printf("Entrez deux nombres.\n");
            continue;
        }
        
        if (ligne < 1 || ligne > p->L || col < 1 || col > p->C) {
            printf("Hors limites.\n");
            continue;
        }
        
        pos = (ligne - 1) * p->C + (col - 1);
        
        if (!positionValide(p, pos)) {
            printf("Carte deja retiree.\n");
            continue;
        }
        
        return pos;
    }
}

/* ==================== BOT ==================== */

void initialiserMemoire(MemoireBot *m) {
    m->nbCartes = 0;
}

void memoriserCarte(MemoireBot *m, int pos, int val) {
    int i;
    
    if (val == JOKER) return;
    
    for (i = 0; i < m->nbCartes; i++) {
        if (m->cartes[i].position == pos) {
            m->cartes[i].valeur = val;
            return;
        }
    }
    
    if (m->nbCartes < MAX_CARTES) {
        m->cartes[m->nbCartes].position = pos;
        m->cartes[m->nbCartes].valeur = val;
        m->nbCartes++;
    }
}

void oublierCarte(MemoireBot *m, int pos) {
    int i, j;
    for (i = 0; i < m->nbCartes; i++) {
        if (m->cartes[i].position == pos) {
            for (j = i; j < m->nbCartes - 1; j++) {
                m->cartes[j] = m->cartes[j + 1];
            }
            m->nbCartes--;
            return;
        }
    }
}

int chercherPaire(MemoireBot *m, int *p1, int *p2) {
    int i, j;
    
    for (i = 0; i < m->nbCartes - 1; i++) {
        for (j = i + 1; j < m->nbCartes; j++) {
            if (m->cartes[i].valeur == m->cartes[j].valeur) {
                *p1 = m->cartes[i].position;
                *p2 = m->cartes[j].position;
                return 1;
            }
        }
    }
    return 0;
}

int chercherValeur(MemoireBot *m, int val, int exclue) {
    int i;
    for (i = 0; i < m->nbCartes; i++) {
        if (m->cartes[i].valeur == val && m->cartes[i].position != exclue) {
            return m->cartes[i].position;
        }
    }
    return -1;
}

int tourBot(Partie *p) {
    int pos1, pos2, val1, val2;
    int paire = 0;
    
    p->coups++;
    p->tour++;
    
    printf("\n--- Tour %d : Bot ---\n", p->tour);
    
    /* Chercher une paire en mémoire */
    paire = chercherPaire(&p->memoire, &pos1, &pos2);
    
    if (!paire) {
        pos1 = p->P[aleatoire(0, p->R - 1)];
    }
    
    val1 = p->T[pos1];
    printf("Bot joue (%d,%d)\n", pos1 / p->C + 1, pos1 % p->C + 1);
    memoriserCarte(&p->memoire, pos1, val1);
    
    if (val1 == JOKER) {
        afficherPlateau(p, pos1, -1);
        sleep(5);
        permuterJoker(p, pos1);
        oublierCarte(&p->memoire, pos1);
        sleep(5);
        return 0;
    }
    
    afficherPlateau(p, pos1, -1);
    sleep(5);
    
    /* Deuxième carte */
    if (paire && positionValide(p, pos2)) {
        /* Utiliser la paire trouvée */
    } else {
        pos2 = chercherValeur(&p->memoire, val1, pos1);
        if (pos2 == -1 || !positionValide(p, pos2)) {
            do {
                pos2 = p->P[aleatoire(0, p->R - 1)];
            } while (pos2 == pos1);
        }
    }
    
    val2 = p->T[pos2];
    printf("Bot joue (%d,%d)\n", pos2 / p->C + 1, pos2 % p->C + 1);
    memoriserCarte(&p->memoire, pos2, val2);
    
    if (val2 == JOKER) {
        afficherPlateau(p, pos1, pos2);
        sleep(5);
        permuterJoker(p, pos2);
        oublierCarte(&p->memoire, pos2);
        sleep(5);
        return 0;
    }
    
    afficherPlateau(p, pos1, pos2);
    sleep(5);
    
    if (val1 == val2) {
        printf("Bot trouve une paire !\n");
        retirerPaire(p, pos1, pos2);
        oublierCarte(&p->memoire, pos1);
        oublierCarte(&p->memoire, pos2);
        p->joueurs[p->joueurActuel].score++;
        afficherScores(p);
        sleep(5);
        return 1;
    }
    
    printf("Pas de paire.\n");
    afficherScores(p);
    sleep(5);
    return 0;
}
