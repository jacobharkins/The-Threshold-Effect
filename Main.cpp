#include "graphs.h"

// Constant Values
constexpr int BUTTON_WIDTH = 150;
constexpr int BUTTON_HEIGHT = 30;
constexpr int GRAPH_PANEL_HEIGHT = 400;   
constexpr int LEFT_PANEL_WIDTH = 200;
constexpr int VERTEX_RADIUS = 10;
constexpr int EDGE_WIDTH = 2;

#define LEFT_START (LEFT_PANEL_WIDTH + 50)
#define TOP_START 50
#define RIGHT_END(width) ((width) - 50)
#define BOTTOM_END(height) ((height) - 50)
#define GRAPH_WIDTH(width) (RIGHT_END(width) - LEFT_START)
#define GRAPH_HEIGHT(height) (BOTTOM_END(height) - TOP_START)
#define GRAPH_SIZE int(rand() % 15)

// Global variables
HINSTANCE hInst;
HWND hLeftPanel;
HWND hGraphPanel;
HWND hWndGraphPanel; 
Graph graph;

float edgeProbability = 0.5;




//Function to Draw the Graph
void draw_graph(HDC hdc, int leftStart, int topStart, int rightEnd, int bottomEnd) {
    int graphWidth = rightEnd - leftStart;
    int graphHeight = bottomEnd - topStart;

    // Example: Divide the area into a grid for vertices
    int numVertices = graph.V.size();
    int cols = (int)sqrt(numVertices) + 1;
    int rows = ((numVertices + cols - 1) / cols) + 1;

    int cellWidth = graphWidth / cols;
    int cellHeight = graphHeight / rows;
    // Draw edges
    for (const Edge edge : graph.E) {
        int v1 = edge.v1, v2 = edge.v2;

        int col1 = v1 % cols, row1 = v1 / cols;
        int col2 = v2 % cols, row2 = v2 / cols;

        int x1 = leftStart + col1 * cellWidth + cellWidth / 2;
        int y1 = topStart + row1 * cellHeight + cellHeight / 2;
        int x2 = leftStart + col2 * cellWidth + cellWidth / 2;
        int y2 = topStart + row2 * cellHeight + cellHeight / 2;

        // Set edge color using GDI
        COLORREF edgeColor = hex_to_rgb(edge.color);
        HPEN hPen = CreatePen(PS_SOLID, EDGE_WIDTH, edgeColor);
        HPEN hOldPen = (HPEN)SelectObject(hdc, hPen);

        // Draw edge as a line
        MoveToEx(hdc, x1, y1, NULL);
        LineTo(hdc, x2, y2);

        // Reset the Brush
        SelectObject(hdc, hOldPen);
        DeleteObject(hPen);
    }

    // Draw vertices
    int i = 0;
    for (const Vertex vertex : graph.V) {
        int col = i % cols;
        int row = i / cols;

        int x = leftStart + col * cellWidth + cellWidth / 2;
        int y = topStart + row * cellHeight + cellHeight / 2;

        std::cout << "Vertex " << i << " at position: (" << x << ", " << y << ")" << std::endl;
        // Set vertex color using GDI
        COLORREF vertexColor = hex_to_rgb(vertex.color); 
        HBRUSH hBrush = CreateSolidBrush(vertexColor);
        HBRUSH oldBrush = (HBRUSH)SelectObject(hdc, hBrush);

        // Draw vertex as a circle
        Ellipse(hdc, x - VERTEX_RADIUS, y - VERTEX_RADIUS, x + VERTEX_RADIUS, y + VERTEX_RADIUS);

        // Reset the Brush
        SelectObject(hdc, oldBrush);
        DeleteObject(hBrush);
        i++;
    }
}

// Function to create the UI buttons and the graph panel
// Create UI elements, dividing the window into left and right panels
static void create_UI_elements(HWND hWnd) {
    // Left panel (buttons)
    hLeftPanel = CreateWindow(L"STATIC", L"Left Panel",
        WS_CHILD | WS_VISIBLE | SS_BLACKFRAME,
        0, 0, LEFT_PANEL_WIDTH, 2000, hWnd, NULL, (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE), NULL);

    // Right panel (graph area)
    hGraphPanel = CreateWindow(L"STATIC", L"Graph Panel",
        WS_CHILD | WS_VISIBLE | SS_BLACKFRAME,
        LEFT_PANEL_WIDTH, 0, 0, 0, hWnd, NULL, (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE), NULL);

    // Create buttons for the left panel (inside hLeftPanel)
    CreateWindow(L"BUTTON", L"Randomize Graph",
        WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
        10, 10, 150, 30, 
        hWnd, (HMENU)1, (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE), NULL);

    CreateWindow(L"BUTTON", L"Color Graph",
        WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
        10, 50, 150, 30, 
        hWnd, (HMENU)2, (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE), NULL);

    CreateWindow(L"BUTTON", L"Export Graph",
        WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
        10, 90, 150, 30, 
        hWnd, (HMENU)3, (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE), NULL);

    CreateWindow(L"BUTTON", L"Import Graph",
        WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
        10, 130, 150, 30, 
        hWnd, (HMENU)4, (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE), NULL);

    CreateWindow(L"BUTTON", L"Check For Edge",
        WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
        10, 170, 150, 30, 
        hWnd, (HMENU)5, (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE), NULL);

    CreateWindow(L"BUTTON", L"Check For k3",
        WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
        10, 210, 150, 30,
        hWnd, (HMENU)6, (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE), NULL);

    CreateWindow(L"BUTTON", L"Check For k4",
        WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
        10, 250, 150, 30,
        hWnd, (HMENU)7, (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE), NULL);

}


static void resize_ui_elements(HWND hWnd, LPARAM lParam) {
    int newWidth = LOWORD(lParam);
    int newHeight = HIWORD(lParam);

    // Adjust the width of the left panel
    SetWindowPos(hLeftPanel, NULL, 0, 0, LEFT_PANEL_WIDTH, newHeight, SWP_NOZORDER);

    // Adjust the right panel (graph area) to take up remaining space
    SetWindowPos(hGraphPanel, NULL, LEFT_PANEL_WIDTH, 0, newWidth - LEFT_PANEL_WIDTH, newHeight, SWP_NOZORDER);
}



// Function to process button clicks
void process_button_click(HWND hWnd, WPARAM wParam) {
    int commandId = LOWORD(wParam);
        switch (LOWORD(wParam)) {
        case 1: {
            // Generate graph
            InvalidateRect(hWnd, NULL, TRUE); 
            graph.gen_rand_graph(GRAPH_SIZE, edgeProbability);
            graph.cprint();
            // Invalidate the window to force a redraw
            InvalidateRect(hWnd, NULL, TRUE); 
            UpdateWindow(hWnd); 
            break;
        }
        case 2: {
            // Color graph
            graph.gen_rand_colors();
            InvalidateRect(hWnd, NULL, TRUE); 
            UpdateWindow(hWnd); 
            break;
        }
        case 3: {
            // Export graph
            wchar_t filename[MAX_PATH] = L"";
            OPENFILENAME ofn = {};
            ofn.lStructSize = sizeof(ofn);
            ofn.hwndOwner = hWnd;
            ofn.lpstrFilter = L"Graph Files\0*.g\0";
            ofn.lpstrFile = filename;
            ofn.nMaxFile = MAX_PATH;
            ofn.Flags = OFN_OVERWRITEPROMPT;
            if (GetSaveFileName(&ofn)) {
                graph.export_graph(filename);
                MessageBox(hWnd, L"Graph Exported!", L"Info", MB_OK);
            }
            break;
        }
        case 4: {
            wchar_t filename[MAX_PATH] = L"";
            OPENFILENAME ofn = {};
            ofn.lStructSize = sizeof(ofn);
            ofn.hwndOwner = hWnd;
            ofn.lpstrFilter = L"Graph Files\0*.g\0";
            ofn.lpstrFile = filename;
            ofn.nMaxFile = MAX_PATH;
            ofn.Flags = OFN_FILEMUSTEXIST;
            if (GetOpenFileName(&ofn)) {
                graph.import_graph(filename);
                MessageBox(hWnd, L"Graph Imported!", L"Info", MB_OK);
            }
            break;
        }
        case 5: {            // Check Edge in graph
            if (graph.has_edge()) {
                MessageBox(hWnd, L"Contains an Edge", L"Info", MB_OK);
            }
            else {
                MessageBox(hWnd, L"Does Not Contain an Edge", L"Info", MB_OK);
            }
            break;
        }
        case 6: {           // Check k3 in graph
            if (graph.has_k3()) {
                MessageBox(hWnd, L"Contains a k3", L"Info", MB_OK);
            }
            else {
                MessageBox(hWnd, L"Does Not Contain a k3", L"Info", MB_OK);
            }
            break;
        }
        case 7: {           // Check k4 in graph 
            if (graph.has_k4()) {
                MessageBox(hWnd, L"Contains a k4", L"Info", MB_OK);
            }
            else {
                MessageBox(hWnd, L"Does Not Contain a k4", L"Info", MB_OK);
            }
            break;
        }
        default: {
            std::wcout << L"Unknown Button Pressed " << commandId << std::endl;
        }
    }
}

// Window procedure to handle messages
LRESULT CALLBACK window_procedure(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message) {
    case WM_CREATE:
        create_UI_elements(hWnd);
        break;
    case WM_PAINT: {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);

        // Retrieve the current window dimensions
        RECT clientRect;
        GetClientRect(hWnd, &clientRect);

        // Calculate graph boundaries
        int leftStart = LEFT_PANEL_WIDTH;
        int topStart = TOP_START;
        int rightEnd = RIGHT_END(clientRect.right);
        int bottomEnd = BOTTOM_END(clientRect.bottom);

        // Use the graph dimensions for drawing
        FillRect(hdc, &ps.rcPaint, (HBRUSH)(COLOR_WINDOW + 2));
        draw_graph(hdc, leftStart, topStart, rightEnd, bottomEnd);

        EndPaint(hWnd, &ps);
        break;
    }
    case WM_COMMAND:
        process_button_click(hWnd, wParam);
        break;

    case WM_SIZE: {
        // Retrieve the new dimensions of the client area
        int width = LOWORD(lParam);  // Width of the window's client area
        int height = HIWORD(lParam); // Height of the window's client area

        // Calculate graph boundaries
        int leftStart = LEFT_START;
        int topStart = TOP_START;
        int rightEnd = RIGHT_END(width);
        int bottomEnd = BOTTOM_END(height);

        int graphWidth = GRAPH_WIDTH(width);
        int graphHeight = GRAPH_HEIGHT(height);

        // Optionally, invalidate the graph area for a redraw
        RECT graphRect = { leftStart, topStart, rightEnd, bottomEnd };
        InvalidateRect(hWnd, &graphRect, TRUE);

        // You can use these dimensions to reposition or resize other UI elements too
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

// Entry point for Win32 applications
int WINAPI WinMain(_In_opt_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    hInst = hInstance;
    std::srand(static_cast<unsigned int>(std::time(nullptr)));

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


    graph.gen_rand_graph(GRAPH_SIZE, edgeProbability);
    graph.gen_rand_colors();
    graph.cprint();
    graph.export_graph(L"Test");


    WNDCLASSEX wcex = {};
    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.lpfnWndProc = window_procedure;     // Set window procedure
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





