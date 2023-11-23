#include "GetLobbyListNotification.h"

Json GetLobbyListNotification::Serialize()
{
    Json j;
    j["type"] = "GetLobbyList";

    return j;
}
