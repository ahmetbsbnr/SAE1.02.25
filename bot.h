/**
 * @file bot.h
 * @brief Intelligence Artificielle du Bot (Section 8 du sujet)
 */

#ifndef BOT_H
#define BOT_H

/* --- Gestion de la mémoire du bot --- */
void bot_initialiser(MemoireBot *mem, int capacite);
void bot_memoriser(MemoireBot *mem, int position, int valeur);
void bot_oublier_carte(MemoireBot *mem, int position);
void bot_oublier_paire(MemoireBot *mem, int valeur);

/* --- Stratégie du bot --- */
int bot_chercher_paire(MemoireBot *mem, Jeu *jeu, int *p1, int *p2);
int bot_chercher_valeur(MemoireBot *mem, Jeu *jeu, int valeur, int exclure);
void bot_jouer_tour(Jeu *jeu, int *pos1, int *pos2);

#endif /* BOT_H */
