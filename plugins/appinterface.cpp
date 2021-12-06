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
#include <cstdarg>
#include <cassert>
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

void* extensionsGetFunc(char const*, char const*)
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
        "plugin debug: "
        "\e[1;39m");

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
        delete[] pixels;
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
    
    PRGBA* pixels = new PRGBA[width * height];
    
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

inline void setPixel(PRGBA color, int x, int y, const PRenderMode* render_mode)
{
    auto alpha = render_mode->blend == PPBM_ALPHA_BLEND ? ::JIMP::MixMode::Alpha : ::JIMP::MixMode::Replace;
    auto buffer = (render_mode->draw_policy == PPDP_PREVIEW) ? appToPi.stdPreview : appToPi.currentLayer->image;

    JG::Color jgColor = { color.r, color.g, color.b, color.a };
    buffer[x][y] = JIMP::mixColors(buffer[x][y], jgColor, alpha);
}

void renderCircle(PVec2f position, float radius, PRGBA color, const PRenderMode* render_mode)
{
    int x = position.x,
        y = position.y;
    
    int thickness = (int)(radius + 0.5f); // rounding

    int canvasWidth = appToPi.currentLayer->width, canvasHeight = appToPi.currentLayer->height;

    for (int currentX = std::max(x - thickness, 0); currentX <= x + thickness; currentX++)
    {
        if (currentX >= canvasWidth)
            break;

        for (int currentY = std::max(y - thickness, 0); currentY <= y + thickness; currentY++)
        {
            if (currentY >= canvasHeight)
                break;

            if ((y - currentY) * (y - currentY) + (x - currentX) * (x - currentX) <= thickness * thickness)
                setPixel(color, currentX, currentY, render_mode);
        }
    }
}

void renderLine(PVec2f start, PVec2f end, PRGBA color, const PRenderMode* render_mode)
{
    if (end.x - start.x < 0)
        std::swap(start, end);

    assert(appToPi.currentLayer);

    auto width  = appToPi.currentLayer->width,
         height = appToPi.currentLayer->height;

    auto dx = end.x - start.x,
         dy = end.y - start.y;

    double derivative = dy / dx;

    int pixelX = (int)start.x,
        finalPixelX = (int)end.x;

    int pixelY = (int)start.y,
        finalPixelY = (int)end.y;

    int yPixelShift = (finalPixelY < pixelY) ? -1 : 1;

    for (int steps = 0; pixelX != finalPixelX; pixelX++, steps++)
    {
        double nextPixelY = (int)(start.y + derivative * steps);

        for (; pixelY <= nextPixelY; pixelY += yPixelShift)
        {
            if (!(0 <= pixelY && pixelY < height && 
                  0 <= pixelX && pixelX < width))
            {
                continue;
            }

            setPixel(color, pixelX, pixelY, render_mode);

            if (yPixelShift < 0 && pixelY == 0 || yPixelShift > 0 && pixelY + 1 == height)
                return;
        }

        if (pixelX + 1 == width)
            return;
    }
}

PVec2f operator-(PVec2f const& first, PVec2f const& second)
{
    return PVec2f(first.x - second.x, first.y - second.y);
}

float operator*(PVec2f const& first, PVec2f const& second)
{
    return first.x * second.y - first.y * second.x;
}


void renderTriangle(PVec2f p1, PVec2f p2, PVec2f p3, PRGBA color, const PRenderMode* render_mode)
{
    assert(appToPi.currentLayer);
    int canvasWidth = appToPi.currentLayer->width, canvasHeight = appToPi.currentLayer->height;

    int minX = (int)std::max(0.0f,
                             std::min({p1.x, p2.x, p3.x, float(canvasWidth)})),

        maxX = (int)std::min(float(canvasWidth),
                             std::max({p1.x, p2.x, p3.x, 0.0f})),

        minY = (int)std::max(0.0f,
                             std::min({p1.y, p2.y, p3.y, float(canvasHeight)})),

        maxY = (int)std::min(float(canvasHeight),
                             std::max({p1.y, p2.y, p3.y, 0.0f}));

    for (int currentX = minX; currentX < maxX; currentX++)
    {
        // if (currentX >= canvasWidth)
        //     break;

        for (int currentY = minY; currentY < maxY; currentY++)
        {
            // if (currentY >= canvasHeight)
            //     break;
            
            PVec2f patient = PVec2f(currentX, currentY);

            auto cross1 = (p1 - p2) * (patient - p2),
                 cross2 = (p2 - p3) * (patient - p3),
                 cross3 = (p3 - p1) * (patient - p1);
        
            if ((cross1 >= 0 && cross2 >= 0 && cross3 >= 0) ||
                (cross1 <= 0 && cross2 <= 0 && cross3 <= 0))
            {
                setPixel(color, currentX, currentY, render_mode);
            }
        }
    }
}

void renderRectangle(PVec2f p1, PVec2f p2, PRGBA color, const PRenderMode* render_mode)
{
    assert(appToPi.currentLayer);
    int canvasWidth = appToPi.currentLayer->width, canvasHeight = appToPi.currentLayer->height;

    int minX = (int)std::max(0.0f,
                             std::min({p1.x, p2.x, float(canvasWidth)})),

        maxX = (int)std::min(float(canvasWidth),
                             std::max({p1.x, p2.x, 0.0f})),

        minY = (int)std::max(0.0f,
                             std::min({p1.y, p2.y, float(canvasHeight)})),

        maxY = (int)std::min(float(canvasHeight),
                             std::max({p1.y, p2.y, 0.0f}));

    for (int currentX = minX; currentX < maxX; currentX++)
    {
        // if (currentX >= canvasWidth)
        //     break;

        for (int currentY = minY; currentY < maxY; currentY++)
        {
            // if (currentY >= canvasHeight)
            //     break;
            
            setPixel(color, currentX, currentY, render_mode);
        }
    }
}

void renderPixels(PVec2f position, const PRGBA* data, size_t width, size_t height, const PRenderMode* render_mode)
{
    assert(appToPi.currentLayer);
    int canvasWidth = appToPi.currentLayer->width, canvasHeight = appToPi.currentLayer->height;

    int index = 0;

    for (int currentY = position.y; currentY < position.y + height; currentY++)
    {
        if (currentY >= canvasHeight)
            break;

        for (int currentX = position.x; currentX < position.x + width; currentX++, index++)
        {
            if (currentX >= canvasWidth || currentX < 0 || currentY < 0)
                continue;

            setPixel(data[index], currentX, currentY, render_mode);
        }
    }
}

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
        renderCircle,
        renderLine,
        renderTriangle,
        renderRectangle,

        renderPixels,
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
};

PAppInterface const* JIMP::Plugins::getPAppInterface()
{
    return &pAppInterface;
}

AppToPI* JIMP::Plugins::getAppToPI()
{
    return &appToPi;
}

void JIMP::Plugins::flush()
{
    assert(appToPi.currentPlugin);
    assert(appToPi.currentLayer);
    int canvasWidth = appToPi.currentLayer->width, canvasHeight = appToPi.currentLayer->height;

    auto flush_policy = appToPi.currentPlugin->interface->general.get_flush_policy();
    auto mode = (flush_policy == PPLP_BLEND) ? JIMP::MixMode::Alpha : JIMP::MixMode::Replace;

    for (int x = 0; x != canvasWidth; x++)
        for (int y = 0; y != canvasHeight; y++)
            appToPi.currentLayer->preview[x][y] = 
                JIMP::mixColors(appToPi.currentLayer->image[x][y],
                                appToPi.stdPreview[x][y], mode);
}