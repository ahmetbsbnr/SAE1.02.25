/**
 * @file utils.h
 * @brief Fonctions utilitaires (saisie, attente, effacement écran)
 */

#ifndef UTILS_H
#define UTILS_H

/* --- Saisie utilisateur --- */
int saisir_position(Jeu *jeu);
void vider_buffer(void);

/* --- Utilitaires système --- */
void attendre(int secondes);
void effacer_ecran(void);

#endif /* UTILS_H */
