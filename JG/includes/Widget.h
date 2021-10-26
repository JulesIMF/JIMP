/*++

Copyright (c) 2021 JulesIMF, MIPT

Module Name:

    Widget.h

Abstract:

    Widget class defenition

Author / Creation date:

    JulesIMF / 06.10.21

Revision History:

--*/


//
// Includes / usings
//

#ifndef VIEW_JULESIMF
#define VIEW_JULESIMF

#include <queue>
#include <vector>
#include <__JGDebug.h>  
#include <Events.h>
#include <Shape.h>

#ifdef SFML_WRAPPER
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#endif

//
// Defines
//

namespace JG
{
    extern char const* defaultFont;
    class Window;

    enum WidgetType
    {
        WTypeRenderable = 1ll << 0,
        WTypeWindow     = 1ll << 1,
        WTypePanelBar   = 1ll << 2,
    };

    class Widget
    {
    public:
        long long type = 0;
        int beginX, beginY,
            width,  height;

        Widget(Window* window, int beginX, int beginY,
                               int width,  int height) : beginX(beginX), beginY(beginY),
                                                         width(width),   height(height),
                                                         window(window)
        {
            type |= WTypeRenderable;
        }

        virtual ~Widget();

        enum class HandlerResponce
        {
            SuccessCapture,             // Handler processed the event and does not let other widgets process it as well
            Success = SuccessCapture,   // Default success responce -- capturing the evemt
            SuccessYield,               // Handler processed the event but lets other widgets process it as well
            Failure,                    // Handler can`t process the event
        };

        /**
         * \brief Обработчик события "нажатие"
         *
         * \param event
         */
        virtual HandlerResponce onKeyPressed(Event event);

        /**
         * \brief Обработчик события "нажатие"
         *
         * \param event
         */
        virtual HandlerResponce onKeyReleased(Event event);

        /**
         * \brief Обработчик уведомления о закрытии
         *
         * \param event
         */
        virtual HandlerResponce onClose(Event event);

        /**
         * \brief Handler of "mouse clicked"
         *
         * \param event
         */
        virtual HandlerResponce onMouseButtonPressed(Event event);

        /**
         * \brief Handler of "mouse clicked"
         *
         * \param event
         */
        virtual HandlerResponce onMouseButtonReleased(Event event);

        virtual HandlerResponce onMouseMoved(Event event);

        /**
         * \brief Handler of "mouse entered"
         *
         * \param event
         */
        virtual HandlerResponce onMouseEntered(Event event);

        /**
        *  \brief Handler of "mouse left"
        *
        *  \param event
        */
        virtual HandlerResponce onMouseLeft(Event event);

        virtual void renderMyself(int shiftX, int shiftY);
        virtual void render(int shiftX, int shiftY);

        virtual void addChild(Widget* Widget);
        virtual void setActive(Widget* Widget);
        virtual void popActive();
        virtual void pushToEraseQueue(Widget* Widget);
        virtual void clearEraseQueue();

    protected:
        std::vector<Widget*> children;
        std::queue<Widget*> deletedChildren;
        Widget* activeChild = nullptr;
        bool mouseOn = false;
        bool mousePressed = false;
        Window* window;
    };

    struct Button : public Widget
    {
    public:
        virtual HandlerResponce onMouseButtonPressed(Event event) override final;
        virtual HandlerResponce onMouseButtonReleased(Event event) override final;
        virtual HandlerResponce onMouseEntered(Event event) override final;
        virtual HandlerResponce onMouseLeft(Event event) override final;
        virtual void renderMyself(int shiftX, int shiftY) override;

        virtual HandlerResponce onClick(Event event);
        virtual HandlerResponce onRightClick(Event event);


        Button(Window* window,
            int beginX, int beginY,
            int width, int height,
            Color color = { 114, 111, 143 });

        void setCaption(char const* caption);
        char const* getCaption();
        void setRectangle(Rectangle rectangle);
        Rectangle getRectangle();

    protected:
        char const* caption = nullptr;
        Rectangle defaultRectangle; // Rectangle that is rendered if not clicked and not hovered
        Rectangle currentRectangle; // Rectangle that is rendered right now
        void setCurrentRectangle();
        void validate();
    };

    struct Canvas : public Widget
    {
        Canvas(Window* window,
            int beginX, int beginY,
            int width, int height);

        virtual ~Canvas();
        virtual void renderMyself(int shiftX, int shiftY) override;

        void flush();
        void setPixel(int x, int y, Color const& color)
        {
            if (x < 0 || x >= (int)width ||
                y < 0 || y >= (int)height)
            {
                return;
            }

            bltCanvas[x][y].color = color;
        }

        Color getPixel(int x, int y) { return bltCanvas[x][y].color; }

        void transferFromBuffer(Color** buffer);

#ifdef SFML_WRAPPER
    protected:
        sf::Vertex** bltRender,      // this blt should be displayed now
                  ** bltCanvas;      // in this blt we draw now

        int lastBeginX, lastBeginY;
#endif
    };

    struct Panel : public Widget
    {
        static const int outline = 3;
        Panel(Window* window, int beginX, int beginY,
                              int width,  int height);

        struct PanelBar : public Widget
        {
            static const int barHeight = 20;

            struct PanelCloseButton : public Button
            {
                Panel* panel;
                PanelCloseButton(Window* window, Panel* panel, int beginX, int beginY);

                virtual void renderMyself(int shiftX, int shiftY) override;
                virtual HandlerResponce onClick(Event event) override;
            };

            PanelBar(Window* window, Panel* panel, int beginX, int beginY, int width);
            virtual HandlerResponce onMouseMoved(Event event) override;
            virtual void renderMyself(int shiftX, int shiftY) override;
            Panel* panel;
        };

        virtual void addChild(Widget* widget) override;
        virtual void renderMyself(int shiftX, int shiftY) override;
        char const* caption = nullptr;

    protected:
        void closePanel();
        friend class PanelBar::PanelCloseButton;
        PanelBar* panelBar;
        bool open = false;
    };

}

#endif 