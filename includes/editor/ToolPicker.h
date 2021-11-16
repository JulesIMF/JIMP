/*++

Copyright (c) 2021 JulesIMF, MIPT

Module Name:

    ToolPicker.h

Abstract:

    

Author / Creation date:

    JulesIMF / 18.10.21

Revision History:

--*/


#ifndef TOOLPICKER_JULESIMF
#define TOOLPICKER_JULESIMF

//
// Includes / usings
//

#include <vector>
#include <editor/Tool.h>

//
// Defines
//

namespace JIMP
{
    struct ToolPicker
    {
        void setColor(JG::Color color);
        void setThickness(int thickness);
        void nextTool();
        void prevTool();
        Tool* getTool();
        void insert(Tool* tool);
        void erase(Tool* tool);

    protected:
        int index = 0;
        std::vector<Tool*> tools;
    };
};

#endif // !TOOLPICKER_JULESIMF