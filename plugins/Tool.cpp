/*++

Copyright (c) 2021 JulesIMF, MIPT

Module Name:

    Tool.cpp

Abstract:

    Tool wrapper for tool-type plugins.

Author / Creation date:

    JulesIMF / 06.12.21

Revision History:

--*/


//
// Includes / usings
//

#include <cassert>
#include <plugins/sdk.h>
using namespace JIMP::Plugins;

//
// Defines
//


void PluginTool::applyOnPress(JIMP::Layer& layer)
{
    assert(getAppToPI()->setLayer(&layer));
    layer.onCorrectionBegin();

    getAppToPI()->currentPlugin = &plugin;

    getAppToPI()->currentPlugin->interface->tool.on_press
        ({float(x), float(y)});

    flush();
}

void PluginTool::applyOnMove(JIMP::Layer& layer)
{
    getAppToPI()->currentPlugin->interface->tool.on_move
        ({float(prevX), float(prevY)}, {float(x), float(y)});

    flush();
}

void PluginTool::applyOnRelease(JIMP::Layer& layer)
{
    getAppToPI()->currentPlugin->interface->tool.on_release
        ({float(x), float(y)});
    
    flush();

    layer.onCorrectionEnd();
    getAppToPI()->releaseLayer();
    getAppToPI()->currentPlugin = nullptr;
}

void PluginTool::applyOnTimer(JIMP::Layer& layer, int passedMs)
{
    plugin.interface->general.on_tick((double)passedMs / 1000.0);
    flush();
}

char const* PluginTool::getName()
{
    return name.c_str();
}

void PluginTool::drawCursor(JG::Window* window, int x, int y, int canvasWidth, int canvasHeight, int shiftX, int shiftY)
{
    (void)window;
    (void)x;
    (void)y;
    (void)canvasHeight;
    (void)canvasWidth;
    (void)shiftX;
    (void)shiftY;
}

