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

void Fill::applyOnPress(JG::Color** image, int canvasWidth, int canvasHeight)
{

}

void Fill::applyOnMove(JG::Color** image, int canvasWidth, int canvasHeight)
{
    
}

void Fill::applyOnRelease(JG::Color** image, int canvasWidth, int canvasHeight)
{

}