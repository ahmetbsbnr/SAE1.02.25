# 🎴 MÉMORYX

## SAE 1.02 - IUT de Metz - Département Informatique - 2025/2026

---

## 📝 Description

MÉMORYX est une variante du jeu Memory classique avec une carte **Joker** qui ajoute une dimension stratégique au jeu. Quand le Joker est retourné, il est permuté aléatoirement avec une autre carte, rendant le jeu plus imprévisible !

---

## 🎯 Règles du jeu

1. Les cartes sont disposées faces cachées sur un plateau de L × C cases
2. Un joueur retourne deux cartes :
   - **Si identiques** : il marque 1 point et rejoue
   - **Sinon** : les cartes sont retournées face cachée, joueur suivant
3. **Le Joker (valeur 0)** : quand il est retourné, le joueur passe son tour et le Joker est échangé avec une autre carte aléatoire
4. Le gagnant est celui qui a trouvé le plus de paires

---

## 📁 Structure du projet

```
SAE1.02.25/
├── MEMORYX.c      # Programme principal (main, menu, boucle de jeu)
├── fonctions.h    # Prototypes + structures + constantes
├── fonctions.c    # Toutes les fonctions du jeu
├── test/          # Fichiers de test
└── README.md      # Documentation
```

---

## 🔧 Compilation et Exécution

```bash
gcc -o MEMORYX MEMORYX.c fonctions.c -Wall
./MEMORYX
```

---

## 🎮 Modes de jeu

| Mode | Description |
|------|-------------|
| **Duel** | Joueur 1 vs Joueur 2 (ou vs Bot) |
| **Solitaire** | 1 joueur seul ou Bot |

---

## 📐 Paramètres

- **L** : Nombre de lignes (impair, 3-21)
- **C** : Nombre de colonnes (impair, 3-21)
- Nombre de cartes = L × C = 2n + 1 (n paires + 1 Joker)

---

## 🔢 Structures de données

### Table des cartes T (taille L × C)

```c
int T[MAX_CARTES];  /* Valeurs des cartes */
```

- `T[k]` = valeur de la carte en position k
- `T[k] = 0` → Joker
- `T[k] = -1` → Carte retirée
- `T[k] = 1, 2, 3...` → Valeur de la carte

### Table des positions P (taille R)

```c
int P[MAX_CARTES];  /* Positions des cartes restantes */
```

- `P[i]` = position de la i-ème carte restante
- R = nombre de cartes restantes (décrémente quand paires trouvées)

### Conversion position ↔ coordonnées

```c
/* Position → Ligne, Colonne */
ligne = pos / C;
colonne = pos % C;

/* Ligne, Colonne → Position */
pos = ligne * C + colonne;
```

### Structure Joueur

```c
typedef struct {
    char pseudo[50];   /* Nom du joueur */
    int score;         /* Nombre de paires trouvées */
    int estBot;        /* 1 si bot, 0 sinon */
} Joueur;
```

### Structure MemoireBot

```c
typedef struct {
    int position;      /* Position de la carte */
    int valeur;        /* Valeur mémorisée */
} CarteMemoire;

typedef struct {
    CarteMemoire cartes[MAX_CARTES];
    int nbCartes;      /* Nombre de cartes mémorisées */
} MemoireBot;
```

### Structure Partie

```c
typedef struct {
    int T[MAX_CARTES];     /* Table des cartes */
    int P[MAX_CARTES];     /* Table des positions */
    int L, C;              /* Dimensions */
    int R;                 /* Cartes restantes */
    int coups;             /* Nombre de coups */
    int tour;              /* Numéro du tour */
    time_t tempsDebut;     /* Chronomètre (solitaire) */
    Joueur joueurs[2];
    int nbJoueurs;
    int joueurActuel;
    MemoireBot memoire;
} Partie;
```

---

## 🎲 Algorithme de génération aléatoire

### Étape 1 : Initialiser P

```c
for (i = 0; i < R; i++) {
    P[i] = i;  /* P = {0, 1, 2, ..., R-1} */
}
```

### Étape 2 : Mélanger P (permutation aléatoire)

```c
for (i = 0; i < R; i++) {
    j = aleatoire(0, R - 1);
    echanger(P[i], P[j]);
}
```

### Étape 3 : Remplir T avec P mélangé

```c
T[P[0]] = JOKER;           /* Joker en position P[0] */

carte = 1;
for (i = 1; i < R; i += 2) {
    T[P[i]] = carte;       /* Première carte de la paire */
    T[P[i+1]] = carte;     /* Deuxième carte de la paire */
    carte++;
}
```

### Étape 4 : Réinitialiser P

```c
for (i = 0; i < R; i++) {
    P[i] = i;  /* P redevient {0, 1, 2, ..., R-1} */
}
```

---

## 🃏 Gestion du Joker

Quand un joueur retourne le Joker en position `posJoker` :

```c
r = aleatoire(0, R - 1);     /* r aléatoire parmi les cartes restantes */
posAlea = P[r];              /* Position de la carte à échanger */

/* Permutation de T[posJoker] et T[posAlea] */
temp = T[posJoker];
T[posJoker] = T[posAlea];
T[posAlea] = temp;
```

**Note** : On utilise `P[r]` pour garantir que l'échange se fait avec une carte encore présente sur le plateau.

---

## 🗑️ Retrait d'une paire

Quand une paire est trouvée en positions `pos1` et `pos2` :

```c
/* Marquer les cartes comme retirées dans T */
T[pos1] = -1;
T[pos2] = -1;

/* Retirer ces positions de P */
j = 0;
for (i = 0; i < R; i++) {
    if (P[i] != pos1 && P[i] != pos2) {
        temp[j++] = P[i];
    }
}
/* Mettre à jour P et R */
P = temp;
R = j;  /* R diminue de 2 */
```

---

## 🤖 Stratégie du Bot

### 1. Le bot ne triche pas

- Il n'a **pas accès** directement à la table T
- Il utilise uniquement la table P pour choisir une position
- Il mémorise les cartes **quand elles sont retournées**

### 2. Structure de la mémoire

```c
MemoireBot memoire;
memoire.cartes[i].position = pos;  /* Où est la carte */
memoire.cartes[i].valeur = val;    /* Quelle valeur */
```

### 3. Algorithme de jeu

```
SI paire trouvée dans mémoire ALORS
    jouer les deux positions de cette paire
SINON
    SI carte connue dans mémoire avec même valeur que carte 1 ALORS
        jouer cette position
    SINON
        choisir aléatoirement parmi P
    FIN SI
FIN SI
```

### 4. Le bot peut se tromper

Le Joker peut avoir bougé → la mémoire devient incorrecte !

```c
/* Le bot oublie une carte quand le Joker bouge */
if (val == JOKER) {
    oublierCarte(&memoire, pos);
}
```

---

## 🎨 Affichage

- **Tableau TRICHE** : toutes les cartes visibles (debug)
- **Tableau de jeu** : cartes cachées `?`, sélectionnées en bleu
- **Délai** : 5 secondes entre chaque action
- **Scores** : affichés après chaque coup
- **Temps** : affiché en mode solitaire (format MM:SS)

---

## 📊 Fonctions principales

| Fonction | Description |
|----------|-------------|
| `initialiserPlateau()` | Génère le plateau avec l'algorithme du sujet |
| `afficherPlateau()` | Affiche le plateau avec cartes sélectionnées |
| `permuterJoker()` | Échange le Joker avec une carte aléatoire |
| `retirerPaire()` | Retire une paire de T et P |
| `saisirPosition()` | Saisie et validation des coordonnées |
| `tourHumain()` | Gère le tour d'un joueur humain |
| `tourBot()` | Gère le tour du bot |
| `memoriserCarte()` | Ajoute une carte à la mémoire du bot |
| `oublierCarte()` | Retire une carte de la mémoire du bot |
| `chercherPaire()` | Cherche une paire dans la mémoire |
| `afficherScores()` | Affiche scores + temps |
| `afficherGagnant()` | Affiche le vainqueur en fin de partie |

---

## 👥 Auteurs

- [Ahmet]

---

## 📅 Date de soutenance

Semaine du **2026**

---

*IUT de Metz - Département Informatique*
