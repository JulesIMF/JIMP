/*++

Copyright (c) 2021 JulesIMF, MIPT

Module Name:

    Layer.cpp

Abstract:

    Different colors functions.
    See includes/editor/Layer.h for 
    Layer struct realization.

Author / Creation date:

    JulesIMF / 19.10.21

Revision History:

--*/


//
// Includes / usings
//

#include <cassert>
#include <editor/Layer.h>
using namespace JIMP;

//
// Defines
//

JG::Color JIMP::mixColors(JG::Color back, JG::Color layer, JIMP::MixMode mode)
{
    switch (mode)
    {
    case JIMP::MixMode::Alpha:
        return
        {
            (back.r * (255 - layer.a) + layer.r * layer.a) >> 8,
            (back.g * (255 - layer.a) + layer.g * layer.a) >> 8,
            (back.b * (255 - layer.a) + layer.b * layer.a) >> 8,
            std::max(back.a, layer.a)
        };
    
    case JIMP::MixMode::PseudoAlpha:
        if (layer.a)
            return layer;
        
        else
            return back;

    case JIMP::MixMode::Replace:
        return layer;

    default:
        assert(!"Unknown mode");
        return {};
    }
}

void JIMP::transferColorBuffer(JG::Color** to, JG::Color const* const* from,
    int toWidth, int toHeight,
    int fromWidth, int fromHeight)
{
    for (int x = 0; x != std::min(toWidth, fromWidth); x++)
        for (int y = 0; y != std::min(toHeight, fromHeight); y++)
            to[x][y] = from[x][y];
}

void JIMP::transferColorBuffer(JG::Color** to, JG::Color const* const* from, int width, int height)
{
    transferColorBuffer(to, from, width, height, width, height);
}

void JIMP::deleteColorBuffer(JG::Color** buffer, int width, int height)
{
    for (int i = 0; i != width; i++)
        delete[] buffer[i];
    
    delete[] buffer;
}