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
#include <common/messages.h>
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

            bool loadPlugin(char const* filename);
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

            bool isSet()
            {
                return currentLayer;
            }

            bool setLayer(Layer* newLayer)
            {
                if (isSet())
                {
                    errorMessage("can`t set new layer in AppToPI cause it`s already set");
                    return false;
                }

                currentLayer = newLayer;
                stdPreview = new JG::Color* [currentLayer->width];

                for (int x = 0; x != currentLayer->width; x++)
                {
                    stdPreview[x] = new JG::Color[currentLayer->height];
                    memset(stdPreview[x], 0, sizeof(JG::Color) * currentLayer->height);
                }

                return true;
            }

            void releaseLayer()
            {
                for (int x = 0; x != currentLayer->width; x++)
                    delete[] stdPreview[x];

                delete[] stdPreview;
                currentLayer = nullptr;
            }


        };

        struct PluginTool : public JIMP::Tool
        {
            PluginTool(Plugin plugin, std::string name) : 
                plugin(plugin),
                name(name)
            {

            }

            virtual void applyOnPress(JIMP::Layer& layer) override;
            virtual void applyOnMove(JIMP::Layer& layer) override;
            virtual void applyOnRelease(JIMP::Layer& layer) override;
            virtual void applyOnTimer(JIMP::Layer& layer, int passedMs) override;
            virtual char const* getName() override;
            virtual void drawCursor(JG::Window* window, int x, int y, int canvasWidth, int canvasHeight, int shiftX, int shiftY) override;

        protected:
            Plugin plugin;
            std::string name;
        };

        struct PluginEffect
        {
            PluginEffect(Plugin plugin, std::string name) :
                plugin(plugin),
                name(name)
            {

            }

            void start(JIMP::Layer& layer);
            void apply();
            void update(...);
            void reset();

        protected:
            Plugin plugin;
            std::string name;
            void end();

        };

        PAppInterface const* getPAppInterface();
        AppToPI* getAppToPI();
        void flush();
    }
}


//
// Defines
//

#endif // !JIMP_PLUGINSSDK