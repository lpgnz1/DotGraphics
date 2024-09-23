// Draw.hpp
#ifndef DRAW_HPP
#define DRAW_HPP

#include <windows.h>  // Include necessary for HDC and other WinAPI types
#include <vector> 

extern const int MIN_RADIUS_FACTOR;
extern const int NUM_CIRCLES_PER_ROW;
extern const int MARGIN;
extern int originalRadius;
extern int currentFrame;

// Function declaration for drawing the grid
void DrawGrid(HDC hdc, int windowWidth, int windowHeight, int gridSize);

// Function for rendering circls in grid to screen
void DrawCircles(HDC hdc, int windowWidth, int windowHeight, int gridSize, int* radiusChangeStep, std::vector<std::vector<int>>& currentRadius);

// Function for setting gloabl variables
void CalculateOriginalRadius(int windowWidth, int windowHeight, int* originalRadius);

#endif // DRAW_HPP
