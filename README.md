# 🎴 MÉMORYX - SAE 1.02

Jeu de mémoire en C - SAE 1.02 / 2025-2026 - IUT de Metz

---

## 📁 Structure du projet

```
/workspaces/SAE1.02.25/
├── memoryx.c       # Programme principal (main)
├── memoryx.h       # Header principal avec structures
├── tableau.c/h     # Fonctions plateau (preparer_jeu, initialiser_plateau...)
├── bot.c/h         # Intelligence artificielle du bot
├── affichage.c/h   # Fonctions d'affichage (plateau, scores, bannière)
├── utils.c/h       # Utilitaires (attendre, effacer_ecran, saisir_position)
├── DEV/            # Anciennes versions et fichiers de développement
└── README.md       # Ce fichier
```

---

## 🔧 Compilation et exécution

```bash
gcc memoryx.c tableau.c bot.c affichage.c utils.c -o memoryx
./memoryx
```

---

## 🎯 Objectifs et modes

- **Duel** : gagner en découvrant le plus de paires.
- **Solitaire** : trouver toutes les paires en un minimum de temps.
- Modes disponibles :
  - `1` Humain vs Humain
  - `2` Humain vs Bot
  - `3` Solitaire (Humain seul)
  - `4` Solitaire Bot (Demo)
  - `0` Mode Triche (affiche toutes les cartes)
  - `5` Quitter

---

## ⚙️ Paramètres et contraintes du plateau

- Plateau de **L lignes** × **C colonnes** avec `L` et `C` impairs, `L × C = 2n + 1` (n paires + 1 Joker), cas `1×1` interdit.
- Nombre de paires : $n = \frac{L \times C - 1}{2}$.
- Conversion position ↔ coordonnées (stockage 1D) :
  - ligne = position div C
  - colonne = position mod C
  - position = ligne × C + colonne

### Exemples de dimensions valides

| L | C | Total | Paires |
|---|---|-------|--------|
| 3 | 3 | 9 | 4 |
| 3 | 5 | 15 | 7 |
| 3 | 7 | 21 | 10 |
| 5 | 5 | 25 | 12 |
| 5 | 7 | 35 | 17 |
| 7 | 7 | 49 | 24 |

---

## 🗂️ Structures de données imposées

### Table des cartes `T` (taille `L×C`)

- Valeurs : `0` (Joker), `1..n` (chaque valeur deux fois), `-1` (carte retirée après paire trouvée).
- Exemple (C = 7) :

```
Position :  0   1   2   3   4   5   6   7   8   9  10  11  12  13  14  15  16  17  18  19  20
Valeur   :  1   7   9   4  10   2  10   6   9   3   7   8   1   5   2   5   6   3   8   0   4
                                                                                      ↑
                                                                                    Joker
```

### Table des positions `P`

- Contient les indices `0..(L×C-1)` mélangés (Fisher-Yates).
- Placement : Joker en `T[P[0]]`, carte 1 en `T[P[1]]` et `T[P[2]]`, carte 2 en `T[P[3]]` et `T[P[4]]`, etc.
- Mise à jour : lorsqu'une paire est trouvée, ses indices sont supprimés de `P` (taille logique diminue).

---

## 🧠 Algorithme de génération du plateau

1) Générer `P` = [0..R-1].
2) Mélanger `P` (Fisher-Yates).
3) Remplir `T` : Joker puis paires successives selon `P`.

Exemple :

```
P initial  : [0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20]
P mélangé  : [19, 12, 0, 14, 5, 9, 17, 20, 3, 13, 15, 16, 7, 10, 1, 11, 18, 8, 2, 6, 4]
Remplissage: T[19]=0, T[12]=1, T[0]=1, T[14]=2, T[5]=2, ...
```

---

## 🎮 Déroulement d'un tour

1) Le joueur choisit une première position (carte visible).
2) Si c'est le Joker (`0`) : le tour s'arrête, permutation Joker (voir ci-dessous), joueur suivant.
3) Sinon, choix d'une seconde carte.
4) Si la seconde carte est le Joker : permutation et fin de tour.
5) Comparaison :
   - Paires identiques : score +1, cartes mises à `-1` dans `T` et retirées de `P`, le joueur rejoue.
   - Différentes : cartes visibles 5 s puis recachées, tour suivant.

### Algorithme du Joker

- Joker retourné en position `i` :
  1. Le joueur passe.
  2. Tirage `r` aléatoire dans les cartes restantes (`0..R-1`).
  3. Permutation de `T[i]` (Joker) avec `T[r]`.
  4. `P` est **inchangé**.

---

## 🤖 Bot (IA sans triche)

- Pas d'accès direct à `T` caché : il joue avec sa propre mémoire des cartes vues.
- Mémoire : (position, valeur) ajoutée lors des révélations ; suppression lors d'une paire validée ; correction possible si Joker a déplacé une carte.
- Stratégie :
  1. Si deux positions connues identiques : les jouer en priorité.
  2. Sinon, choix prudent tenant compte des infos possiblement périmées par le Joker.
  3. Par défaut, tirage aléatoire contrôlé.

### Structures minimales

```c
typedef struct {
    char pseudo[50];
    int score;
    int estBot; // 1 si bot
} Joueur;

typedef struct {
    int position;
    int valeur;
} Memoire;
```

---

## 📊 Affichage du plateau

### Menu principal

```
 __  __  ______  __  __   ____   _____  __   __ __   __
|  \/  ||  ____||  \/  | / __ \ |  __ \ \ \ / / \ \ / /
| \  / || |__   | \  / || |  | || |__) | \ V /   \ V / 
| |\/| ||  __|  | |\/| || |  | ||  _  /   | |     > <  
| |  | || |____ | |  | || |__| || | \ \   | |    / . \ 
|_|  |_||______||_|  |_| \____/ |_|  \_\  |_|   /_/ \_\
             SAE 1.2 - IUT DE METZ

=== CONFIGURATION ===
Choisissez votre mode de jeu :
  0. Activer le Mode Triche
  1. Duel : Humain vs Humain
  2. Duel : Humain vs Bot
  3. Solitaire : Humain seul
  4. Solitaire : Bot seul (Demo)
  5. Quitter
```

### Cartes cachées

```
       0  1  2  3  4
     +---------------+
   0 | .  .  .  .  . |
   1 | .  .  .  .  . |
   2 | .  .  .  .  . |
     +---------------+
```

### Cartes visibles (Mode Triche)

```
      [TRICHE]
       0  1  2  3  4
     +---------------+
   0 |  1  7  9  4 10 |
   1 |  6  9  3  7  8 |
   2 |  2  5  6  0  4 |
     +---------------+
```

Légende : `.` carte cachée, ` ` carte retirée, `0` Joker (rouge), `1..n` valeurs des paires (jaune)

---

## 🔧 Compilation et exécution

- Prérequis : GCC (ou équivalent) et terminal.
- Compilation :

```bash
gcc MEMORYX.c fonction.c -o memoryx
```

- Exécution :

```bash
./memoryx
```

Au lancement : menu de configuration avec choix du mode, option triche (activable/désactivable), puis saisie de `L` et `C` (impairs).

---

## 📁 Organisation du dépôt

```
SAE1.02.25/
├── MEMORYX.c        # Code principal du jeu (main)
├── fonction.c       # Fonctions du jeu
├── fonction.h       # Header avec structures et prototypes
├── memoryx          # Exécutable compilé
├── memoryx_25.pdf   # Sujet du projet
├── README.md        # Ce document
└── SAUV/            # Sauvegardes des anciennes versions
    ├── MEMORYXcopy.c
    ├── fonctioncopy.c
    └── fonctioncopy.h
```

---

## 🔢 Formules utiles

- Paires : $n = \frac{L \times C - 1}{2}$
- Coordonnées : $\text{ligne} = \lfloor \frac{\text{position}}{C} \rfloor$, $\text{colonne} = \text{position} \bmod C$
- Position : $\text{position} = \text{ligne} \times C + \text{colonne}$

---

## 👨‍💻 Auteur

Projet réalisé dans le cadre de la **SAE 1.02 / 1.2 - Initiation au développement**

IUT de Metz - Département Informatique - 2025/2026

---

## 📜 Licence

Projet éducatif - Usage académique uniquement
