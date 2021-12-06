/*++

Copyright (c) 2021 JulesIMF, MIPT

Module Name:

    Effect.cpp

Abstract:

    

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

void PluginEffect::start(JIMP::Layer& layer)
{
    assert(getAppToPI()->setLayer(&layer));
    layer.onCorrectionBegin();

    getAppToPI()->currentPlugin = &plugin;

    update();
}

void PluginEffect::update(...)
{

    if (plugin.interface->general.on_settings_update)
    {
        plugin.interface->general.on_settings_update();
        flush();
    }

}


void PluginEffect::apply()
{
    plugin.interface->effect.apply();
    flush();
    getAppToPI()->currentLayer->onCorrectionEnd();
    end();
}

void PluginEffect::reset()
{
    getAppToPI()->currentLayer->onCorrectionReset();
    end();
}

void PluginEffect::end()
{
    getAppToPI()->releaseLayer();
    getAppToPI()->currentPlugin = nullptr;
}
