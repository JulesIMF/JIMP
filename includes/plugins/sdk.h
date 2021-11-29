/*++

Copyright (c) 2021 JulesIMF, MIPT

Module Name:

    sdk.h

Abstract:

    A wrapper for "plugin_std.hpp"

Author / Creation date:

    JulesIMF / 28.11.21

Revision History:

--*/

#ifndef JIMP_PLUGINSSDK
#define JIMP_PLUGINSSDK

//
// Includes / usings
//

#include <chrono>
#include "../../plugins/EditorPluginAPI/plugin_std.hpp"
#include <editor/Layer.h>
#include <editor/ToolPicker.h>


namespace JIMP
{
    namespace Plugins
    {
        static uint32_t constexpr MinStdVersion = 0;

        struct Plugin
        {
            PPluginType type;
            void* libHandle = nullptr;
            PPluginInterface* interface = nullptr;
            PPluginInfo const* info = nullptr;
            PPreviewLayerPolicy flushPolicy;

            bool loadPlugin(char const* filename, const PAppInterface* appInterface);
            void closePlugin();
            bool isLoaded() const;
            double timeFromStart() const;
            // char const* getName() const;

        protected:
            bool loaded = false;
            char const* filenameLoadedFrom = nullptr;
            std::chrono::high_resolution_clock clock;
            std::chrono::system_clock::time_point startTimer;
        };

        struct AppToPI
        {
            Plugin* currentPlugin = nullptr;
            Layer* currentLayer = nullptr;
            ToolPicker* picker = nullptr;
            PRGBA* pixels = nullptr;
            JG::Color** stdPreview = nullptr;
        };

        #ifndef JIMP_APPINTERFACE
            extern PAppInterface const* getPAppInterface();
        #else
            PAppInterface const* getPAppInterface();
        #endif
    }
}


//
// Defines
//

#endif // !JIMP_PLUGINSSDK