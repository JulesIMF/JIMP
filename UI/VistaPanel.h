/*++

Copyright (c) 2021 JulesIMF, MIPT

Module Name:

    VistaPanel.h

Abstract:

    

Author / Creation date:

    JulesIMF / 27.10.21

Revision History:

--*/


//
// Includes / usings
//

#include <JG.h>

//
// Defines
//

namespace JIMP
{
    namespace UI
    {
        struct VistaPanel : public JG::Panel
        {
            static const int vistaOutline = 8;
            
            VistaPanel(JG::Window* window, int beginX, int beginY,
                                           int width,  int height) : 
                JG::Panel(window, beginX, beginY, width, height),
                
                left(0, VistaPanelBar::vistaBarHeight, vistaOutline, height - VistaPanelBar::vistaBarHeight, "resources/images/Panel/left.png"),
                right(vistaOutline + width, VistaPanelBar::vistaBarHeight, vistaOutline, height - VistaPanelBar::vistaBarHeight, "resources/images/Panel/right.png"),
                lb(0, height, vistaOutline, vistaOutline, "resources/images/Panel/lb.png"),
                rb(vistaOutline + width, height, vistaOutline, vistaOutline, "resources/images/Panel/rb.png"),
                bottom(vistaOutline, height, width, vistaOutline, "resources/images/Panel/bottom.png")
            {
                children.push_back(panelBar = new VistaPanelBar(window, this, -outline, 0, width + outline * 2));
            }
                                
            struct VistaPanelBar : public JG::Panel::PanelBar
            {
                static const int vistaBarHeight = 28;

                struct VistaPanelCloseButton : public JG::Panel::PanelBar::PanelCloseButton
                {
                    static const int closeWidth = 28, closeHeight = 15;

                    VistaPanelCloseButton(JG::Window* window, VistaPanel* panel, int beginX, int beginY) : 
                        JG::Panel::PanelBar::PanelCloseButton(window, panel, beginX, beginY),

                        buttonImg(0, 0, closeWidth, closeHeight, "resources/images/Close/close.png")
                    {

                    }

                    virtual void renderMyself(int shiftX, int shiftY) override
                    {
                        int moveTexture = 0;

                        if (mouseOn)
                            moveTexture += closeHeight;
                        
                        if (mousePressed)
                            moveTexture += closeHeight;

                        buttonImg.moveTexture(0, moveTexture);
                        buttonImg.move(shiftX, shiftY);

                        buttonImg.draw(*window);

                        buttonImg.moveTexture(0, -moveTexture);
                        buttonImg.move(-shiftX, -shiftY);
                    }
                    
                    JG::Image buttonImg;
                };

                VistaPanelBar(JG::Window* window, VistaPanel* panel, int beginX, int beginY, int width) : 
                    JG::Panel::PanelBar(window, panel, beginX, beginY, width),

                    lu(0, 0, vistaOutline, vistaBarHeight, "resources/images/Panel/lu.png"),
                    up(vistaOutline, 0, vistaOutline, vistaBarHeight, "resources/images/Panel/up.png"),
                    ru(vistaOutline + width, 0, vistaOutline, vistaBarHeight, "resources/images/Panel/ru.png")
                {
                    addChild(new VistaPanelCloseButton(window, panel, width - barHeight + Panel::outline, Panel::outline));
                }

                virtual void renderMyself(int shiftX, int shiftY) override
                {
                    lu.move(shiftX, shiftY);
                    up.move(shiftX, shiftY);
                    ru.move(shiftX, shiftY);

                    lu.draw(*window);
                    up.draw(*window);
                    ru.draw(*window);

                    lu.move(-shiftX, -shiftY);
                    up.move(-shiftX, -shiftY);
                    ru.move(-shiftX, -shiftY);

                    sf::Font font;

                    if (!font.loadFromFile(JG::defaultFont))
                    {
                        printf("Can`t load %s\n", JG::defaultFont);
                        return;
                    }

                    int charSize = 12;

                    int const referenceChar = 'A';

                    auto glyph = font.getGlyph(referenceChar, charSize, false);

                    sf::Text text;
                    text.setFont(font);
                    text.setString(panel->caption);
                    text.setCharacterSize(charSize);
                    text.setFillColor(JG::Color::White);

                    float textShiftY = ((float)height - glyph.bounds.height + glyph.bounds.top) / 2.0;
                    float textShiftX = VistaPanel::outline * 2;

                    text.move({ (float)beginX + (float)shiftX + textShiftX, (float)beginY + (float)shiftY + textShiftY });
                    window->getSfWindow().draw(text);
                }

                JG::Image lu, up, ru;
            };

            virtual void addChild(Widget* widget) override
            {
                widget->beginY += VistaPanelBar::vistaBarHeight;
                Widget::addChild(widget);
            }

            virtual void renderMyself(int shiftX, int shiftY) override
            {
                JG::Rectangle centerRectangle(shiftX + beginX,    shiftY + beginY,
                                              width,              height);

                centerRectangle.setColor({0,0,0});
                centerRectangle.draw(*window);

                left.move(shiftX, shiftY);
                lb.move(shiftX, shiftY);
                bottom.move(shiftX, shiftY);
                rb.move(shiftX, shiftY);
                right.move(shiftX, shiftY);

                left.draw(*window);
                lb.draw(*window);
                bottom.draw(*window);
                rb.draw(*window);
                right.draw(*window);

                left.move(-shiftX, -shiftY);
                lb.move(-shiftX, -shiftY);
                bottom.move(-shiftX, -shiftY);
                rb.move(-shiftX, -shiftY);
                right.move(-shiftX, -shiftY);
            }

            JG::Image left, lb, bottom, rb, right; // for drawing the outline            
        };
        
    }
}