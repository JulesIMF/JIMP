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
#include "Vista.h"

//
// Defines
//

namespace JIMP
{
    namespace UI
    {
        Palette* palette = nullptr;
        ToolPicker* toolPicker = nullptr;
        LayerSwitcher* layerSwitcher = nullptr;

        JIMP::BMP bmp;
        char const* filename = nullptr;

        static int const windowWidth  = 1600, windowHeight  = 900;
        static int const editorWidth  = 800,  editorHeight  = 600;
        static int const space        = 40;
        static int const paletteWidth = 350,  paletteHeight = 300;

        static int const buttonWidth  = 200,  buttonHeight  = VistaButton::buttonHeight;
        static int const switchN      = 2,    switchSpace   = 25;

        static int const switchWidth  = buttonWidth + 2 * switchSpace,
                         switchHeight = switchSpace + (switchSpace + buttonHeight) * switchN;

        // Editor

        class   EditorCanvas : public JG::Canvas
        {
        public:
            Editor* editor;

            EditorCanvas(JG::Window* window, int beginX, int beginY,
                                             int width, int height) : 
                JG::Canvas(window, beginX, beginY, width, height),
                editor(new Editor(editorWidth, editorHeight))
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

        class EditorCanvasPanel : public VistaPanel
        {
        public:
            EditorCanvas* editorCanvas;

            EditorCanvasPanel(JG::Window* window, int width = editorWidth, int height = editorHeight) : 
                VistaPanel(window, 
                           (windowWidth - width) / 2 + nAdded * space, 
                           (windowHeight - height) / 2 + nAdded * space,
                           width,
                           height),
                editorCanvas(new EditorCanvas(window, 0, 0, width, height))
            {
                addChild(editorCanvas);
                caption = "Editor canvas";
                nAdded++;
            }

            static int nAdded;
            };
        
        int EditorCanvasPanel::nAdded = 0;

        // Switcher
        class Switcher : public VistaPanel
        {
            class AddEditor : public VistaButton
            {
            public:
                AddEditor(JG::Window* window, int beginX, int beginY, int width) :
                    VistaButton(window, beginX, beginY, width)
                {
                    caption = "Add editor";
                }

                virtual JG::Widget::HandlerResponce onClick(JG::Event event) override
                {
                    window->addChild(new EditorCanvasPanel(window));
                    return JG::Widget::HandlerResponce::Success;
                }
            };

            class NextInstrument : public VistaButton
            {
            public:
                NextInstrument(JG::Window* window, int beginX, int beginY, int width) :
                    VistaButton(window, beginX, beginY, width)
                {
                    caption = "Next instrument";
                }

                virtual JG::Widget::HandlerResponce onClick(JG::Event event) override
                {
                    toolPicker->nextTool();
                    return JG::Widget::HandlerResponce::Success;
                }
            };

            class NextLayer : public VistaButton
            {
            public:
                NextLayer(JG::Window* window, int beginX, int beginY, int width) :
                    VistaButton(window, beginX, beginY, width)
                {
                    caption = "Next layer";
                }

                virtual JG::Widget::HandlerResponce onClick(JG::Event event) override
                {
                    toolPicker->nextTool();
                    return JG::Widget::HandlerResponce::Success;
                }
            };

        public:
            Switcher(JG::Window* window) : 
                VistaPanel(window, 0, 0, switchWidth, switchHeight)
            {
                // addChild(new AddEditor(window, switchSpace, switchSpace,
                //                                  buttonWidth));

                addChild(new NextInstrument(window, switchSpace, switchSpace,
                                                      buttonWidth));
                setCaption();
            }

            virtual void renderMyself(int shiftX, int shiftY)
            {
                setCaption();
                VistaPanel::renderMyself(shiftX, shiftY);
            }

        protected:
            void setCaption()
            {
                free((void*)caption);
                char const* prefix = "Switcher [", 
                          * suffix = "]";
                
                int size = strlen(prefix) + strlen(suffix) + 1;
                auto name = toolPicker->getTool()->getName();
                size += strlen(name);

                char* captionConcat = (char*)calloc(size, sizeof(char));
                strcat(captionConcat, prefix);
                strcat(captionConcat, name);
                strcat(captionConcat, suffix);

                caption = captionConcat;
            }
        };

        Switcher* switcher = nullptr;
        EditorCanvasPanel* mainEditorCanvasPanel = nullptr;
        JG::Panel* palettePanel = nullptr;

        class MainWindow : public JG::Window
        {
        public:
            MainWindow(int sizeX, int sizeY) : JG::Window(sizeX, sizeY, "JIMP", JG::Window::Style::Default)
            {
                addChild(palettePanel = new VistaPanel(this, JG::Panel::outline, windowHeight -
                                                                                 paletteHeight - 
                                                                                 VistaPanel::vistaOutline - 
                                                                                 VistaPanel::VistaPanelBar::vistaBarHeight, 
                                                                                 paletteWidth, paletteHeight));

                

                palettePanel->caption = "Palette";
                palettePanel->addChild(new Palette(this, 0, 0, paletteWidth, paletteHeight, toolPicker = new ToolPicker));

                toolPicker->insert(new Brush);
                toolPicker->insert(new Eraser);
                toolPicker->insert(new Fill);

                addChild(mainEditorCanvasPanel = new EditorCanvasPanel(this));
                layerSwitcher = new LayerSwitcher(mainEditorCanvasPanel->editorCanvas->editor);

                addChild(switcher = new Switcher(this));
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