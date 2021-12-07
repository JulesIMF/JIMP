/*++

Copyright (c) 2021 JulesIMF, MIPT

Module Name:

    sfimg.cpp

Abstract:

    Opening/saving images with SFML.

Author / Creation date:

    JulesIMF / 07.12.21

Revision History:

--*/


//
// Includes / usings
//

#include <SFML/Graphics.hpp>
#include <editor/Layer.h>
#include <file/sfimg.h>

//
// Defines
//



JIMP::Layer* JIMP::openImage(char const* fileName)
{
    JIMP::Layer* layer = nullptr;

    try
    {
        if (fileName == nullptr)
            return nullptr;

        sf::Image image;
        image.loadFromFile(fileName);
        
        auto width = (int)image.getSize().x;
        auto height = (int)image.getSize().y;
        JIMP::Layer* layer = new JIMP::Layer;
        layer->width = width;
        layer->height = height;
        layer->init();

        auto pixels = image.getPixelsPtr();
        int index = 0;

        
        for (int y = 0; y != height; y++)
            for (int x = 0; x != width; x++)
                layer->image[x][y] = ((JG::Color*)pixels)[index++];

        return layer;
    }

    catch(...)
    {
        if (layer)
            delete layer;

        return nullptr;
    }    
}

bool JIMP::saveImage(JG::Color** image, int width, int height, char const* filename)
{
    sf::RenderTexture texture;
    texture.create(width, height);

    for (int x = 0; x != width; x++)
        for (int y = 0; y != height; y++)
        {
            sf::Vertex vertex[] =
            {
                { { float(x), float(y) }, image[x][y] }
            };

            texture.draw(vertex, 1, sf::Points);
        }

    texture.display();
    sf::Image sfImage = texture.getTexture().copyToImage();

    bool result = true;
    try
    {        
        sfImage.saveToFile(filename);
    }

    catch(...)
    {
        result = false;
    }

    return result;    
}
