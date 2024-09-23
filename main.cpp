#include <windows.h>
#include <tchar.h>
#include <cmath>
#include "Draw.hpp"

// Global variables
const TCHAR szClassName[] = _T("MyWindowClass");
const int NUM_CIRCLES_PER_ROW = 15;
const int MARGIN = 10;  
const int windowWidth = 800, windowHeight = 800;  // Dimenstions for displayable area 

const int TIMER_ID = 1;             // Timer ID for animation
const int FRAME_DELAY = 30;         // Delay between frames (in milliseconds)
const int MIN_RADIUS_FACTOR = 4;    // Shrink to 1/4 of the original size

int originalRadius;                 // Original radius of the circles
int radiusChangeStep = 1;          // Amount to change radius each frame (positive/negative)
int currentFrame = 1;

// Array to store the current radius for each circle
std::vector<std::vector<int>> currentRadius;

// Step 4: Window Procedure - This processes events/messages like painting, closing, etc.
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
        case WM_CREATE:
        {
            // Dynamically calculate the original radius
            CalculateOriginalRadius(windowWidth, windowHeight, &originalRadius);

            // Resize the 2D vector to match the grid size
            currentRadius.resize(NUM_CIRCLES_PER_ROW, std::vector<int>(NUM_CIRCLES_PER_ROW));

            // Initialize all radii to the original size
            for (int row = 0; row < NUM_CIRCLES_PER_ROW; row++) {
                for (int col = 0; col < NUM_CIRCLES_PER_ROW; col++) {
                    currentRadius[row][col] = originalRadius;
                }
            }

            // Start the timer for animation with a delay of FRAME_DELAY milliseconds
            SetTimer(hwnd, TIMER_ID, FRAME_DELAY, NULL);
        }
        break;

        case WM_TIMER:
        {
            // Handle the timer: Update the frame number and redraw
            if (wParam == TIMER_ID)
            {
                currentFrame++;

                // Force the window to repaint with the updated radii
                InvalidateRect(hwnd, NULL, TRUE);
            }
        }
        break;
        case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);

            // Get the window's client area dimensions
            RECT rect;
            GetClientRect(hwnd, &rect);
            int windowWidth = rect.right - rect.left;
            int windowHeight = rect.bottom - rect.top;

            // Calculate the available width and height for drawing, leaving margins
            int availableWidth = windowWidth - 2 * MARGIN;
            int availableHeight = windowHeight - 2 * MARGIN;

            // Determine the size of each grid cell based on available space
            int gridSize = min(availableWidth, availableHeight) / NUM_CIRCLES_PER_ROW;  // Ensure the grid is square

            DrawGrid(hdc, windowWidth, windowHeight, gridSize);

            // Draw current iterations circles
            DrawCircles(hdc, windowWidth, windowHeight, gridSize, &radiusChangeStep, currentRadius);
            
            EndPaint(hwnd, &ps);
        }
        break;

        case WM_CLOSE:
            KillTimer(hwnd, TIMER_ID);
            DestroyWindow(hwnd);
            break;

        case WM_DESTROY:
            PostQuitMessage(0);
            break;

        default:
            return DefWindowProc(hwnd, msg, wParam, lParam);
    }
    return 0;
}

// Step 1: The entry point of a Windows application
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    // Step 2: Define and register the window class
    WNDCLASSEX wc;
    wc.cbSize        = sizeof(WNDCLASSEX);
    wc.style         = 0;
    wc.lpfnWndProc   = WndProc;
    wc.cbClsExtra    = 0;
    wc.cbWndExtra    = 0;
    wc.hInstance     = hInstance;
    wc.hIcon         = LoadIcon(NULL, IDI_APPLICATION);
    wc.hCursor       = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);
    wc.lpszMenuName  = NULL;
    wc.lpszClassName = szClassName;
    wc.hIconSm       = LoadIcon(NULL, IDI_APPLICATION);

    // Register the window class
    if (!RegisterClassEx(&wc))
    {
        MessageBox(NULL, _T("Window Registration Failed!"), _T("Error"), MB_ICONEXCLAMATION | MB_OK);
        return 0;
    }

    // Define the window style
    DWORD windowStyle = WS_OVERLAPPEDWINDOW;

    // Calculate the actual window size (including frame, title bar, etc.) to get a client area of 800x800
    RECT adjustedRect = { 0, 0, windowWidth, windowHeight };
    AdjustWindowRect(&adjustedRect, windowStyle, FALSE);

    // Step 3: Create the window
    HWND hwnd = CreateWindowEx(
        WS_EX_CLIENTEDGE,
        szClassName,                            // Window class name
        _T("My First Window"),                  // Window title
        WS_OVERLAPPEDWINDOW,                    // Window style
        CW_USEDEFAULT, CW_USEDEFAULT,           // Position
        adjustedRect.right - adjustedRect.left, // Window Width
        adjustedRect.bottom - adjustedRect.top, // Window Height
        NULL, NULL, hInstance, NULL);

    if (hwnd == NULL)
    {
        MessageBox(NULL, _T("Window Creation Failed!"), _T("Error"), MB_ICONEXCLAMATION | MB_OK);
        return 0;
    }

    // Step 4: Show the window and update it
    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);

    // Step 5: The message loop
    MSG Msg;
    while (GetMessage(&Msg, NULL, 0, 0) > 0)
    {
        TranslateMessage(&Msg);
        DispatchMessage(&Msg);
    }

    return Msg.wParam;
}