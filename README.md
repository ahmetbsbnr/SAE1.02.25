# � MÉMORYX

## SAE 1.02 - IUT de Metz - Département Informatique - 2025/2026

### 📝 Description

MÉMORYX est une variante du jeu Memory classique avec une carte **Joker** qui ajoute une dimension stratégique au jeu. Quand le Joker est retourné, il est permuté aléatoirement avec une autre carte, rendant le jeu plus imprévisible !

### 🎯 Règles du jeu

1. Les cartes sont disposées faces cachées sur un plateau de L × C cases
2. Un joueur retourne deux cartes :
   - **Si identiques** : il marque 1 point et rejoue
   - **Sinon** : les cartes sont retournées face cachée, joueur suivant
3. **Le Joker (valeur 0)** : quand il est retourné, le joueur passe son tour et le Joker est échangé avec une autre carte aléatoire
4. Le gagnant est celui qui a trouvé le plus de paires

### 📁 Structure du projet

```
SAE1.02.25/
├── MEMORYX.c      # Programme principal (main)
├── fonctions.h    # Prototypes + structures + constantes
├── fonctions.c    # Toutes les fonctions
└── README.md      # Documentation
```

### 🔧 Compilation

```bash
gcc -o memoryx MEMORYX.c fonctions.c -Wall -Wextra
```

### ▶️ Exécution

```bash
./memoryx
```

### 🎮 Modes de jeu

| Mode | Description |
|------|-------------|
| **Duel** | 2 joueurs s'affrontent (humain vs humain ou humain vs bot) |
| **Solitaire** | 1 joueur seul (avec chronomètre) |

### 📐 Paramètres

- **L** : Nombre de lignes (impair, 3-11)
- **C** : Nombre de colonnes (impair, 3-11)
- Nombre de cartes = L × C = 2n + 1 (n paires + 1 Joker)

### 🤖 Le Bot

Le bot possède une mémoire et mémorise toutes les cartes vues. Sa stratégie :
1. Chercher une paire connue dans sa mémoire
2. Si trouvée → jouer ces positions
3. Sinon → choisir aléatoirement

⚠️ Le bot ne triche pas ! Il n'a pas accès à la table des cartes T.

### 👥 Auteurs

- [Votre nom 1]
- [Votre nom 2]

### 📅 Date de soutenance

Semaine du **05 janvier 2026**

---

*IUT de Metz - Département Informatique*
