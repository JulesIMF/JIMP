/*++

Copyright (c) 2021 JulesIMF, MIPT

Module Name:

    Panel.cpp

Abstract:

    

Author / Creation date:

    JulesIMF / 18.10.21

Revision History:

--*/


//
// Includes / usings
//

#include <JG.h>
using namespace JG;

//
// Defines
//

Panel::Panel(Window* window, int beginX, int beginY,
                             int width,  int height) : Widget(window, beginX, beginY,
                                                                                width,  height + PanelBar::barHeight)
{
    children.push_back(panelBar = new PanelBar(window, this, -outline, 0, width + outline * 2));
}

void Panel::addChild(Widget* widget)
{
    widget->beginY += PanelBar::barHeight;
    Widget::addChild(widget);
}

void Panel::renderMyself(int shiftX, int shiftY)
{
    Color panelColor = { 0x2f, 0x2f, 0x2f }; // Серый

    Rectangle borderRectngle(shiftX + beginX - outline,    shiftY + beginY,
                             width + outline * 2,          height + outline);

    borderRectngle.setColor(panelColor);
    borderRectngle.draw(*window);

    Rectangle centerRectangle(shiftX + beginX,    shiftY + beginY,
                              width,              height);

    centerRectangle.setColor({0,0,0});
    
    centerRectangle.draw(*window);
}

void Panel::closePanel()
{
    window->sendEvent(Event::PaintEvent());
    window->pushToEraseQueue(this);
}

Panel::PanelBar::PanelBar(Window* window, Panel* panel, int beginX, int beginY, int width) : Widget(window, beginX, beginY,
                                                                                                                      width,  Panel::PanelBar::barHeight), 
                                                                                             panel(panel)
{
    addChild(new PanelCloseButton(window, panel, width - barHeight + Panel::outline, Panel::outline));
}

Widget::HandlerResponce Panel::PanelBar::onMouseMoved(Event event)
{
    if (!mousePressed)
        return HandlerResponce::SuccessYield;
    
    window->setActive(panel);
    panel->beginX += event.mouseMove.dx;
    panel->beginY += event.mouseMove.dy;
    window->sendEvent(Event::PaintEvent());

    return HandlerResponce::Success;
}

void Panel::PanelBar::renderMyself(int shiftX, int shiftY)
{
    Color panelColor = { 0x2f, 0x2f, 0x2f }; // Серый
    Rectangle rectangle(shiftX + beginX, shiftY + beginY,
                        width,           height);
        
    rectangle.setColor(panelColor);
    
    rectangle.draw(*window);

    if (panel->caption == nullptr)
        return;
    
    sf::Font font;
    
    if (!font.loadFromFile(defaultFont))
    {
        printf("Can`t load %s\n", defaultFont);
        return;
    }

    int charSize = 12;

    int const referenceChar = 'A';

    auto glyph = font.getGlyph(referenceChar, charSize, false);

    sf::Text text;
    text.setFont(font);
    text.setString(panel->caption);
    text.setCharacterSize(charSize);
    text.setFillColor(Color::White);

    float textShiftY = ((float)height - glyph.bounds.height + glyph.bounds.top) / 2.0;
    float textShiftX = Panel::outline * 2;

    text.move({ (float)beginX + (float)shiftX + textShiftX, (float)beginY + (float)shiftY + textShiftY });
    window->getSfWindow().draw(text);
}

Panel::PanelBar::PanelCloseButton::PanelCloseButton(Window* window, 
                                                    Panel* panel, 
                                                    int beginX, 
                                                    int beginY) : Button(window, beginX, beginY,
                                                                                 barHeight - 2 * Panel::outline,
                                                                                 barHeight - 2 * Panel::outline),
                                                                                 panel(panel)
{

}

void Panel::PanelBar::PanelCloseButton::renderMyself(int shiftX, int shiftY)
{
    Color buttonColor = { 255, 0, 0 }; // красный
    Rectangle redSquare(shiftX + beginX, shiftY + beginY,
                        width, height);
    
    redSquare.setColor(buttonColor);
    
    redSquare.draw(*window);

    int crossSpace = 2;

    for (int i = crossSpace; i < width - crossSpace; i++)
    {
        sf::Vertex cross[] = {
            { {(float)(i + beginX + shiftX), float(i + beginY + shiftY + 1)}, {255, 255, 255}},
            { {(float)(i + beginX + shiftX), float(width - i + beginY + shiftY)}, {255, 255, 255}}
        };
        window->getSfWindow().draw(cross, 2, sf::Points);
    }
}

#pragma GCC diagnostic ignored "-Wunused-parameter"

Widget::HandlerResponce Panel::PanelBar::PanelCloseButton::onClick(Event event)
{
    panel->closePanel();
    return Widget::HandlerResponce::Success;
}
