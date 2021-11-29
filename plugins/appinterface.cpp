/*++

Copyright (c) 2021 JulesIMF, MIPT

Module Name:

    AppInterface.cpp

Abstract:

    AppInterface is required for plugins to interact
    with JIMP.

Author / Creation date:

    JulesIMF / 29.11.21

Revision History:

--*/


//
// Includes / usings
//

#define JIMP_APPINTERFACE

#include <dlfcn.h>
#include <cstdio>
#include <plugins/sdk.h>
#include <common/messages.h>
#include "EditorPluginAPI/plugin_std.hpp"

using namespace JIMP::Plugins;

//
// Defines
//

static AppToPI appToPi;

// 
// extensions
//

bool extensonsEnable(char const*)
{
    return false;
}

void* extensionsGetFunc(char const*)
{
    return nullptr;
}

// 
// general
//

static int const generalFeatureLevel = 0;

void generalLog(char const* format, ...)
{
    // copy of common/messages.cpp
    va_list list = {};
    va_start(list, format);
    fprintf(stderr, "\e[1;94m"
        "debug (from plugin \"%s\"): "
        "\e[1;39m", appToPi.currentPlugin->info->name);

    vfprintf(stderr, format, list);
    va_end(list);
    fprintf(stderr, "\e[m\n");
}

double generalGetAbsoluteTime()
{
    return appToPi.currentPlugin->timeFromStart();
}

void generalReleasePixels(PRGBA* pixels)
{
    if (pixels != appToPi.pixels)
    {
        warningMessage("plugin \"%s\" attempts to release pixels different from appToPi.pixels; skipping...",
                                 appToPi.currentPlugin->info->name);
    }

    else
    {
        delete pixels;
        appToPi.pixels = nullptr;
    }
}

PRGBA generalGetColor()
{
    PRGBA color = {};
    JG::Color appColor = appToPi.picker->getColor();
    color.ui32 = *(int*)&appColor;
    return color;
}

float generalGetSize()
{
    return (float)appToPi.picker->getThickness();
}

// 
// target
//

PRGBA* targetGetPixels()
{
    if (appToPi.pixels)
        warningMessage("plugin \"%s\" requests pixels while others were not released",
            appToPi.currentPlugin->info->name);

    auto width = appToPi.currentLayer->width, height = appToPi.currentLayer->height;

    auto image = appToPi.currentLayer->image;
    
    PRGBA* pixels = new PRGBA(width * height);
    
    int i = 0;
    for (int y = 0; y != height; y++)
        for (int x = 0; x != width; x++)
            pixels[i++] = *(PRGBA*)&image[x][y];
    
    appToPi.pixels = pixels;
    return pixels;
}

void targetGetSize(size_t* width, size_t* height)
{
    *width = appToPi.currentLayer->width;
    *height = appToPi.currentLayer->height;
}

// 
// render
//



static const PAppInterface pAppInterface = 
{
    .extensions = 
    {
        extensonsEnable,
        extensionsGetFunc,
    },

    .general = 
    {
        generalFeatureLevel,
        generalLog,
        generalGetAbsoluteTime,
        generalReleasePixels,
        generalGetColor,
        generalGetSize,
    },

    .target = 
    {
        targetGetPixels,
        targetGetSize,
    },

    .render = 
    {

    },

    .settings = 
    {
        nullptr, // create_surface
        nullptr, // destroy_surface

        nullptr, // add
        nullptr, // get
    },

    .shader = 
    {
        nullptr, // apply

        nullptr, // compile
        nullptr, // release

        nullptr, // set_uniform_int
        nullptr, // set_uniform_int_arr

        nullptr, // set_uniform_float
        nullptr, // set_uniform_float_arr
    }
}
