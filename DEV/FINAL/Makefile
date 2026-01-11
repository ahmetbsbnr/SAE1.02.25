# ============================================================
# MÉMORYX - SAE 1.02 - IUT Metz
# Makefile pour la version finale
# ============================================================

CC = gcc
CFLAGS = -Wall -Wextra -std=c99
TARGET = memoryx
SRCS = memoryx_main.c memoryx_fonctions.c
OBJS = $(SRCS:.c=.o)
HEADER = memoryx.h

# Règle par défaut
all: $(TARGET)

# Compilation du programme
$(TARGET): $(SRCS) $(HEADER)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRCS)

# Compilation avec symboles de debug
debug: CFLAGS += -g -DDEBUG
debug: $(TARGET)

# Nettoyage
clean:
	rm -f $(TARGET) $(OBJS)

# Exécution
run: $(TARGET)
	./$(TARGET)

# Aide
help:
	@echo "Cibles disponibles:"
	@echo "  all   - Compile le jeu (par défaut)"
	@echo "  debug - Compile avec symboles de debug"
	@echo "  clean - Nettoie les fichiers générés"
	@echo "  run   - Compile et lance le jeu"
	@echo "  help  - Affiche cette aide"

.PHONY: all debug clean run help
