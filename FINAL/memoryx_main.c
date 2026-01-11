/**
 * @file memoryx_main.c
 * @brief MÉMORYX - SAE 1.02 - IUT Metz - VERSION FINALE
 * @description Programme principal avec menu complet
 * 
 * Compilation :
 *   gcc memoryx_main.c memoryx_fonctions.c -o memoryx
 * 
 * Exécution :
 *   ./memoryx
 */

#include "memoryx.h"

/* ============================================================
   PROGRAMME PRINCIPAL
   ============================================================ */

int main(void) {
    Jeu jeu;
    Joueur *j;
    int L, C;
    int choix_menu;
    int p1, p2;
    int tourTermine;
    int modeTriche = 0;
    int modeSolitaire = 0;
    int ligne, col;

    /* Initialisation du générateur aléatoire */
    srand((unsigned int)time(NULL));

    /* ==================== MENU PRINCIPAL ==================== */
    do {
        effacer_ecran();
        afficher_banniere();

        printf(BOLD "=== MENU PRINCIPAL ===\n" RESET);

        if (modeTriche) {
            printf(RED ">>> Mode Triche ACTIVE <<<\n" RESET);
        }

        printf("\nChoisissez votre mode de jeu :\n\n");

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
        printf("\nVotre choix : ");
        scanf("%d", &choix_menu);

        /* Gestion du quitter */
        if (choix_menu == 5) {
            printf(GREEN "\nMerci d'avoir joue ! A bientot !\n" RESET);
            return 0;
        }

        /* Gestion du mode triche (bascule on/off) */
        if (choix_menu == 0) {
            modeTriche = !modeTriche;
            if (modeTriche) {
                printf(RED "\n>>> Mode Triche ACTIVE ! <<<\n" RESET);
            } else {
                printf(GREEN "\n>>> Mode Triche DESACTIVE ! <<<\n" RESET);
            }
            attendre(1);
        }

    } while (choix_menu < 1 || choix_menu > 4);

    /* ==================== CONFIGURATION DU PLATEAU ==================== */
    printf("\n" BOLD "=== CONFIGURATION DU PLATEAU ===\n" RESET);
    printf("Rappel : L et C doivent etre impairs (3, 5, 7, ...)\n");
    printf("Formule : n = (L*C - 1) / 2 paires + 1 Joker\n\n");

    do {
        printf("Nombre de lignes " CYAN "L" RESET " (impair, >= 3) : ");
        scanf("%d", &L);
        printf("Nombre de colonnes " CYAN "C" RESET " (impair, >= 3) : ");
        scanf("%d", &C);

        if (L % 2 == 0 || C % 2 == 0) {
            printf(RED "Erreur : L et C doivent etre impairs.\n" RESET);
        } else if (L < 3 || C < 3) {
            printf(RED "Erreur : L et C doivent etre >= 3.\n" RESET);
        } else if (L * C < 3) {
            printf(RED "Erreur : Le plateau doit contenir au moins 3 cases.\n" RESET);
        }
    } while (L % 2 == 0 || C % 2 == 0 || L < 3 || C < 3 || L * C < 3);

    printf("\nPlateau : %d x %d = %d cases\n", L, C, L * C);
    printf("Nombre de paires : %d + 1 Joker\n", (L * C - 1) / 2);

    /* ==================== INITIALISATION ==================== */
    preparer_jeu(&jeu, L, C);
    initialiser_plateau(&jeu);

    /* Configuration des joueurs selon le mode */
    modeSolitaire = (choix_menu >= 3);
    jeu.nbJoueurs = modeSolitaire ? 1 : 2;

    switch (choix_menu) {
        case 1:  /* Humain vs Humain */
            strcpy(jeu.joueurs[0].pseudo, "Joueur 1");
            jeu.joueurs[0].estBot = 0;
            jeu.joueurs[0].score = 0;

            strcpy(jeu.joueurs[1].pseudo, "Joueur 2");
            jeu.joueurs[1].estBot = 0;
            jeu.joueurs[1].score = 0;
            break;

        case 2:  /* Humain vs Bot */
            strcpy(jeu.joueurs[0].pseudo, "Joueur");
            jeu.joueurs[0].estBot = 0;
            jeu.joueurs[0].score = 0;

            strcpy(jeu.joueurs[1].pseudo, "Bot-Memoryx");
            jeu.joueurs[1].estBot = 1;
            jeu.joueurs[1].score = 0;
            break;

        case 3:  /* Solitaire Humain */
            strcpy(jeu.joueurs[0].pseudo, "Solo-Player");
            jeu.joueurs[0].estBot = 0;
            jeu.joueurs[0].score = 0;
            break;

        case 4:  /* Solitaire Bot (Demo) */
            strcpy(jeu.joueurs[0].pseudo, "Bot-Demo");
            jeu.joueurs[0].estBot = 1;
            jeu.joueurs[0].score = 0;
            break;
    }

    /* Choix de l'ordre de passage en mode Duel */
    jeu.joueurActuel = 0;
    if (!modeSolitaire) {
        int choixPremier;
        printf("\n" BOLD "=== ORDRE DE PASSAGE ===\n" RESET);
        printf("1. %s commence\n", jeu.joueurs[0].pseudo);
        printf("2. %s commence\n", jeu.joueurs[1].pseudo);
        printf("Votre choix (1 ou 2) : ");
        scanf("%d", &choixPremier);
        if (choixPremier == 2) {
            jeu.joueurActuel = 1;
        }
    }
    
    jeu.tempsDebut = time(NULL);

    printf(GREEN "\nPartie lancee ! Bonne chance.\n" RESET);
    attendre(2);

    /* ==================== BOUCLE DE JEU ==================== */
    while (jeu.R > 1) {
        effacer_ecran();
        afficher_banniere();

        j = &jeu.joueurs[jeu.joueurActuel];
        tourTermine = 0;
        jeu.tour++;
        jeu.coups++;

        /* Affichage des scores */
        afficher_scores(&jeu, modeSolitaire);
        printf("\nTour %d - C'est au tour de : " BOLD "%s" RESET "\n", jeu.tour, j->pseudo);

        /* Affichage du plateau */
        if (modeTriche) {
            afficher_plateau_triche(&jeu);
        } else {
            afficher_plateau(&jeu, -1, -1);
        }

        /* ========== PREMIÈRE CARTE ========== */
        if (j->estBot) {
            printf(YELLOW "\n%s reflechit...\n" RESET, j->pseudo);
            attendre(1);
            bot_jouer_tour(&jeu, &p1, &p2);
            convertir_k_en_ij(p1, jeu.C, &ligne, &col);
            printf("%s choisit : Ligne %d, Colonne %d\n", j->pseudo, ligne, col);
            attendre(1);
        } else {
            printf("\nCarte 1 : ");
            p1 = saisir_position(&jeu);
        }

        /* Révélation de la première carte */
        printf("\n=> Carte revelee : ");
        if (jeu.T[p1] == JOKER) {
            printf(RED "[JOKER]" RESET "\n");
        } else {
            printf(YELLOW "[%d]" RESET "\n", jeu.T[p1]);
        }

        /* Le bot mémorise */
        bot_memoriser(&jeu.bot, p1, jeu.T[p1]);

        /* Vérification Joker sur première carte */
        if (jeu.T[p1] == JOKER) {
            afficher_plateau(&jeu, p1, -1);
            printf(RED "\n!!! JOKER !!! %s passe son tour !\n" RESET, j->pseudo);
            gestion_joker(&jeu, p1);
            attendre(3);
            tourTermine = 1;
        }

        /* ========== DEUXIÈME CARTE ========== */
        if (!tourTermine) {
            afficher_plateau(&jeu, p1, -1);

            if (j->estBot) {
                /* Le bot a déjà calculé p2 */
                if (p2 != -1) {
                    printf(YELLOW "\n%s choisit sa 2eme carte...\n" RESET, j->pseudo);
                    attendre(1);
                    convertir_k_en_ij(p2, jeu.C, &ligne, &col);
                    printf("%s choisit : Ligne %d, Colonne %d\n", j->pseudo, ligne, col);
                    attendre(1);
                }
            } else {
                printf("\nCarte 2 : ");
                do {
                    p2 = saisir_position(&jeu);
                    if (p1 == p2) {
                        printf(RED "Vous avez deja retourne cette carte !\n" RESET);
                    }
                } while (p1 == p2);
            }

            /* Révélation de la deuxième carte */
            printf("\n=> Carte revelee : ");
            if (jeu.T[p2] == JOKER) {
                printf(RED "[JOKER]" RESET "\n");
            } else {
                printf(YELLOW "[%d]" RESET "\n", jeu.T[p2]);
            }

            /* Le bot mémorise */
            bot_memoriser(&jeu.bot, p2, jeu.T[p2]);

            /* Vérification Joker sur deuxième carte */
            if (jeu.T[p2] == JOKER) {
                afficher_plateau(&jeu, p1, p2);
                printf(RED "\n!!! JOKER !!! %s passe son tour !\n" RESET, j->pseudo);
                gestion_joker(&jeu, p2);
                attendre(3);
                tourTermine = 1;
            }
        }

        /* ========== RÉSOLUTION ========== */
        if (!tourTermine) {
            afficher_plateau(&jeu, p1, p2);

            /* === Les deux cartes restent visibles 5 secondes (sujet section 5) === */
            printf(CYAN "\n[Les cartes restent visibles 5 secondes...]\n" RESET);
            attendre(5);
            
            if (verifier_paire(&jeu, p1, p2)) {
                printf(GREEN "\n>>> PAIRE TROUVEE ! (+1 Point) <<<\n" RESET);
                j->score++;
                printf("Vous rejouez !\n");
                attendre(1);
                /* Le joueur rejoue, pas de changement */
            } else {
                printf(RED "\nRate..." RESET " Retenez bien les positions.\n");
                attendre(1);
                /* Changement de joueur (sauf solitaire) */
                if (!modeSolitaire) {
                    jeu.joueurActuel = (jeu.joueurActuel + 1) % MAX_JOUEURS;
                }
            }
        } else {
            /* Après un Joker, changement de joueur (sauf solitaire) */
            if (!modeSolitaire) {
                jeu.joueurActuel = (jeu.joueurActuel + 1) % MAX_JOUEURS;
            }
        }
    }

    /* ==================== FIN DE PARTIE ==================== */
    effacer_ecran();
    afficher_banniere();
    afficher_fin_partie(&jeu, modeSolitaire);

    /* Libération mémoire */
    liberer_jeu(&jeu);

    return 0;
}
