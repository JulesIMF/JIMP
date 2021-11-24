/*++

Copyright (c) 2021 JulesIMF, MIPT

Module Name:

    Tool.cpp

Abstract:

    Tool is an abstraction that changes given layer in
    a specific way.

Author / Creation date:

    JulesIMF / 18.10.21

Revision History:
	22.11.21  22:29		Three "on": on press, on move and on release. Should add on timer

--*/


//
// Includes / usings
//

#include <cmath>
#include <algorithm>
#include <editor/Editor.h>
#include <editor/Tool.h>
#include <editor/ToolPicker.h>
#include <common/messages.h>

//
// Defines
//

using namespace JIMP;

static int outline = 3;

static void translateAndDraw(JG::Image& image, JG::Window* window, int shiftX, int shiftY, int shiftTextureX = 0, int shiftTextureY = 0)
{
    image.moveTexture(shiftTextureX, shiftTextureY);
    image.move(shiftX, shiftY);

    image.draw(*window);

    image.moveTexture(-shiftTextureX, -shiftTextureY);
    image.move(-shiftX, -shiftY);
}

// ****************************************
// Brush
// ****************************************

void Brush::applyOnPress(Layer& layer)
{
    int canvasWidth = layer.width, canvasHeight = layer.height;
        
    for (int currentX = std::max(x - thickness, 0); currentX <= x + thickness; currentX++)
    {
        if (currentX >= canvasWidth)
            break;

        for (int currentY = std::max(y - thickness, 0); currentY <= y + thickness; currentY++)
        {
            if (currentY >= canvasHeight)
                break;

            if ((y - currentY) * (y - currentY) + (x - currentX) * (x - currentX) <= thickness * thickness)
                layer[currentX][currentY] = color;
        }
    }
}

void Brush::applyOnMove(Layer& layer)
{
    applyOnPress(layer);
}

void Brush::applyOnRelease(Layer& layer)
{

}

void Brush::applyOnTimer(Layer& layer, int passedMs)
{

}

char const* Brush::getName()
{
    return "Brush";
}

void Brush::drawCursor(JG::Window* window, int x, int y, int canvasWidth, int canvasHeight, int shiftX, int shiftY)
{
    sf::RenderTexture texture;
    texture.create(canvasWidth, canvasHeight);
    int circleX = x - thickness;
    int circleY = y - thickness;

    sf::CircleShape black;
    black.move({circleX, circleY});
    black.setRadius(thickness);
    black.setFillColor(sf::Color::Black);
    texture.draw(black);

    sf::CircleShape fillColor;
    fillColor.move({ circleX + outline, circleY + outline });
    fillColor.setRadius(thickness - outline);
    fillColor.setFillColor(color);
    texture.draw(fillColor);

    texture.display();
    sf::Transform transform; 
    transform.translate({shiftX, shiftY});
    
    window->getSfWindow().draw(sf::Sprite(texture.getTexture()), transform);
}

// ****************************************
// Eraser
// ****************************************

void Eraser::applyOnPress(Layer& layer)
{
    int canvasWidth = layer.width, canvasHeight = layer.height;

    for (int currentX = std::max(x - thickness, 0); currentX <= x + thickness; currentX++)
    {
        if (currentX >= canvasWidth)
            break;

        for (int currentY = std::max(y - thickness, 0); currentY <= y + thickness; currentY++)
        {
            if (currentY >= canvasHeight)
                break;

            if ((y - currentY) * (y - currentY) + (x - currentX) * (x - currentX) <= thickness * thickness)
                layer[currentX][currentY] = {255, 255, 255, 0};
        }
    }
}

void Eraser::applyOnMove(Layer& layer)
{
    applyOnPress(layer);
}

void Eraser::applyOnRelease(Layer& layer)
{

}

void Eraser::applyOnTimer(Layer& layer, int passedMs)
{

}

char const* Eraser::getName()
{
    return "Eraser";
}

void Eraser::drawCursor(JG::Window* window, int x, int y, int canvasWidth, int canvasHeight, int shiftX, int shiftY)
{
    sf::RenderTexture texture;
    texture.create(canvasWidth, canvasHeight);
    int circleX = x - thickness;
    int circleY = y - thickness;

    sf::CircleShape black;
    black.move({ circleX, circleY });
    black.setRadius(thickness);
    black.setFillColor(sf::Color::Black);
    texture.draw(black);

    sf::CircleShape fillColor;
    fillColor.move({ circleX + outline, circleY + outline });
    fillColor.setRadius(thickness - outline);
    fillColor.setFillColor(sf::Color::White);
    texture.draw(fillColor);

    texture.display();
    sf::Transform transform;
    transform.translate({ shiftX, shiftY });

    window->getSfWindow().draw(sf::Sprite(texture.getTexture()), transform);
}

// ****************************************
// Fill
// ****************************************

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

void Fill::applyOnPress(Layer& layer)
{
    auto image = layer.image;
    if (!image)
        return;

    int canvasWidth = layer.width, canvasHeight = layer.height;

    std::queue<JG::Vector2i> pointsQueue;
    auto oldColor = layer[x][y];
    layer[x][y] = color;

    walkAndSet(image, oldColor, { x, y }, pointsQueue, canvasWidth, canvasHeight);
    while (!pointsQueue.empty())
    {
        walkAndSet(image, oldColor, pointsQueue.front(), pointsQueue, canvasWidth, canvasHeight);
        pointsQueue.pop();
    }
    
}

void Fill::applyOnMove(Layer& layer)
{
    
}

void Fill::applyOnRelease(Layer& layer)
{

}

void Fill::applyOnTimer(Layer& layer, int passedMs)
{

}

char const* Fill::getName()
{
    return "Fill";
}

void Fill::drawCursor(JG::Window* window, int x, int y, int canvasWidth, int canvasHeight, int shiftX, int shiftY)
{
    int cursorX = std::max(x - cursorSize / 2, 0);
    int cursorY = std::max(y - cursorSize / 2, 0);
    cursorX = std::min(cursorX, canvasWidth - cursorSize / 2);
    cursorY = std::min(cursorY, canvasHeight - cursorSize / 2);

    translateAndDraw(cursor, window, shiftX + cursorX, shiftY + cursorY);
}

// ****************************************
// Drag
// ****************************************

void Drag::applyOnPress(Layer& layer)
{
    
}

void Drag::applyOnMove(Layer& layer)
{
    layer.beginX += dx;
    layer.beginY += dy;
    x += dx;
    y += dy;
}

void Drag::applyOnRelease(Layer& layer)
{

}

void Drag::applyOnTimer(Layer& layer, int passedMs)
{

}

char const* Drag::getName()
{
    return "Drag";
}

void Drag::drawCursor(JG::Window* window, int x, int y, int canvasWidth, int canvasHeight, int shiftX, int shiftY)
{
    int cursorX = std::max(x - cursorSize / 2, 0);
    int cursorY = std::max(y - cursorSize / 2, 0);
    cursorX = std::min(cursorX, canvasWidth - cursorSize / 2);
    cursorY = std::min(cursorY, canvasHeight - cursorSize / 2);

    translateAndDraw(cursor, window, shiftX + cursorX, shiftY + cursorY);
}

// ****************************************
// Eyedropper
// ****************************************

void Eyedropper::applyOnPress(Layer& layer)
{
    toolPicker->setColor(layer[x][y]);
    debugMessage("eyedropper sent (%d, %d, %d) to all tools", 
                 layer[x][y].r, layer[x][y].g, layer[x][y].b);
}

void Eyedropper::applyOnMove(Layer& layer)
{
    
}

void Eyedropper::applyOnRelease(Layer& layer)
{

}

void Eyedropper::applyOnTimer(Layer& layer, int passedMs)
{

}

char const* Eyedropper::getName()
{
    return "Eyedropper";
}

void Eyedropper::drawCursor(JG::Window* window, int x, int y, int canvasWidth, int canvasHeight, int shiftX, int shiftY)
{
    int cursorX = std::max(x - cursorSize / 2, 0);
    int cursorY = std::max(y - cursorSize / 2, 0);
    cursorX = std::min(cursorX, canvasWidth - cursorSize / 2);
    cursorY = std::min(cursorY, canvasHeight - cursorSize / 2);

    translateAndDraw(cursor, window, shiftX + cursorX, shiftY + cursorY);
}

// ****************************************
// Airbrush
// ****************************************

void Airbrush::applyOnPress(Layer& layer)
{
    
}

void Airbrush::applyOnMove(Layer& layer)
{

}

void Airbrush::applyOnRelease(Layer& layer)
{

}

void Airbrush::applyOnTimer(Layer& layer, int passedMs)
{
    int nDrops = passedMs * intensity * thickness * thickness / 1000;
    for (int i = 0; i < nDrops; i++)
    {
        int dx = (int)(rand() % (2 * thickness + 1) - thickness);
        int dy = (int)(rand() % (2 * thickness + 1) - thickness);
        if (dx * dx + dy * dy > thickness * thickness)
            continue;

        int xDrop = x + dx;
        int yDrop = y + dy;

        if (0 <= xDrop && xDrop < layer.width && 0 <= yDrop && yDrop < layer.height)
            layer[xDrop][yDrop] = color;
    }
}

char const* Airbrush::getName()
{
    return "Airbrush";
}

void Airbrush::drawCursor(JG::Window* window, int x, int y, int canvasWidth, int canvasHeight, int shiftX, int shiftY)
{
    int cursorX = std::max(x - cursorSize / 2, 0);
    int cursorY = std::max(y - cursorSize / 2, 0);
    cursorX = std::min(cursorX, canvasWidth - cursorSize / 2);
    cursorY = std::min(cursorY, canvasHeight - cursorSize / 2);

    translateAndDraw(cursor, window, shiftX + cursorX, shiftY + cursorY);
}

// ****************************************
// Pen
// ****************************************

void Pen::applyOnPress(Layer& layer)
{
    int canvasWidth = layer.width, canvasHeight = layer.height;

    for (int currentX = std::max(x - outline, 0); currentX <= x + outline; currentX++)
    {
        if (currentX >= canvasWidth)
            break;

        for (int currentY = std::max(y - thickness, 0); currentY <= y + thickness; currentY++)
        {
            if (currentY >= canvasHeight)
                break;

            layer[currentX][currentY] = color;
        }
    }
}

void Pen::applyOnMove(Layer& layer)
{
    applyOnPress(layer);
}

void Pen::applyOnRelease(Layer& layer)
{

}

void Pen::applyOnTimer(Layer& layer, int passedMs)
{

}

char const* Pen::getName()
{
    return "Pen";
}

void Pen::drawCursor(JG::Window* window, int x, int y, int canvasWidth, int canvasHeight, int shiftX, int shiftY)
{
    sf::RenderTexture texture;
    texture.create(canvasWidth, canvasHeight);
    int rectX = x - 1;
    int rectY = y - thickness;

    sf::RectangleShape fillColor;
    fillColor.move({ rectX, rectY });
    fillColor.setSize( { float(2 * outline + 1), thickness * 2 + 1 } );
    fillColor.setFillColor(color);
    texture.draw(fillColor);

    texture.display();
    sf::Transform transform;
    transform.translate({ shiftX, shiftY });

    window->getSfWindow().draw(sf::Sprite(texture.getTexture()), transform);
}

// ****************************************
// Rectangle
// ****************************************

void Rectangle::applyOnPress(Layer& layer)
{
    pressX = x;
    pressY = y;
    layer.onCorrectionBegin();
}

void Rectangle::applyOnMove(Layer& layer)
{
    layer.resetPreview();
    int canvasWidth = layer.width, canvasHeight = layer.height;

    int endX = x, endY = y;
    int beginX = pressX, beginY = pressY;
    auto preview = layer.preview;

    if (endX < beginX)
        std::swap(beginX, endX);

    if (endY < beginY)
        std::swap(beginY, endY);

    for (int currentX = std::max(beginX, 0); currentX <= endX; currentX++)
    {
        if (currentX >= canvasWidth)
            break;

        for (int currentY = std::max(beginY, 0); currentY <= endY; currentY++)
        {
            if (currentY >= canvasHeight)
                break;

            preview[currentX][currentY] = color;
        }
    }
}

void Rectangle::applyOnRelease(Layer& layer)
{
    layer.onCorrectionEnd();
}

void Rectangle::applyOnTimer(Layer& layer, int passedMs)
{
    
}

char const* Rectangle::getName()
{
    return "Rectangle";
}

void Rectangle::drawCursor(JG::Window* window, int x, int y, int canvasWidth, int canvasHeight, int shiftX, int shiftY)
{
    int cursorX = std::max(x - cursorSize / 2, 0);
    int cursorY = std::max(y - cursorSize / 2, 0);
    cursorX = std::min(cursorX, canvasWidth - cursorSize / 2);
    cursorY = std::min(cursorY, canvasHeight - cursorSize / 2);

    translateAndDraw(cursor, window, shiftX + cursorX, shiftY + cursorY);
}