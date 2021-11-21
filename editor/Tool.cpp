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
#include <algorithm>
#include <editor/Editor.h>
#include <editor/Tool.h>
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

void Brush::applyOnPress(Layer& layer)
{
    JG::Color** image = layer.image;
    if (image == nullptr)
        return;

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
                image[currentX][currentY] = color;
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

void Eraser::applyOnPress(Layer& layer)
{
    JG::Color** image = layer.image;
    
    if (image == nullptr)
        return;

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
                image[currentX][currentY] = {255, 255, 255, 0};
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
    auto oldColor = image[x][y];
    image[x][y] = color;

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