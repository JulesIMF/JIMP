/*++

Copyright (c) 2021 JulesIMF, MIPT

Module Name:

    sfimg.h

Abstract:

    Opening/saving images with SFML.

Author / Creation date:

    JulesIMF / 07.12.21

Revision History:

--*/

#ifndef JIMP_SFIMG
#define JIMP_SFIMG

//
// Includes / usings
//


#include <editor/Layer.h>

//
// Defines
//

namespace JIMP
{
    JIMP::Layer* openImage(char const* fileName);

    bool saveImage(JG::Color** image, int width, int height, char const* filename);
}


#endif // !JIMP_SFIMG