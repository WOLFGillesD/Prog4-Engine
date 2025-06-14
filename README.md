# Prog4-Engine

Project for programming 4 course of DAE Game Development.

Link: https://github.com/WOLFGillesD/Prog4-Engine

# Controls
## Controller
* D-Pad to move.
* D-Pad to move in menu A to select.
* X to inflict damage.
* A and B to pickup pallets.

## Keyboard
* WASD to move.
* WS to move  in menu, space to select

# Sound
* Background music starts playing on level startup

# Engine

- **Component-Based Design**: Game objects are composed from reusable `Component` classes (movement, rendering, health, AI), keeping systems decoupled and easy to extend.

- **Grid System**: `GridComponent` models the level as a 2D grid of tiles (with 5×5 subcells) for collision, digging, and pathfinding.

- **Movement**: `MovementComponent` handles smooth, cardinal-direction movement for both player and AI, with support for full-cell stepping.

- **AI Modules**: Enemies (`Nobbin`, `Hobbin`) use simple, pluggable AI components. `Hobbin` can tunnel through dirt, reusing the same movement interface.

- **Event-Driven Health & Scoring**: `HealthComponent` raises events on death, and AI components award points via `ScoreComponent` observers.

- **Enemy Spawner**: `EnemySpawner` limits active enemies, controls spawn timing, and randomly chooses enemy types with configurable probabilities.

