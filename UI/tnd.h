/*++

Copyright (c) 2021 JulesIMF, MIPT

Module Name:

    tdr.h

Abstract:

    

Author / Creation date:

    JulesIMF / 16.11.21

Revision History:

--*/


//
// Includes / usings
//

#include <JG.h>


//
// Defines
//

#ifndef JIMP_TND
#define JIMP_TND


namespace JIMP
{
    namespace UI
    {
        static void translateAndDraw(JG::Image& image, JG::Window* window, int shiftX, int shiftY, int shiftTextureX = 0, int shiftTextureY = 0)
        {
            image.moveTexture(shiftTextureX, shiftTextureY);
            image.move(shiftX, shiftY);

            image.draw(*window);

            image.moveTexture(-shiftTextureX, -shiftTextureY);
            image.move(-shiftX, -shiftY);
        }
    }
}


#endif // !JIMP_TND