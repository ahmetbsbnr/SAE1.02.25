/**
 * @file tableau.h
 * @brief Fonctions de gestion du tableau/plateau de jeu
 */

#ifndef TABLEAU_H
#define TABLEAU_H

/* --- Initialisation et libération --- */
void preparer_jeu(Jeu *jeu, int L, int C);
void initialiser_plateau(Jeu *jeu);
void liberer_jeu(Jeu *jeu);

/* --- Conversion coordonnées (Section 3 du sujet) --- */
void convertir_k_en_ij(int k, int C, int *i, int *j);
int convertir_ij_en_k(int i, int j, int C);

/* --- Logique du jeu (Sections 5 et 6) --- */
void gestion_joker(Jeu *jeu, int positionJoker);
int verifier_paire(Jeu *jeu, int p1, int p2);
int est_dans_P(Jeu *jeu, int pos);

#endif /* TABLEAU_H */
