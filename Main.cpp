//#include "graphs.h"
//#include <string>
//
//
//int main() {
//	srand(time(0));
//	Graph graphs[2];
//	//for (int i = 0; i < std::size(graphs); i++) {
//	//	graphs[i].gen_rand_graph(rand() % 20, float(rand()) / RAND_MAX);
//	//	graphs[i].gen_rand_colors();
//	//	graphs[i].export_graph("graph" + std::to_string(i));
//	//}
//	graphs[0].import_graph("graph0.g");
//	graphs[1].import_graph("graph1.g");
//	graphs[0].cprint();
//	graphs[1].cprint();
//}

#include "graphs.h"

// Constant Values
constexpr int BUTTON_WIDTH = 150;
constexpr int BUTTON_HEIGHT = 30;
constexpr int GRAPH_PANEL_HEIGHT = 400;   
constexpr int LEFT_PANEL_WIDTH = 200;
constexpr int VERTEX_RADIUS = 10;


// Global variables
HINSTANCE hInst;
HWND hLeftPanel;
HWND hGraphPanel;
HWND hWndGraphPanel; 
Graph graph;
int graphSize = 0;
float edgeProbability = 0;
int gridSize;



// Function to create the UI buttons and the graph panel
// Create UI elements, dividing the window into left and right panels
static void create_UI_elements(HWND hWnd) {
    // Left panel (buttons)
    hLeftPanel = CreateWindow(L"STATIC", L"Left Panel",
        WS_CHILD | WS_VISIBLE | SS_BLACKFRAME,
        0, 0, LEFT_PANEL_WIDTH, 600, hWnd, NULL, (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE), NULL);

    // Right panel (graph area)
    hGraphPanel = CreateWindow(L"STATIC", L"Graph Panel",
        WS_CHILD | WS_VISIBLE | SS_BLACKFRAME,
        LEFT_PANEL_WIDTH, 0, 600, 600, hWnd, NULL, (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE), NULL);

    // Create buttons for the left panel (inside hLeftPanel)
    CreateWindow(L"BUTTON", L"Generate Graph",
        WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
        10, 10, 150, 30, hLeftPanel, (HMENU)1, (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE), NULL);

    CreateWindow(L"BUTTON", L"Export Graph",
        WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
        10, 50, 150, 30, hLeftPanel, (HMENU)2, (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE), NULL);

    // Add more buttons as needed
}

static void resize_ui_elements(HWND hWnd, LPARAM lParam) {
    // Get the new size of the window
    int newWidth = LOWORD(lParam);
    int newHeight = HIWORD(lParam);

    // Adjust the width of the left panel (keep it fixed)
    SetWindowPos(hLeftPanel, NULL, 0, 0, LEFT_PANEL_WIDTH, newHeight, SWP_NOZORDER);

    // Adjust the right panel (graph area) to take up remaining space
    SetWindowPos(hGraphPanel, NULL, LEFT_PANEL_WIDTH, 0, newWidth - LEFT_PANEL_WIDTH, newHeight, SWP_NOZORDER);
}

// Function to handle drawing the graph on the panel
static void draw_graph(HDC hdc){
    // Set up the drawing context
    HPEN hPen, hOldPen;
    HBRUSH hBrush, hOldBrush;
    hPen = CreatePen(PS_SOLID, 2, RGB(0, 0, 0));  // Default pen for edges (black)
    hBrush = CreateSolidBrush(RGB(0, 0, 0));    // Default brush for vertices (black)

    hOldPen = (HPEN)SelectObject(hdc, hPen);
    hOldBrush = (HBRUSH)SelectObject(hdc, hBrush);

    // Draw edges
    for (const Edge& edge : graph.E) {
        const int& v1 = edge.v1;
        const int& v2 = edge.v2;

        // Set the edge color
        HPEN hPen = CreatePen(PS_SOLID, 2, hex_to_rgb(edge.color));  // Use edge color
        SelectObject(hdc, hPen);

        // Draw the line between vertices
        MoveToEx(hdc, get_2d_coords(v1, gridSize).first, get_2d_coords(v1, gridSize).second, NULL);
        LineTo(hdc, get_2d_coords(v2, gridSize).first, get_2d_coords(v2, gridSize).second);

        // Clean up the pen after use
        DeleteObject(hPen);
    }

    // Draw vertices
    for (const auto& vertex : graph.V) {
        const int& id = vertex.id;
        const int& x = get_2d_coords(id, gridSize).first;
        const int& y = get_2d_coords(id, gridSize).second;
        // Set the vertex color
        hBrush = CreateSolidBrush(hex_to_rgb(vertex.color));  // Use vertex color
        SelectObject(hdc, hBrush);

        // Draw a circle for each vertex
        Ellipse(hdc, x - VERTEX_RADIUS, y - VERTEX_RADIUS,
            x + VERTEX_RADIUS, y + VERTEX_RADIUS);

        // Clean up the brush after use
        DeleteObject(hBrush);
    }

    // Restore the original pen and brush
    SelectObject(hdc, hOldPen);
    SelectObject(hdc, hOldBrush);

    // Delete objects
    DeleteObject(hPen);
    DeleteObject(hBrush);
}

// Function to process button clicks
void process_button_click(HWND hWnd, WPARAM wParam) {
    switch (LOWORD(wParam)) {
    case 1: {
        // Generate graph
        MessageBox(hWnd, L"Generating Graph...", L"Info", MB_OK);
        // Invalidate the window to force a redraw
        InvalidateRect(hWnd, NULL, TRUE); // NULL means invalidate the entire window
        UpdateWindow(hWnd); // Optional: immediately trigger WM_PAINT
        break;
    }
    case 2: {
        // Export graph
        MessageBox(hWnd, L"Exporting Graph...", L"Info", MB_OK);
        // Call export function here
        break;
    }
    case 3: {
        // Import graph
        MessageBox(hWnd, L"Importing Graph...", L"Info", MB_OK);
        // Call import function here
        break;
    }
    }
}

// Window procedure to handle messages
LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message) {
    case WM_CREATE:
        create_UI_elements(hWnd);
        break;
    case WM_PAINT: {
        // Retrieve the HDC for the window
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);

        // Draw your graph
        draw_graph(hdc);  // Example: 10 vertices, canvas size 400x400

        EndPaint(hWnd, &ps);  // End painting, releasing HDC
        break;
    }
    case WM_COMMAND:
        process_button_click(hWnd, wParam);
        break;

    case WM_SIZE: {
        // Handle window resizing, update the layout of UI elements
        int width = LOWORD(lParam);
        int height = HIWORD(lParam);

        // Resize graph panel
        MoveWindow(hWndGraphPanel, 0, GRAPH_PANEL_HEIGHT, width, height - GRAPH_PANEL_HEIGHT, TRUE);

        // You can resize other controls (buttons) based on the window size too.
        break;
    }

    case WM_DESTROY:
        PostQuitMessage(0);
        break;

    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}
//// Function declarations
//LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);




// Entry point for Win32 applications
int WINAPI WinMain(_In_opt_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    hInst = hInstance;

    graph;
    if (!AllocConsole()) {
        std::cerr << "Error allocating console" << std::endl;
        return -1;
    }
    FILE* consoleOut;
    if (freopen_s(&consoleOut, "CONOUT$", "w", stdout) != 0) {
        std::cerr << "Failed to redirect stdout to console!" << std::endl;
        return -1;
    }
    FILE* consoleIn;
    if (freopen_s(&consoleIn, "CONOUT$", "w", stdin) != 0) {
        std::cerr << "Failed to redirect stdin to console!" << std::endl;
        return -1;
    }
    FILE* consoleErr;
    if (freopen_s(&consoleErr, "CONOUT$", "w", stderr) != 0) {
        std::cerr << "Failed to redirect stderr to console!" << std::endl;
        return -1;
    }

    WNDCLASSEX wcex = {};
    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.lpfnWndProc = WindowProc;     // Set window procedure
    wcex.hInstance = hInstance;
    wcex.lpszClassName = L"GraphAppClass";

    RegisterClassEx(&wcex);


    // Create the window
    HWND hWnd = CreateWindow(L"GraphAppClass", L"Graph Application",
        WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
        800, 600, NULL, NULL, hInstance, NULL);

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    if (!hWnd) {
        MessageBox(nullptr, L"Failed to create window!", L"Error", MB_OK | MB_ICONERROR);
        return 0;
    }


    // Message loop
    MSG msg = {};
    while (GetMessage(&msg, nullptr, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    FreeConsole();
    return (int)msg.wParam;
}





