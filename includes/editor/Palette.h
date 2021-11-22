/*++

Copyright (c) 2021 JulesIMF, MIPT

Module Name:

    Palette.h

Abstract:

    Palette is JG::Widget that allows to pick
    HUE color visually. Once color is changed,
    Palette sends it to all Tools via ToolPicker.

Author / Creation date:

    JulesIMF / 18.10.21

Revision History:

--*/

#ifndef JIMP_PALETTE
#define JIMP_PALETTE


//
// Includes / usings
//

#include <JG.h>
#include <editor/ToolPicker.h>

//
// Defines
//

namespace JIMP
{
    struct Palette : public JG::Widget
    {
        static int const space = 20;

        static int const smoothRadius = 10;

        struct PaletteSquare : public JG::Canvas
        {
            double positionX = 0, positionY = 0;
            JG::Color keyColor = { 255, 0, 0 };
            JG::Color color = { 255, 0, 0 };
            Palette* palette;
            void updateKeyColor(JG::Color keyColor);
            JG::Color calcColor(double positionX, double positionY);
            void calcBlt();

            PaletteSquare(JG::Window* window, int beginX, int beginY,
                          int width, int height, Palette* palette);

            virtual JG::Widget::HandlerResponce onMouseButtonPressed(JG::Event event);
            virtual JG::Widget::HandlerResponce onMouseMoved(JG::Event event);
        };

        struct PalettePanel : public JG::Canvas
        {
            static int const panelWidth = 30;
            static int const nKnots = 7;
            static int const smoothRadius = 5;

            JG::Color const knots[nKnots] =
            {
                {255, 0, 0},
                {255, 0, 255},
                {0, 0, 255},
                {0, 255, 255},
                {0, 255, 0},
                {255, 255, 0},
                {255, 0, 0},
            };

            double position = 0;
            JG::Color keyColor = {255, 0, 0};

            PalettePanel(JG::Window* window, int beginX, int beginY, 
                         int height, PaletteSquare* paletteSquare);

            JG::Color calcKeyColor(double position);
            PaletteSquare* paletteSquare;
            void calcBlt();

            virtual JG::Widget::HandlerResponce onMouseButtonPressed(JG::Event event);
            virtual JG::Widget::HandlerResponce onMouseMoved(JG::Event event);
        };

        void updateColor(JG::Color color);
        virtual void renderMyself(int shiftX, int shiftY);
        
        Palette(JG::Window* window, int beginX, int beginY,
                int width, int height, ToolPicker* toolPicker);

        JG::Color color = { 255, 255, 255 };
        PaletteSquare* paletteSquare;
        PalettePanel* palettePanel;
        ToolPicker* toolPicker;
    };
}

#endif // !JIMP_PALETTE