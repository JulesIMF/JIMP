/*++

Copyright (c) 2021 JulesIMF, MIPT

Module Name:

    LayerSwitcher.cpp

Abstract:

    Layer switcher contains all layers for its canvas.
    It IS able to give reference to the vector of them
    for mixing in Editor.

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

int LayerSwitcher::getCurrentLayerIndex()
{
    return currentLayer;
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
    layers.push_back(new Layer{ false, 0, 0, layerWidth, layerHeight });
    layers.back()->init();
    return nLayers() - 1;
}

int LayerSwitcher::addLayer(Layer* layer)
{
    layers.push_back(layer);
    return currentLayer = nLayers() - 1;
}

void LayerSwitcher::removeLayer(int index)
{
    auto oldWidth = layers[index]->width, oldHeight = layers[index]->height;

    layers[index]->free();
    layers.erase(layers.begin() + index);
    if (!nLayers())
        addLayer(oldWidth, oldHeight);

    currentLayer = std::min(nLayers() - 1, currentLayer);
}

void LayerSwitcher::forward()
{
    if (currentLayer + 1 == nLayers())
        return;
    
    std::swap(layers[currentLayer], layers[currentLayer + 1]);
    currentLayer++;
}

void LayerSwitcher::backward()
{
    if (!currentLayer)
        return;

    std::swap(layers[currentLayer], layers[currentLayer - 1]);
    currentLayer--;
}

void LayerSwitcher::copy()
{
    auto layer = layers[currentLayer];
    auto newLayer = new Layer;
    newLayer->width = layer->width;
    newLayer->height = layer->height;
    newLayer->init();
    newLayer->beginX = layer->beginX;
    newLayer->beginY = layer->beginY;
    for (int x = 0; x != layer->width; x++)
        for (int y = 0; y != layer->height; y++)
            newLayer->image[x][y] = layer->image[x][y];
    layers.insert(layers.begin() + currentLayer + 1, newLayer);
    currentLayer++;
}

void LayerSwitcher::mute()
{
    auto layer = layers[currentLayer];
    layer->muted = !layer->muted;
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