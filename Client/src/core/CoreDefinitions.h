#pragma once

#if defined(DEBUG) | defined(_DEBUG)
#include <crtdbg.h>
// Dynamically allocate memory with memory leak detection. (debug only)
#define new new(_NORMAL_BLOCK, __FILE__, __LINE__)
#endif

// Release a pointer and set it to nullptr
#define RELEASE(ptr) if(ptr) { delete ptr; ptr = nullptr; }

// Set a pointer to nullptr
#define NULLPTR(ptr) if(ptr) { ptr = nullptr; }

#define PLAYER_TURN_DELAY (0.2f)
#define OUTLINE_THICKNESS (15.0f)
#define DEFAULT_PIECE_SIZE (150.f)
