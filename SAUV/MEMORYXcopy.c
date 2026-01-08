#include "fonctioncopy.h"

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
    srand((unsigned int)time(NULL));

    // --- 1. MENU PRINCIPAL ---
    system("clear"); // ou system("cls") sous windows
    afficher_banniere();

    printf(BOLD "\n=== CONFIGURATION ===\n" RESET);
    printf("Choisissez votre mode de jeu :\n");
    printf("  1. Duel : Humain vs Humain\n");
    printf("  2. Duel : Humain vs Bot\n");
    printf("  3. Solitaire : Humain seul\n");
    printf("  4. Solitaire : Bot seul (Demo)\n"); // Mode demandé
    printf("Votre choix : ");
    scanf("%d", &choix_menu);

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

    // Initialisation
    preparer_jeu(&jeu, L, C);
    initialiser_plateau(&jeu);

    // Configuration des Joueurs selon le menu
    // Par défaut
    strcpy(jeu.joueurs[0].pseudo, "Joueur 1");
    jeu.joueurs[0].estBot = 0;
    jeu.joueurs[0].score = 0;
    
    strcpy(jeu.joueurs[1].pseudo, "Joueur 2");
    jeu.joueurs[1].estBot = 0;
    jeu.joueurs[1].score = 0;

    if (choix_menu == 2) { // Humain vs Bot
        strcpy(jeu.joueurs[1].pseudo, "Bot-Mémoryx");
        jeu.joueurs[1].estBot = 1;
    } 
    else if (choix_menu == 3) { // Solitaire Humain
        strcpy(jeu.joueurs[0].pseudo, "Solo-Player");
        // En solitaire, le joueur 2 n'existe pas vraiment
    }
    else if (choix_menu == 4) { // Solitaire Bot
        strcpy(jeu.joueurs[0].pseudo, "Bot-Solo");
        jeu.joueurs[0].estBot = 1; // Le joueur 1 EST un bot
    }

    jeu.joueurActuel = 0;
    printf(GREEN "\nPartie lancee ! Bonne chance.\n" RESET);
    attendre(2);

    // --- 2. BOUCLE DE JEU ---
    while (jeu.R > 1) {
        system("clear"); // Nettoyage écran pour fluidité
        afficher_banniere();

        Joueur *j = &jeu.joueurs[jeu.joueurActuel];
        tourTermine = 0;

        // Affichage scores
        printf("--------------------------------------------------\n");
        if (choix_menu > 2) { // Solitaires
             printf(" Mode Solitaire | Joueur : %s | Score : %d\n", j->pseudo, j->score);
        } else {
             printf(" %s : %d  VS  %s : %d\n", 
                jeu.joueurs[0].pseudo, jeu.joueurs[0].score,
                jeu.joueurs[1].pseudo, jeu.joueurs[1].score);
        }
        printf("--------------------------------------------------\n");
        printf("C'est au tour de : " BOLD "%s" RESET "\n", j->pseudo);

        afficher_plateau(&jeu, -1, -1);

        // --- ETAPE 1 ---
        if (j->estBot) {
            printf(YELLOW "Le Bot reflechit...\n" RESET);
            bot_jouer_tour(&jeu, &jeu.bot, &p1, &p2);
            attendre(1);
            printf("Bot choisit : Ligne %d, Col %d\n", p1/C, p1%C); // Affichage en coordonnées pour le style
            attendre(1);
        } else {
            printf("\nCarte 1 : ");
            p1 = saisir_position_coordonnees(&jeu);
        }

        // Révélation 1
        printf("=> Carte revelee : " BOLD "[%d]" RESET "\n", jeu.T[p1]);
        bot_memoriser(&jeu.bot, p1, jeu.T[p1]);

        if (jeu.T[p1] == JOKER) {
            afficher_plateau(&jeu, p1, -1); // Montrer le joker
            printf(RED "\n!!! JOKER !!! %s passe son tour !\n" RESET, j->pseudo);
            gestion_joker(&jeu, p1);
            attendre(3);
            tourTermine = 1;
        }

        if (!tourTermine) {
            afficher_plateau(&jeu, p1, -1);

            // --- ETAPE 2 ---
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

        // --- RESOLUTION ---
        if (!tourTermine) {
            afficher_plateau(&jeu, p1, p2);

            if (verifier_paire(&jeu, p1, p2)) {
                printf(GREEN "\n>>> PAIRE TROUVEE ! (+1 Point) <<<\n" RESET);
                j->score++;
                printf("Vous rejouez !\n");
                attendre(2);
                // Pas de changement de joueur
            } else {
                printf("\nRate... Retenez bien les positions.\n");
                attendre(3);
                // Changement de joueur (Sauf si solitaire pur)
                if (choix_menu <= 2) {
                    jeu.joueurActuel = (jeu.joueurActuel + 1) % MAX_JOUEURS;
                }
            }
        } else {
            // Après un Joker, on change toujours de main
            if (choix_menu <= 2) {
                jeu.joueurActuel = (jeu.joueurActuel + 1) % MAX_JOUEURS;
            }
        }
    }

    // --- FIN ---
    printf(BOLD "\n=== FIN DE PARTIE ===\n" RESET);
    // Logique de fin identique (affichage score final)
    // ...
    liberer_jeu(&jeu);
    return 0;
}