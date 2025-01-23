# MorpinOnFire

A Tic-Tac-Toe game with both an online multiplayer mode and a single-player mode against an AI. The project is structured in two main parts: an engine that handles the server, graphical interface, and networking, and a game module that manages the game logic, including the Minimax algorithm for the AI in solo mode.

## Features

- **Single-player mode**: Play against an AI using the Minimax algorithm to determine the best moves.
- **Multiplayer mode**: Connect with another player online via a central server.
- **Player customization**: Choose player symbols and colors.
- **Real-time chat**: Communicate with your opponent during the game.
- **Turn management**: Players have a limited time to make their move.

## Technologies Used

- **C++** for core programming
- **WinSock2** for networking (UDP)
- **SFML** for the graphical interface
- **Minimax algorithm** for AI in single-player mode

## Getting Started

To get a local copy up and running, follow these simple steps.

### Prerequisites

- C++ compiler (e.g., GCC, Clang, or Visual Studio)
- WinSock2 library for networking (Windows)
- SFML library for graphics
- CMake (optional, for building the project)

### Installation

1. Clone the repository:
```bash  
   git clone https://github.com/chenlaug/MorpinOnFire.git
```
2. Install the required dependencies:
	* [SFML](https://www.sfml-dev.org/fr/)
    * [WinSock2 ](https://learn.microsoft.com/en-us/windows/win32/api/winsock2/)

### Compilation 

1. Install SFML following the instructions from [SFML](https://www.sfml-dev.org/tutorials/3.0/)
2. Open the project in your preferred IDE.
3. Compile the project with the appropriate configurations for SFML.

### Run the game

Once the project is compiled, you can run the game by executing the generated binary file.

## Contributing

Contributions are welcome! If you'd like to contribute to the project, here's how to do it:
1. Fork the project
2. Create a new branch (git checkout -b feature/feature-name)
3. Commit your changes (git commit -am 'Add feature X')
4. Push the branch (git push origin feature/feature-name)
5. Create a Pull Request

## License

This project is licensed under the MIT License - see the LICENSE file for details.

## How to Play

* **Single-player mode**: You play against the AI, which uses the Minimax algorithm for decision-making. The game ends when either player wins or the board is full (draw).

* **Multiplayer mode**: Connect with another player using the server-client setup. You can chat during the game and make moves on the board.

## Acknowledgements

* SFML for providing an excellent graphics library for C++.
* Minimax algorithm for the AI logic in single-player mode.
* WinSock2 for networking functionalities on Windows.