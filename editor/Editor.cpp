/*++

Copyright (c) 2021 JulesIMF, MIPT

Module Name:

    Editor.cpp

Abstract:

    Editor mixes layers into one image.

Author / Creation date:

    JulesIMF / 18.10.21

Revision History:

--*/


//
// Includes / usings
//

#include <editor/Editor.h>

//
// Defines
//

using namespace JIMP;

Editor::Editor(int width, int height) : width(width), height(height)
{
    image = new JG::Color*[width];
    for (int x = 0; x != width; x++)
    {
        image[x] = new JG::Color[height];
        for (int y = 0; y != height; y++)
            image[x][y] = { 255, 255, 255, 255 };
    }
}

Editor::~Editor()
{
    for (int x = 0; x != width; x++)
        delete[] image[x];

    delete[] image;
}

JG::Color** Editor::getImage()
{
    return image;
}

void Editor::mix(std::vector<Layer*>& layers)
{
    for (int x = 0; x < width; x++)
        for (int y = 0; y < height; y++)
            image[x][y] = ( (((x - shiftX) >> 4) + ((y - shiftY) >> 4)) & 1 ) ?
            JG::Color{255, 255, 255, 255} : // white
            JG::Color{ 128, 128, 128, 255 }; // gray

    for (auto layer : layers)
    {
        int beginShiftX = layer->beginX + shiftX;
        int beginShiftY = layer->beginY + shiftY;
        int endX = std::min(width,  layer->width  + beginShiftX);
        int endY = std::min(height, layer->height + beginShiftY);

        for (int x = std::max(beginShiftX, 0); x < endX; x++)
        {
            int layerX = x - beginShiftX;

            for (int y = std::max(beginShiftY, 0); y < endY; y++)
            {
                image[x][y] = mixColors(image[x][y], layer->preview[layerX][y - beginShiftY]);
            }
        }
    }
}
