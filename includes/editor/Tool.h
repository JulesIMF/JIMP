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
        virtual void applyOnPress(JG::Color** image, int canvasWidth, int canvasHeight) = 0;
        virtual void applyOnMove(JG::Color** image, int canvasWidth, int canvasHeight) = 0;
        virtual void applyOnRelease(JG::Color** image, int canvasWidth, int canvasHeight) = 0;

    protected:
        inline void savePrev();
        int prevX = -1, prevY = -1;
    };

    class Brush : public Tool
    {
    public:
        virtual void applyOnPress(JG::Color** image, int canvasWidth, int canvasHeight) override;
        virtual void applyOnMove(JG::Color** image, int canvasWidth, int canvasHeight) override;
        virtual void applyOnRelease(JG::Color** image, int canvasWidth, int canvasHeight) override;
    };

    class Eraser : public Tool
    {
    public:
        virtual void applyOnPress(JG::Color** image, int canvasWidth, int canvasHeight) override;
        virtual void applyOnMove(JG::Color** image, int canvasWidth, int canvasHeight) override;
        virtual void applyOnRelease(JG::Color** image, int canvasWidth, int canvasHeight) override;
    };

    class Fill : public Tool
    {
    public:
        virtual void applyOnPress(JG::Color** image, int canvasWidth, int canvasHeight) override;
        virtual void applyOnMove(JG::Color** image, int canvasWidth, int canvasHeight) override;
        virtual void applyOnRelease(JG::Color** image, int canvasWidth, int canvasHeight) override;
    
    private:
        inline void walkAndSet(JG::Color oldColor, int x, int y, std::queue<JG::Vector2i>& pointsQueue);
    };
}

#endif // !TOOL_JULESIMF