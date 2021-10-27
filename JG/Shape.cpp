/*++

Copyright (c) 2021 JulesIMF, MIPT

Module Name:

    Shape.cpp

Abstract:

    

Author / Creation date:

    JulesIMF / 06.10.21

Revision History:
	12.10.21  02:41		Canvas draw introduced

--*/


//
// Includes / usings
//

#include <cassert>
#include <cmath>
#include <JG.h>
using namespace JG;

//
// Defines
//

#ifdef SFML_WRAPPER

void Shape::setPosition(int newX, int newY)
{
    beginX = newX;
    beginY = newY;
}

void Shape::setColor(Color color)
{
    this->color = color;
}

void Shape::move(int deltaX, int deltaY)
{
    beginX += deltaX;
    beginY += deltaY;
}

Color Shape::getColor()
{
    return color;
}

Circle::Circle(int x, int y, int radius) : Shape{x, y}, 
                                           radius(radius),
                                           circle_((float)radius)
{
    circle_.setOrigin({ -(float)x, -(float)y });
}

void Circle::draw(Window& window) const
{
    window.getSfWindow().draw(circle_);
}

void Circle::draw(Canvas& canvas) const
{
    for (int x = beginX; x != beginX + 2 * radius && x < (int)canvas.width; x++)
    {
        bool drawn = false;
        for (int y = beginY; y != beginY + 2 * radius && y < (int)canvas.height; y++)
        {   
            if (sqrt((y - beginY - radius) * (y - beginY - radius) + (x - beginX - radius) * (x - beginX - radius)) > (float)radius)
            {
                if (drawn) break;
                continue;
            }

            drawn = true;
            canvas.setPixel(x, y, color);
        }
    }
}
 
void Circle::setPosition(int newX, int newY)
{
    Shape::setPosition(newX, newY);
    circle_.setOrigin({ -(float)newX, -(float)newY });
}

void Circle::setColor(Color color)
{
    Shape::setColor(color);
    circle_.setFillColor(color);
}

void Circle::move(int deltaX, int deltaY)
{
    Shape::move(deltaX, deltaY);
    circle_.move({ (float)deltaX, (float)deltaY });
}

Rectangle::Rectangle(int x, int y, int width, int height) : Shape{ x, y }, 
                                                            width(width),
                                                            height(height),
                                                            rectangle_({ (float)width, (float)height })
{
    rectangle_.setOrigin({ -(float)x, -(float)y });
}

void Rectangle::draw(Window& window) const
{
    window.getSfWindow().draw(rectangle_);
}

void Rectangle::draw(Canvas& canvas) const
{
    for (int x = beginX; x != beginX + width && x < (int)canvas.width; x++)
        for (int y = beginY; y != beginY + height && y < (int)canvas.height; y++)
            canvas.setPixel(x, y, color);
}

void Rectangle::setPosition(int newX, int newY)
{
    Shape::setPosition(newX, newY);
    rectangle_.setOrigin({ -(float)newX, -(float)newY });
}

void Rectangle::setColor(Color color)
{

    Shape::setColor(color);
    rectangle_.setFillColor(color);
}

void Rectangle::move(int deltaX, int deltaY)
{
    Shape::move(deltaX, deltaY);
    rectangle_.move({ (float)deltaX, (float)deltaY });
}

Image::Image(int x, int y, int width, int height, char const* textureName) : 
    Shape{ x, y },
    width(width),
    height(height), 
    virtualWidth(width),
    virtualHeight(height)
{
    if (!texture.loadFromFile(textureName))
        throw std::runtime_error("cant find texture");       

    sprite.setTexture(texture);
    sprite.setTextureRect({textureX, textureY, width, height});
}



void Image::draw(Window& window) const
{
    sf::Transform transform;
    transform.translate({(float)beginX, (float)beginY});
    window.getSfWindow().draw(sprite, transform);
}

void Image::draw(Canvas& canvas) const
{
    canvas = canvas;
    assert(!"Image::draw(Canvas&) not implemented!");
}

void Image::setPosition(int newX, int newY)
{
    Shape::setPosition(newX, newY);
}

void Image::setColor(Color color)
{
    Shape::setColor(color);
}

void Image::move(int deltaX, int deltaY)
{
    Shape::move(deltaX, deltaY);
}

void Image::moveTexture(int deltaX, int deltaY)
{
    textureX += deltaX;
    textureY += deltaY;
    sprite.setTextureRect({ textureX, textureY, width, height });
}

void Image::setTexturePosition(int newX, int newY)
{
    textureX = newX;
    textureY = newY;
    sprite.setTextureRect({ textureX, textureY, width, height });
}

void Image::scale(int virtualWidth, int virtualHeight)
{
    this->virtualWidth = virtualWidth;
    this->virtualHeight = virtualHeight;
    sprite.scale((float)virtualWidth  / (float)width,
                 (float)virtualHeight / (float)height);
}

#endif //!SFML_WRAPPER