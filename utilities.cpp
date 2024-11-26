#include "utilities.h"



void setConsoleColor(int color) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, color);
}


void resetConsoleColor() {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, 15);
}


COLORREF hex_to_rgb(const std::string& hex) {
    // Hex string must be in the format "#RRGGBB"
    if (hex[0] != L'#' || hex.length() != 7) {
        return RGB(1, 1, 1); 
    }

    // Parse the hex string
    int r = std::stoi(hex.substr(1, 2), nullptr, 16);
    int g = std::stoi(hex.substr(3, 2), nullptr, 16);
    int b = std::stoi(hex.substr(5, 2), nullptr, 16);

    return RGB(r, g, b);
}


// Helper function to compute the distance between two COLORREF colors
int color_distance(COLORREF c1, COLORREF c2) {
    int r1 = GetRValue(c1), g1 = GetGValue(c1), b1 = GetBValue(c1);
    int r2 = GetRValue(c2), g2 = GetGValue(c2), b2 = GetBValue(c2);
    return (r1 - r2) * (r1 - r2) + (g1 - g2) * (g1 - g2) + (b1 - b2) * (b1 - b2);
}


// Map a COLORREF to the closest console color
ConsoleColor color_to_console_color(COLORREF color) {
    int closestIndex = 0;
    int minDistance = color_distance(color, consoleColors[0]);

    for (int i = 1; i < 16; ++i) {
        int distance = color_distance(color, consoleColors[i]);
        if (distance < minDistance) {
            closestIndex = i;
            minDistance = distance;
        }
    }

    return static_cast<ConsoleColor>(closestIndex);
}


int calculate_grid_size(int numVertices) {
    return static_cast<int>(std::ceil(std::sqrt(numVertices)));
}


std::pair<int, int> get_2d_coords(int id, int gridSize) {
    int x = id % gridSize;                                  // Column (horizontal position)
    int y = id / gridSize;                                  // Row (vertical position)
    return { x, y };
}



wchar_t* string_to_wchar(const std::string& str) {
    size_t len = str.length() + 1; 
    wchar_t* wcharArray = new wchar_t[len];

    size_t convertedChars = 0;
    errno_t err = mbstowcs_s(&convertedChars, wcharArray, len, str.c_str(), len - 1);
    if (err != 0) {
        delete[] wcharArray; 
        return nullptr;
    }

    return wcharArray;
}