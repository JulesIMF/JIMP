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
#include <editor/Curves.h>
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

        class EditorCanvas;
        EditorCanvas* activeCanvas = nullptr;

        // Editor

        class EditorCanvas : public JG::Canvas
        {
        public:
            Editor* editor;
            LayerSwitcher* layerSwitcher = nullptr;

            int toolX = 0, toolY = 0;

            EditorCanvas(JG::Window* window, int beginX, int beginY,
                                             int width, int height) : 
                JG::Canvas(window, beginX, beginY, width, height),
                editor(new Editor(editorWidth, editorHeight))
            {
                if (editor && editor->getImage())
                {
                    transferFromBuffer(editor->getImage());
                }

                layerSwitcher = new LayerSwitcher(editor);
            }

            virtual void renderMyself(int shiftX, int shiftY) override
            {
                transferFromBuffer(editor->getImage());
                flush();

                Canvas::renderMyself(shiftX, shiftY);
                if (this == activeCanvas)
                {
                    toolPicker->getTool()->drawCursor(window,
                                                      toolX,
                                                      toolY,
                                                      editorWidth, 
                                                      editorHeight, 
                                                      shiftX, 
                                                      shiftY + VistaPanel::VistaPanelBar::vistaBarHeight);
                }
            }

            void moveTool(int x, int y, int dx, int dy)
            {
                auto tool = toolPicker->getTool();
                auto layer = layerSwitcher->getCurrentLayer();

                if (layer == nullptr)
                    return;

                tool->dx = dx;
                tool->dy = dy;

                toolX = x;
                toolY = y;

                tool->x = x - layer->beginX - editor->shiftX;
                tool->y = y - layer->beginY - editor->shiftY;
            }

            void applyTool(int x, int y, int dx, int dy, bool onMove) // true - applyOnMove, false - applyOnRelease
            {
                moveTool(x, y, dx, dy);

                auto tool = toolPicker->getTool();
                auto layer = layerSwitcher->getCurrentLayer();

                if (onMove)
                    tool->applyOnMove(*layer);

                else
                    tool->applyOnPress(*layer);

                editor->mix(layerSwitcher->getLayerVector());
                window->sendEvent(JG::Event::PaintEvent());                
            }

            virtual JG::Widget::HandlerResponce onMouseButtonPressed(JG::Event event) override
            {
                applyTool(event.mouseButton.x, event.mouseButton.y, event.mouseMove.dx, event.mouseMove.dy, false);
                return JG::Widget::HandlerResponce::Success;
            }

            virtual JG::Widget::HandlerResponce onMouseMoved(JG::Event event) override
            {
                if (mousePressed)                
                    applyTool(event.mouseMove.x, event.mouseMove.y, event.mouseMove.dx, event.mouseMove.dy, true);
                else
                    moveTool(event.mouseMove.x, event.mouseMove.y, event.mouseMove.dx, event.mouseMove.dy);

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
                            editor->shiftY += deltaXY;

                        editor->mix(layerSwitcher->getLayerVector());
                        window->sendEvent(JG::Event::PaintEvent());

                        break;

                    case JG::Keyboard::Down:
                        if (event.key.control)
                            layerSwitcher->getCurrentLayer()->beginY += deltaXY;

                        else
                            editor->shiftY -= deltaXY;

                        editor->mix(layerSwitcher->getLayerVector());
                        window->sendEvent(JG::Event::PaintEvent());

                        break;

                    case JG::Keyboard::Left:
                        if (event.key.control)
                            layerSwitcher->getCurrentLayer()->beginX -= deltaXY;

                        else
                            editor->shiftX += deltaXY;

                        editor->mix(layerSwitcher->getLayerVector());
                        window->sendEvent(JG::Event::PaintEvent());

                        break;

                    case JG::Keyboard::Right:
                        if (event.key.control)
                            layerSwitcher->getCurrentLayer()->beginX += deltaXY;

                        else
                            editor->shiftX -= deltaXY;

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
                        return JG::Widget::HandlerResponce::Success;
                }

                return JG::Widget::HandlerResponce::Success;
            }
        };

        class EditorCanvasPanel : public VistaPanel
        {
        public:
            EditorCanvas* editorCanvas;

            EditorCanvasPanel(JG::Window* window, char const* fileName = nullptr, int width = editorWidth, int height = editorHeight) : 
                VistaPanel(window, 
                           (windowWidth - width) / 2 + nAdded * space, 
                           (windowHeight - height) / 2 + nAdded * space,
                           width,
                           height),
                editorCanvas(new EditorCanvas(window, 0, 0, width, height))
            {
                addChild(editorCanvas);
                if (activeCanvas == nullptr)
                    activeCanvas = editorCanvas;

                caption = "Editor canvas";
                nAdded++;

                if (fileName)
                {
                    FILE* file = fopen(filename, "r");
                    if (!file)
                    {
                        errorMessage("Wrong file");
                    }

                    else
                    {
                        JIMP::BMP bmp = JIMP::loadImage(file);
                        fclose(file);

                        if (bmp.bitCount / 8 != 4)
                        {
                            errorMessage("Wrong format");
                        }

                        else
                        {
                            auto image = JIMP::imageToColorBuffer(bmp);
                            auto bmpLayer = editorCanvas->layerSwitcher->addLayer(bmp.xSize, bmp.ySize);
                            JIMP::transferColorBuffer(editorCanvas->layerSwitcher->getLayer(bmpLayer)->image, image, bmp.xSize, bmp.ySize);
                            JIMP::deleteColorBuffer(image, bmp.xSize, bmp.ySize);
                        }
                    }
                }

                editorCanvas->layerSwitcher->addLayer(JIMP::UI::editorWidth, JIMP::UI::editorHeight);
                editorCanvas->layerSwitcher->nextLayer();

                editorCanvas->editor->mix(editorCanvas->layerSwitcher->getLayerVector());
            }

            virtual JG::Widget::HandlerResponce onFocusEntered(JG::Event event) override
            {
                debugMessage("Now 0x%p got focus", this);
                activeCanvas = editorCanvas;
                return JG::Widget::HandlerResponce::Success;
            }

            virtual JG::Widget::HandlerResponce onFocusLeft(JG::Event event) override
            {
                debugMessage("Now 0x%p lost focus", this);
                return JG::Widget::HandlerResponce::Success;
            }

            virtual JG::Widget::HandlerResponce onDelete(JG::Event event) override
            {
                if (activeCanvas == editorCanvas)
                    activeCanvas = nullptr;

                return JG::Widget::HandlerResponce::Success;
            }

            static int nAdded;
        };
        
        class Bublick : public JG::Panel
        {
        public:
            static int const bublickWidth = 300, bublickHeight = 300;

            class BublickBar : public JG::Panel::PanelBar
            {
            public:
                static int const bbarWidth = 300, bbarHeight = 105;
                BublickBar(JG::Window* window, Bublick* bublick, int beginX, int beginY) :
                    JG::Panel::PanelBar(window, bublick, beginX, beginY, bbarWidth),
                    barImage(0, 0, bbarWidth, bbarHeight, "UI/debug/bar.png")
                {
                    height = bbarHeight;
                    auto button = children.at(0);
                    button->setPosition((bbarWidth - JG::Panel::PanelBar::barHeight) / 2,
                                        (bbarHeight - JG::Panel::PanelBar::barHeight) / 2);
                }

                virtual void renderMyself(int shiftX, int shiftY) override
                {
                    translateAndDraw(barImage, window, shiftX + beginX, shiftY + beginY);
                }

                virtual bool checkHover(int x, int y)
                {
                    return barImage.getPixel(x, y).a;
                }

            protected:    
                JG::Image barImage;
            };

            Bublick(JG::Window* window, int beginX, int beginY) :
                JG::Panel(window, beginX, beginY, bublickWidth, bublickHeight),
                roundImage(0, 0, bublickWidth, bublickHeight, "UI/debug/round.png")
            {
                children.clear();
                addChild(new BublickBar(window, this, 0, 0));
            }

            virtual void addChild(JG::Widget* widget)
            {
                Widget::addChild(widget);
            }

            virtual void renderMyself(int shiftX, int shiftY) override
            {
                translateAndDraw(roundImage, window, shiftX + beginX, shiftY + beginY, 0, mousePressed * bublickHeight);
            }

            virtual bool checkHover(int x, int y)
            {
                return roundImage.getPixel(x, y).a;
            }

        protected:
            JG::Image roundImage;
        };

        int EditorCanvasPanel::nAdded = 0;

        // ToolPickerPanel
        class ToolPickerPanel : public VistaPanel
        {
            class NextTool : public VistaButton
            {
            public:
                NextTool(JG::Window* window, int beginX, int beginY, int width) :
                    VistaButton(window, beginX, beginY, width)
                {
                    caption = "Next tool";
                }

                virtual JG::Widget::HandlerResponce onClick(JG::Event event) override
                {
                    toolPicker->nextTool();
                    return JG::Widget::HandlerResponce::Success;
                }
            };

            struct ThicknessSlider : public VistaSlider
            {
                ThicknessSlider(JG::Window* window, int beginX, int beginY, int width, float to = 100, float from = 0) : 
                    VistaSlider(window, beginX, beginY, width, to, from)
                {
                    
                }

                virtual void onMove(float dx)
                {
                    toolPicker->setThickness((int)position);
                }
            };

        public:
            ToolPickerPanel(JG::Window* window) : 
                VistaPanel(window, VistaPanel::vistaOutline, (windowHeight - editorHeight) / 2, switchWidth, switchHeight)
            {
                // addChild(new AddEditor(window, switchSpace, switchSpace,
                //                                  buttonWidth));

                addChild(new NextTool(window, switchSpace, switchSpace,
                                                      buttonWidth));

                addChild(new ThicknessSlider(window, switchSpace, 2 * switchSpace + VistaButton::buttonHeight,
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
                char const* prefix = "Tool switcher [", 
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

        class TestPanel : public VistaPanel
        {
        public:
            TestPanel(JG::Window* window) :
                VistaPanel(window, 0, 0, 2 * space + Spline::fullSize, switchHeight + Spline::fullSize + space)
            {
                addChild(new VistaTextBox(window, space, space, width - 2 * space, "papa u vani silen v matematike"));
                addChild(new Spline(window, space, switchHeight));
            }
        };

        class PalettePanel : public VistaPanel
        {
        public:
            PalettePanel(JG::Window* window) :
                VistaPanel(window, VistaPanel::vistaOutline, windowHeight -
                                                             paletteHeight - 
                                                             VistaPanel::vistaOutline - 
                                                             VistaPanel::VistaPanelBar::vistaBarHeight, 
                                                             paletteWidth, paletteHeight)
            {
                caption = "Palette";
                addChild(new Palette(window, 0, 0, paletteWidth, paletteHeight, toolPicker));
            }
        };

        class CurvesPanel : public VistaPanel
        {
        public:

            struct ApplyButton : public VistaButton
            {
                ApplyButton(JG::Window* window, int beginX, int beginY, int width, CurvesPanel* panel) :
                    VistaButton(window, beginX, beginY, width),
                    panel(panel)
                {
                    caption = "Apply";
                }

                virtual JG::Widget::HandlerResponce onClick(JG::Event event) override
                {
                    panel->layer->onCorrectionEnd();
                    panel->closePanel();
                    return JG::Widget::HandlerResponce::Success;
                }

            protected:
                CurvesPanel* panel;
            };

            CurvesPanel(JG::Window* window, Layer* layer, EditorCanvas* editorCanvas) :
                VistaPanel(window, space, space, 2 * space + Spline::fullSize, space + Spline::fullSize + space + VistaButton::buttonHeight + space),
                layer(layer),
                editorCanvas(editorCanvas),
                spline(new Spline (window, space, space)),
                curves(layer, spline->getPlot())
            {
                caption = "Curves";
                addChild(spline);
                addChild(new ApplyButton(window, space, space + Spline::fullSize + space, width - 2 * space, this));
                layer->onCorrectionBegin();
            }

            virtual JG::Widget::HandlerResponce onTimer(JG::Event event) override
            {
                static int lastUpdate = 0;
                if ((event.timer.timeMs - lastUpdate) > updateIntervalMs)
                {
                    lastUpdate = event.timer.timeMs;
                    curves.apply();
                    editorCanvas->editor->mix(editorCanvas->layerSwitcher->getLayerVector());
                }

                return JG::Widget::HandlerResponce::SuccessYield;
            }

            virtual JG::Widget::HandlerResponce onDelete(JG::Event event) override
            {
                layer->onCorrectionReset();
                editorCanvas->editor->mix(editorCanvas->layerSwitcher->getLayerVector());
                return JG::Widget::HandlerResponce::Success;
            }

        protected:
            friend ApplyButton;
            int const updateIntervalMs = 40;
            Layer* layer;
            EditorCanvas* editorCanvas;
            Spline* spline;
            JIMP::Curves curves;
        };

        struct MainWindowMenuStrip : public VistaMenuStrip
        {
            struct ExitItem : public VistaMenuItem
            {
                ExitItem(JG::Window* window) :
                    VistaMenuItem(window, "Exit")
                {

                }

                virtual JG::Widget::HandlerResponce onClick(JG::Event event) override
                {
                    debugMessage("Exit...");
                    window->sendEvent(JG::Event::CloseEvent());
                    return JG::Widget::HandlerResponce::Success;
                }
            };

            struct PaletteItem : public VistaMenuItem
            {
                PaletteItem(JG::Window* window) :
                    VistaMenuItem(window, "Palette")
                {

                }

                virtual JG::Widget::HandlerResponce onClick(JG::Event event) override
                {
                    window->addChild(new PalettePanel(window));
                    return JG::Widget::HandlerResponce::Success;
                }
            };

            struct SwitcherItem : public VistaMenuItem
            {
                SwitcherItem(JG::Window* window) :
                    VistaMenuItem(window, "Tool switcher")
                {

                }

                virtual JG::Widget::HandlerResponce onClick(JG::Event event) override
                {
                    window->addChild(new ToolPickerPanel(window));
                    return JG::Widget::HandlerResponce::Success;
                }
            };

            struct CanvasItem : public VistaMenuItem
            {
                CanvasItem(JG::Window* window) :
                    VistaMenuItem(window, "Empty canvas")
                {

                }

                virtual JG::Widget::HandlerResponce onClick(JG::Event event) override
                {
                    window->addChild(new EditorCanvasPanel(window));
                    return JG::Widget::HandlerResponce::Success;
                }
            };

            struct CurvesItem : public VistaMenuItem
            {
                CurvesItem(JG::Window* window) :
                    VistaMenuItem(window, "Curves")
                {
                    
                }

                virtual JG::Widget::HandlerResponce onClick(JG::Event event) override
                {
                    if (activeCanvas)
                    {
                        window->addChild(new CurvesPanel(window, activeCanvas->layerSwitcher->getCurrentLayer(),
                                                                 activeCanvas));
                    }
                    return JG::Widget::HandlerResponce::Success;
                }
            };

            struct TestItem : public VistaMenuItem
            {
                TestItem(JG::Window* window) :
                    VistaMenuItem(window, "Test panel")
                {

                }

                virtual JG::Widget::HandlerResponce onClick(JG::Event event) override
                {
                    window->addChild(new TestPanel(window));
                    return JG::Widget::HandlerResponce::Success;
                }
            };


            MainWindowMenuStrip(JG::Window* window) :
                VistaMenuStrip(window)
            {
                VistaMenuButton* file = new VistaMenuButton(window, this, "File");
                VistaMenuButton* panels = new VistaMenuButton(window, this, "Panels");
                VistaMenuButton* correction = new VistaMenuButton(window, this, "Correction");
                addChild(file);
                addChild(panels);
                addChild(correction);

                VistaMenu* fileMenu = new VistaMenu(window, file);
                fileMenu->addChild(new ExitItem(window));
                file->setMenu(fileMenu);

                VistaMenu* panelsMenu = new VistaMenu(window, panels);
                panelsMenu->addChild(new PaletteItem(window));
                panelsMenu->addChild(new SwitcherItem(window));
                panelsMenu->addChild(new CanvasItem(window));
                panelsMenu->addChild(new TestItem(window));
                panels->setMenu(panelsMenu);

                VistaMenu* correctionMenu = new VistaMenu(window, correction);
                correctionMenu->addChild(new CurvesItem(window));
                correction->setMenu(correctionMenu);
            }
        };

        EditorCanvasPanel* mainEditorCanvasPanel = nullptr;

        class MainWindow : public JG::Window
        {
        public:
            MainWindow(int sizeX, int sizeY) : JG::Window(sizeX, sizeY, "JIMP", JG::Window::Style::Default)
            {
                beginDrawing();
                endDrawing();

                toolPicker = new ToolPicker;
                toolPicker->insert(new Brush);
                toolPicker->insert(new Eraser);
                toolPicker->insert(new Fill);
                toolPicker->insert(new Drag);

                addChild(mainEditorCanvasPanel = new EditorCanvasPanel(this, filename));

                addChild(new ToolPickerPanel(this));
                addChild(new PalettePanel(this));

                // addChild(new TestPanel(this));

                addChild(new MainWindowMenuStrip(this));

                // addChild(new Bublick(this, 0, 0));
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