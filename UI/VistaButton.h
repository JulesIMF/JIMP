/*++

Copyright (c) 2021 JulesIMF, MIPT

Module Name:

    VistaPanel.h

Abstract:



Author / Creation date:

    JulesIMF / 27.10.21

Revision History:

--*/

#ifndef JIMP_VISTABUTTON
#define JIMP_VISTABUTTON

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
        struct VistaButton : public JG::Button
        {
            static int const buttonHeight = 21;
            static int const buttonEdge = 3;
            static int const cutTextureSize = 32;
            VistaButton(JG::Window* window, int beginX, int beginY, int width) :
                JG::Button(window, beginX, beginY, width, buttonHeight),

                left(0, 0, buttonEdge, buttonHeight, "resources/images/Button/left.png"),
                mid_cut(buttonEdge, 0, cutTextureSize, buttonHeight, "resources/images/Button/mid_cut.png"),
                right(width - buttonEdge, 0, buttonEdge, buttonHeight, "resources/images/Button/right.png")
            {
                mid_cut.scale(width - 2 * buttonEdge, height);
            }

            virtual void renderMyself(int shiftX, int shiftY) override
            {
                int moveTexture = 0;

                if (mouseOn)
                    moveTexture += buttonHeight;

                if (mousePressed)
                    moveTexture += buttonHeight;

                translateAndDraw(left, window, beginX + shiftX, beginY + shiftY, 0, moveTexture);
                translateAndDraw(mid_cut, window, beginX + shiftX, beginY + shiftY, 0, moveTexture);
                translateAndDraw(right, window, beginX + shiftX, beginY + shiftY, 0, moveTexture);

                if (caption == nullptr)
                    return;

                sf::Font font;

                if (!font.loadFromFile(JG::defaultFont))
                {
                    printf("Can`t load %s\n", JG::defaultFont);
                    return;
                }

                int charSize = 15;

                sf::Text text;
                text.setFont(font);
                text.setString(caption);
                text.setCharacterSize(charSize);
                text.setFillColor(JG::Color::Black);

                auto localBounds = text.getLocalBounds();

                float textWidth = localBounds.width;
                float textHeight = localBounds.height;
                float textShiftY = 0;
                float textShiftX = ((float)width - textWidth + 1) / 2.0;

                text.move({ (float)beginX + (float)shiftX + textShiftX, (float)beginY + (float)shiftY + textShiftY });
                window->getSfWindow().draw(text);
            }
        protected:
            JG::Image left, mid_cut, right;
        };
    }
}

#endif // vista