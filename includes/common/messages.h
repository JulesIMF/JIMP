/*++

Copyright (c) 2021 JulesIMF, MIPT

Module Name:

    messages.h

Abstract:

    Output messages functions.

Author / Creation date:

    JulesIMF / 26.06.2021

Revision History:
    26.06.2021      Taken from HCC project (JulesIMF)

--*/

#ifndef JCALC_MESSAGES
#define JCALC_MESSAGES

//
// Includes / usings
//

//
// Defines
//

void setDebug(bool newMode);
int errorMessage(char const* format, ...);
int warningMessage(char const* format, ...);
int debugMessage(char const* format, ...);

#endif // !JCALC_MESSAGES