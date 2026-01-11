# 🎴 MÉMORYX - VERSION FINALE

## SAE 1.02 / 1.2 - IUT de Metz - 2025/2026

Cette version est la **fusion complète et stable** de toutes les versions précédentes, 
entièrement conforme aux exigences du sujet.

---

## ✅ Fonctionnalités implémentées

### Conformité au sujet

| Exigence | Statut |
|----------|--------|
| L et C impairs, L×C ≥ 3 | ✅ |
| Cas L=C=1 interdit | ✅ |
| Table T (valeurs des cartes) | ✅ |
| Table P (positions restantes) | ✅ |
| Joker = 0 | ✅ |
| Carte retirée = -1 | ✅ |
| Mélange Fisher-Yates | ✅ |
| Joker permute via T[P[r]] | ✅ |
| P inchangé après Joker | ✅ |
| P réduit après paire trouvée | ✅ |
| Coordonnées base 0 | ✅ |
| Allocation dynamique | ✅ |

### Modes de jeu

| Mode | Description |
|------|-------------|
| 1 | Duel : Humain vs Humain |
| 2 | Duel : Humain vs Bot |
| 3 | Solitaire : Humain seul |
| 4 | Solitaire : Bot seul (Demo) |
| 0 | Toggle Mode Triche (on/off) |
| 5 | Quitter |

### Fonctionnalités bonus

- 🎨 **Couleurs ANSI** pour un affichage amélioré
- ⏱️ **Chronomètre** en mode solitaire
- 🔢 **Compteur de coups** 
- 🤖 **Bot intelligent** avec mémoire des cartes vues
- 🔀 **Mode Triche** activable/désactivable

---

## 📁 Structure des fichiers

```
FINAL/
├── memoryx.h           # Header avec structures et prototypes
├── memoryx_fonctions.c # Implémentation des fonctions
├── memoryx_main.c      # Programme principal (main)
├── memoryx             # Exécutable compilé
├── Makefile            # Automatisation de la compilation
└── README.md           # Ce fichier
```

---

## 🔧 Compilation et exécution

### Avec Make
```bash
make        # Compile le programme
make run    # Compile et exécute
make clean  # Nettoie les fichiers compilés
```

### Sans Make
```bash
gcc -Wall -Wextra -o memoryx memoryx_main.c memoryx_fonctions.c
./memoryx
```

---

## 🎮 Règles du jeu

1. Le plateau contient **n paires** (valeurs 1 à n) et **1 Joker** (valeur 0)
2. À chaque tour, le joueur retourne **2 cartes**
3. Si les cartes sont **identiques** : +1 point, cartes retirées, le joueur **rejoue**
4. Si les cartes sont **différentes** : les cartes sont recachées, tour suivant
5. Si le **Joker** est retourné : le joueur passe, le Joker est déplacé aléatoirement

### Formules

- Nombre de paires : $n = \frac{L \times C - 1}{2}$
- Position → Coordonnées : `ligne = k / C`, `colonne = k % C`
- Coordonnées → Position : `k = ligne × C + colonne`

---

## 🤖 Stratégie du Bot

Le bot utilise une **mémoire des cartes vues** et applique cette stratégie :

1. **Chercher une paire connue** dans sa mémoire
2. Si aucune paire connue : **tirer une carte au hasard**
3. Après la première carte : **chercher si la paire est en mémoire**
4. Sinon : **tirer une deuxième carte au hasard**

Le bot **ne mémorise pas le Joker** (car il se déplace).

---

## 📊 Exemple de plateau

### Cartes cachées
```
       0    1    2    3    4 
     +------------------------+
   0 |  .    .    .    .    . |
   1 |  .    .    .    .    . |
   2 |  .    .    .    .    . |
     +------------------------+
```

### Mode Triche (toutes visibles)
```
      *** MODE TRICHE ***
       0    1    2    3    4 
     +------------------------+
   0 |  1    7    9    4   10 |
   1 |  6    9    3    7    8 |
   2 |  2    5    6   JK    4 |
     +------------------------+
```

---

## 👨‍💻 Auteur

Projet réalisé dans le cadre de la **SAE 1.02 / 1.2 - Initiation au développement**

IUT de Metz - Département Informatique - 2025/2026
