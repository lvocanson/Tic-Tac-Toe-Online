#pragma once
#include "TicTacToe.h"
#include <cstdlib>

class IDGenerator
{
public:
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
