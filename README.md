# 🎴 MÉMORYX - Jeu de Memory en C

## 📋 Description du Projet

**Mémoryx** est une variante du jeu de mémoire classique (Memory) développée en langage C dans le cadre de la SAE 1.02 (2025/2026). Le but du jeu est de retrouver des paires de cartes identiques sur un plateau.

---

## 🎯 Objectif du Jeu

Le gagnant est celui qui découvre le **plus grand nombre de paires** de cartes identiques.

### Règles de Base

1. Les cartes sont disposées **faces cachées** sur un plateau rectangulaire
2. À chaque tour, un joueur retourne **deux cartes** :
   - ✅ **Paire trouvée** : Le joueur marque **1 point** et **rejoue**
   - ❌ **Pas de paire** : Les cartes sont retournées faces cachées, le joueur suivant joue

---

## 🃏 Particularité : Le Joker

Mémoryx ajoute une carte spéciale : **le Joker** (valeur `0`)

| Événement | Conséquence |
|-----------|-------------|
| Un joueur retourne le Joker | Il **passe son tour** immédiatement |
| Après le passage de tour | Le Joker est **déplacé secrètement** à une autre position aléatoire |

---

## 📐 Structure du Plateau

### Dimensions

Le plateau forme un rectangle de **L lignes × C colonnes** avec les contraintes suivantes :

| Contrainte | Explication |
|------------|-------------|
| `L × C = 2n + 1` | Nombre total de cartes (n paires + 1 Joker) |
| L et C **impairs** | Obligatoire pour avoir un nombre impair de cartes |
| `L = C = 1` interdit | Le cas 1×1 n'est pas autorisé |

### Exemples de Dimensions Valides

| Lignes (L) | Colonnes (C) | Total | Paires |
|------------|--------------|-------|--------|
| 3 | 3 | 9 | 4 |
| 3 | 5 | 15 | 7 |
| 3 | 7 | 21 | 10 |
| 5 | 5 | 25 | 12 |
| 5 | 7 | 35 | 17 |
| 7 | 7 | 49 | 24 |

---

## 🗂️ Structures de Données

### Table des Cartes `T[]`

Table à **une dimension** de taille `L × C` qui contient les valeurs des cartes.

```
Position :  0   1   2   3   4   5   6   7   8   9  10  11  12  13  14  15  16  17  18  19  20
Valeur   :  1   7   9   4  10   2  10   6   9   3   7   8   1   5   2   5   6   3   8   0   4
                                                                                      ↑
                                                                                    Joker
```

### Table des Positions `P[]`

Table utilisée pour le **mélange aléatoire** des cartes.

### Correspondance Position ↔ Coordonnées

| Formule | Description |
|---------|-------------|
| `ligne = position / C` | Calcul de la ligne |
| `colonne = position % C` | Calcul de la colonne |
| `position = ligne × C + colonne` | Calcul de la position |

#### Exemple avec C = 7

La carte en position **11** :
- Ligne : `11 / 7 = 1`
- Colonne : `11 % 7 = 4`
- Donc la carte est en **ligne 1, colonne 4**

---

## 📊 Affichage du Plateau

### Cartes Faces Cachées (Positions)

```
     0    1    2    3    4    5    6
   -----------------------------------
 0 |  #    #    #    #    #    #    #
 1 |  #    #    #    #    #    #    #
 2 |  #    #    #    #    #    #    #
```

### Cartes Faces Visibles (Valeurs)

```
     0    1    2    3    4    5    6
   -----------------------------------
 0 |  1    7    9    4   10    2   10
 1 |  6    9    3    7    8    1    5
 2 |  2    5    6    3    8    0    4
```

### Légende de l'Affichage

| Symbole | Signification |
|---------|---------------|
| `#` | Carte face cachée |
| `.` | Carte retirée (paire trouvée) |
| `0-n` | Valeur de la carte (0 = Joker) |

---

## 🎮 Modes de Jeu Disponibles

### 1. Mode Multijoueur (2-4 joueurs)

- a faire

---

## 🧠 Algorithme de Génération Aléatoire

### Étapes

1. **Création** de la table des positions `P[]` avec les valeurs `0` à `R-1`
2. **Mélange** de `P[]` avec l'algorithme de Fisher-Yates
3. **Remplissage** de la table des cartes `T[]` :
   - Joker en position `P[0]`
   - Carte 1 en positions `P[1]` et `P[2]`
   - Carte 2 en positions `P[3]` et `P[4]`
   - ... et ainsi de suite

### Exemple de Génération

```
Étape 1 - P initial :
[0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20]

Étape 2 - P après mélange :
[19, 12, 0, 14, 5, 9, 17, 20, 3, 13, 15, 16, 7, 10, 1, 11, 18, 8, 2, 6, 4]

Étape 3 - Table T résultante :
- T[19] = 0 (Joker)
- T[12] = 1, T[0] = 1
- T[14] = 2, T[5] = 2
- ...
```

---

## 🔧 Compilation et Exécution

### Prérequis

- Compilateur C (GCC recommandé)
- Terminal/Console

### Compilation

```bash
gcc -o memoryx MEMORYX.c
```

### Exécution

```bash
./memoryx
```

---

## 📁 Structure des Fichiers

```
SAE1.02/
├── MEMORYX.c        # Code source principal du jeu
├── README.md        # Documentation (ce fichier)
├── chrono.c         # Fonctions de chronométrage
├── color.c          # Gestion des couleurs terminal
└── nb_aleatoire.c   # Générateur de nombres aléatoires
```

---

## 🛠️ Dépendances et Bibliothèques

### Bibliothèques Standard C

| Bibliothèque | Utilisation |
|--------------|-------------|
| `<stdio.h>` | Entrées/sorties (printf, scanf) |
| `<stdlib.h>` | Allocation mémoire (malloc, free), rand() |
| `<string.h>` | Manipulation de chaînes (strcpy, strcspn) |
| `<time.h>` | Initialisation du générateur aléatoire (srand) |

### Fichiers Auxiliaires (Optionnels)

| Fichier | Description |
|---------|-------------|
| `chrono.c` | Mesure du temps de jeu |
| `color.c` | Affichage coloré dans le terminal |
| `nb_aleatoire.c` | Fonctions personnalisées pour l'aléatoire |

---

## 📝 Structures du Code

### Structure Joueur

```c
typedef struct {
    char pseudo[50];    // Nom du joueur
    int score;          // Nombre de paires trouvées
    int estBot;         // 1 si c'est le bot, 0 sinon
} Joueur;
```

### Structure Mémoire (pour le Bot)

```c
typedef struct {
    int position;       // Position de la carte vue
    int valeur;         // Valeur de la carte
} Memoire;
```

---

## 🎲 Déroulement d'une Partie

```
┌─────────────────────────────────────┐
│  1. Choix du mode de jeu            │
├─────────────────────────────────────┤
│  2. Configuration du plateau (L×C)  │
├─────────────────────────────────────┤
│  3. Saisie des pseudos              │
├─────────────────────────────────────┤
│  4. Génération aléatoire du plateau │
├─────────────────────────────────────┤
│  5. Boucle de jeu :                 │
│     - Affichage du plateau          │
│     - Choix de 2 cartes             │
│     - Vérification paire/Joker      │
│     - Mise à jour des scores        │
├─────────────────────────────────────┤
│  6. Fin de partie (1 carte restante)│
├─────────────────────────────────────┤
│  7. Affichage du gagnant            │
└─────────────────────────────────────┘
```

---

## 💡 Exemple de Partie

```
╔═══════════════════════════════════════╗
║         BIENVENUE DANS MÉMORYX        ║
╠═══════════════════════════════════════╣
║  1. Mode Multijoueur (2-4 joueurs)    ║
║  2. Mode Solitaire                    ║
║  3. Jouer contre le Bot               ║
║  4. Afficher les règles               ║
║  5. Quitter                           ║
╚═══════════════════════════════════════╝
Votre choix : 1

=== MODE MULTIJOUEUR ===
Entrez le nombre de lignes (impair, >= 3) : 3
Entrez le nombre de colonnes (impair, >= 3) : 5
Nombre de joueurs (2 à 4) : 2
Pseudo du joueur 1 : Alice
Pseudo du joueur 2 : Bob

===== Tour de Alice (Score: 0) =====

     0    1    2    3    4
   -------------------------
 0 |  #    #    #    #    #
 1 |  #    #    #    #    #
 2 |  #    #    #    #    #

Alice, entrez une position (0 à 14) : 3

Carte en position 3 : 5

     0    1    2    3    4
   -------------------------
 0 |  #    #    #    5    #
 1 |  #    #    #    #    #
 2 |  #    #    #    #    #

Alice, entrez une position (0 à 14) : 11

Carte en position 11 : 5

*** PAIRE TROUVÉE ! Alice marque 1 point ! ***
Alice rejoue !
```

---

## 🔢 Formules Mathématiques

### Calcul du nombre de paires

$$n = \frac{L \times C - 1}{2}$$

### Conversion position → coordonnées

$$\text{ligne} = \lfloor \frac{\text{position}}{C} \rfloor$$

$$\text{colonne} = \text{position} \mod C$$

### Conversion coordonnées → position

$$\text{position} = \text{ligne} \times C + \text{colonne}$$

---

## 👨‍💻 Auteur

Projet réalisé dans le cadre de la **SAE 1.02 - Initiation au développement**

IUT de Metz - Département Informatique - 2025/2026

---

## 📜 Licence

Projet éducatif - Usage académique uniquement
