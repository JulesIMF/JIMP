/*++

Copyright (c) 2021 JulesIMF, MIPT

Module Name:

    layer.h

Abstract:

    

Author / Creation date:

    JulesIMF / 18.10.21

Revision History:

--*/

#ifndef LAYER_JULESIMF
#define LAYER_JULESIMF

//
// Includes / usings
//

#include <JG.h>

//
// Defines
//

namespace JIMP
{
    struct Layer
    {
        int beginX, beginY;
        int width, height;

        JG::Color** image;

        void init()
        {
            image = new JG::Color * [width];
            for (int x = 0; x != width; x++)
            {
                image[x] = new JG::Color[height];
                for (int y = 0; y != height; y++)
                    image[x][y] = { 255, 255, 255 , 0};
            }
        }

        void free()
        {
            for (int x = 0; x != width; x++)
                delete[] image[x];

            delete[] image;
        }
    };

    JG::Color mixColors(JG::Color back, JG::Color layer);
    void transferColorBuffer(JG::Color** to, JG::Color const* const* from, 
                             int toWidth,   int toHeight,
                             int fromWidth, int fromHeight);

    void transferColorBuffer(JG::Color** to, JG::Color const* const* from, int width, int height);
}

#endif // !LAYER_JULESIMF