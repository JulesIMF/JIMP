/*++

Copyright (c) 2021 JulesIMF, MIPT

Module Name:

    Button.cpp

Abstract:

    Button class defenition (simple gray rectangle).

Author / Creation date:

    JulesIMF / 11.10.21

Revision History:

--*/


//
// Includes / usings
//

#include <Shape.h>
#include <Widget.h>
#include <Window.h>
using namespace JG;

//
// Defines
//

void Button::setCaption(char const* caption)
{
    this->caption = caption;
    window->sendEvent(Event::PaintEvent());
}

char const* Button::getCaption()
{
    return caption;
}

void Button::setRectangle(Rectangle rectangle)
{
    defaultRectangle = currentRectangle = rectangle;
    validate();
    window->sendEvent(Event::PaintEvent());
}

Button::Button(Window* window,
               int beginX, int beginY,
               int width,  int height,
               Color color) : Widget(window, beginX, beginY, width, height),

                              defaultRectangle(Rectangle(beginX, beginY, width, height)),
                              
                              currentRectangle(defaultRectangle)
{
    defaultRectangle = Rectangle(beginX, beginY, width, height);
    defaultRectangle.setColor(color);
    // // setCurrentRectangle();();();
}

Rectangle Button::getRectangle()
{
    return defaultRectangle;
}

void Button::validate()
{
    
}

#pragma GCC diagnostic ignored "-Wunused-parameter"

Widget::HandlerResponce Button::onMouseEntered(Event event)
{
    // setCurrentRectangle();();
    window->sendEvent(Event::PaintEvent());
    return Widget::HandlerResponce::Success;
}

Widget::HandlerResponce Button::onMouseLeft(Event event)
{
    // setCurrentRectangle();
    window->sendEvent(Event::PaintEvent());
    return Widget::HandlerResponce::Success;
}

Widget::HandlerResponce Button::onMouseButtonPressed(Event event)
{
    // setCurrentRectangle();
    window->sendEvent(Event::PaintEvent());
    return Widget::HandlerResponce::Success;
}

Widget::HandlerResponce Button::onMouseButtonReleased(Event event)
{
    // setCurrentRectangle();
    if (event.mouseButton.button == Mouse::Button::Left)
        onClick(event);

    if (event.mouseButton.button == Mouse::Button::Right)
        onRightClick(event);
        
    window->sendEvent(Event::PaintEvent());
    return Widget::HandlerResponce::Success;
}

Widget::HandlerResponce Button::onKeyPressed(Event event)
{
    // setCurrentRectangle();
    if(event.key.code == Keyboard::Enter)
    {
        onClick(event);
        return Widget::HandlerResponce::Success;
    }

    return Widget::HandlerResponce::Failure;
}

void Button::setCurrentRectangle()
{
    currentRectangle = defaultRectangle;
    auto currentColor = defaultRectangle.getColor();

    double hc = 1.3;

    if (mouseOn)
        currentColor.r = int(currentColor.r * hc),
        currentColor.g = int(currentColor.g * hc),
        currentColor.b = int(currentColor.b * hc);

    if (mousePressed)
        currentColor.r = int(currentColor.r * hc),
        currentColor.g = int(currentColor.g * hc),
        currentColor.b = int(currentColor.b * hc);
    
    currentRectangle.setColor(currentColor);
    currentRectangle.setPosition(beginX, beginY);
}

void Button::renderMyself(int shiftX, int shiftY)
{    
    setCurrentRectangle();
    auto shiftRectangle = currentRectangle;
    shiftRectangle.move(shiftX, shiftY);
    shiftRectangle.draw(*window);

    if (caption == nullptr)
        return;

    sf::Font font;

    if (!font.loadFromFile(defaultFont))
    {
        printf("Can`t load %s\n", defaultFont);
        return;
    }

    int charSize = 24;

    int const referenceChar = 'A';
    float const spaceCoef   = 1.3;

    auto  glyph             = font.getGlyph(referenceChar, charSize, false);
    int   textLength        = strlen(caption);
    float textWidth         = 0;

    while ( (textWidth = (float)textLength * glyph.bounds.width) * spaceCoef > (float)width ||
             glyph.bounds.height                                 * spaceCoef > (float)height )
    {
        charSize--;
        glyph = font.getGlyph(referenceChar, charSize, false);
    }

    sf::Text text;
    text.setFont(font);
    text.setString(caption);
    text.setCharacterSize(charSize);
    text.setFillColor(Color::White);

    float textShiftY = ((float)height - glyph.bounds.height) / 2.0;
    float textShiftX = ((float)width  - textWidth)           / 2.0;

    text.move({ (float)beginX + (float)shiftX + textShiftX, (float)beginY + (float)shiftY + textShiftY });
    window->getSfWindow().draw(text);
}

Widget::HandlerResponce Button::onClick(Event event)
{
    return Widget::HandlerResponce::Success;
}

Widget::HandlerResponce Button::onRightClick(Event event)
{
    return Widget::HandlerResponce::Success;
}
