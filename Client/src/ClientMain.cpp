#include "core/ClientApp.h"

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) && !defined(__CYGWIN__)

// Entry point for a Windows application (unicode)
int wWinMain(HINSTANCE, HINSTANCE, PWSTR, int)
{
#if defined(DEBUG) | defined(_DEBUG)
    // Enable run-time memory check for debug builds.
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

    // Seed random number generator
    srand((unsigned int)time(NULL));

    ClientApp& app = ClientApp::GetInstance();
    app.Init();
    app.Run();

    return 0;
}

#else
#error Only Windows is supported
#endif
