#pragma once
#include <string>
#include "../tcp-ip/ISerializable.h"
#include "TicTacToe.h"
#include "GameData.h"
#include "GameMode.h"

struct LobbyData : ISerializable
{
    LobbyData() = default;
    LobbyData(const Json& j);
    LobbyData(const int id, GameModeType gameMode, const std::string& playerX, const std::string& playerO);

    Json Serialize() override;

    int ID = -1;
    GameModeType GameMode;
    std::string PlayerX, PlayerO;
};

struct Lobby
{
    Lobby();
    Lobby(GameModeType gameModeType);
    Lobby(const std::string& playerX, const std::string& playerO);
    Lobby(const int id, const std::string& playerX, const std::string& playerO);
    ~Lobby() = default;

    std::string& GetOpponentName(const std::string&);
    void AddPlayerToLobby(const std::string& name);
    void RemovePlayerFromLobby(const std::string& name);
    void AddPlayerMove(const std::string&, const TicTacToe::Piece, const unsigned int Cell);
    void ResetGame();
    
    bool IsInLobby(const std::string& name) const { return Data.PlayerX == name || Data.PlayerO == name; }
    bool IsLobbyFull() const {  return !Data.PlayerX.empty() && !Data.PlayerO.empty(); }
    bool IsLobbyEmpty() const { return Data.PlayerX.empty() && Data.PlayerO.empty(); }

    unsigned int PlayerCount = 0;
    LobbyData Data;
    TicTacToe::Board Board;
    std::vector<PlayerMove> CurrentGame;
};
