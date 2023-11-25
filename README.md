# TicTacToe-Online

## Context 
This project was made during our course of Windows Online Programming and Multi-Threading.
The goal was to develop a basic Tic Tac Toe game with a simple GUI and a server capable of handling multiple games simultaneously.

## FEATURES
- Basic TicTacToe game using the [SFML](https://www.sfml-dev.org/) and C++
    - State machine to easily create multiple views (i.e : Home, Login, History, Lobby, Game)
    - Two different game modes:
        - CLASSIC: The original TicTacToe experience.
        - FAST: Each player has a limited time to make a move!
- Server sending and receiving messages from multiple clients
    - `send` and `receive` procedure via Windows window events:
        - Custom window messages
        - Send and Read data as JSON using [Niels Lohmann's library](https://github.com/nlohmann/json)
    - Lobby management to handle multiple games
- Multi-threading paradigms and functionalities
    - Main client loop on the main thread
    - Communications with the server are on a secondary thread
- Web server accessible via any browser to observer all ongoing games

## How to use

Follow these steps to play the game:
1. Download the project from the GitHub release section.
2. Run the local server by executing the `Server.exe` file. The server is needed to play as it will host your games.
3. Run the client by executing the `Client.exe` file.
4. Enjoy!

**NOTE:** You can run multiple clients on the same computer.
**WARNING:** You need to be on the same network to connect to the server.

## How to play

> Make sure the server is running 😉

### Connection

When you start `Client.exe`, you will need to connect to the server with a PassPhrase.
The PassPhrase can be found in the Server console and look like this: `=> Game Server Phrase: PASSPHRASE`

---
![Connection screen](Screenshots/TicTacToe_screenshot_connect.png)

### Lobby

Once connected, you can enter in a **CLASSIC** or **FAST** lobby.
We recommend the **FAST** mode, it's more fun!

---
![Lobby selection scren](Screenshots/TicTacToe_screenshot_lobby.png)

### Game

The game will automatically start when there are 2 players in the lobby.
When the game is over, the server will automatically reset the board.
If any player leaves, the board will be reset, and lobby will return to a waiting state.

---
|              Waiting               |             In Game (Fast)             |
|:----------------------------------:|:--------------------------------:|
| ![Waiting in a lobby screenshot](Screenshots/TicTacToe_screenshot_game.png) | ![Ongoing "Fast" game screenshot](Screenshots/TicTacToe_screenshot_ongoing.png) |

### History

Once connected, you can view all the games played in the server.
The games history is lost when the server shuts down.

---
![History screen](Screenshots/TicTacToe_screenshot_history.png)

## How to access the web server

> Make sure the server is running 😉

When you start `Server.exe`, a web server is also started.
The URL of the web server is printed in the console like this: `=> Web Server Address: YOUR_IP`
You can `Ctrl + Click` it to open the link in your browser, instead of copying.

**NOTE:** You cannot play the game on your browser, it is only used to observe the ongoing games.

---
![Ongoing game on browser](Screenshots/TicTacToe_screenshot_web.png)

## CREDITS

### Supervisor
* [Sylvain SECCIA](https://www.seccia.com/)

### Collaborators
* [Hugo MAESTRACCI](https://github.com/sh4dy01)
* [Nathanael VOGT](https://github.com/Gotvna)
* [Jun](https://github.com/Juuunnne)
