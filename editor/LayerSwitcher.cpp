/*++

Copyright (c) 2021 JulesIMF, MIPT

Module Name:

    LayerSwitcher.cpp

Abstract:

    

Author / Creation date:

    JulesIMF / 19.10.21

Revision History:

--*/


//
// Includes / usings
//

#include <common/messages.h>
#include <editor/LayerSwitcher.h>

//
// Defines
//

using namespace JIMP;

LayerSwitcher::LayerSwitcher(Editor* editor) : 
    editor(editor)
{

}

Layer* LayerSwitcher::getCurrentLayer()
{
    if (nLayers() == 0)
        return nullptr;
    
    return getLayer(currentLayer);
}

bool LayerSwitcher::setLayer(int newCurrentLayer)
{
    if (newCurrentLayer < 0 || newCurrentLayer >= nLayers())
        return true;
    
    currentLayer = newCurrentLayer;
    return true;
}

void LayerSwitcher::nextLayer()
{
    if (nLayers() == 0)
        return;

    currentLayer = (currentLayer + 1) % nLayers();
    debugMessage("nextLayer: currentLayer set to %d", currentLayer);
}

void LayerSwitcher::prevLayer()
{
    if (nLayers() == 0)
        return;

    currentLayer = (currentLayer - 1 + nLayers()) % nLayers();
    debugMessage("prevLayer: currentLayer set to %d", currentLayer);
}

int LayerSwitcher::addLayer(int layerWidth, int layerHeight)
{
    layers.push_back(new Layer{ 0, 0, layerWidth, layerHeight });
    layers.back()->init();
    return nLayers() - 1;
}

void LayerSwitcher::removeLayer(int index)
{
    layers[index]->free();
    layers.erase(layers.begin() + index);
}

Layer* LayerSwitcher::getLayer(int index)
{
    return layers[index];
}

int LayerSwitcher::nLayers()
{
    return (int)layers.size();
}

std::vector<Layer*>& LayerSwitcher::getLayerVector()
{
    return layers;
}