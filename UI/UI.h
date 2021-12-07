/*++

Copyright (c) 2021 JulesIMF, MIPT

Module Name:

    UI.h

Abstract:

    UI is a header that describes JIMP UI.

Author / Creation date:

    JulesIMF / 18.10.21

Revision History:

--*/


//
// Includes / usings
//

#include <dirent.h> 
#include <JG.h>
#include <editor/Palette.h>
#include <editor/Editor.h>
#include <editor/LayerSwitcher.h>
#include <editor/Curves.h>
#include <plugins/sdk.h>
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

        static int const buttonWidth  = 200,  buttonHeight  = VistaButton::buttonHeight,
                                              switchButtonSize = 32;
        static int const switchNY     = 2,    switchSpace   = 20,
                         switchNX     = 4;

        static int const switchWidth  = (switchSpace + switchButtonSize) * switchNX + switchSpace,
                         switchHeight = switchSpace + 
                                        (switchSpace + switchButtonSize) * switchNY + + VistaSlider::VistaThumb::thumbHeight +
                                        switchSpace;

        static int const layerPreviewWidth = 200, layerPreviewHeight = 175,
                         layerButtonSize = 50;

        char const* const pluginsDirName = "./soplugins";

        std::vector<::JIMP::Plugins::Plugin> loadedPlugins;

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

            enum onWhat
            {
                eventOnPress,
                eventOnMove,
                eventOnRelease,
                eventOnTimer,
            };

            void applyTool(int x, int y, int dx, int dy, onWhat what, int passedMs = 0) // true - applyOnMove, false - applyOnRelease
            {
                if (what != eventOnTimer)
                    moveTool(x, y, dx, dy);

                auto tool = toolPicker->getTool();
                auto layer = layerSwitcher->getCurrentLayer();

                switch(what)
                {
                case eventOnMove:
                    tool->applyOnMove(*layer);
                    break;
                
                case eventOnPress:
                    tool->applyOnPress(*layer);
                    break;

                case eventOnRelease:
                    tool->applyOnRelease(*layer);
                    break;

                case eventOnTimer:
                    tool->applyOnTimer(*layer, passedMs);
                    break;
                }

                editor->mix(layerSwitcher->getLayerVector());
                window->sendEvent(JG::Event::PaintEvent());                
            }

            virtual JG::Widget::HandlerResponce onMouseButtonPressed(JG::Event event) override
            {
                applyTool(event.mouseButton.x, event.mouseButton.y, event.mouseMove.dx, event.mouseMove.dy, eventOnPress);
                return JG::Widget::HandlerResponce::Success;
            }

            virtual JG::Widget::HandlerResponce onMouseButtonReleased(JG::Event event) override
            {
                applyTool(event.mouseButton.x, event.mouseButton.y, event.mouseMove.dx, event.mouseMove.dy, eventOnRelease);
                return JG::Widget::HandlerResponce::Success;
            }

            virtual JG::Widget::HandlerResponce onMouseMoved(JG::Event event) override
            {
                if (mousePressed)                
                    applyTool(event.mouseMove.x, event.mouseMove.y, event.mouseMove.dx, event.mouseMove.dy, eventOnMove);
                else
                    moveTool(event.mouseMove.x, event.mouseMove.y, event.mouseMove.dx, event.mouseMove.dy);

                return JG::Widget::HandlerResponce::Success;
            }

            virtual JG::Widget::HandlerResponce onTimer(JG::Event event) override
            {
                if (mousePressed && toolPicker->getTool()->timerTool)
                   applyTool(0, 0, 0, 0, eventOnTimer, event.timer.passedMs);
                
                return JG::Widget::HandlerResponce::SuccessYield;
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

                bool openedFile = false;

                if (fileName)
                {
                    FILE* file = fopen(fileName, "r");
                    if (!file)
                    {
                        errorMessage("Wrong file \"%s\"", fileName);
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
                            openedFile = true;
                        }
                    }
                }

                if (!openedFile) 
                {
                    editorCanvas->layerSwitcher->addLayer(JIMP::UI::editorWidth, JIMP::UI::editorHeight);
                    editorCanvas->layerSwitcher->nextLayer();
                }

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

            class ToolSwitch : public JG::Button
            {
            public:
                ToolSwitch(JG::Window* window, int beginX, int beginY, char const* name, char const* imageName):
                    JG::Button(window, beginX, beginY, switchButtonSize, switchButtonSize),
                    image(0, 0, switchButtonSize, switchButtonSize, imageName),
                    toolName(name)
                {

                }

                virtual void renderMyself(int shiftX, int shiftY) override
                {
                    int moveTexture = 0;

                    if (mouseOn)
                        moveTexture += height;

                    if (mousePressed)
                        moveTexture += height;

                    translateAndDraw(image, window, beginX + shiftX, beginY + shiftY, 0, moveTexture);
                }

                virtual JG::Widget::HandlerResponce onClick(JG::Event) override
                {
                    toolPicker->selectByName(toolName);
                    return JG::Widget::HandlerResponce::Success;
                }

            protected:
                JG::Image image;
                char const* toolName;
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
                int y = switchSpace;
                int x = -switchButtonSize;

                addChild(new ToolSwitch(window, x += (switchButtonSize + switchSpace), y, "Brush", "resources/images/ToolsSelectors/Brush.png"));
                addChild(new ToolSwitch(window, x += (switchButtonSize + switchSpace), y, "Pen", "resources/images/ToolsSelectors/Pen.png"));
                addChild(new ToolSwitch(window, x += (switchButtonSize + switchSpace), y, "Airbrush", "resources/images/ToolsSelectors/Airbrush.png"));
                addChild(new ToolSwitch(window, x += (switchButtonSize + switchSpace), y, "Fill", "resources/images/ToolsSelectors/Fill.png"));
                y = switchSpace * 2 + switchButtonSize;
                x = -switchButtonSize;

                addChild(new ToolSwitch(window, x += (switchButtonSize + switchSpace), y, "Eraser", "resources/images/ToolsSelectors/Eraser.png"));
                addChild(new ToolSwitch(window, x += (switchButtonSize + switchSpace), y, "Rectangle", "resources/images/ToolsSelectors/Rectangle.png"));
                addChild(new ToolSwitch(window, x += (switchButtonSize + switchSpace), y, "Eyedropper", "resources/images/ToolsSelectors/Eyedropper.png"));
                addChild(new ToolSwitch(window, x += (switchButtonSize + switchSpace), y, "Drag", "resources/images/ToolsSelectors/Drag.png"));

                addChild(new ThicknessSlider(window, switchSpace, 3 * switchSpace + 2 * switchButtonSize,
                                                     width - 2 * switchSpace));

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
                char const* prefix = "[", 
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

        struct LayerSwitcherPanel : public VistaPanel
        {
            class LayerSwitch : public VistaButton
            {
            public:
                LayerSwitch(JG::Window* window, int beginX, int beginY, bool next) :
                    VistaButton(window, beginX, beginY, layerButtonSize),
                    next(next)
                {
                    caption = next ? "Next" : "Prev";
                }

                virtual JG::Widget::HandlerResponce onClick(JG::Event) override
                {
                    if (!activeCanvas)
                        return JG::Widget::HandlerResponce::Success;

                    if (next)
                        activeCanvas->layerSwitcher->nextLayer();
                    
                    else
                        activeCanvas->layerSwitcher->prevLayer();                        

                    return JG::Widget::HandlerResponce::Success;
                }

            protected:
                bool next;
            };

            class LayerCreate : public VistaButton
            {
            public:
                LayerCreate(JG::Window* window, int beginX, int beginY, bool create) :
                    VistaButton(window, beginX, beginY, layerButtonSize),
                    create(create)
                {
                    caption = create ? "New" : "Delete";
                }

                virtual JG::Widget::HandlerResponce onClick(JG::Event) override
                {
                    if (!activeCanvas)
                        return JG::Widget::HandlerResponce::Success;

                    if (create)
                    {
                        auto newLayer = activeCanvas->layerSwitcher->addLayer(editorHeight, editorWidth);
                        activeCanvas->layerSwitcher->setLayer(newLayer);
                    }

                    else
                    {
                        activeCanvas->layerSwitcher->removeLayer(activeCanvas->layerSwitcher->getCurrentLayerIndex());
                    }

                    return JG::Widget::HandlerResponce::Success;
                }

            protected:
                bool create;
            };

            LayerSwitcherPanel(JG::Window* window) : 
                VistaPanel(window, windowWidth - (switchSpace * 3 + layerPreviewWidth + layerButtonSize) -
                                                             VistaPanel::vistaOutline,
                                                             windowHeight -
                                                             (switchSpace * 2 + layerPreviewHeight) - 
                                                             VistaPanel::vistaOutline - 
                                                             VistaPanel::VistaPanelBar::vistaBarHeight, 
                                   switchSpace * 3 + layerPreviewWidth + layerButtonSize,
                                   switchSpace * 2 + layerPreviewHeight)
            {
                addChild(previewCanvas = new JG::Canvas(window, switchSpace, switchSpace, layerPreviewWidth, layerPreviewHeight));
                addChild(new LayerSwitch(window, switchSpace * 2 + layerPreviewWidth, switchSpace + (switchSpace + buttonHeight) * 0, true));
                addChild(new LayerSwitch(window, switchSpace * 2 + layerPreviewWidth, switchSpace + (switchSpace + buttonHeight) * 1, false));
                addChild(new LayerCreate(window, switchSpace * 2 + layerPreviewWidth, switchSpace + (switchSpace + buttonHeight) * 2, true));
                addChild(new LayerCreate(window, switchSpace * 2 + layerPreviewWidth, switchSpace + (switchSpace + buttonHeight) * 3, false));
            }

            virtual void renderMyself(int shiftX, int shiftY) override
            {
                if (activeCanvas)
                {
                    auto layer = activeCanvas->layerSwitcher->getCurrentLayer();
                    double scaleX = double(layer->width) / double(layerPreviewWidth);
                    double scaleY = double(layer->height) / double(layerPreviewHeight);

                    auto beginX = layer->beginX,
                         beginY = layer->beginY;

                    auto width  = layer->width,
                         height = layer->height;

                    for (int x = 0; x != layerPreviewWidth; x++)
                        for (int y = 0; y != layerPreviewHeight; y++)
                        {
                            JG::Color back = (((x >> 2) + (y >> 2)) & 1) ?
                                JG::Color{ 255, 255, 255, 255 } : // white
                                JG::Color{ 128, 128, 128, 255 };  // gray
                            
                            auto virtualX = int(double(x) * scaleX) - beginX;
                            auto virtualY = int(double(y) * scaleY) - beginY;
                            if (0 <= virtualX && virtualX < width && 
                                0 <= virtualY && virtualY < height)
                            {
                                back = mixColors(back, layer->preview[virtualX][virtualY]);
                            }

                            previewCanvas->setPixel(x, y, back);
                        }
                }

                else
                {
                    for (int x = 0; x != layerPreviewWidth; x++)
                        for (int y = 0; y != layerPreviewHeight; y++)
                            previewCanvas->setPixel(x, y, { 255, 255, 255, 255 });
                }

                previewCanvas->flush();

                setCaption();

                VistaPanel::renderMyself(shiftX, shiftY);
            }

            protected:
                JG::Canvas* previewCanvas;
                void setCaption()
                {
                    free((void*)caption);
                    
                    static char buffer[256];

                    if (activeCanvas)
                        snprintf(buffer, 255, "Layer switcher [%d/%d]", activeCanvas->layerSwitcher->getCurrentLayerIndex() + 1,
                                                                        activeCanvas->layerSwitcher->nLayers());

                    else
                        snprintf(buffer, 255, "Layer switcher [none]");

                    int size = strlen(buffer) + 1;

                    char* captionConcat = (char*)calloc(size, sizeof(char));
                    strcpy(captionConcat, buffer);

                    caption = captionConcat;
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
                curves(layer, spline->getPlot(Spline::R), spline->getPlot(Spline::G), spline->getPlot(Spline::B))
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

        class EffectPanel : public VistaPanel
        {
        public:
            struct ApplyButton : public VistaButton
            {
                ApplyButton(JG::Window* window, int beginX, int beginY, int width, EffectPanel* panel, EditorCanvas* editorCanvas) :
                    VistaButton(window, beginX, beginY, width),
                    panel(panel),
                    editorCanvas(editorCanvas)
                {
                    caption = "Apply";
                }

                virtual JG::Widget::HandlerResponce onClick(JG::Event event) override
                {
                    panel->effect.apply();
                    editorCanvas->editor->mix(editorCanvas->layerSwitcher->getLayerVector());
                    panel->applied = true;
                    panel->closePanel();
                    return JG::Widget::HandlerResponce::Success;
                }

            protected:
                EffectPanel* panel;
                EditorCanvas* editorCanvas;
            };

            EffectPanel(JG::Window* window, Layer* layer, EditorCanvas* editorCanvas, ::JIMP::Plugins::PluginEffect effect, char const* name) :
                VistaPanel(window, space, space, 2 * space + Spline::fullSize, space + VistaButton::buttonHeight + space),
                effect(effect),
                editorCanvas(editorCanvas)
            {
                caption = name;
                addChild(new ApplyButton(window, space, space, width - 2 * space, this, editorCanvas));
                this->effect.start(*layer);
            }

            virtual JG::Widget::HandlerResponce onTimer(JG::Event event) override
            {
                static int lastUpdate = 0;
                if ((event.timer.timeMs - lastUpdate) > updateIntervalMs)
                {
                    lastUpdate = event.timer.timeMs;
                    effect.update();
                    editorCanvas->editor->mix(editorCanvas->layerSwitcher->getLayerVector());
                }

                return JG::Widget::HandlerResponce::SuccessYield;
            }

            virtual JG::Widget::HandlerResponce onDelete(JG::Event event) override
            {
                if (!applied) effect.reset();
                editorCanvas->editor->mix(editorCanvas->layerSwitcher->getLayerVector());
                return JG::Widget::HandlerResponce::Success;
            }

        protected:
            ::JIMP::Plugins::PluginEffect effect;
            EditorCanvas* editorCanvas;
            bool applied = false;
            int const updateIntervalMs = 40;
        };

        struct OpenBmpPanel : public VistaPanel
        {
            static int const bmpPanelWidth = 300;

            struct OpenButton : public VistaButton
            {
                OpenButton(JG::Window* window, int beginX, int beginY, int width, OpenBmpPanel* panel) :
                    VistaButton(window, beginX, beginY, width),
                    panel(panel)
                {
                    caption = "Open by name";
                }

                virtual JG::Widget::HandlerResponce onClick(JG::Event event) override
                {
                    window->addChild(new EditorCanvasPanel(window, panel->textBox->getText()));
                    panel->closePanel();
                    return JG::Widget::HandlerResponce::Success;
                }

            protected:
                OpenBmpPanel* panel;
            };

            OpenBmpPanel(JG::Window* window, int width = bmpPanelWidth + 2 * space, int height = space * 3 + OpenButton::buttonHeight + VistaTextBox::tbHeight) :
                VistaPanel(window,
                           (windowWidth - width) / 2, 
                           (windowHeight - height) / 2,
                           width,
                           height)
                {
                    caption = "Open BMP file";
                    addChild(textBox = new VistaTextBox(window, space, space, width - 2 * space));
                    addChild(button = new OpenButton(window, space, space * 2 + VistaTextBox::tbHeight, width - 2 * space, this));
                }

            OpenButton* button;
            VistaTextBox* textBox;
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

            struct OpenBmpItem : public VistaMenuItem
            {
                OpenBmpItem(JG::Window* window) :
                    VistaMenuItem(window, "Open BMP file")
                {

                }

                virtual JG::Widget::HandlerResponce onClick(JG::Event event) override
                {
                    window->addChild(new OpenBmpPanel(window));
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

            struct LayerItem : public VistaMenuItem
            {
                LayerItem(JG::Window* window) :
                    VistaMenuItem(window, "Layer switcher")
                {

                }

                virtual JG::Widget::HandlerResponce onClick(JG::Event event) override
                {
                    window->addChild(new LayerSwitcherPanel(window));
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

            struct PluginToolItem : public VistaMenuItem
            {
                PluginToolItem(JG::Window* window, std::string const& name) :
                    VistaMenuItem(window, name.c_str()),
                    name(name)
                {
                    caption = this->name.c_str();
                }

                virtual JG::Widget::HandlerResponce onClick(JG::Event event) override
                {
                    toolPicker->selectByName(name.c_str());
                    return JG::Widget::HandlerResponce::Success;
                }

            protected:
                std::string const name;
            };

            struct PluginEffectItem : public VistaMenuItem
            {
                PluginEffectItem(JG::Window* window, ::JIMP::Plugins::PluginEffect effect, std::string const& name) :
                    VistaMenuItem(window, name.c_str()),
                    name(name),
                    effect(effect)
                {
                    caption = this->name.c_str();
                }

                virtual JG::Widget::HandlerResponce onClick(JG::Event event) override
                {
                    if (activeCanvas)
                    {
                        window->addChild(new EffectPanel(window, activeCanvas->layerSwitcher->getCurrentLayer(),
                                                         activeCanvas, effect, name.c_str()));
                    }

                    return JG::Widget::HandlerResponce::Success;
                }

            protected:
                std::string const name;
                ::JIMP::Plugins::PluginEffect effect;
            };

            MainWindowMenuStrip(JG::Window* window) :
                VistaMenuStrip(window)
            {
                file = new VistaMenuButton(window, this, "File");
                panels = new VistaMenuButton(window, this, "Panels");
                correction = new VistaMenuButton(window, this, "Correction");
                pluginTools = new VistaMenuButton(window, this, "Plugin tools");
                pluginEffects = new VistaMenuButton(window, this, "Plugin effects");
                addChild(file);
                addChild(panels);
                addChild(correction);
                addChild(pluginTools);
                addChild(pluginEffects);

                fileMenu = new VistaMenu(window, file);
                fileMenu->addChild(new ExitItem(window));
                fileMenu->addChild(new OpenBmpItem(window));
                file->setMenu(fileMenu);

                panelsMenu = new VistaMenu(window, panels);
                panelsMenu->addChild(new CanvasItem(window));
                panelsMenu->addChild(new SwitcherItem(window));
                panelsMenu->addChild(new LayerItem(window));
                panelsMenu->addChild(new PaletteItem(window));
                panels->setMenu(panelsMenu);

                correctionMenu = new VistaMenu(window, correction);
                correctionMenu->addChild(new CurvesItem(window));
                correction->setMenu(correctionMenu);

                toolsMenu = new VistaMenu(window, pluginTools);
                pluginTools->setMenu(toolsMenu);

                effectsMenu = new VistaMenu(window, pluginEffects);
                pluginEffects->setMenu(effectsMenu);
            }

            VistaMenuButton* file;
            VistaMenuButton* panels;
            VistaMenuButton* correction;
            VistaMenuButton* pluginTools;
            VistaMenuButton* pluginEffects;

            VistaMenu* fileMenu;
            VistaMenu* panelsMenu;
            VistaMenu* correctionMenu;
            VistaMenu* toolsMenu;
            VistaMenu* effectsMenu;
        };

        EditorCanvasPanel* mainEditorCanvasPanel = nullptr;

        bool loadPlugins(JG::Window* window,  MainWindowMenuStrip* strip)
        {
            ::JIMP::Plugins::getAppToPI()->picker = toolPicker;

            DIR* pluginsDir = nullptr;
            struct dirent* ent = nullptr;
            pluginsDir = opendir(pluginsDirName);
            if (pluginsDir)
            {
                debugMessage("starting loading plugins from \"%s\"", pluginsDirName);
                while ((ent = readdir(pluginsDir)))
                {
                    if (ent->d_type == DT_REG)
                    {
                        std::string address = "soplugins/";
                        address += ent->d_name;

                        ::JIMP::Plugins::Plugin plugin;
                        if (!plugin.loadPlugin(address.c_str()))
                            continue;
                        
                        if (plugin.type == PPT_TOOL)
                        {
                            std::string name = plugin.info->author;
                            name += " - ";
                            name += plugin.info->name;

                            toolPicker->insert(new ::JIMP::Plugins::PluginTool(plugin, name));
                            strip->toolsMenu->addChild(new MainWindowMenuStrip::PluginToolItem(window, name));

                            loadedPlugins.push_back(plugin);
                            continue;
                        }

                        if (plugin.type == PPT_EFFECT)
                        {
                            std::string name = plugin.info->author;
                            name += " - ";
                            name += plugin.info->name;

                            ::JIMP::Plugins::PluginEffect effect(plugin, name);

                            strip->effectsMenu->addChild(new MainWindowMenuStrip::PluginEffectItem(window, effect, name));

                            loadedPlugins.push_back(plugin);
                            continue;
                        }
                    }
                }

                closedir(pluginsDir);

                debugMessage("plugins loaded");
                return true;
            }

            else
            {
                warningMessage("failed to open \"%s\" to load plugins", pluginsDirName);
                return false;
            }
        }

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
                toolPicker->insert(new Pen);
                toolPicker->insert(new Fill);
                toolPicker->insert(new Airbrush);
                toolPicker->insert(new Rectangle);
                toolPicker->insert(new Drag);
                toolPicker->insert(new Eyedropper(toolPicker));

                addChild(mainEditorCanvasPanel = new EditorCanvasPanel(this, filename));

                addChild(new ToolPickerPanel(this));
                addChild(new PalettePanel(this));
                addChild(new LayerSwitcherPanel(this));

                addChild(strip = new MainWindowMenuStrip(this));
                loadPlugins(this, strip);
                timerTickIntervalMs = 50;
            }

            virtual void renderMyself(int shiftX, int shiftY)
            {
                JG::Rectangle rect(0, 0, width, height);
                rect.setColor({255, 255, 255});
                rect.draw(*this);
            }  

            MainWindowMenuStrip* strip = nullptr;
        };
    }
}