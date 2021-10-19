/*++

Copyright (c) 2021 JulesIMF, MIPT

Module Name:

    JG.h

Abstract:

    An abstract JG for a graphics library.

Author / Creation date:

    JulesIMF / 15.09.21

Revision History:

    15.09.21        SFML

--*/

#ifndef WINDOW_JULESIMF
#define WINDOW_JULESIMF
#include <thread>
#include <queue>
#include <__JGDebug.h>
//
// To turn SFML realization on, define the next macros:
// #define SFML_WRAPPER
//

#ifdef SFML_WRAPPER
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <Widget.h>
#endif

namespace JG
{
    struct Window : public Widget
    {
        enum Style
        {
        #ifdef SFML_WRAPPER
            None            = sf::Style::None,
            Titlebar        = sf::Style::Titlebar,
            Resize          = sf::Style::Resize,
            Close           = sf::Style::Close,
            Fullscreen      = sf::Style::Fullscreen,
            Default         = sf::Style::Default ^ sf::Style::Resize,
        #endif
        };

        Window(int sizeX, int sizeY, char const* title, int style);
        ~Window();

        void beginDrawing();
        void endDrawing();
        void close();

        bool pollEvent(Event& event);        
        void sendEvent(Event event);
        static void eventPicker(Window& window);

        virtual void addChild(Widget* Widget) override
        {
            Widget::addChild(Widget);
            window->sendEvent(Event::PaintEvent());
        }


        HandlerResponce onPaint(Event event);
        virtual void popActive();

    protected:
        std::queue<Event> eventQueue;
        bool hasPaintEvent = false;

    private:
        // Event picker thread constantly picks events from this window
        // and proceeds it -- maybe calling event handlers, maybe inserting
        // them into high-level queue; it works until 
        void createEventPicker();
        void destroyEventPicker();
        std::thread* eventPickerThread = nullptr;
        bool isAlive;


        Mouse mouse;

    #ifdef SFML_WRAPPER
        sf::RenderWindow window_;

    public:
        sf::RenderWindow& getSfWindow() { return window_; }
    #endif

    };
}

#endif // !WINDOW_JULESIMF