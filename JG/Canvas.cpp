/*++

Copyright (c) 2021 JulesIMF, MIPT

Module Name:

    Canvas.cpp

Abstract:

    Canvas is a widget that allows to use it as a
    render context.

Author / Creation date:

    JulesIMF / 11.10.21

Revision History:

--*/


//
// Includes / usings
//

#include <algorithm>
#include <Widget.h>
#include <Window.h>
using namespace JG;

//
// Defines
//

Canvas::~Canvas()
{
    for (int x = 0; x != (int)width; x++)
        delete[] bltCanvas[x];

    for (int x = 0; x != (int)width; x++)
        delete[] bltRender[x];

    delete[] bltCanvas;
    delete[] bltRender;
}

Canvas::Canvas(Window* window,
               int beginX, int beginY,
               int width,  int height) : 
    Widget(window, beginX, beginY, width, height)
{
    bltCanvas = new sf::Vertex* [width];
    bltRender = new sf::Vertex* [width];

    for (int x = 0; x != (int)width; x++)
    {
        bltCanvas[x] = new sf::Vertex[height];
        for (int y = 0; y != (int)height; y++)
            bltCanvas[x][y] = { { float(x), float(y) }, { 0, 0, 0 } };
    }


    for (int x = 0; x != (int)width; x++)
    {
        bltRender[x] = new sf::Vertex[height];
        for (int y = 0; y != (int)height; y++)
            bltRender[x][y] = { { float(x), float(y) }, { 0, 0, 0 } };
    }
}

void Canvas::flush()
{
    std::swap(bltRender, bltCanvas);
    for (int x = 0; x != (int)width; x++)
        for (int y = 0; y != (int)height; y++)
            bltCanvas[x][y] = { { float(x), float(y) }, { 0, 0, 0 } };
    
    window->sendEvent(Event::PaintEvent());
}

void Canvas::transferFromBuffer(Color** buffer)
{
    for (int x = 0; x != (int)width; x++)
        for (int y = 0; y != (int)height; y++)
            bltCanvas[x][y] = { { float(x), float(y) }, buffer[x][y] };
}

void Canvas::renderMyself(int shiftX, int shiftY)
{
    sf::Transform transform;
    transform.translate({float(shiftX + beginX), float(shiftY + beginY)});

    for (int x = 0; x != (int)width; x++)
        window->getSfWindow().draw(bltRender[x], height, sf::Points, transform);
}