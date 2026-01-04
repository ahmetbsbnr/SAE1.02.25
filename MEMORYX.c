/**
 * @file MEMORYX.c
 * @brief MÉMORYX - SAE 1.02 - IUT Metz
 */

#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include "fonctions.h"



void afficherMenu(void) {
    printf("\n");
    printf("+=======================+\n");
    printf("|      M E M O R Y X    |\n");
    printf("|    SAE 1.02 - 2026    |\n");
    printf("+=======================+\n");
    printf("|  1. Mode Duel         |\n");
    printf("|  2. Mode Solitaire    |\n");
    printf("|  0. Quitter           |\n");
    printf("+=======================+\n");
    printf("Choix : ");
}

int configurerPlateau(int *L, int *C) {
    printf("Lignes (impair, 3-21) : ");
    scanf("%d", L);
    printf("Colonnes (impair, 3-21) : ");
    scanf("%d", C);
    
    if (!dimensionsValides(*L, *C)) {
        printf("Dimensions invalides !\n");
        return 0;
    }
    return 1;
}

void tourHumain(Partie *p) {
    int pos1, pos2, val1, val2;
    Joueur *j = &p->joueurs[p->joueurActuel];
    
    p->coups++;
    p->tour++;
    
    printf("\n=== Tour %d : %s ===\n", p->tour, j->pseudo);
    afficherPlateau(p, -1, -1);
    
    /* Carte 1 */
    printf("Carte 1 - ");
    pos1 = saisirPosition(p);
    val1 = p->T[pos1];
    
    /* Bot observe */
    if (p->nbJoueurs == 2 && p->joueurs[1].estBot) {
        memoriserCarte(&p->memoire, pos1, val1);
    }
    
    if (val1 == JOKER) {
        printf("C'est le JOKER !\n");
        afficherPlateau(p, pos1, -1);
        sleep(5);
        permuterJoker(p, pos1);
        sleep(5);
        return;
    }
    
    afficherPlateau(p, pos1, -1);
    sleep(5);
    
    /* Carte 2 */
    do {
        printf("Carte 2 - ");
        pos2 = saisirPosition(p);
        if (pos2 == pos1) printf("Choisissez une autre carte.\n");
    } while (pos2 == pos1);
    
    val2 = p->T[pos2];
    
    if (p->nbJoueurs == 2 && p->joueurs[1].estBot) {
        memoriserCarte(&p->memoire, pos2, val2);
    }
    
    if (val2 == JOKER) {
        printf("C'est le JOKER !\n");
        afficherPlateau(p, pos1, pos2);
        sleep(5);
        permuterJoker(p, pos2);
        sleep(5);
        return;
    }
    
    afficherPlateau(p, pos1, pos2);
    sleep(5);
    
    if (val1 == val2) {
        printf("PAIRE TROUVEE !\n");
        retirerPaire(p, pos1, pos2);
        
        if (p->nbJoueurs == 2 && p->joueurs[1].estBot) {
            oublierCarte(&p->memoire, pos1);
            oublierCarte(&p->memoire, pos2);
        }
        
        j->score++;
        afficherScores(p);
        sleep(5);
        
        if (!partieTerminee(p)) {
            tourHumain(p);
        }
    } else {
        printf("Pas de paire.\n");
        afficherScores(p);
        sleep(5);
    }
}

void boucleJeu(Partie *p) {
    Joueur *j;
    int paire;
    
    printf("\nPartie lancee !\n");
    
    while (!partieTerminee(p)) {
        j = &p->joueurs[p->joueurActuel];
        
        if (j->estBot) {
            do {
                paire = tourBot(p);
            } while (paire && !partieTerminee(p));
        } else {
            tourHumain(p);
        }
        
        if (!partieTerminee(p)) {
            joueurSuivant(p);
        }
    }
}

int main(void) {
    Partie partie;
    int L, C, choix, fin;
    int continuer = 1;
    int memeConfig = 0;
    
    initialiserAleatoire();
    
    while (continuer) {
        if (!memeConfig) {
            afficherMenu();
            scanf("%d", &choix);
            
            if (choix == 0) break;
            if (choix < 1 || choix > 2) continue;
            if (!configurerPlateau(&L, &C)) continue;
        }
        
        initialiserPlateau(&partie, L, C);
        partie.coups = 0;
        partie.tour = 0;
        partie.tempsDebut = time(NULL);
        
        configurerJoueurs(&partie, choix == 1);
        initialiserMemoire(&partie.memoire);
        
        boucleJeu(&partie);
        afficherGagnant(&partie);
        
        printf("\n1=Rejouer, 2=Menu, 0=Quitter : ");
        scanf("%d", &fin);
        
        if (fin == 1) {
            memeConfig = 1;
        } else if (fin == 2) {
            memeConfig = 0;
        } else {
            continuer = 0;
        }
    }
    
    printf("Au revoir !\n");
    return 0;
}
