#pragma once
#include <conio.h>
#include <wincon.h>

enum class Color
{
    Black = 0,
    Blue = FOREGROUND_BLUE,
    Green = FOREGROUND_GREEN,
    Cyan = FOREGROUND_GREEN | FOREGROUND_BLUE,
    Red = FOREGROUND_RED,
    Magenta = FOREGROUND_RED | FOREGROUND_BLUE,
    Yellow = FOREGROUND_RED | FOREGROUND_GREEN,
    White = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE,
    Gray = FOREGROUND_INTENSITY,
    LightBlue = FOREGROUND_INTENSITY | FOREGROUND_BLUE,
    LightGreen = FOREGROUND_INTENSITY | FOREGROUND_GREEN,
    LightCyan = FOREGROUND_INTENSITY | FOREGROUND_GREEN | FOREGROUND_BLUE,
    LightRed = FOREGROUND_INTENSITY | FOREGROUND_RED,
    LightMagenta = FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_BLUE,
    LightYellow = FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN,
    BrightWhite = FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE
};

/// <summary>
/// Hash a string to a color.
/// </summary>
Color HshClr(const std::string& str)
{
    int hash = 0;
    for (char c : str)
        hash += c;
    return static_cast<Color>(hash % 15 + 1);
}


// Set the color of the console.
// Example: std::cout << Color::Red << "Hello World!" << Color::White;
inline std::ostream& operator<<(std::ostream& os, Color clr)
{
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), static_cast<WORD>(clr));
    return os;
}

/// <summary>
/// Check if a key is pressed.
/// </summary>
inline bool IsKeyPressed(char key)
{
    while (_kbhit())
        if (_getch() == key)
            return true;
    return false;
}
