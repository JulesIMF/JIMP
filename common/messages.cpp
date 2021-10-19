/*++

Copyright (c) 2021 JulesIMF, MIPT

Module Name:

    messages.cpp

Abstract:

    Output messages functions.

Author / Creation date:

    JulesIMF / 26.06.2021

Revision History:
    26.06.2021      Taken from HCC project (JulesIMF)

--*/

//
// Includes / usings
//

#include <cstdarg>
#include <cstdio>
#include <common/messages.h>

//
// Defines
//

static bool debugLevel = false;

void setDebug(bool newMode)
{
    debugLevel = newMode;
}

int errorMessage(char const* format, ...)
{
    va_list list = {};
    va_start(list, format);
    fprintf(stderr, "\e[1;91m"
        "Error: "
        "\e[1;39m");
    int returned = vfprintf(stderr, format, list);
    va_end(list);
    fprintf(stderr, "\e[m\n");
    return returned;
}

int warningMessage(char const* format, ...)
{
    va_list list = {};
    va_start(list, format);
    fprintf(stderr, "\e[1;95m"
        "Warning: "
        "\e[1;39m");
    int returned = vfprintf(stderr, format, list);
    va_end(list);
    fprintf(stderr, "\e[m\n");
    return returned;
}

int debugMessage(char const* format, ...)
{
    if (!debugLevel)
        return 0;
    va_list list = {};
    va_start(list, format);
    fprintf(stderr, "\e[1;94m"
        "debug: "
        "\e[1;39m");
    int returned = vfprintf(stderr, format, list);
    va_end(list);
    fprintf(stderr, "\e[m\n");
    return returned;
}