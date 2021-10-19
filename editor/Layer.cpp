/*++

Copyright (c) 2021 JulesIMF, MIPT

Module Name:

    Layer.cpp

Abstract:

    

Author / Creation date:

    JulesIMF / 19.10.21

Revision History:

--*/


//
// Includes / usings
//

#include <editor/Layer.h>
using namespace JIMP;

//
// Defines
//

JG::Color JIMP::mixColors(JG::Color back, JG::Color layer)
{
    if (layer.a)
        return layer;
    
    else
        return back;

    // return
    // {
    //     (back.r * (255 - layer.a) + layer.r * layer.a) >> 8,
    //     (back.g * (255 - layer.a) + layer.g * layer.a) >> 8,
    //     (back.b * (255 - layer.a) + layer.b * layer.a) >> 8,
    //     255
    // };
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