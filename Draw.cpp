#include "Draw.hpp"  // Include the header file
#include <windows.h>   // Include necessary for WinAPI
#include <cmath>
#include <vector>

// Function to calculate the distance between two points (for the ripple effect)
double CalculateDistance(int x1, int y1, int x2, int y2)
{
    return sqrt(pow(x1 - x2, 2) + pow(y1 - y2, 2));
}

// Function to dynamically calculate the original radius
void CalculateOriginalRadius(int windowWidth, int windowHeight, int* originalRadius)
{
    // Calculate the available width and height for drawing, leaving margins
    int availableWidth = windowWidth - 2 * MARGIN;
    int availableHeight = windowHeight - 2 * MARGIN;

    // Determine the size of each grid cell
    int gridSize = min(availableWidth, availableHeight) / NUM_CIRCLES_PER_ROW;

    // Set the original radius to 80% of half the grid size
    *originalRadius = (gridSize / 2) * 0.8;
}

// Function to draw the grid lines
void DrawGrid(HDC hdc, int windowWidth, int windowHeight, int gridSize)
{
    // Create a red pen for the grid lines
    HPEN redPen = CreatePen(PS_SOLID, 2, RGB(255, 0, 0));
    HPEN oldPen = (HPEN)SelectObject(hdc, redPen);

    // Draw vertical grid lines
    for (int col = 0; col <= NUM_CIRCLES_PER_ROW; col++)
    {
        int x = MARGIN + col * gridSize;
        if (x <= windowWidth - MARGIN)  // Ensure the grid stays inside the right margin
        {
            MoveToEx(hdc, x, MARGIN, NULL);          // Start from the top margin
            LineTo(hdc, x, windowHeight - MARGIN);   // Draw to the bottom margin
        }
    }

    // Draw horizontal grid lines
    for (int row = 0; row <= NUM_CIRCLES_PER_ROW; row++)
    {
        int y = MARGIN + row * gridSize;
        if (y <= windowHeight - MARGIN)  // Ensure the grid stays inside the bottom margin
        {
            MoveToEx(hdc, MARGIN, y, NULL);          // Start from the left margin
            LineTo(hdc, windowWidth - MARGIN, y);    // Draw to the right margin
        }
    }

    // Restore the original pen and delete the red pen
    SelectObject(hdc, oldPen);
    DeleteObject(redPen);
}


void DrawCircles(HDC hdc, int windowWidth, int windowHeight, int gridSize, int* radiusChangeStep, std::vector<std::vector<int>>& currentRadius)
{
    // Calculate the center of the grid
    int centerRow = NUM_CIRCLES_PER_ROW / 2;
    int centerCol = NUM_CIRCLES_PER_ROW / 2;

    // Loop to draw circles in a 5x5 grid
    for (int row = 0; row < NUM_CIRCLES_PER_ROW; row++)
    {
        for (int col = 0; col < NUM_CIRCLES_PER_ROW; col++)
        {
            // Calculate the center of each circle, adding margins
            int xCenter = MARGIN + (col * gridSize) + (gridSize / 2);
            int yCenter = MARGIN + (row * gridSize) + (gridSize / 2);

            // Calculate the distance from the center of the grid
            double distanceFromCenter = CalculateDistance(row, col, centerRow, centerCol);

            // Delay the ripple effect based on the distance from the center
            int delayFrames = static_cast<int>(distanceFromCenter * 5);  // Adjust delay multiplier as needed

            // Only start the animation for circles after their delay has passed
            if (currentFrame >= delayFrames)
            {
                currentRadius[row][col] += *radiusChangeStep;

                // Reverse direction if the radius exceeds the original or the minimum size
                if (currentRadius[row][col] <= originalRadius / MIN_RADIUS_FACTOR || currentRadius[row][col] >= originalRadius)
                {
                    *radiusChangeStep = -(*radiusChangeStep);  // Reverse the growth/shrink direction
                }
            }

            // Draw the circle with the current animated radius
            Ellipse(hdc, xCenter - currentRadius[row][col], yCenter - currentRadius[row][col], 
                    xCenter + currentRadius[row][col], yCenter + currentRadius[row][col]);
        }
    }
}