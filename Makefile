# compilator
CC = gcc

# flags
CFLAGS = -Wall -Wextra -Werror -Iinclude
LDFLAGS = -Llib -lSDL2 -lSDL2_ttf

# fisiere
SRC = main.c
TARGET = hangman

# target default
all: $(TARGET)

# compilare
$(TARGET): $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET) $(LDFLAGS)

# adaugare librarii
run: $(TARGET)
	LD_LIBRARY_PATH=lib ./$(TARGET)

# curatare
clean:
	rm -f $(TARGET)
