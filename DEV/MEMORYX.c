#include "fonction.h"

#ifdef _WIN32
#include <windows.h>
void attendre(int secondes) { Sleep(secondes * 1000); }
#else
#include <unistd.h>
void attendre(int secondes) { sleep(secondes); }
#endif

void vider_buffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

int main() {
    Jeu jeu;
    int L, C, choix_menu, p1, p2, tourTermine;
    int modeTriche = 0;
    srand((unsigned int)time(NULL));

    do {
        system("clear");
        afficher_banniere();

        printf(BOLD "\n=== CONFIGURATION ===\n" RESET);
        
        if (modeTriche) {
            printf(RED ">>> Mode de triche ACTIVE <<<\n" RESET);
        }
        
        printf("Choisissez votre mode de jeu :\n");
        if (modeTriche) {
            printf(GREEN "  0. Desactiver le Mode Triche\n" RESET);
        } else {
            printf(RED "  0. Activer le Mode Triche\n" RESET);
        }
        printf("  1. Duel : Humain vs Humain\n");
        printf("  2. Duel : Humain vs Bot\n");
        printf("  3. Solitaire : Humain seul\n");
        printf("  4. Solitaire : Bot seul (Demo)\n");
        printf("  5. Quitter\n");
        printf("Votre choix : ");
        scanf("%d", &choix_menu);

        if (choix_menu == 5) {
            printf(GREEN "\nA bientot !\n" RESET);
            return 0;
        }

        if (choix_menu == 0) {
            modeTriche = !modeTriche;
            if (modeTriche) {
                printf(RED "\n>>> Mode de triche ACTIVE ! <<<\n" RESET);
            } else {
                printf(GREEN "\n>>> Mode de triche DESACTIVE ! <<<\n" RESET);
            }
            attendre(1);
        }

    } while (choix_menu == 0 || choix_menu < 1 || choix_menu > 4);

    printf("\nConfiguration du plateau :\n");
    do {
        printf("Nombre de lignes " CYAN "L" RESET " (impair) : ");
        scanf("%d", &L);
        printf("Nombre de colonnes " CYAN "C" RESET " (impair) : ");
        scanf("%d", &C);
        if (L % 2 == 0 || C % 2 == 0 || (L * C) < 3) {
            printf(RED "Erreur : L et C doivent etre impairs et le produit >= 3.\n" RESET);
        }
    } while (L % 2 == 0 || C % 2 == 0 || (L * C) < 3);

    preparer_jeu(&jeu, L, C);
    initialiser_plateau(&jeu);

    strcpy(jeu.joueurs[0].pseudo, "Joueur 1");
    jeu.joueurs[0].estBot = 0;
    jeu.joueurs[0].score = 0;
    
    strcpy(jeu.joueurs[1].pseudo, "Joueur 2");
    jeu.joueurs[1].estBot = 0;
    jeu.joueurs[1].score = 0;

    if (choix_menu == 2) {
        strcpy(jeu.joueurs[1].pseudo, "Bot-Mémoryx");
        jeu.joueurs[1].estBot = 1;
    } 
    else if (choix_menu == 3) {
        strcpy(jeu.joueurs[0].pseudo, "Solo-Player");
    }
    else if (choix_menu == 4) {
        strcpy(jeu.joueurs[0].pseudo, "Bot-Solo");
        jeu.joueurs[0].estBot = 1;
    }

    jeu.joueurActuel = 0;
    printf(GREEN "\nPartie lancee ! Bonne chance.\n" RESET);
    attendre(2);

    while (jeu.R > 1) {
        system("clear");
        afficher_banniere();

        Joueur *j = &jeu.joueurs[jeu.joueurActuel];
        tourTermine = 0;

        printf("--------------------------------------------------\n");
        if (modeTriche) {
            printf(RED " [MODE TRICHE ACTIVE]" RESET "\n");
        }
        if (choix_menu > 2) {
             printf(" Mode Solitaire | Joueur : %s | Score : %d\n", j->pseudo, j->score);
        } else {
             printf(" %s : %d  VS  %s : %d\n", 
                jeu.joueurs[0].pseudo, jeu.joueurs[0].score,
                jeu.joueurs[1].pseudo, jeu.joueurs[1].score);
        }
        printf("--------------------------------------------------\n");
        printf("C'est au tour de : " BOLD "%s" RESET "\n", j->pseudo);

        if (modeTriche) {
            afficher_plateau_triche(&jeu);
        } else {
            afficher_plateau(&jeu, -1, -1);
        }

        if (j->estBot) {
            printf(YELLOW "Le Bot reflechit...\n" RESET);
            bot_jouer_tour(&jeu, &jeu.bot, &p1, &p2);
            attendre(1);
            printf("Bot choisit : Ligne %d, Col %d\n", p1/C, p1%C);
            attendre(1);
        } else {
            printf("\nCarte 1 : ");
            p1 = saisir_position_coordonnees(&jeu);
        }

        printf("=> Carte revelee : " BOLD "[%d]" RESET "\n", jeu.T[p1]);
        bot_memoriser(&jeu.bot, p1, jeu.T[p1]);

        if (jeu.T[p1] == JOKER) {
            afficher_plateau(&jeu, p1, -1);
            printf(RED "\n!!! JOKER !!! %s passe son tour !\n" RESET, j->pseudo);
            gestion_joker(&jeu, p1);
            attendre(3);
            tourTermine = 1;
        }

        if (!tourTermine) {
            afficher_plateau(&jeu, p1, -1);

            if (j->estBot) {
                 printf(YELLOW "Le Bot choisit sa 2eme carte...\n" RESET);
                 attendre(1);
                 printf("Bot choisit : Ligne %d, Col %d\n", p2/C, p2%C);
                 attendre(1);
            } else {
                printf("\nCarte 2 : ");
                do {
                    p2 = saisir_position_coordonnees(&jeu);
                    if (p1 == p2) printf(RED "Impossible, vous avez deja retourne cette carte !\n" RESET);
                } while (p1 == p2);
            }

            printf("=> Carte revelee : " BOLD "[%d]" RESET "\n", jeu.T[p2]);
            bot_memoriser(&jeu.bot, p2, jeu.T[p2]);

            if (jeu.T[p2] == JOKER) {
                afficher_plateau(&jeu, p1, p2);
                printf(RED "\n!!! JOKER !!! %s passe son tour !\n" RESET, j->pseudo);
                gestion_joker(&jeu, p2);
                attendre(3);
                tourTermine = 1;
            }
        }

        if (!tourTermine) {
            afficher_plateau(&jeu, p1, p2);

            if (verifier_paire(&jeu, p1, p2)) {
                printf(GREEN "\n>>> PAIRE TROUVEE ! (+1 Point) <<<\n" RESET);
                j->score++;
                printf("Vous rejouez !\n");
                attendre(2);
            } else {
                printf("\nRate... Retenez bien les positions.\n");
                attendre(3);
                if (choix_menu <= 2) {
                    jeu.joueurActuel = (jeu.joueurActuel + 1) % MAX_JOUEURS;
                }
            }
        } else {
            if (choix_menu <= 2) {
                jeu.joueurActuel = (jeu.joueurActuel + 1) % MAX_JOUEURS;
            }
        }
    }

    printf(BOLD "\n=== FIN DE PARTIE ===\n" RESET);
    liberer_jeu(&jeu);
    return 0;
}
