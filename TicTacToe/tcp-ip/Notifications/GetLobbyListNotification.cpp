#include "GetLobbyListNotification.h"

Json GetLobbyListNotification::Serialize()
{
    Json j;
    j["Type"] = "GetLobbyList";

    return j;
}
