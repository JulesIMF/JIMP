/*++

Copyright (c) 2021 JulesIMF, MIPT

Module Name:

    Curves.h

Abstract:

    

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
        Curves(Layer* layer, int const* plot) :
            layer(layer),
            plot(plot)
        {

        }

        void apply()
        {
            // double effectivePlot[plotSize];

            // for (int i = 0; i != plotSize; i++)
            //     effectivePlot[i] = (double)plot[i];

            for (int x = 0; x != layer->width; x++)
            {
                for (int y = 0; y != layer->height; y++)
                {
                    auto color = layer->image[x][y];
                    // double brightness = 0.3 * (double)color.r + 0.59 * (double)color.g + 0.11 * (double)color.b;
                    // int index = std::min(255, (int)brightness);
                    // double coef = effectivePlot[index] / brightness;
                    // JG::Color newColor
                    // {
                    //     (uint8_t)std::min(255, (int)((float)color.r * coef)),
                    //     (uint8_t)std::min(255, (int)((float)color.g * coef)),
                    //     (uint8_t)std::min(255, (int)((float)color.b * coef)),
                    //     color.a
                    // };

                    // layer->preview[x][y] = newColor;

                    layer->preview[x][y] = 
                        {
                            plot[color.r],
                            plot[color.g],
                            plot[color.b],
                            color.a
                        };
                        
                }
            }
        }

    protected:
        static int const plotSize = 256;
        Layer* layer;
        int const* const plot;
    };
}
