#pragma once
#include "TicTacToe.h"

class IDGenerator
{
public:
    static TicTacToe::PieceID  GenerateID()
    {
        static TicTacToe::PieceID id = 1;
        return id++;
    }
};

