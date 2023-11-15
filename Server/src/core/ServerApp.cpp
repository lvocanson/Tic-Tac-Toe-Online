#include "ServerApp.h"
#include "src/tcp-ip/TcpIpServer.h"
#include "ConsoleHelper.h"

void AnalyseData(const std::string& data, Client sender);

void ServerApp::Run()
{
    TcpIpServer& server = TcpIpServer::GetInstance();
    server.Open(DEFAULT_PORT);
    std::cout
        << Color::Green <<
        "Server is listening on port " << DEFAULT_PORT << "..." << std::endl
        << Color::Gray <<
        "Press ESC to shut down the server." << std::endl
        << Color::White;

    std::stringstream ss;
    Client client;
    while (!IsKeyPressed(27)) // 27 = ESC
    {
        try
        {
            int count = server.AcceptAllPendingConnections();
            if (count > 0)
            {
                std::cout
                    << Color::Blue <<
                    count << " new connection" << (count > 1 ? "s" : "") << " accepted." << std::endl
                    << Color::White;
            }

            while (server.FetchPendingData(ss, client))
                AnalyseData(ss.str(), client);

            count = server.KillClosedConnections();
            if (count > 0)
            {
                std::cout
                    << Color::LightMagenta <<
                    count << " connection" << (count > 1 ? "s" : "") << " closed." << std::endl
                    << Color::White;
            }
        }
        catch (const TcpIp::TcpIpException& e)
        {
            std::cout
                << Color::Red <<
                "The server has encountered an error: " << std::endl
                << Color::Yellow <<
                "  " << e.what() << std::endl
                << Color::White;
        }

        // Sleep for 1ms to avoid 100% CPU usage
        Sleep(1);
    }

    server.Close();
    std::cout
        << Color::Green <<
        "Server successfully shut down on user request." << std::endl
        << Color::White;
}

void AnalyseData(const std::string& data, Client sender)
{
    Color clr = HshClr(sender->Address + ":" + std::to_string(sender->Port));
    std::cout << clr << sender->Address << ":" << sender->Port << Color::White << " sent " << data.size() << " bytes of data." << std::endl;

    std::ostringstream oss;
    oss << "Dear " << sender->Address << ":" << sender->Port << ", I received " << data.size() << " bytes of data from you.";

    /* ... */

    TcpIpServer::GetInstance().Send(sender, oss.str());
    std::cout << "-> Replied with " << oss.str().size() << " bytes of data." << std::endl;
}
