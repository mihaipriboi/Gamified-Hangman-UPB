# compilator
CC = gcc

# flaguri de compilare
CFLAGS = -Wall -Wextra -Werror -Iinclude
OBJDIR = build
SRCDIR = src
BINARY = $(OBJDIR)/hangman

# sursa
SRC = $(wildcard $(SRCDIR)/*.c)

# headere
HEADERS = $(wildcard include/*.h)

# determinare OS
UNAME := $(shell uname)

ifeq ($(UNAME), Linux)
  LIBDIR = lib/linux
  LDFLAGS = -L$(LIBDIR) -lSDL2 -lSDL2_ttf
  RUN_CMD = LD_LIBRARY_PATH=$(LIBDIR) ./$(BINARY)
endif

ifeq ($(UNAME), Darwin)
  LIBDIR = lib/macos
  LDFLAGS = $(LIBDIR)/libSDL2.dylib $(LIBDIR)/libSDL2_ttf.dylib
  RUN_CMD = DYLD_LIBRARY_PATH=$(LIBDIR) ./$(BINARY)
endif

# target default
all: $(BINARY)

# compilare
$(BINARY): $(SRC) $(HEADERS)
	mkdir -p $(OBJDIR)
	$(CC) $(CFLAGS) $(SRC) -o $(BINARY) $(LDFLAGS)

# executabil
run: $(BINARY)
	$(RUN_CMD)

# curatare
clean:
	rm -rf $(OBJDIR)
