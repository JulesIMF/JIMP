/*++

Copyright (c) 2021 JulesIMF, MIPT

Module Name:

    editor.h

Abstract:

    Editor mixes layers into one image.

Author / Creation date:

    JulesIMF / 18.10.21

Revision History:

--*/

#ifndef EDITOR_JULESIMF
#define EDITOR_JULESIMF

//
// Includes / usings
//

#include <unordered_set>
#include <vector>
#include <deque>
#include <editor/Layer.h>
#include <editor/Tool.h>

//
// Defines
//

namespace JIMP
{
    struct Editor
    {
    public:
        Editor(int sizeX, int sizeY);
        ~Editor();

        void mix(std::vector<Layer*>& layers, bool preview = true);
        JG::Color** getImage();

        int width, height;
        int shiftX = 0, shiftY = 0;
        
    protected:
        Tool* tool = nullptr;
        JG::Color** image;
        JG::Color fillColor = {0, 0, 0, 0};
    };
}

#endif // !EDITOR_JULESIMF