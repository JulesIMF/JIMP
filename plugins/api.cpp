/*++

Copyright (c) 2021 JulesIMF, MIPT

Module Name:

    loadclose.cpp

Abstract:

    This module loads plugin into the app
    and releases it.

Author / Creation date:

    JulesIMF / 29.11.21

Revision History:

--*/

#ifndef JIMP_PLUGINLOAD
#define JIMP_PLUGINLOAD

//
// Includes / usings
//

#include <dlfcn.h>
#include <plugins/sdk.h>
#include <common/messages.h>
#include "EditorPluginAPI/plugin_std.hpp"

//
// Defines
//

namespace JIMP
{
    namespace Plugins
    { 
        bool Plugin::loadPlugin(char const* filename)
        {
            libHandle = dlopen(filename, RTLD_NOW);
            if (libHandle == nullptr)
            {
                errorMessage("failed to open library \"%s\"", filename);
                return false;    
            }

            PPluginInterface* (*get_plugin_interface)(...);

            get_plugin_interface = (decltype(get_plugin_interface))dlsym(libHandle, PGET_INTERFACE_FUNC);
            if (get_plugin_interface == nullptr)
            {
                errorMessage("failed to get plugin \"%s\" interface", filename);
                closePlugin();
                return false;
            }

            interface = get_plugin_interface();
            if (interface == nullptr)
            {
                errorMessage("plugin \"%s\" returned nullptr interface", filename);
                closePlugin();
                return false;
            }



            return true;
        }

        void Plugin::closePlugin()
        {
            if (interface)
                interface->general.deinit();
            
            dlclose(libHandle);
        }
    }
}

#endif 