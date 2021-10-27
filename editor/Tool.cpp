/*++

Copyright (c) 2021 JulesIMF, MIPT

Module Name:

    Tool.cpp

Abstract:

    

Author / Creation date:

    JulesIMF / 18.10.21

Revision History:

--*/


//
// Includes / usings
//

#include <cmath>
#include <editor/Editor.h>
#include <editor/Tool.h>
#include <common/messages.h>

//
// Defines
//

using namespace JIMP;

void Brush::applyOnPress(JG::Color** image, int canvasWidth, int canvasHeight)
{
    if (image == nullptr)
        return;
        
    for (int currentX = std::max(x - thickness, 0); currentX <= x + thickness; currentX++)
    {
        if (currentX >= canvasWidth)
            break;

        for (int currentY = std::max(y - thickness, 0); currentY <= y + thickness; currentY++)
        {
            if (currentY >= canvasHeight)
                break;

            if ((y - currentY) * (y - currentY) + (x - currentX) * (x - currentX) <= thickness * thickness)
                image[currentX][currentY] = color;
        }
    }
}

void Brush::applyOnMove(JG::Color** image, int canvasWidth, int canvasHeight)
{
    applyOnPress(image, canvasWidth, canvasHeight);
}

void Brush::applyOnRelease(JG::Color** image, int canvasWidth, int canvasHeight)
{

}

char const* Brush::getName()
{
    return "Brush";
}

void Eraser::applyOnPress(JG::Color** image, int canvasWidth, int canvasHeight)
{
    if (image == nullptr)
        return;

    for (int currentX = std::max(x - thickness, 0); currentX <= x + thickness; currentX++)
    {
        if (currentX >= canvasWidth)
            break;

        for (int currentY = std::max(y - thickness, 0); currentY <= y + thickness; currentY++)
        {
            if (currentY >= canvasHeight)
                break;

            if ((y - currentY) * (y - currentY) + (x - currentX) * (x - currentX) <= thickness * thickness)
                image[currentX][currentY] = {255, 255, 255, 0};
        }
    }
}

void Eraser::applyOnMove(JG::Color** image, int canvasWidth, int canvasHeight)
{
    applyOnPress(image, canvasWidth, canvasHeight);
}

void Eraser::applyOnRelease(JG::Color** image, int canvasWidth, int canvasHeight)
{

}

char const* Eraser::getName()
{
    return "Eraser";
}

inline void Fill::walkAndSet(JG::Color** image, JG::Color oldColor, JG::Vector2i point, 
                             std::queue<JG::Vector2i>& pointsQueue, 
                             int canvasWidth, int canvasHeight)
{
    int x = point.x, y = point.y;
    // debugMessage("process (%d, %d)", x, y);
    if (x - 1 >= 0 && image[x - 1][y] == oldColor && image[x - 1][y] != color)
    {
        image[x - 1][y] = color;
        pointsQueue.push({x - 1, y});
    }

    if (x + 1 < canvasWidth && image[x + 1][y] == oldColor && image[x + 1][y] != color)
    {
        image[x + 1][y] = color;
        pointsQueue.push({ x + 1, y });
    }

    if (y - 1 >= 0 && image[x][y - 1] == oldColor && image[x][y - 1] != color)
    {
        image[x][y - 1] = color;
        pointsQueue.push({ x, y - 1 });
    }

    if (y + 1 < canvasHeight && image[x][y + 1] == oldColor && image[x][y + 1] != color)
    {
        image[x][y + 1] = color;
        pointsQueue.push({ x, y + 1 });
    }
}

void Fill::applyOnPress(JG::Color** image, int canvasWidth, int canvasHeight)
{
    std::queue<JG::Vector2i> pointsQueue;
    auto oldColor = image[x][y];
    image[x][y] = color;

    walkAndSet(image, oldColor, { x, y }, pointsQueue, canvasWidth, canvasHeight);
    while (!pointsQueue.empty())
    {
        walkAndSet(image, oldColor, pointsQueue.front(), pointsQueue, canvasWidth, canvasHeight);
        pointsQueue.pop();
    }
    
}

void Fill::applyOnMove(JG::Color** image, int canvasWidth, int canvasHeight)
{
    
}

void Fill::applyOnRelease(JG::Color** image, int canvasWidth, int canvasHeight)
{

}

char const* Fill::getName()
{
    return "Fill";
}