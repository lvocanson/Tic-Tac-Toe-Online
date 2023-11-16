#include "ServerApp.h"
#include "ConsoleHelper.h"


void ServerApp::Run()
{
    m_Server = new TcpIpServer();
    m_Server->Open(DEFAULT_PORT);
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
            int count = m_Server->AcceptAllPendingConnections();
            if (count > 0)
            {
                std::cout
                    << Color::LightMagenta <<
                    count << " new connection" << (count > 1 ? "s" : "") << " accepted. (" << m_Server->ConnectionCount() << " total)" << std::endl
                    << Color::White;
            }

            while (m_Server->FetchPendingData(ss, client))
                AnalyseData(ss.str(), client);

            count = m_Server->KillClosedConnections();
            if (count > 0)
            {
                std::cout
                    << Color::LightMagenta <<
                    count << " connection" << (count > 1 ? "s" : "") << " closed. (" << m_Server->ConnectionCount() << " total)" << std::endl
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

    m_Server->Close();
    delete m_Server;
    std::cout
        << Color::Green <<
        "Server successfully shut down on user request." << std::endl
        << Color::White;
}

void ServerApp::AnalyseData(const std::string& data, Client sender)
{
    Color clr = HshClr(sender->Address + ":" + std::to_string(sender->Port));
    std::cout << clr << sender->Address << ":" << sender->Port << Color::White << " sent " << data.size() << " bytes of data." << std::endl;

    std::ostringstream oss;
    oss << "Dear " << sender->Address << ":" << sender->Port << ", I received " << data.size() << " bytes of data from you.";

    /* ... */

    m_Server->Send(sender, oss.str());
    std::cout << "-> Replied with " << oss.str().size() << " bytes of data." << std::endl;
}
