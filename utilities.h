#pragma once
#include <windows.h>
#include <commdlg.h>
#include <cmath>
#include <string>
// Console Color List Enum
enum ConsoleColor {
    BLACK = 0, BLUE = 1, GREEN = 2, CYAN = 3,
    RED = 4, MAGENTA = 5, YELLOW = 6, WHITE = 7,
    BRIGHT_BLACK = 8, BRIGHT_BLUE = 9, BRIGHT_GREEN = 10, BRIGHT_CYAN = 11,
    BRIGHT_RED = 12, BRIGHT_MAGENTA = 13, BRIGHT_YELLOW = 14, BRIGHT_WHITE = 15
};

// Predefined RGB values for console colors
const COLORREF consoleColors[16] = {
    RGB(0, 0, 0),       RGB(0, 0, 128),     RGB(0, 128, 0),     RGB(0, 128, 128),
    RGB(128, 0, 0),     RGB(128, 0, 128),   RGB(128, 128, 0),   RGB(192, 192, 192),
    RGB(128, 128, 128), RGB(0, 0, 255),     RGB(0, 255, 0),     RGB(0, 255, 255),
    RGB(255, 0, 0),     RGB(255, 0, 255),   RGB(255, 255, 0),   RGB(255, 255, 255)
};

// Set console color 
void setConsoleColor(int color);

// Reset console color to default(white)
void resetConsoleColor();

// Convert Hex string into an rgb COLORREF
COLORREF hex_to_rgb(const std::string& hex);

// Helper function to compute the distance between two COLORREF colors
int color_distance(COLORREF c1, COLORREF c2);

// Map a COLORREF to the closest console color
ConsoleColor color_to_console_color(COLORREF color);

// Get 2d Coords in the grid given id
std::pair<int, int> get_2d_coords(int id, int gridSize);

// Calculate grid size to fit all vertices
int calculate_grid_size(int numVertices);

// Convert string to wchar_t
wchar_t* string_to_wchar(const std::string& str);