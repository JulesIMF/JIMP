/*++

Copyright (c) 2021 JulesIMF, MIPT

Module Name:

    BMP.cpp

Abstract:

    BMP format module

Author / Creation date:

    JulesIMF / 17.10.21

Revision History:

--*/


//
// Includes / usings
//

#include <sys/stat.h>
#include <file/formats.h>
#include <file/bmpheader.h>
using namespace JIMP;

//
// Defines
//

BMP JIMP::loadImage(FILE* file)
{
    BMP bmp = {};

    BITMAPFILEHEADER fileHeader = {};
    BITMAPINFOHEADER infoHeader = {};

    fseek(file, 0, SEEK_SET);
    if (!fread(&fileHeader, sizeof(fileHeader), 1, file))
        return bmp;

    if (fileHeader.bfType != 'BM' && fileHeader.bfType != 'MB')
        return bmp;

    if (!fread(&infoHeader, sizeof(infoHeader), 1, file))
        return bmp;

    bmp.bitCount = infoHeader.biBitCount;
    bmp.image = (char*)calloc(infoHeader.biWidth * infoHeader.biHeight, bmp.bitCount / 8);
    if (!bmp.image)
        return bmp;

    fseek(file, fileHeader.bfOffBits, SEEK_SET);
    if (fread(bmp.image,
        bmp.bitCount / 8,
        infoHeader.biWidth * infoHeader.biHeight,
        file)
        != infoHeader.biWidth * infoHeader.biHeight)
    {
        return bmp;
    }

    bmp.xSize = infoHeader.biWidth;
    bmp.ySize = infoHeader.biHeight;

    return bmp;
}

JG::Color** JIMP::imageToColorBuffer(BMP const& bmp)
{
    JG::Color* imageColor = (JG::Color*)bmp.image;

    JG::Color** buffer = new JG::Color*[bmp.xSize];

    for (int x = 0; x != bmp.xSize; x++)
    {
        buffer[x] = new JG::Color[bmp.ySize];
        for (int y = 0; y != bmp.ySize; y++)
        {
            int index = (bmp.ySize - y - 1) * bmp.xSize + x;
            buffer[x][y].r = imageColor[index].b;
            buffer[x][y].g = imageColor[index].g;
            buffer[x][y].b = imageColor[index].r;
            buffer[x][y].a = imageColor[index].a;
        }
    }

    return buffer;
}