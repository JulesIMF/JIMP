/*++

Copyright (c) 2021 JulesIMF, MIPT

Module Name:

    UI.h

Abstract:

    

Author / Creation date:

    JulesIMF / 18.10.21

Revision History:

--*/


//
// Includes / usings
//

#include <JG.h>
#include <editor/Palette.h>
#include <editor/Editor.h>
#include <editor/LayerSwitcher.h>

//
// Defines
//

namespace JIMP
{
    namespace UI
    {
        Editor* editor = nullptr;
        Palette* palette = nullptr;
        ToolPicker* toolPicker = nullptr;
        LayerSwitcher* layerSwitcher = nullptr;

        JIMP::BMP bmp;
        char const* filename = nullptr;

        class EditorCanvas : public JG::Canvas
        {
        public:
            EditorCanvas(JG::Window* window, int beginX, int beginY,
                                             int width, int height) : 
                JG::Canvas(window, beginX, beginY, width, height)
            {
                if (editor && editor->getImage())
                {
                    transferFromBuffer(editor->getImage());
                }
            }

            virtual void renderMyself(int shiftX, int shiftY) override
            {
                transferFromBuffer(editor->getImage());
                flush();
                
                Canvas::renderMyself(shiftX, shiftY);
            }

            void applyTool(int x, int y, bool onMove) // true - applyOnMove, false - applyOnRelease
            {
                auto tool = toolPicker->getTool();
                auto layer = layerSwitcher->getCurrentLayer();

                if (layer == nullptr)
                    return;

                tool->x = x - layer->beginX - editor->shiftX;
                tool->y = y - layer->beginY - editor->shiftY;

                if (onMove)
                    tool->applyOnMove(layer->image, layer->width, layer->height);

                else
                    tool->applyOnPress(layer->image, layer->width, layer->height);

                editor->mix(layerSwitcher->getLayerVector());
                window->sendEvent(JG::Event::PaintEvent());                
            }

            virtual JG::Widget::HandlerResponce onMouseButtonPressed(JG::Event event) override
            {
                applyTool(event.mouseButton.x, event.mouseButton.y, false);
                return JG::Widget::HandlerResponce::Success;
            }

            virtual JG::Widget::HandlerResponce onMouseMoved(JG::Event event) override
            {
                if (!mousePressed)
                    return JG::Widget::HandlerResponce::Failure;
                
                applyTool(event.mouseMove.x, event.mouseMove.y, true);
                return JG::Widget::HandlerResponce::Success;
            }

            virtual JG::Widget::HandlerResponce onKeyPressed(JG::Event event) override
            {
                int deltaXY = 10;
                switch(event.key.code)
                {
                    case JG::Keyboard::Up:
                        if (event.key.control)
                            layerSwitcher->getCurrentLayer()->beginY -= deltaXY;
                        
                        else
                            editor->shiftY -= deltaXY;

                        editor->mix(layerSwitcher->getLayerVector());
                        window->sendEvent(JG::Event::PaintEvent());

                        break;

                    case JG::Keyboard::Down:
                        if (event.key.control)
                            layerSwitcher->getCurrentLayer()->beginY += deltaXY;

                        else
                            editor->shiftY += deltaXY;

                        editor->mix(layerSwitcher->getLayerVector());
                        window->sendEvent(JG::Event::PaintEvent());

                        break;

                    case JG::Keyboard::Left:
                        if (event.key.control)
                            layerSwitcher->getCurrentLayer()->beginX -= deltaXY;

                        else
                            editor->shiftX -= deltaXY;

                        editor->mix(layerSwitcher->getLayerVector());
                        window->sendEvent(JG::Event::PaintEvent());

                        break;

                    case JG::Keyboard::Right:
                        if (event.key.control)
                            layerSwitcher->getCurrentLayer()->beginX += deltaXY;

                        else
                            editor->shiftX += deltaXY;

                        editor->mix(layerSwitcher->getLayerVector());
                        window->sendEvent(JG::Event::PaintEvent());

                        break;
                    
                    case JG::Keyboard::PageDown:
                        if (event.key.control)
                            layerSwitcher->prevLayer();

                        else
                            toolPicker->prevTool();

                        editor->mix(layerSwitcher->getLayerVector());
                        window->sendEvent(JG::Event::PaintEvent());

                        break;
                    
                    case JG::Keyboard::PageUp:
                        if (event.key.control)
                            layerSwitcher->nextLayer();

                        else
                            toolPicker->nextTool();

                        editor->mix(layerSwitcher->getLayerVector());
                        window->sendEvent(JG::Event::PaintEvent());

                        break;

                    default:
                        return JG::Widget::HandlerResponce::Failure;
                }

                return JG::Widget::HandlerResponce::Success;
            }
        };

        static int const windowWidth  = 1600, windowHeight  = 900;
        static int const editorWidth  = 1000,  editorHeight = 750;
        static int const paletteWidth = 350,  paletteHeight = 300;


        EditorCanvas* editorCanvas = nullptr;
        JG::Panel* palettePanel = nullptr;
        JG::Panel* editorCanvasPanel = nullptr;

        class MainWindow : public JG::Window
        {
        public:
            MainWindow(int sizeX, int sizeY) : JG::Window(sizeX, sizeY, "JIMP", JG::Window::Style::Default)
            {
                editor = new Editor(editorWidth, editorHeight);

                addChild(palettePanel = new JG::Panel(this, JG::Panel::outline, windowHeight -
                                                                                paletteHeight - 
                                                                                JG::Panel::outline - 
                                                                                JG::Panel::PanelBar::barHeight, 
                                                                                paletteWidth, paletteHeight));
                palettePanel->caption = "Palette";
                palettePanel->addChild(new Palette(this, 0, 0, paletteWidth, paletteHeight, toolPicker = new ToolPicker));

                toolPicker->insert(new Brush);
                toolPicker->insert(new Eraser);
                toolPicker->insert(new Fill);

                layerSwitcher = new LayerSwitcher(editor);

                addChild(editorCanvasPanel = new JG::Panel(this, 
                                                           (windowWidth - editorWidth)/2, 
                                                           (windowHeight - editorHeight)/2, 
                                                           editorWidth, 
                                                           editorHeight));

                editorCanvasPanel->caption = "Editor canvas";
                editorCanvasPanel->addChild(editorCanvas = new EditorCanvas(this, 0, 0, editorWidth, editorHeight));
            }

            virtual void renderMyself(int shiftX, int shiftY)
            {
                JG::Rectangle rect(0, 0, width, height);
                rect.setColor({255, 255, 255});
                rect.draw(*this);
            }
        };
    }
}