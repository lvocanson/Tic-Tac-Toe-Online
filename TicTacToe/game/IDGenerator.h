#pragma once
#include "TicTacToe.h"
#include <cstdlib>
#include <ctime>


class IDGenerator
{
public:
    static TicTacToe::PieceID  GenerateID()
    {
        static TicTacToe::PieceID id = 1;
        return id++;
    }

    static int GenerateLobbyID()
    {
        int id = 0;
        if (id == 0)
        {
            id = rand() % 9000 + 1000;
        }
        return id;
    }
};

