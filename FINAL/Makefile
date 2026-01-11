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
	@echo "Commandes disponibles :"
	@echo "  make        - Compile le programme"
	@echo "  make debug  - Compile avec symboles de debug"
	@echo "  make run    - Compile et exécute"
	@echo "  make clean  - Supprime les fichiers compilés"
	@echo "  make help   - Affiche cette aide"

.PHONY: all clean run debug help
