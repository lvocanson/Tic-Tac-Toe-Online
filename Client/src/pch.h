#pragma once

/*
    All dependencies used in the project are included here.
    This file is pre-compiled and included in all source files.
*/

#include <iostream>
#include <SFML/Graphics.hpp>
#include <TcpIp.h>
#include <json.hpp>
using Json = nlohmann::json;

#include "TicTacToe.h"
#include "src/core/Managers/InputHandler.h"
#include "core/CoreDefinitions.h"
#include <cassert>

#pragma region SFML

#include <SFML/Graphics.hpp>

#pragma endregion
