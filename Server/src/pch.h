#pragma once

/*
    All dependencies used in the project are included here.
    This file is pre-compiled and included in all source files.
*/

#include <iostream>

#pragma region Our defines

#if defined(DEBUG) | defined(_DEBUG)
#include <crtdbg.h>
// Dynamically allocate memory with memory leak detection. (debug only)
#define new new(_NORMAL_BLOCK, __FILE__, __LINE__)
#endif

// Release a pointer and set it to nullptr
#define RELEASE(ptr) if(ptr) { delete ptr; ptr = nullptr; }

#pragma endregion
