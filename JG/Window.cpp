/*++

Copyright (c) 2021 JulesIMF, MIPT

Module Name:

    Window.cpp

Abstract:

    An abstract JG for a graphics library.

Author / Creation date:

    JulesIMF / 15.09.21

Revision History:
	12.10.21  14:41		Events & Canvas
    15.09.21            SFML

--*/

//
// Includes / usings
//

#include <Window.h>
#include <X11/Xlib.h>

//
// Defines
//

#ifdef SFML_WRAPPER

// TODO: сделать как снизу

JG::Window::Window(int sizeX, int sizeY, char const* title, int style) : 
    Widget(this, 0, 0, sizeX, sizeY)
{
    createEventPicker();
    beginDrawing();
    renderMyself(0, 0);
    endDrawing();
    beginDrawing();
    startTimer = clock.now();
    window_.create({ (unsigned)sizeX, (unsigned)sizeY }, title, style);
}

JG::Window::~Window()
{
    destroyEventPicker();
}

void JG::Window::beginDrawing(JG::Color color)
{
    window_.clear(color);
}


void JG::Window::endDrawing()
{
    window_.display();
}

void JG::Window::close()
{
    window_.close();
}

void JG::Window::createEventPicker()
{
    isAlive = true;
    // eventPickerThread = new std::thread(eventPicker, this);
}

void JG::Window::destroyEventPicker()
{
    isAlive = false;
    // eventPickerThread->join();
   //  delete eventPickerThread;
}

void JG::Window::eventPicker(JG::Window& window)
{
    while (window.isAlive)
    {
        long long tickMs = std::chrono::duration_cast<std::chrono::milliseconds>(window.clock.now() - window.startTimer).count();
        if (tickMs - window.lastTickMs >= window.timerTickIntervalMs)
        {
            JG::Event eventTimer;
            eventTimer.type = JG::Event::Timer;
            eventTimer.timer = { .timeMs = tickMs,
                                 .passedMs = tickMs - window.lastTickMs };

            window.sendEvent(eventTimer);
            window.lastTickMs = tickMs;
        }

        sf::Event sfEvent = {};
        bool pulled = window.window_.pollEvent(sfEvent);

        if (!pulled)
            // continue;
            return;

        JG::Event event;
        event.window = &window;

        switch (sfEvent.type)
        {
        case sf::Event::Closed:
            event.type = Event::Close;
            window.onClose(event);
            break;

        case sf::Event::KeyPressed:
            event.type = JG::Event::KeyPressed;
            event.key = {
                .code       = (JG::Keyboard::Key)sfEvent.key.code,
                .alt        = sfEvent.key.alt,
                .control    = sfEvent.key.control,
                .shift      = sfEvent.key.shift,
                .system     = sfEvent.key.system
            };
            break;

        case sf::Event::MouseWheelScrolled:
            event.type = JG::Event::MouseScrolled;
            event.mouseWheel = { sfEvent.mouseWheelScroll.delta, sfEvent.mouseWheelScroll.x, sfEvent.mouseWheelScroll.y };
            break;

        case sf::Event::MouseMoved:
            event.type = JG::Event::MouseMoved;
            event.mouseMove = { sfEvent.mouseMove.x, sfEvent.mouseMove.y, 0, 0 };
            // window->onMouseMoved(event);
            break;

        case sf::Event::MouseButtonPressed:
            event.type = JG::Event::MouseButtonPressed;
            event.mouseButton = { .button = (JG::Mouse::Button)sfEvent.mouseButton.button,
                                  .x = sfEvent.mouseButton.x, .y = sfEvent.mouseButton.y };
            break;

        case sf::Event::MouseButtonReleased:
            event.type = Event::MouseButtonReleased;
            event.mouseButton = { .button = (JG::Mouse::Button)sfEvent.mouseButton.button,
                                  .x = sfEvent.mouseButton.x, .y = sfEvent.mouseButton.y };
            break;

        case sf::Event::MouseEntered:
            event.type = JG::Event::MouseEntered;
            event.mouseMove = { sfEvent.mouseMove.x, sfEvent.mouseMove.y, 0, 0 };
            break;

        case sf::Event::MouseLeft:
            event.type = JG::Event::MouseLeft;
            event.mouseMove = { sfEvent.mouseMove.x, sfEvent.mouseMove.y, 0, 0 };
            break;

        default:
            event.type = JG::Event::Other;
            break;
        }

        window.sendEvent(event);
    }
}

void JG::Window::sendEvent(Event event)
{
    if (hasPaintEvent && event.type == Event::Paint)
        return;

    if (event.type == Event::Paint)
        hasPaintEvent = true;
        
    eventQueue.push(event);
}

bool JG::Window::pollEvent(JG::Event& event)
{
    JG::Window::eventPicker(*this);
    
    if (eventQueue.empty())
        return false;

    event = eventQueue.front();
    eventQueue.pop();

    switch (event.type)
    {
    case Event::Close:
        window->onClose(event);
        break;

    case Event::Paint:
        window->onPaint(event);
        break;

    case Event::KeyPressed:
        window->onKeyPressed(event);
        break;

    case Event::MouseMoved:
        event.mouseMove.dx = event.mouseMove.x - mouse.x;
        event.mouseMove.dy = event.mouseMove.y - mouse.y;
        mouse.x = event.mouseMove.x;
        mouse.y = event.mouseMove.y;
        window->onMouseMoved(event);
        break;

    case Event::MouseButtonPressed:
        window->onMouseButtonPressed(event);
        break;

    case Event::MouseButtonReleased:
        window->onMouseButtonReleased(event);
        break;

    case Event::MouseEntered:
        window->onMouseEntered(event);
        break;

    case Event::MouseLeft:
        window->onMouseLeft(event);
        break;

    case Event::MouseScrolled:
        window->onMouseScrolled(event);
        break;

    case Event::Timer:
        window->onTimer(event);
        break;

    default:
        event.type = JG::Event::Other;
        break;
    }

    return true;
}

void JG::Window::popActive()
{
    JG::Widget::popActive();
    sendEvent(JG::Event::PaintEvent());
}

#pragma GCC diagnostic ignored "-Wunused-parameter"
JG::Widget::HandlerResponce JG::Window::onPaint(Event event)
{
    render(0, 0);

    hasPaintEvent = false;

    endDrawing();
    beginDrawing();
    return JG::Widget::HandlerResponce::SuccessCapture;
}

#endif //!SFML_WRAPPER