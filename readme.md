# Duo-Othello Game Agent

This repository contains the implementation of a game-playing agent for Duo-Othello, a variant of the classic Reversi/Othello game, developed for CSCI-561: Foundations of Artificial Intelligence, Spring 2024. The game is played on a 12x12 board, a twist from the standard 8x8 board, with specific rules elaborated below.

## Game Description

Duo-Othello extends the classic Reversi rules with the following twists:
- The game is played on a 12x12 board instead of the standard 8x8.
- The initial setup starts with 8 pieces on the board in a specific configuration.
- The game's end is determined not only by the number of pieces but also considers a bonus for the second player to mitigate the first-player advantage.

## Installation

To run this project, you will need a C++17 compliant compiler. Clone the repository to your local machine, and compile the source code using the following command:

```bash
g++ -std=c++17 -o homework homework.cpp
```

## Usage

Execute the compiled program without any command-line arguments. The program reads the game state from `input.txt`, performs the game logic, and writes the chosen move to `output.txt`.

```bash
./homework
```

## Input Format

The `input.txt` file consists of:
- The first line indicating the player to move (`X` or `O`).
- The second line showing the remaining time for both players.
- The next 12 lines represent the game board state.

Example:
```
X
300 286.46
............
............
..XO........
..OX........
............
............
........O...
........OO..
........OX..
............
............
```

## Output Format

The `output.txt` file will contain a single line with the chosen move in the format `columnrow` (e.g., `c2`).

## Game Rules

Please refer to the [Reversi Wikipedia page](http://en.wikipedia.org/wiki/Reversi) for the basic game rules. The specific rules and adjustments for Duo-Othello, including the initial setup and scoring system, are detailed within the project instructions.

## Contributing

This project is an academic assignment. Contributions, collaboration, or code sharing is strictly against the course's academic honesty policy.

## License

This project is proprietary and confidential. Unauthorized copying of files, via any medium, is strictly prohibited.

## Acknowledgments

Thanks to the CSCI-561 staff for providing the project guidelines.
