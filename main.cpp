/*++

Copyright (c) 2021 JulesIMF, MIPT

Module Name:

    main.cpp

Abstract:

    Main JIMP module. Only runs the window and
    proceeds incoming events.

Author / Creation date:

    JulesIMF / 06.10.21

Revision History:

--*/


//
// Includes / usings
//

#include <JG.h>
#include <common/messages.h>
#include <common/colors.h>
#include <file/formats.h>
#include <editor/Layer.h>
#include "UI/UI.h"
#include <plugins/sdk.h>


//
// Defines
//


#define VERSION "alpha v1.3.0 (22 nov 2021)"

void displayInfo(void)
{        
    printf("\nRunning %sJIMP " VERSION ".%s\n"
           "\t*To open a BMP file, type ./jimp \"<filepath>\".\n"
           "\t*To switch current layer, use Ctrl+PgUp and Ctrl+PgDown.\n"
           "\t*To switch tool, use PgUp and PgDown.\n\n", TerminalColor::BlueB, TerminalColor::Default);
}

void testPlugins()
{
    JIMP::Plugins::Plugin plugin;
    plugin.loadPlugin("soplugins/kctf_rainbow_stamp.so", nullptr);
    plugin.closePlugin();
}

int main(int argc, char const** argv)
{
    setDebug(true);
    // testPlugins();
    displayInfo();

    if (argc == 1)
        warningMessage("No file specified!");

    else
        JIMP::UI::filename = argv[1];

    JIMP::UI::MainWindow window(JIMP::UI::windowWidth, JIMP::UI::windowHeight);
    
    window.sendEvent(JG::Event::PaintEvent());

    JG::Event event = {};
    while (true)
    {
        while(window.pollEvent(event))
        {
            switch (event.type)
            {
            case JG::Event::Close:
                window.close();
                return 0;
                break;
            
            default:
                break;
            }
        }
    }
}