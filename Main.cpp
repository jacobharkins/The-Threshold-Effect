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

#include <windows.h>
#include <string>
#include "graphs.h" // Your graph header file

// Global variables
HINSTANCE hInst;
Graph graph;

// Function declarations
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

// Entry point for Win32 applications
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    hInst = hInstance;

    if (!AllocConsole()) {
        std::cerr << "Error allocating console" << std::endl;
        return -1;
    }
    FILE* consoleOut;
    if (freopen_s(&consoleOut, "CONOUT$", "w", stdout) != 0) {
        std::cerr << "Failed to redirect stdout to console!" << std::endl;
        return -1;
    }
    FILE* consoleErr;
    if (freopen_s(&consoleErr, "CONOUT$", "w", stderr) != 0) {
        std::cerr << "Failed to redirect stderr to console!" << std::endl;
        return -1;
    }

    // Define the window class
    WNDCLASS wc = {};
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = L"GraphApp";

    RegisterClass(&wc);

    // Create the window
    HWND hWnd = CreateWindowEx(
        0,
        L"GraphApp",
        L"Graph Visualization",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
        nullptr,
        nullptr,
        hInstance,
        nullptr
    );

    if (!hWnd) {
        MessageBox(nullptr, L"Failed to create window!", L"Error", MB_OK | MB_ICONERROR);
        return 0;
    }

    ShowWindow(hWnd, nCmdShow);

    // Message loop
    MSG msg = {};
    while (GetMessage(&msg, nullptr, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return (int)msg.wParam;
}

// Window procedure for handling messages
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message) {
        case WM_PAINT: {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);

            // Example rendering: draw vertices and edges
            for (const Edge& edge : graph.E) {
                MoveToEx(hdc, edge.v1 * 50, edge.v1 * 50, nullptr);
                LineTo(hdc, edge.v2 * 50, edge.v2 * 50);
            }

            for (const Vertex& vertex : graph.V) {
                Ellipse(hdc, vertex.id * 50 - 10, vertex.id * 50 - 10, vertex.id * 50 + 10, vertex.id * 50 + 10);
            }

            EndPaint(hWnd, &ps);
            break;
        }

        case WM_DESTROY: {
            PostQuitMessage(0);
            break;
        }
        case WM_CREATE: {
            HMENU hMenu = CreateMenu();
            HMENU hFileMenu = CreateMenu();

            AppendMenu(hFileMenu, MF_STRING, 3, L"Import");
            AppendMenu(hFileMenu, MF_STRING, 4, L"Export");
            AppendMenu(hMenu, MF_POPUP, (UINT_PTR)hFileMenu, L"File");
            SetMenu(hWnd, hMenu);
            CreateWindow(
                L"BUTTON", L"Generate Graph",
                WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
                50, 100, 150, 30,
                hWnd, (HMENU)1, hInst, nullptr
            );
            CreateWindow(
                L"BUTTON", L"Export Graph",
                WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
                50, 150, 150, 30,
                hWnd, (HMENU)2, hInst, nullptr
            );
            break;
        }
        case WM_COMMAND: {
            switch (LOWORD(wParam)) {
            case 1:
                graph.gen_rand_graph(10, 0.5f);                     // Example function call
                MessageBox(hWnd, L"Graph Generated!", L"Info", MB_OK);
                break;
            case 2:
                graph.export_graph(L"graph.g");
                MessageBox(hWnd, L"Graph Exported!", L"Info", MB_OK);
                break;
            case 3: { // Import
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
            case 4: { // Export
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

            }
            break;
        }
        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
    }
    FreeConsole();
    return 0;
}



