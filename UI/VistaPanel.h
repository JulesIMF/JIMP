/*++

Copyright (c) 2021 JulesIMF, MIPT

Module Name:

    VistaPanel.h

Abstract:

    Vista-style panel.

Author / Creation date:

    JulesIMF / 27.10.21

Revision History:

--*/

#ifndef JIMP_VISTAPANEL
#define JIMP_VISTAPANEL

//
// Includes / usings
//

#include <JG.h>
#include "tnd.h"

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
            static const int cutTextureSize = 32;

            VistaPanel(JG::Window* window, int beginX, int beginY,
                int width, int height) :
                JG::Panel(window, beginX, beginY, width, height),

                left_cut(-vistaOutline, VistaPanelBar::vistaBarHeight, vistaOutline, cutTextureSize, "resources/images/Panel/left.png"),
                right_cut(width, VistaPanelBar::vistaBarHeight, vistaOutline, cutTextureSize, "resources/images/Panel/right.png"),
                lb(-vistaOutline, height + VistaPanelBar::vistaBarHeight, vistaOutline, vistaOutline, "resources/images/Panel/lb.png"),
                rb(width, height + VistaPanelBar::vistaBarHeight, vistaOutline, vistaOutline, "resources/images/Panel/rb.png"),
                bottom_cut(0, height + VistaPanelBar::vistaBarHeight, cutTextureSize, vistaOutline, "resources/images/Panel/bottom.png")
            {
                this->height = height + VistaPanelBar::vistaBarHeight;

                left_cut.scale(vistaOutline, height);
                bottom_cut.scale(width, vistaOutline);
                right_cut.scale(vistaOutline, height);

                children.clear();
                children.push_back(panelBar = new VistaPanelBar(window, this, 0, 0, width));
            }

            struct VistaPanelBar : public JG::Panel::PanelBar
            {
                static const int vistaBarHeight = 28;

                struct VistaPanelCloseButton : public JG::Panel::PanelBar::PanelCloseButton
                {
                    static const int closeWidth = 28, closeHeight = 15;

                    VistaPanelCloseButton(JG::Window* window, VistaPanel* panel, int beginX, int beginY) :
                        JG::Panel::PanelBar::PanelCloseButton(window, panel, beginX, beginY),

                        buttonImg(beginX, beginY, closeWidth, closeHeight, "resources/images/Close/button.png")
                    {
                        width = closeWidth;
                        height = closeHeight;
                    }

                    virtual void renderMyself(int shiftX, int shiftY) override
                    {
                        int moveTexture = 0;

                        if (mouseOn)
                            moveTexture += closeHeight;

                        if (mousePressed)
                            moveTexture += closeHeight;

                        translateAndDraw(buttonImg, window, shiftX, shiftY, 0, moveTexture);
                    }

                    JG::Image buttonImg;
                };

                VistaPanelBar(JG::Window* window, VistaPanel* panel, int beginX, int beginY, int width) :
                    JG::Panel::PanelBar(window, panel, beginX, beginY, width),

                    lu(-vistaOutline, 0, vistaOutline, vistaBarHeight, "resources/images/Panel/lu.png"),
                    up_cut(0, 0, VistaPanel::cutTextureSize, vistaBarHeight, "resources/images/Panel/up_cut.png"),
                    ru(width, 0, vistaOutline, vistaBarHeight, "resources/images/Panel/ru.png")
                {
                    height = vistaBarHeight;
                    up_cut.scale(width, height);
                    children.clear();
                    addChild(new VistaPanelCloseButton(window, panel, width - VistaPanelCloseButton::closeWidth,
                        (vistaBarHeight - VistaPanelCloseButton::closeHeight) / 2));
                }

                virtual void renderMyself(int shiftX, int shiftY) override
                {
                    translateAndDraw(lu, window, shiftX, shiftY);
                    translateAndDraw(up_cut, window, shiftX, shiftY);
                    translateAndDraw(ru, window, shiftX, shiftY);

                    if (panel->caption == nullptr)
                        return;

                    sf::Font font;

                    if (!font.loadFromFile(JG::defaultFont))
                    {
                        printf("Can`t load %s\n", JG::defaultFont);
                        return;
                    }

                    int charSize = 13;

                    sf::Text text;
                    text.setFont(font);
                    text.setString(panel->caption);
                    text.setCharacterSize(charSize);
                    text.setFillColor(JG::Color::Black);

                    float textShiftY = ((float)height - text.getLocalBounds().height) / 2.0;
                    float textShiftX = VistaPanel::outline * 2;

                    text.move({ (float)beginX + (float)shiftX + textShiftX, (float)beginY + (float)shiftY + textShiftY });
                    window->getSfWindow().draw(text);
                }

                JG::Image lu, up_cut, ru;
            };

            virtual void addChild(Widget* widget) override
            {
                widget->beginY += VistaPanelBar::vistaBarHeight;
                Widget::addChild(widget);
            }

            virtual void renderMyself(int shiftX, int shiftY) override
            {
                // Shadow
                int const shadowShiftX = 6,
                    shadowShiftY = shadowShiftX;

                JG::Color shadowColor = { 0, 0, 0, 100 };

                JG::Rectangle shadow(shiftX + beginX + shadowShiftX, shiftY + beginY + shadowShiftY,
                    width + 2 * outline, height + 2 * outline);

                shadow.setColor(shadowColor);
                shadow.draw(*window);

                // White space
                JG::Rectangle centerRectangle(shiftX + beginX, shiftY + beginY + VistaPanelBar::vistaBarHeight,
                    width, height - VistaPanelBar::vistaBarHeight);

                centerRectangle.setColor({ 255,255,255 });
                centerRectangle.draw(*window);

                // Borders
                translateAndDraw(left_cut, window, beginX + shiftX, beginY + shiftY);
                translateAndDraw(lb, window, beginX + shiftX, beginY + shiftY);
                translateAndDraw(bottom_cut, window, beginX + shiftX, beginY + shiftY);
                translateAndDraw(rb, window, beginX + shiftX, beginY + shiftY);
                translateAndDraw(right_cut, window, beginX + shiftX, beginY + shiftY);
            }

            JG::Image left_cut, lb, bottom_cut, rb, right_cut; // for drawing the outline            
        };
    }
}

#endif // vista