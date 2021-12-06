/*++

Copyright (c) 2021 JulesIMF, MIPT

Module Name:

    Plugin.cpp

Abstract:

    Plugin is a container for PPluginInterface and
    other related information.

Author / Creation date:

    JulesIMF / 29.11.21

Revision History:

--*/

//
// Includes / usings
//

#include <dlfcn.h>
#include <plugins/sdk.h>
#include <common/messages.h>
#include "EditorPluginAPI/plugin_std.hpp"

using namespace JIMP::Plugins;

//
// Defines
//

bool Plugin::loadPlugin(char const* filename)
{
    // ******************************************
    // * Collecting main info
    // ******************************************

    filenameLoadedFrom = filename;

    libHandle = dlopen(filename, RTLD_LAZY);
    if (libHandle == nullptr)
    {
        errorMessage("failed to open library \"%s\"", filename);
        return false;
    }

    PPluginInterface* (*get_plugin_interface)(...);

    get_plugin_interface = (decltype(get_plugin_interface))dlsym(libHandle, PGET_INTERFACE_FUNC);
    if (get_plugin_interface == nullptr)
    {
        errorMessage("failed to get plugin \"%s\" interface; probably it was not a plugin, but ", filename);
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

    // ******************************************
    // * Checking compatibility
    // ******************************************

    if (interface->std_version < MinStdVersion)
    {
        errorMessage("plugin \"%s\" follows version %u; "
            "it`s lower than the app requires (%u)",
            interface->std_version,
            MinStdVersion);

        closePlugin();
        return false;
    }

#define CHECK_FUNCTION_IF_NONZERO(fnc) \
                if (interface->fnc == nullptr) \
                { \
                    errorMessage("plugin \"%s\" interface has %s == nullptr", filename, #fnc); \
                    closePlugin(); \
                    return false; \
                } \

    CHECK_FUNCTION_IF_NONZERO(general.get_info);
    CHECK_FUNCTION_IF_NONZERO(general.init);
    CHECK_FUNCTION_IF_NONZERO(general.deinit);
    CHECK_FUNCTION_IF_NONZERO(general.on_tick);
    // CHECK_FUNCTION_IF_NONZERO(general.on_settings_update);
    CHECK_FUNCTION_IF_NONZERO(general.get_flush_policy);

    // ******************************************
    // * Settings, depending on plugin type
    // ******************************************

    info = interface->general.get_info();
    flushPolicy = interface->general.get_flush_policy();

    type = info->type;

    switch (type)
    {
    case PPluginType::PPT_EFFECT:
        CHECK_FUNCTION_IF_NONZERO(effect.apply);
        break;

    case PPluginType::PPT_TOOL:
        CHECK_FUNCTION_IF_NONZERO(tool.on_press);
        CHECK_FUNCTION_IF_NONZERO(tool.on_release);
        CHECK_FUNCTION_IF_NONZERO(tool.on_move);
        break;

    default:
        errorMessage("plugin \"%s\" has unsupported or wrong type (type == %x)", filename, type);
        closePlugin();
        return false;
    }

    debugMessage("from file \"%s\" loading plugin:\n"
        "            std_version = %u\n"
        "            reserved = %p\n"
        "            interface = %p (init returned %p)\n"

        "            name = %s\n"
        "            version = %s\n"
        "            author = %s\n"
        "            descripton = \"%s\"\n"
        "            type = %s",
        filename,
        info->std_version,
        info->reserved,
        info->interface,
        interface,
        info->name,
        info->version,
        info->author,
        info->description,
        info->type == PPT_EFFECT ? "PPT_EFFECT" : "PPT_TOOL");

    // ******************************************
    // * Reverse handshaking
    // ******************************************

    auto handshakingStatus = interface->general.init(getPAppInterface());
    
    switch (handshakingStatus)
    {
    case PPluginStatus::PPS_ERR:
        errorMessage("plugin \"%s\" failed to init", filename);
        closePlugin();
        return false;

    default:
        debugMessage("plugin \"%s\" has been loaded", filename);
        break;
    }

    loaded = true;
    startTimer = clock.now();
    return true;
}

void Plugin::closePlugin()
{
    if (interface && interface->general.deinit)
        interface->general.deinit();

    if (libHandle)
        dlclose(libHandle);

    *this = {};
}

bool Plugin::isLoaded() const
{
    return loaded;
}

double Plugin::timeFromStart() const
{
    return double(
        std::chrono::duration_cast<std::chrono::microseconds>(clock.now() - startTimer).count()
    ) / double(1'000'000);
}

