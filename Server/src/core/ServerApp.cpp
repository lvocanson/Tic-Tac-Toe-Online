#include "ServerApp.h"
#include "src/tcp-ip/TcpIpServer.h"
#include <conio.h>

void ServerApp::Run()
{
    TcpIpServer& server = TcpIpServer::GetInstance();
    server.Open(DEFAULT_PORT);
    std::cout << "Server is listening on port " << DEFAULT_PORT << "..." << std::endl;

    std::stringstream ss;
    Client client;
    char kbhit = 0;

    std::cout << "Press ESC to shut down the server." << std::endl;
    while (!(kbhit == 27))
    {
        try
        {
            int count = 0;
            while (server.AcceptPendingConnection()) count++;
            if (count > 0)
                std::cout << count << " connections were established." << std::endl << "-> Total number of connections: " << server.ConnectionCount() << std::endl;

            // Echo back to the clients
            while (server.FetchPendingData(ss, client))
            {
                std::cout << "Received: " << ss.str() << std::endl;

                server.Send(client, ss.str());
                std::cout << "Sent back." << std::endl;

                ss.str(std::string());
            }

            count = server.KillClosedConnections();
            if (count > 0)
                std::cout << count << " connections were closed." << std::endl << "-> Total number of connections: " << server.ConnectionCount() << std::endl;
        }
        catch (const TcpIp::TcpIpException& e)
        {
            std::cout << "The server has encountered an error: " << e.what() << std::endl;
        }

        // Sleep for 1ms to avoid 100% CPU usage
        Sleep(1);

        // Check for keyboard input
        _kbhit() && (kbhit = _getch());
    }

    server.Close();
    std::cout << "Server successfully shut down on user request." << std::endl;
}
