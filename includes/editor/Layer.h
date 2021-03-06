/*++

Copyright (c) 2021 JulesIMF, MIPT

Module Name:

    layer.h

Abstract:

    Different colors functions.
    See includes/editor/Layer.h for
    Layer struct realization.

Author / Creation date:

    JulesIMF / 18.10.21

Revision History:

--*/

#ifndef LAYER_JULESIMF
#define LAYER_JULESIMF

//
// Includes / usings
//

#include <string>
#include <JG.h>

//
// Defines
//

namespace JIMP
{
    struct Layer
    {
        bool muted = false;
        int beginX = 0, beginY = 0;
        int width = -1, height = -1;

        JG::Color** image;
        JG::Color** preview;

        std::string name;

        void init(char const* name = "")
        {
            image = new JG::Color* [width];
            for (int x = 0; x != width; x++)
            {
                image[x] = new JG::Color[height];
                for (int y = 0; y != height; y++)
                    image[x][y] = { 255, 255, 255 , 0};
            }

            preview = image;
            this->name = name;
        }

        void onCorrectionBegin()
        {
            preview = new JG::Color* [width];
            for (int x = 0; x != width; x++)
            {
                preview[x] = new JG::Color[height];
                memcpy(preview[x], image[x], sizeof(JG::Color) * height);
            }
        }

        void onCorrectionEnd()
        {
            for (int x = 0; x != width; x++)
            {
                memcpy(image[x], preview[x], sizeof(JG::Color) * height);
                delete[] preview[x];
            }

            delete[] preview;
            preview = image;
        }

        void onCorrectionReset()
        {
            if (preview == image)
                return;
                
            for (int x = 0; x != width; x++)
                delete[] preview[x];

            delete[] preview;

            preview = image;
        }

        void resetPreview()
        {
            for (int x = 0; x != width; x++)
                memcpy(preview[x], image[x], sizeof(JG::Color) * height);
        }

        void free()
        {
            for (int x = 0; x != width; x++)
                delete[] image[x];

            delete[] image;
        }

        JG::Color*& operator[](int index)
        {
            return image[index];
        }
    };

    enum class MixMode
    {
        Alpha,
        PseudoAlpha,
        Replace
    };

    JG::Color mixColors(JG::Color back, JG::Color layer, MixMode mode = MixMode::PseudoAlpha);
    void transferColorBuffer(JG::Color** to, JG::Color const* const* from, 
                             int toWidth,   int toHeight,
                             int fromWidth, int fromHeight);

    void transferColorBuffer(JG::Color** to, JG::Color const* const* from, int width, int height);
    void deleteColorBuffer(JG::Color** buffer, int width, int height);
}

#endif // !LAYER_JULESIMF