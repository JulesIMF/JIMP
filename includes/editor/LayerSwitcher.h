/*++

Copyright (c) 2021 JulesIMF, MIPT

Module Name:

    LayerSwitcher.h

Abstract:

    

Author / Creation date:

    JulesIMF / 19.10.21

Revision History:

--*/

#ifndef LAYERSWITCHER_JULESIMF
#define LAYERSWITCHER_JULESIMF

//
// Includes / usings
//

#include <vector>
#include <editor/Editor.h>
#include <editor/Layer.h>

//
// Defines
//

namespace JIMP
{
    struct LayerSwitcher
    {
        LayerSwitcher(Editor* editor);
        Editor* editor;
        bool setLayer(int newCurrentLayer);
        void nextLayer();
        void prevLayer();
        Layer* getCurrentLayer();

        int addLayer(int layerWidth, int layerHeight);
        Layer* getLayer(int index);
        int nLayers();
        void removeLayer(int index);

        std::vector<Layer*>& getLayerVector();

    protected:
        std::vector<Layer*> layers;
        int currentLayer = 0;
    };
}

#endif // !LAYERSWITCHER_JULESIMF