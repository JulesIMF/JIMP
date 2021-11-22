/*++

Copyright (c) 2021 JulesIMF, MIPT

Module Name:

    bmpheader.h

Abstract:

    BMP format header.

Author / Creation date:

    JulesIMF / 17.10.21

Revision History:

--*/

#ifndef BMPHEADER_JIMP
#define BMPHEADER_JIMP

//
// Includes / usings
//

typedef short WORD;
typedef int   DWORD, LONG;

typedef struct tagBITMAPFILEHEADER {
    WORD bfType;
    DWORD bfSize;
    WORD bfReserved1;
    WORD bfReserved2;
    DWORD bfOffBits;
} __attribute__((aligned(1), packed)) BITMAPFILEHEADER, * LPBITMAPFILEHEADER, * PBITMAPFILEHEADER;

typedef struct tagBITMAPINFOHEADER {
    DWORD biSize;
    LONG biWidth;
    LONG biHeight;
    WORD biPlanes;
    WORD biBitCount;
    DWORD biCompression;
    DWORD biSizeImage;
    LONG biXPelsPerMeter;
    LONG biYPelsPerMeter;
    DWORD biClrUsed;
    DWORD biClrImportant;
} __attribute__((aligned(1), packed)) BITMAPINFOHEADER, * LPBITMAPINFOHEADER, * PBITMAPINFOHEADER;

//
// Defines
//

#endif // !BMPHEADERS_JIMP