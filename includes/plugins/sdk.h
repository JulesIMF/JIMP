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

#include "../../plugins/EditorPluginAPI/plugin_std.hpp"

namespace JIMP
{
    namespace Plugins
    {
        struct Plugin
        {
            void* libHandle;
            PPluginInterface* interface;

            bool loadPlugin(char const* filename);
            void closePlugin();
            bool isOpen();

        protected:
            char const* filenameLoadedFrom;
        };
    }
}


//
// Defines
//

#endif // !JIMP_PLUGINSSDK