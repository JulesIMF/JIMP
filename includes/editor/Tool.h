/*++

Copyright (c) 2021 JulesIMF, MIPT

Module Name:

    Tool.h

Abstract:

    

Author / Creation date:

    JulesIMF / 18.10.21

Revision History:

--*/

#ifndef TOOL_JULESIMF
#define TOOL_JULESIMF

//
// Includes / usings
//

#include <JG.h>
#include <editor/Layer.h>

//
// Defines
//
namespace JIMP
{
    class Tool
    {
    public:
        JG::Color color = { 255, 0, 0 , 255}; // Red
        int thickness = 10;
        int x = 0, y = 0;
        int dx = 0, dy = 0;
        virtual void applyOnPress(Layer& layer) = 0;
        virtual void applyOnMove(Layer& layer) = 0;
        virtual void applyOnRelease(Layer& layer) = 0;
        virtual char const* getName() = 0;
        virtual void drawCursor(JG::Window* window, int x, int y, int canvasWidth, int canvasHeight, int shiftX, int shiftY) = 0;

    protected:
        inline void savePrev();
        int prevX = -1, prevY = -1;
    };

    class Brush : public Tool
    {
    public:
        virtual void applyOnPress(Layer& layer) override;
        virtual void applyOnMove(Layer& layer) override;
        virtual void applyOnRelease(Layer& layer) override;
        virtual char const* getName() override;
        virtual void drawCursor(JG::Window* window, int x, int y, int canvasWidth, int canvasHeight, int shiftX, int shiftY);
    };

    class Eraser : public Tool
    {
    public:
        virtual void applyOnPress(Layer& layer) override;
        virtual void applyOnMove(Layer& layer) override;
        virtual void applyOnRelease(Layer& layer) override;
        virtual char const* getName() override;
        virtual void drawCursor(JG::Window* window, int x, int y, int canvasWidth, int canvasHeight, int shiftX, int shiftY);
    };

    class Fill : public Tool
    {
    public:
        static int const cursorSize = 32;
        Fill() : 
            cursor(0, 0, cursorSize, cursorSize, "resources/images/Canvas/cursor/fill.png")
        {

        }
        virtual void applyOnPress(Layer& layer) override;
        virtual void applyOnMove(Layer& layer) override;
        virtual void applyOnRelease(Layer& layer) override;
        virtual char const* getName() override;
        virtual void drawCursor(JG::Window* window, int x, int y, int canvasWidth, int canvasHeight, int shiftX, int shiftY);
    
    private:
        inline void walkAndSet(JG::Color** image, JG::Color oldColor, JG::Vector2i point, 
                               std::queue<JG::Vector2i>& pointsQueue,
                               int canvasWidth, int canvasHeight);
        
        JG::Image cursor;
    };

    class Drag : public Tool
    {
    public:
        static int const cursorSize = 32;
        Drag() :
            cursor(0, 0, cursorSize, cursorSize, "resources/images/Canvas/cursor/drag.png")
        {

        }
        virtual void applyOnPress(Layer& layer) override;
        virtual void applyOnMove(Layer& layer) override;
        virtual void applyOnRelease(Layer& layer) override;
        virtual void drawCursor(JG::Window* window, int x, int y, int canvasWidth, int canvasHeight, int shiftX, int shiftY);
        virtual char const* getName() override;

        JG::Image cursor;
    };
}

#endif // !TOOL_JULESIMF