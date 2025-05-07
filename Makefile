# Compilateur
CC = gcc

# Options de compilation
CFLAGS = -std=c11 -Wall -Wextra

# Noms des exécutables
TARGET_ENIGMA = enigma
TARGET_BRUTEFORCE = bruteforce

# Fichiers sources pour chaque exécutable
SRCS_ENIGMA = wiring.c enigma_rotor.c
SRCS_BRUTEFORCE = bute_force.c enigma_rotor.c

# Fichiers objets pour chaque exécutable
OBJS_ENIGMA = $(SRCS_ENIGMA:.c=.o)
OBJS_BRUTEFORCE = $(SRCS_BRUTEFORCE:.c=.o)

# Règle par défaut
all: $(TARGET_ENIGMA) $(TARGET_BRUTEFORCE)

# Règle pour créer l'exécutable enigma
$(TARGET_ENIGMA): $(OBJS_ENIGMA)
	$(CC) $(CFLAGS) -o $(TARGET_ENIGMA) $(OBJS_ENIGMA)

# Règle pour créer l'exécutable bruteforce
$(TARGET_BRUTEFORCE): $(OBJS_BRUTEFORCE)
	$(CC) $(CFLAGS) -o $(TARGET_BRUTEFORCE) $(OBJS_BRUTEFORCE)

# Règle pour compiler les fichiers objets
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Règle pour nettoyer les fichiers générés
clean:
	rm -f $(OBJS_ENIGMA) $(OBJS_BRUTEFORCE) $(TARGET_ENIGMA) $(TARGET_BRUTEFORCE)

# Règle pour exécuter le programme enigma
run-enigma: $(TARGET_ENIGMA)
	./$(TARGET_ENIGMA)

# Règle pour exécuter le programme bruteforce
run-bruteforce: $(TARGET_BRUTEFORCE)
	./$(TARGET_BRUTEFORCE)

.PHONY: all clean run-enigma run-bruteforce