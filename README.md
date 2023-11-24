# TicTacToe-Online
## Context 
This project was made for the course of Windows Online Programing and Multi-Threading.
The goal was to create a basic Tic Tac Toe game with a GUI and a server that can handle multiple games at the same time.

## Features
- Basic TicTacToe game using the [SFML](https://www.sfml-dev.org/) and C++
    - View State to easily create multiple views (i.e : Home, Login, History, Lobby, Game)
    - Two different game modes
        - CLASSIC
        - FAST (PLayer turn timer is enabled)
- Server sending and receiving messages from multiple clients
    - 'send' and 'receive' procedure via Windows window events
        - Custom window messages
        - Send and Read data as json using [C++ json library](https://github.com/nlohmann/json)
    - Lobby management to manage multiple games
    - Send all sessions updates to a Web server
- Multi-threading paradigms and functionalities
    - Main client loop on the main threas
    - Receiving messages from server is running on an independent thread

## How to use
1. Download the project in the GitHub release section.
2. Run the local server by executing the `server.exe` file.
3. Run the client by executing the `client.exe` file.
4. Enjoy the game!

**NOTE :** You can run multiple clients at the same time.

## How to play

> Make sure the server is running. :)

### Connection
When you start `client.exe`, you will need to connect to the server.

---


### Lobby
Once connected, you can enter in a CLASSIC or FAST lobby.  
(The fast game mode is really fun)

---



### Game
The game will automatically start when there is 2 players in the lobby.  
When the game is over, the server will automatically reset the board.  
If the opponent leave the lobby, the lobby will return to a waiting state.

---
|              In-Game               |             Game End             |
|:----------------------------------:|:--------------------------------:|
