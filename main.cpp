/*++

Copyright (c) 2021 JulesIMF, MIPT

Module Name:

    main.cpp

Abstract:

    Main JIMP module.

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


//
// Defines
//


#define VERSION "alpha v1.2.2 (20 oct 2021)"

void displayInfo(void)
{        
    printf("\nRunning %sJIMP " VERSION ".%s\n"
           "\t*To open a BMP file, type ./jimp \"<filepath>\".\n"
           "\t*To switch current layer, use Ctrl+PgUp and Ctrl+PgDown.\n"
           "\t*To switch instrument, use PgUp and PgDown.\n\n", TerminalColor::BlueB, TerminalColor::Default);
}

int main(int argc, char const** argv)
{
    displayInfo();
    setDebug(true);

    JIMP::UI::MainWindow window(JIMP::UI::windowWidth, JIMP::UI::windowHeight);

    if (argc == 1)
    {
        warningMessage("No file specified!");
    }
    else
    {
        FILE* file = fopen(argv[1], "r");
        JIMP::BMP bmp = JIMP::loadImage(file);
        fclose(file);

        if (bmp.bitCount / 8 != 4)
        {
            errorMessage("Wrong format");
        }

        else
        {
            auto image = JIMP::imageToColorBuffer(bmp);

            auto bmpLayer = JIMP::UI::layerSwitcher->addLayer(bmp.xSize, bmp.ySize);
            JIMP::transferColorBuffer(JIMP::UI::layerSwitcher->getLayer(bmpLayer)->image, image, bmp.xSize, bmp.ySize);
        }
    }
    
    JIMP::UI::layerSwitcher->addLayer(JIMP::UI::editorWidth, JIMP::UI::editorHeight);
    JIMP::UI::layerSwitcher->nextLayer();

    JIMP::UI::editor->mix(JIMP::UI::layerSwitcher->getLayerVector());
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