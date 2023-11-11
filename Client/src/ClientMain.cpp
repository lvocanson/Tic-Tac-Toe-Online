#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) && !defined(__CYGWIN__)

// Entry point for a Windows program (Unicode)
int wmain()
{
#if defined(DEBUG) | defined(_DEBUG)
    // Enable run-time memory check for debug builds.
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

     std::cout << "Hello World! I'm a client!\n";

    return 0;
}

#else
#error Only Windows is supported
#endif
