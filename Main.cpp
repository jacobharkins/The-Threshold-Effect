#include "graphs.h"



struct ButtonInfo {
    const wchar_t* name;
    const int index;
    const int order;
};

// Constant Values
constexpr int BUTTON_WIDTH = 150;
constexpr int BUTTON_HEIGHT = 30;
constexpr int GRAPH_PANEL_HEIGHT = 400;   
constexpr int LEFT_PANEL_WIDTH = 200;
constexpr int VERTEX_RADIUS = 10;
constexpr int EDGE_WIDTH = 2;
constexpr int TOP_START = 50;

// Macros
#define LEFT_START (LEFT_PANEL_WIDTH + 50)
#define RIGHT_END(width) ((width) - 50)
#define BOTTOM_END(height) ((height) - 50)
#define GRAPH_WIDTH(width) (RIGHT_END(width) - LEFT_START)
#define GRAPH_HEIGHT(height) (BOTTOM_END(height) - TOP_START)
#define GRAPH_SIZE int(rand() % 15)
#define BUTTON_Y(index) (BUTTON_HEIGHT + 10)* index
#define BUTTON_X (LEFT_PANEL_WIDTH / 2) - (BUTTON_WIDTH / 2)

#define CREATE_PANEL_BUTTON(buttonName, index, order)\
     CreateWindowEx( WS_EX_WINDOWEDGE , \
     L"BUTTON", buttonName,\
     WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON | BS_CENTER,\
     BUTTON_X, BUTTON_Y(order), BUTTON_WIDTH, BUTTON_HEIGHT,\
     hWnd, (HMENU)index, (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE), NULL)

#define REDRAW_GRAPH InvalidateRect(hGraphWnd, NULL, TRUE); UpdateWindow(hGraphWnd);

const std::vector<ButtonInfo> buttonList = {
    {L"Randomize Graph", 1, 1},
    {L"Color Graph", 2, 2},
    {L"Export Graph", 3, 3},
    {L"Import Graph", 4, 4},
    {L"Check For Edge", 5, 5},
    {L"Check For k3", 6, 6},
    {L"Check For k4", 7, 7},
    {L"Check Hamiltonian", 8, 8},
    {L"CPrint", 9, 10},
    {L"Add Vertex", 10, 9},
    {L"Generate SVG", 11, 11 }
};

// Global variables
HINSTANCE hInst;
HWND hLeftPanel;
HWND hGraphPanel;
HWND hWndGraphPanel; 
HWND hGraphSizeInput;
HWND hEdgeProbInput;
HWND hGraphWnd;
HWND hMainWindow;
Graph graph;

float edgeProbability = 0.5;





//Function to Draw the Graph
void draw_graph(HDC hdc, int leftStart, int topStart, int rightEnd, int bottomEnd) {
    int graphWidth = rightEnd - leftStart;
    int graphHeight = bottomEnd - topStart;

    // Example: Divide the area into a grid for vertices
    int numVertices = graph.V.size();
    int cols = (int)sqrt(numVertices) ? (int)sqrt(numVertices) : 1;
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
    hLeftPanel = CreateWindowEx(WS_EX_CONTROLPARENT | WS_EX_CLIENTEDGE,
        L"STATIC", L"Control",
        WS_CHILD | WS_VISIBLE | SS_SUNKEN | WS_BORDER,
        0, 0, LEFT_PANEL_WIDTH, 2000, 
        hWnd, NULL, (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE), NULL);

    // Right panel (graph area)
    hGraphPanel = CreateWindowEx(WS_EX_CLIENTEDGE,
        L"STATIC", L"Graph Panel",
        WS_CHILD | WS_VISIBLE | SS_SUNKEN | WS_BORDER,
        LEFT_PANEL_WIDTH, 0, 0, 0, 
        hWnd, NULL, (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE), NULL);

    hGraphWnd = hWnd;
    // Create buttons for the left panel (inside hLeftPanel)
    for (ButtonInfo button : buttonList) {
        CREATE_PANEL_BUTTON(button.name, button.index, button.order);
    }
}


// Function to handle window resizing
static void resize_ui_elements(HWND hWnd, LPARAM lParam) {
    int newWidth = LOWORD(lParam);
    int newHeight = HIWORD(lParam);

    // Adjust the width of the left panel
    SetWindowPos(hLeftPanel, NULL, 0, 0, LEFT_PANEL_WIDTH, newHeight, SWP_NOZORDER);

    // Adjust the right panel (graph area) to take up remaining space
    SetWindowPos(hGraphPanel, NULL, LEFT_PANEL_WIDTH, 0, newWidth - LEFT_PANEL_WIDTH, newHeight, SWP_NOZORDER);
}


// Popup Procedure to handle popup made by create_input_popup
LRESULT CALLBACK popup_procedure(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    static HWND hSizeEdit, hProbEdit;

    switch (message) {
        case WM_CREATE:
            // Create controls once, when the popup is first created
            hSizeEdit = CreateWindowEx(
                WS_EX_CLIENTEDGE,
                L"EDIT", L"",
                WS_CHILD | WS_VISIBLE | WS_BORDER | ES_RIGHT | WS_TABSTOP,
                150, 10, 80, 20,
                hWnd, (HMENU)1, NULL, NULL);

            CreateWindowEx(
                0,
                L"STATIC", L"Graph Size:",
                WS_CHILD | WS_VISIBLE,
                10, 10, 100, 20,
                hWnd, NULL, NULL, NULL);

            hProbEdit = CreateWindowEx(
                WS_EX_CLIENTEDGE,
                L"EDIT", L"",
                WS_CHILD | WS_VISIBLE | WS_BORDER | ES_RIGHT | WS_TABSTOP,
                150, 40, 80, 20, hWnd, (HMENU)2, NULL, NULL);

            CreateWindowEx(
                0,
                L"STATIC", L"Edge Probability:",
                WS_CHILD | WS_VISIBLE,
                10, 40, 120, 20,
                hWnd, NULL, NULL, NULL);

            CreateWindowEx(
                0,
                L"BUTTON", L"Generate",
                WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | WS_TABSTOP,
                10, 80, 100, 30,
                hWnd, (HMENU)3, NULL, NULL);

            CreateWindowEx(0,
                L"BUTTON", L"Cancel",
                WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | WS_TABSTOP,
                120, 80, 100, 30,
                hWnd, (HMENU)4, NULL, NULL);
            break;

        case WM_COMMAND:
            switch (LOWORD(wParam)) {
            case 3: // "Generate" button
            {
                TCHAR sizeBuffer[10], probBuffer[10];
                GetWindowText(hSizeEdit, sizeBuffer, 10);
                GetWindowText(hProbEdit, probBuffer, 10);

                int size = _wtoi(sizeBuffer);
                float prob = static_cast<float>(_wtof(probBuffer));

                if (size <= 0 || prob < 0.0f || prob > 1.0f) {
                    MessageBox(hWnd, L"Invalid input. Please enter valid numbers.", L"Error", MB_OK | MB_ICONERROR);
                }
                else {
                    std::cout << prob;
                    graph.gen_rand_graph(size, prob);
                    REDRAW_GRAPH;
                    MessageBox(hWnd, L"Graph generated successfully!", L"Success", MB_OK);
                    SetForegroundWindow(hMainWindow);
                    DestroyWindow(hWnd);
                }
                break;
            }
            case 4: // "Cancel" button
                DestroyWindow(hWnd);
                break;
            }
            break;

        case WM_CLOSE:
            DestroyWindow(hWnd);
            break;
        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
        }
    
    return 0;
}


// Function to create the popUI for generating a random graph
void create_input_popup(HWND parent) {
    // Register a window class for the popup
    WNDCLASS wc = {};
    wc.lpfnWndProc = popup_procedure;
    wc.hInstance = GetModuleHandle(NULL);
    wc.lpszClassName = L"PopupClass";
    wc.hbrBackground = (HBRUSH)(COLOR_3DFACE + 1); // Classic gray background
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    RegisterClass(&wc);

    // Create the popup window with classic styles
    HWND hPopup = CreateWindowEx(
        WS_EX_DLGMODALFRAME,    // Classic modal dialog frame
        L"PopupClass",          // Registered class name
        L"Input Graph Details", // Title
        WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU, // Classic styles
        CW_USEDEFAULT, CW_USEDEFAULT, 400, 300,  // Position and size
        hMainWindow, NULL, GetModuleHandle(NULL), NULL
    );
    ShowWindow(hPopup, SW_SHOW);
    UpdateWindow(hPopup);
}


// Function to process button clicks
void process_button_click(HWND hWnd, WPARAM wParam) {
    int commandId = LOWORD(wParam);
        switch (LOWORD(wParam)) {
        // Generate random graph
        case 1: {
            create_input_popup(hWnd);
            break;
        }
        // Randomly color graph
        case 2: {
            graph.gen_rand_colors();
            REDRAW_GRAPH;
            break;
        }
        // Export graph to file
        case 3: {
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
        // Import graph from file
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
            REDRAW_GRAPH;
            break;
        }
        // Check Edge in graph
        case 5: {
            if (graph.has_edge()) {
                MessageBox(hWnd, L"Contains an Edge", L"Info", MB_OK);
            }
            else {
                MessageBox(hWnd, L"Does Not Contain an Edge", L"Info", MB_OK);
            }
            break;
        }
        // Check k3 in graph
        case 6: {
            if (graph.has_k3()) {
                MessageBox(hWnd, L"Contains a k3", L"Info", MB_OK);
            }
            else {
                MessageBox(hWnd, L"Does Not Contain a k3", L"Info", MB_OK);
            }
            break;
        }
        // Check k4 in graph 
        case 7: {    
            if (graph.has_k4()) {
                MessageBox(hWnd, L"Contains a k4", L"Info", MB_OK);
            }
            else {
                MessageBox(hWnd, L"Does Not Contain a k4", L"Info", MB_OK);
            }
            break;
        }
        // Check if graph is Hamiltonian
        case 8: {
            if (graph.is_hamiltonian()) {
                MessageBox(hWnd, L"Is Hamiltonian", L"Info", MB_OK);
            }
            else {
                MessageBox(hWnd, L"Not Hamiltonian", L"Info", MB_OK);
            }
            break;
        }
        // CPrint graph
        case 9: {
            graph.cprint();
            break;
        }
        // Add vertex
        case 10: {
            graph.add_vertex();
            REDRAW_GRAPH;
            break;
        }
        // Generate SVG
        case 11: {
            wchar_t filename[MAX_PATH] = L"";
            OPENFILENAME ofn = {};
            ofn.lStructSize = sizeof(ofn);
            ofn.hwndOwner = hWnd;
            ofn.lpstrFilter = L"Vector graphics\0*.svg\0";
            ofn.lpstrFile = filename;
            ofn.nMaxFile = MAX_PATH;
            ofn.Flags = OFN_OVERWRITEPROMPT;
            if (GetSaveFileName(&ofn)) {
                graph.generate_svg(filename);
                MessageBox(hWnd, L"Graph Exported to SVG!", L"Info", MB_OK);
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
        int leftStart = LEFT_PANEL_WIDTH + 5;
        int topStart = TOP_START;
        int rightEnd = RIGHT_END(clientRect.right);
        int bottomEnd = BOTTOM_END(clientRect.bottom);

        // Use the graph dimensions for drawing
        FillRect(hdc, &ps.rcPaint, (HBRUSH)(COLOR_BTNFACE + 2));
        draw_graph(hdc, leftStart, topStart, rightEnd, bottomEnd);

        EndPaint(hWnd, &ps);
        break;
    }
    case WM_COMMAND:
        process_button_click(hWnd, wParam);
        break;

    case WM_SIZE: {
        // Retrieve the new dimensions of the client area
        int width = LOWORD(lParam);  
        int height = HIWORD(lParam); 

        // Calculate graph boundaries
        int leftStart = LEFT_START;
        int topStart = TOP_START;
        int rightEnd = RIGHT_END(width);
        int bottomEnd = BOTTOM_END(height);

        int graphWidth = GRAPH_WIDTH(width);
        int graphHeight = GRAPH_HEIGHT(height);

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


    WNDCLASSEX wcex = {};
    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.lpfnWndProc = window_procedure;     
    wcex.hInstance = hInstance;
    wcex.lpszClassName = L"GraphAppClass";

    
    RegisterClassEx(&wcex);
    HFONT hFont = (HFONT)GetStockObject(DEFAULT_GUI_FONT);
    SendMessage(hLeftPanel, WM_SETFONT, (WPARAM)hFont, TRUE);

    // Create the window
    HWND hWnd = CreateWindowEx(WS_EX_WINDOWEDGE | WS_EX_CLIENTEDGE,
        L"GraphAppClass", L"Graph Application",
        WS_OVERLAPPEDWINDOW | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
        CW_USEDEFAULT, CW_USEDEFAULT,
        800, 600, NULL, NULL, hInstance, NULL);

    hMainWindow = hWnd;
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





