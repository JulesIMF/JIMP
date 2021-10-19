/*++

Copyright (c) 2021 JulesIMF, MIPT

Module Name:

    colors.h

Abstract:

    Color escapes.

Author / Creation date:

    JulesIMF / 28.06.2021

Revision History:

--*/

#ifndef JCALC_COLORS
#define JCALC_COLORS


//
// Includes / usings
//


//
// Defines
//


namespace TerminalColor
{
    // 
    // Restoration escape seq
    //
    auto    Default = "\e[0m";

    //
    // Regular text
    //

    auto    Black   = "\e[0;30m";
    auto    Red     = "\e[0;31m";
    auto    Green   = "\e[0;32m";
    auto    Yellow  = "\e[0;33m";
    auto    Blue    = "\e[0;34m";
    auto    Purple  = "\e[0;35m";
    auto    Cyan    = "\e[0;36m";
    auto    White   = "\e[0;37m";

    //
    // Bold text
    //

    auto    BlackB  = "\e[1;30m";
    auto    RedB    = "\e[1;31m";
    auto    GreenB  = "\e[1;32m";
    auto    YellowB = "\e[1;33m";
    auto    BlueB   = "\e[1;34m";
    auto    PurpleB = "\e[1;35m";
    auto    CyanB   = "\e[1;36m";
    auto    WhiteB  = "\e[1;37m";

    //
    // Underlined text
    // 
    
    auto    BlackU  = "\e[4;30m";
    auto    RedU    = "\e[4;31m";
    auto    GreenU  = "\e[4;32m";
    auto    YellowU = "\e[4;33m";
    auto    BlueU   = "\e[4;34m";
    auto    PurpleU = "\e[4;35m";
    auto    CyanU   = "\e[4;36m";
    auto    WhiteU  = "\e[4;37m";
}

#endif // !JCALC_COLORS