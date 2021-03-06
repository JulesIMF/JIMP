/*++

Copyright (c) 2021 JulesIMF, MIPT

Module Name:

    JG.h

Abstract:

    Window is a class that holds sf::Window and first
    proceeds all events. For better logic it was made
    as JG::Widget derivative: it allowes to add Widgets
    with addChild(...) and proceed events just by calling
    corresponding JG:on... with the event. But user CAN
    proceed events by himself --- once event proceeded
    (or not) by a handler (on... function), pollEvent
    returns it to the user.

    Note that user MUST constantly call pollEvent() to let JG
    proceed incoming SFML events.

Author / Creation date:

    JulesIMF / 15.09.21

Revision History:

    15.09.21        SFML

--*/

#ifndef WINDOW_JULESIMF
#define WINDOW_JULESIMF
#include <thread>
#include <queue>
#include <chrono>
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

        void beginDrawing(JG::Color color = {0, 0, 0});
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
        long long timerTickIntervalMs = 100;

    protected:
        std::queue<Event> eventQueue;
        bool hasPaintEvent = false;
        std::chrono::high_resolution_clock clock;
        std::chrono::system_clock::time_point startTimer;
        long long lastTickMs = 0;
        

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