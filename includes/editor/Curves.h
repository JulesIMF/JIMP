/*++

Copyright (c) 2021 JulesIMF, MIPT

Module Name:

    Curves.h

Abstract:

    An abstraction that takes RGB remaping plots
    and changes chanels brightness.

Author / Creation date:

    JulesIMF / 22.11.21

Revision History:

--*/


//
// Includes / usings
//

#include <algorithm>
#include <editor/Layer.h>


//
// Defines
//
namespace JIMP
{
    struct Curves
    {
        enum Chanel
        {
            R = 1,
            G = 2,
            B = 4,
        };

        Curves(Layer* layer, int const* rPlot, int const* gPlot, int const* bPlot) :
            layer(layer),
            rPlot(rPlot),
            gPlot(gPlot),
            bPlot(bPlot)
        {

        }

        void apply(int chanels = R | G | B)
        {
            bool rMod = chanels & R,
                 gMod = chanels & G,
                 bMod = chanels & B;

            for (int x = 0; x != layer->width; x++)
            {
                for (int y = 0; y != layer->height; y++)
                {
                    auto color = layer->image[x][y];

                    layer->preview[x][y] = 
                        {
                            rMod ? rPlot[color.r] : color.r,
                            gMod ? gPlot[color.g] : color.g,
                            bMod ? bPlot[color.b] : color.b,
                            color.a
                        };
                        
                }
            }
        }

    protected:
        static int const plotSize = 256;
        Layer* layer;
        int const* rPlot,
                 * gPlot,
                 * bPlot;
    };
}
