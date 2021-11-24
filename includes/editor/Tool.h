/*++

Copyright (c) 2021 JulesIMF, MIPT

Module Name:

    Tool.h

Abstract:

    Tool is an abstraction that changes given layer in
    a specific way.

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
#include <ctime>
#include <editor/Layer.h>

//
// Defines
//
namespace JIMP
{
    class Tool
    {
    public:
        JG::Color color = { 255, 255, 255 , 255}; // Red
        int thickness = 10;
        int x = 0, y = 0;
        int dx = 0, dy = 0;
        virtual void applyOnPress(Layer& layer) = 0;
        virtual void applyOnMove(Layer& layer) = 0;
        virtual void applyOnRelease(Layer& layer) = 0;
        virtual void applyOnTimer(Layer& layer, int passedMs) = 0;
        virtual char const* getName() = 0;
        virtual void drawCursor(JG::Window* window, int x, int y, int canvasWidth, int canvasHeight, int shiftX, int shiftY) = 0;
        bool timerTool = false;

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
        virtual void applyOnTimer(Layer& layer, int passedMs) override;
        virtual char const* getName() override;
        virtual void drawCursor(JG::Window* window, int x, int y, int canvasWidth, int canvasHeight, int shiftX, int shiftY);
    };

    class Eraser : public Tool
    {
    public:
        virtual void applyOnPress(Layer& layer) override;
        virtual void applyOnMove(Layer& layer) override;
        virtual void applyOnRelease(Layer& layer) override;
        virtual void applyOnTimer(Layer& layer, int passedMs) override;
        virtual char const* getName() override;
        virtual void drawCursor(JG::Window* window, int x, int y, int canvasWidth, int canvasHeight, int shiftX, int shiftY);
    };

    class Fill : public Tool
    {
    public:
        static int const cursorSize = 32;
        Fill() : 
            cursor(0, 0, cursorSize, cursorSize, "resources/cursor/fill_cen.png")
        {

        }
        virtual void applyOnPress(Layer& layer) override;
        virtual void applyOnMove(Layer& layer) override;
        virtual void applyOnRelease(Layer& layer) override;
        virtual void applyOnTimer(Layer& layer, int passedMs) override;
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
            cursor(0, 0, cursorSize, cursorSize, "resources/cursor/drag.png")
        {

        }
        virtual void applyOnPress(Layer& layer) override;
        virtual void applyOnMove(Layer& layer) override;
        virtual void applyOnRelease(Layer& layer) override;
        virtual void applyOnTimer(Layer& layer, int passedMs) override;
        virtual void drawCursor(JG::Window* window, int x, int y, int canvasWidth, int canvasHeight, int shiftX, int shiftY);
        virtual char const* getName() override;

        JG::Image cursor;
    };

    struct ToolPicker;

    class Eyedropper : public Tool
    {
    public:
        static int const cursorSize = 32;

        Eyedropper(ToolPicker* toolPicker) :
            cursor(0, 0, cursorSize, cursorSize, "resources/cursor/color_cur.png"),
            toolPicker(toolPicker)
        {

        }
        virtual void applyOnPress(Layer& layer) override;
        virtual void applyOnMove(Layer& layer) override;
        virtual void applyOnRelease(Layer& layer) override;
        virtual void applyOnTimer(Layer& layer, int passedMs) override;
        virtual void drawCursor(JG::Window* window, int x, int y, int canvasWidth, int canvasHeight, int shiftX, int shiftY);
        virtual char const* getName() override;

        JG::Image cursor;
        ToolPicker* toolPicker;
    };

    class Airbrush : public Tool
    {
    public:
        static int const cursorSize = 32;

        Airbrush() :
            cursor(0, 0, cursorSize, cursorSize, "resources/cursor/airbrush_cur.png")
        {
            srand(time(nullptr));
            timerTool = true;
        }
        virtual void applyOnPress(Layer& layer) override;
        virtual void applyOnMove(Layer& layer) override;
        virtual void applyOnRelease(Layer& layer) override;
        virtual void applyOnTimer(Layer& layer, int passedMs) override;
        virtual void drawCursor(JG::Window* window, int x, int y, int canvasWidth, int canvasHeight, int shiftX, int shiftY);
        virtual char const* getName() override;

        JG::Image cursor;
        int intensity = 3; // per pi pixels for 1 second
    };

    class Pen : public Tool
    {
    public:
        virtual void applyOnPress(Layer& layer) override;
        virtual void applyOnMove(Layer& layer) override;
        virtual void applyOnRelease(Layer& layer) override;
        virtual void applyOnTimer(Layer& layer, int passedMs) override;
        virtual char const* getName() override;
        virtual void drawCursor(JG::Window* window, int x, int y, int canvasWidth, int canvasHeight, int shiftX, int shiftY);
        int outline = 1;
    };

    class Rectangle : public Tool
    {
    public:
        static int const cursorSize = 32;

        Rectangle() :
            cursor(0, 0, cursorSize, cursorSize, "resources/cursor/rect_cur.png")
        {
            srand(time(nullptr));
        }

        virtual void applyOnPress(Layer& layer) override;
        virtual void applyOnMove(Layer& layer) override;
        virtual void applyOnRelease(Layer& layer) override;
        virtual void applyOnTimer(Layer& layer, int passedMs) override;
        virtual void drawCursor(JG::Window* window, int x, int y, int canvasWidth, int canvasHeight, int shiftX, int shiftY);
        virtual char const* getName() override;

        JG::Image cursor;
        int pressX = 0, pressY = 0;
    };
}

#endif // !TOOL_JULESIMF