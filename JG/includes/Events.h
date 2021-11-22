/*++

Copyright (c) 2021 JulesIMF, MIPT

Module Name:

    Events.h

Abstract:

    Events are objects that are sent if something
    interest (related to JG) happens.

Author / Creation date:

    JulesIMF / 06.10.21

Revision History:

--*/


//
// Includes / usings
//

#ifndef EVENTS_JULESIMF
#define EVENTS_JULESIMF
#include <__JGDebug.h>
#include <cstring>

#ifdef SFML_WRAPPER
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#endif

#include <Device.h>

//
// Defines
//

namespace JG
{
    struct Window;

    struct Event
    {
        enum EventType
        {
            Other,
            Close,
            KeyPressed,
            KeyReleased,
            MouseScrolled,
            MouseButtonPressed,     
            MouseButtonReleased,    
            MouseMoved,             
            MouseEntered,           
            MouseLeft,
            FocusEntered,
            FocusLeft,
            Paint,
            Timer,
            Delete,
        };

        EventType type;

        struct EventClose
        {

        };

        struct EventKey
        {
            Keyboard::Key code;

            bool          alt;
            bool          control;
            bool          shift;
            bool          system;
        };

        struct EventMouseButton
        {
            enum Action
            {
                Press,
                Release,
            };

            Mouse::Button button; ///< Code of the button that has been pressed
            int           x;      ///< X position of the mouse pointer, relative to the left of the owner window
            int           y;      ///< Y position of the mouse pointer, relative to the top of the owner window
        };

        struct EventMouseMove
        {
            int           x;      ///< X position of the mouse pointer, relative to the left of the owner window
            int           y;      ///< Y position of the mouse pointer, relative to the top of the owner window
            int           dx;
            int           dy;
        };

        struct EventMouseWheel
        {
            float       delta;
            int         x;
            int         y;
        };

        struct EventTimer
        {
            long long timeMs;
            long long passedMs;
        };
        

        union
        {
            EventClose              close;
            EventKey                key;
            EventMouseButton        mouseButton;
            EventMouseMove          mouseMove;
            EventTimer              timer;
            EventMouseWheel         mouseWheel;
        };

        Window* window;

        static Event PaintEvent()  { Event event; event.type = Paint; return event; }
        static Event CloseEvent()  { Event event; event.type = Close; return event; }
        static Event DeleteEvent() { Event event; event.type = Delete; return event; }

    protected:
#ifdef SFML_WRAPPER
        sf::Event sfEvent;
#endif
    };
}

#endif // !EVENTS_JULESIMF