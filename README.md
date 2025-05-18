# Gamified Hangman

This is a cross-platform Hangman game developed in C using SDL2 and SDL2_ttf, as part of a team project for the Algorithms Programming course at the University Politehnica of Bucharest, Faculty of Automatics and Computers.

The game is designed to support learning through gamification and includes precompiled libraries for Linux, macOS, and Windows.

---

## Build and Run

### Linux / macOS

```bash
make
make run
```

### Windows

Double-click:

```bash
build_run_windows.bat
```

---

## Requirements

- GCC and Make (Linux/macOS)
- GCC (MinGW or MSYS2) for Windows

---

## Project Structure

```bash
include/      # SDL2 headers
lib/          # Platform-specific SDL2 libraries
assets/       # Game images
data/         # Game question files
build/        # Output directory
```

---

## Notes

- Compatible with Linux, macOS, and Windows
- No installation required; just build and run
