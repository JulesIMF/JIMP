/*++

Copyright (c) 2021 JulesIMF, MIPT

Module Name:

    formats.h

Abstract:

    Different file formats

Author / Creation date:

    JulesIMF / 17.10.21

Revision History:

--*/

#ifndef FORMATS_JIMP
#define FORMATS_JIMP

//
// Includes / usings
//

#include <Shape.h>

//
// Defines
//

namespace JIMP
{
    struct BMP
    {
        int xSize = 0, ySize = 0;
        int bitCount = 0;
        char* image;
    };

    BMP loadImage(FILE* file);

    JG::Color** imageToColorBuffer(BMP const& bmp);
}

#endif // !FORMATS_JIMP