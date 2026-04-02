# 🟡 Multiplayer Pac-Man

A couch competitive twist on the classic arcade game.

---

## Features

- Classic Pac-Man gameplay with all four ghosts (Blinky, Pinky, Inky, Clyde)
- Up to 4 simultaneous local players (up to 2 on the same keyboard)
- Player profiles with persistent high scores per level with a leaderboard
- Customizable levels via JSON board files
- Keyboard and gamepad support

---

## Downloads

Pre-built binaries are available on the [Releases](https://github.com/chanuka-williams/Multiplayer-Pac-Man/releases) page.

| Platform              | File                      |
|-----------------------|---------------------------|
| Windows               | `Pac-Man-windows.zip`     |
| macOS (Apple Silicon) | `Pac-Man-macos-arm.zip`   |
| macOS (Intel)         | `Pac-Man-macos-x64.zip`   |
| Linux (GCC)           | `Pac-Man-linux-gcc.zip`   |
| Linux (Clang)         | `Pac-Man-linux-clang.zip` |

### Running on macOS
macOS will block the binary by default since it isn't code-signed. To run it:
```bash
xattr -cr Pac-Man
chmod +x Pac-Man
./Pac-Man
```
If macOS still blocks it, go to **System Settings → Privacy & Security** and click **Open Anyway**.

---

## How to Play

### Creating Profiles
1. On the profile selection menu, select **Create Profile** and enter a username
    - Press `Enter` to activate the text box (it turns blue when active)
    - Press `Enter` again to unselect the textbox
2. Press **Continue** to proceed to the **Main Menu**

### Multiplayer
- Each player needs their own profile
- The currently logged in profile becomes **Player 1**
- **Player 1** should press the join key first - *see below*
- After selecting Play, additional players join by pressing the movement key for their chosen control scheme:
    - Press `W` to join with WASD
    - Press `↑` to join with arrow keys
    - Press the `Middle Left Button` to join with a controller

> ⚠️ **Mouse input is not supported.** All navigation and selection must be done with a keyboard or gamepad — clicking buttons with a mouse will not work.

### Controls

**In-game movement:**

|          | WASD (left side) | Arrow Keys (right side) | Gamepad |
|----------|------------------|-------------------------|---------|
| **Move** | `W` `A` `S` `D`  | `↑` `←` `↓` `→`         | D-Pad   |

**Joining & profile selection** (after selecting Play):

|                      | WASD (left side) | Arrow Keys (right side) | Gamepad              |
|----------------------|------------------|-------------------------|----------------------|
| **Join**             | `w`              | `↑`                     | `Middle Left Button` |
| **Confirm / Select** | `Left Shift`     | `Right Shift`           | A / Cross            |
| **Back / Deselect**  | `Left Ctrl`      | `Right Ctrl`            | B / Circle           |
 
---

## Building from Source

### Prerequisites

- A C++20 compatible compiler (GCC, Clang, or MSVC)
- CMake 3.20 or newer

### Steps

```bash
# 1. Clone the repository
git clone https://github.com/chanuka-williams/Multiplayer-Pac-Man.git
cd Multiplayer-Pac-Man

# 2. Configure
cmake -S . -B build

# 3. Build
cmake --build build

# 4. Run
#   Linux / macOS:
./build/game/Pac-Man
#   Windows:
.\build\game\Pac-Man.exe
```

### Linux Dependencies
On Ubuntu/Debian, install the following before configuring:
```bash
sudo apt-get install -y \
  libasound2-dev libx11-dev libxrandr-dev libxi-dev \
  libgl1-mesa-dev libglu1-mesa-dev libxcursor-dev \
  libxinerama-dev libwayland-dev libxkbcommon-dev
```

---

## Custom Boards

Board layouts are defined as JSON files in `resources/boards/`. Add your own `.json` file there, and it will appear in the map selection menu automatically.

## Contributing
This is still in its early stages so the codebase is still evolving — contributions and improvements are very welcome!