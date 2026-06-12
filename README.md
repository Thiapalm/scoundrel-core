# ScoundrelCore

[![C++20](https://img.shields.io/badge/C++-20-blue.svg)](https://en.wikipedia.org/wiki/C%2B%2B20)
[![CMake](https://img.shields.io/badge/CMake-3.16+-green.svg)](https://cmake.org/)

**ScoundrelCore** is a C++20 library that implements the core logic and mechanics of **Scoundrel**, a card-based dungeon crawler game. This library is designed to be the "brain" of the game, providing all the rules and mathematical logic while remaining agnostic to the final user interface (TUI, Mobile, Web, etc.).

Based on the original game design by **Zach Gage** and **Kurt Bieg**.

---

## 🎮 The Game: Scoundrel

Scoundrel is a survival strategy game played with a standard 52-card deck. Your goal is to navigate through a dungeon full of dangers and make it out alive.

### Core Mechanics
1.  **The Dungeon:** The deck represents the dungeon, containing monsters, weapons, and potions.
2.  **The Rooms:** You encounter the dungeon in "rooms" of **4 cards** at a time.
3.  **Your Choices:** In each room, you must deal with the cards (fight monsters, equip weapons, or drink potions). You can skip a room if it's too dangerous, but you cannot skip twice in a row!
4.  **Health (HP):** You start with 20 HP. If your HP reaches 0, it's game over.

### Card Types
*   **👹 Monsters (Clubs & Spades):** Deal damage to the player. You can fight them barehanded (losing HP) or use a weapon.
*   **⚔️ Weapons (Diamonds):** Help defeat monsters with less HP loss. Weapons have "memory" and can only defeat monsters weaker than the previous one they killed.
*   **🧪 Potions (Hearts):** Restore your HP (up to a maximum of 20).

---

## 🛠️ Key Features

- **Pure Logic Engine:** Decoupled from rendering, making it easy to port to any platform.
- **Character Classes:** Play as a **Warrior**, **Healer**, or **Peasant**, each with unique passive abilities.
- **Internationalization (i18n):** Built-in support for multiple languages (English, Portuguese, Spanish).
- **Persistence System:** Framework for saving/loading game state and leaderboards.
- **Cross-Platform:** Compiles on Linux, Windows, and Android.

---

## 🚀 Getting Started

### Prerequisites
- A **C++20** compatible compiler (GCC, Clang, or MSVC).
- **CMake** 3.16 or higher.

### Building the Library
```bash
mkdir build-core
cd build-core
cmake .. -DBUILD_WITH_SDL=OFF
cmake --build .
```
For more detailed instructions, see [BUILD.md](BUILD.md).

---

## 💻 Developer Integration

ScoundrelCore provides the mathematical model and game rules. You only need to build the UI layer.

### Basic Usage Example
```cpp
#include "GameLogic.h"
#include "ui/UserInterface.h"

// 1. Create your Interface
class MyInterface : public UserInterface {
    void display_room(const GameContext& ctx) override {
        // Code to show cards on screen
    }
};

int main() {
    // 2. Initialize the Game Engine
    auto game = GameLogic::create(GameType::DEFAULT);
    
    // 3. Connect your Interface
    MyInterface ui;
    game->set_interface(&ui);
    
    // 4. Start the adventure!
    game->run_game_loop();
    
    return 0;
}
```
Check the [USAGE.md](USAGE.md) for a deeper dive into the API.

---

## 📂 Project Structure

- `src/`: Main source code.
- `src/actions/`: Logic for specific game actions (Attack, Equip, etc.).
- `src/assets/`: Data structures for Cards, Dungeon, Player, etc.
- `src/classes/`: Character class definitions.
- `tests/`: Unit tests using **GoogleTest**.
- `assets/locales/`: Localization files (JSON).

---

## 🧪 Testing
The project uses GoogleTest for verification. To run the tests:
```bash
cd build-core
ctest
```

---

## 📜 Credits
ScoundrelCore is an implementation of the Scoundrel game rules designed by **Zach Gage** and **Kurt Bieg**.
