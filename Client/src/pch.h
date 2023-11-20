#pragma once

/*
    All dependencies used in the project are included here.
    This file is pre-compiled and included in all source files.
*/

#include <iostream>
#include <SFML/Graphics.hpp>
#include <tcp-ip/TcpIp.h>
#include <tcp-ip/json.hpp>
using Json = nlohmann::json;

#include <threading/Shared.h>
#include <game/TicTacToe.h>
#include "src/core/Managers/InputHandler.h"
#include "core/CoreDefinitions.h"
#include <cassert>
