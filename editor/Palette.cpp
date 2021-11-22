/*++

Copyright (c) 2021 JulesIMF, MIPT

Module Name:

    Palette.cpp

Abstract:

    Palette is JG::Widget that allows to pick
    HUE color visually. Once color is changed,
    Palette sends it to all Tools via ToolPicker.

Author / Creation date:

    JulesIMF / 18.10.21

Revision History:

--*/


//
// Includes / usings
//

#include <cmath>
#include <editor/Palette.h>
#include <common/messages.h>

//
// Defines
//

using namespace JIMP;

Palette::Palette(JG::Window* window, int beginX, int beginY,
                 int width, int height, ToolPicker* toolPicker) : 
    Widget(window, beginX, beginY, width, height), 

    paletteSquare(new PaletteSquare(window, space, space, width - 3 * space - PalettePanel::panelWidth,
                                    width - 3 * space - PalettePanel::panelWidth, this)),

    palettePanel(new PalettePanel(window, width - space - PalettePanel::panelWidth, space, width - 4 * space - 2 * PalettePanel::panelWidth,
                                  paletteSquare)),

    toolPicker(toolPicker)

{
    addChild(paletteSquare);
    addChild(palettePanel);
}

void Palette::updateColor(JG::Color color)
{
    this->color = color;
    toolPicker->setColor(color);
    window->sendEvent(JG::Event::PaintEvent());
}

void Palette::renderMyself(int shiftX, int shiftY)
{
    JG::Rectangle blackRect(shiftX + width - space - PalettePanel::panelWidth,
                            shiftY + beginY + height - space - PalettePanel::panelWidth,
                            PalettePanel::panelWidth,
                            PalettePanel::panelWidth);

    blackRect.setColor(JG::Color::Black);
    blackRect.draw(*window);

    static int const colorExampleSpace = 5;

    JG::Rectangle currentColorRect(shiftX + width - space - PalettePanel::panelWidth + colorExampleSpace,
                                   shiftY + beginY + height - space - PalettePanel::panelWidth + colorExampleSpace,
                                   PalettePanel::panelWidth - 2 * colorExampleSpace,
                                   PalettePanel::panelWidth - 2 * colorExampleSpace);

    currentColorRect.setColor(color);
    currentColorRect.draw(*window);
}

Palette::PaletteSquare::PaletteSquare(JG::Window* window, int beginX, int beginY,
                                      int width, int height, Palette* palette) : 
    JG::Canvas(window, beginX, beginY, width, height),
    palette(palette)
{
    calcBlt();
}

void Palette::PaletteSquare::updateKeyColor(JG::Color color)
{
    this->keyColor = color;
    this->color = calcColor(positionX, positionY);
    palette->updateColor(this->color);
    calcBlt();
}

JG::Color Palette::PaletteSquare::calcColor(double positionX, double positionY)
{
    return JG::Color
    {
        (int)((1.0 - positionY) * (keyColor.r + (int)((1.0 - positionX) * ((double)(255 - keyColor.r))))),
        (int)((1.0 - positionY) * (keyColor.g + (int)((1.0 - positionX) * ((double)(255 - keyColor.g))))),
        (int)((1.0 - positionY) * (keyColor.b + (int)((1.0 - positionX) * ((double)(255 - keyColor.b))))),
        255
    };
}

void Palette::PaletteSquare::calcBlt()
{
    for (int i = 0; i != width; i++)
    {
        double positionX = (double)i / (double)width;
        for (int j = 0; j != height; j++)
        {
            double positionY = (double)j / (double)height;
            setPixel(i, j, calcColor(positionX, positionY));
        }
    }

    int xSwitcher = int(positionX * (double)width);
    int ySwitcher = int(positionY * (double)height);
    
    int radius = 4;
    int outRadius = 8;
    
    for (int i = xSwitcher - outRadius; i <= xSwitcher + outRadius; i++)
        for (int j = ySwitcher - outRadius; j <= ySwitcher + outRadius; j++)
        {
            double distance = sqrt( (xSwitcher - i) * (xSwitcher - i) + (ySwitcher - j) * (ySwitcher - j) );

            if (distance > (double)outRadius)
                continue;

            if (distance < (double)radius)
                continue;
                
            setPixel(i, j, {0, 0, 0});
        }   

    for (int radX = 0; radX <= smoothRadius; radX++)
        for (int radY = 0; radY <= smoothRadius; radY++)
        {
            if (radX * radX + radY * radY < smoothRadius * smoothRadius)
                continue;

            static JG::Color const transparent = {0, 0, 0, 0};

            setPixel(smoothRadius - radX,             smoothRadius - radY,          transparent);
            setPixel(width - smoothRadius + radX - 1, smoothRadius - radY,          transparent);
            setPixel(smoothRadius - radX,             height - smoothRadius + radY - 1, transparent);
            setPixel(width - smoothRadius + radX - 1, height - smoothRadius + radY - 1, transparent);
            
            // debugMessage("set transparent on %d %d", radX, radY);
        }

    flush();
}

JG::Widget::HandlerResponce Palette::PaletteSquare::onMouseButtonPressed(JG::Event event)
{
    positionX = (double)event.mouseButton.x / (double)width,
    positionY = (double)event.mouseButton.y / (double)height;

    positionX = std::min(positionX, 1.0);
    positionX = std::max(positionX, 0.0);
    positionY = std::min(positionY, 1.0);
    positionY = std::max(positionY, 0.0);

    this->color = calcColor(positionX, positionY);
    palette->updateColor(color);

    calcBlt();
    Widget::onMouseButtonPressed(event);
    return HandlerResponce::Success;
}

JG::Widget::HandlerResponce Palette::PaletteSquare::onMouseMoved(JG::Event event)
{
    if (!mousePressed)
        return JG::Widget::HandlerResponce::Success;
    
    positionX = (double)event.mouseMove.x / (double)width,
    positionY = (double)event.mouseMove.y / (double)height;

    positionX = std::min(positionX, 1.0);
    positionX = std::max(positionX, 0.0);
    positionY = std::min(positionY, 1.0);
    positionY = std::max(positionY, 0.0);

    this->color = calcColor(positionX, positionY);
    palette->updateColor(color);

    calcBlt();
    Widget::onMouseMoved(event);
    return HandlerResponce::Success;
}

Palette::PalettePanel::PalettePanel(JG::Window* window, int beginX, int beginY,
                                    int height, PaletteSquare* paletteSquare) : 
    JG::Canvas(window, beginX, beginY, panelWidth, height),
    paletteSquare(paletteSquare)
{
    calcBlt();
}                                    

JG::Color Palette::PalettePanel::calcKeyColor(double position)
{
    int sector = (int)position;
    double mixCoef = 1.0 - (position - (double)sector);
    return
    {
        int(mixCoef * (double)knots[sector].r + (1.0 - mixCoef) * (double)knots[sector + 1].r),
        int(mixCoef * (double)knots[sector].g + (1.0 - mixCoef) * (double)knots[sector + 1].g),
        int(mixCoef * (double)knots[sector].b + (1.0 - mixCoef) * (double)knots[sector + 1].b)
    };
}

void Palette::PalettePanel::calcBlt()
{
    for (int j = 0; j != height; j++)
    {
        double positionY = (double)j / (double)height;
        positionY *= 6.0;
        auto kColor = calcKeyColor(positionY);
        for (int i = 0; i != width; i++)
            setPixel(i, j, kColor);
    }

    int ySwitcher = int(position * (double)height / 6.0);

    int radius = 2;

    for (int i = 0; i != width; i++)
        for (int j = ySwitcher - radius; j <= ySwitcher + radius; j++)
            setPixel(i, j, { 0, 0, 0 });

    for (int radX = 0; radX <= smoothRadius; radX++)
        for (int radY = 0; radY <= smoothRadius; radY++)
        {
            if (radX * radX + radY * radY < smoothRadius * smoothRadius)
                continue;

            static JG::Color const transparent = { 0, 0, 0, 0 };

            setPixel(smoothRadius - radX, smoothRadius - radY, transparent);
            setPixel(width - smoothRadius + radX - 1, smoothRadius - radY, transparent);
            setPixel(smoothRadius - radX, height - smoothRadius + radY - 1, transparent);
            setPixel(width - smoothRadius + radX - 1, height - smoothRadius + radY - 1, transparent);

            // debugMessage("set transparent on %d %d", radX, radY);
        }

    flush();
}

JG::Widget::HandlerResponce Palette::PalettePanel::onMouseButtonPressed(JG::Event event)
{
    position = (double)event.mouseButton.y / (double)height;

    position = std::min(position, 1.0);
    position = std::max(position, 0.0);

    position *= 6;

    this->keyColor = calcKeyColor(position);
    paletteSquare->updateKeyColor(keyColor);

    calcBlt();
    Widget::onMouseButtonPressed(event);
    return HandlerResponce::Success;
}
JG::Widget::HandlerResponce Palette::PalettePanel::onMouseMoved(JG::Event event)
{
    if (!mousePressed)
        return JG::Widget::HandlerResponce::Success;

    position = (double)event.mouseMove.y / (double)height;

    position = std::min(position, 1.0);
    position = std::max(position, 0.0);

    position *= 6;

    this->keyColor = calcKeyColor(position);
    paletteSquare->updateKeyColor(keyColor);

    calcBlt();
    Widget::onMouseMoved(event);
    return HandlerResponce::Success;
}