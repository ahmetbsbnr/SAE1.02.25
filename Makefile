# ============================================================
# Makefile pour MÉMORYX - SAE 1.02 - IUT Metz
# ============================================================

CC = gcc
CFLAGS = -Wall -Wextra -std=c99
TARGET = memoryx

# Fichiers sources
SRCS = memoryx.c tableau.c bot.c affichage.c utils.c
OBJS = $(SRCS:.c=.o)

# Règle par défaut
all: $(TARGET)

# Compilation de l'exécutable
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

# Compilation des fichiers objets
%.o: %.c memoryx.h tableau.h bot.h affichage.h utils.h
	$(CC) $(CFLAGS) -c $< -o $@

# Nettoyage
clean:
	rm -f $(OBJS) $(TARGET)

# Rebuild complet
rebuild: clean all

# Exécution
run: $(TARGET)
	./$(TARGET)

# Aide
help:
	@echo "Commandes disponibles :"
	@echo "  make        - Compile le projet"
	@echo "  make clean  - Supprime les fichiers objets et l'exécutable"
	@echo "  make rebuild- Nettoie et recompile"
	@echo "  make run    - Compile et lance le jeu"
	@echo "  make help   - Affiche cette aide"

.PHONY: all clean rebuild run help
