/*++

Copyright (c) 2021 JulesIMF, MIPT

Module Name:

    Widget.cpp

Abstract:

    Widget is an abstract class that handles other widgets
    within itself, manages incoming events and reacts to them.
    All widgets have a pointer to its Window for rendering and 
    adding children.

Author / Creation date:

    JulesIMF / 10.10.21

Revision History:
	11.10.21  13:13		Widget and Widget released

--*/


//
// Includes / usings
//

#include <cassert>
#include <Widget.h>
#include <Window.h>
using namespace JG;

//
// Defines
//

char const* JG::defaultFont = "JG/resources/fonts/Segoe UI.ttf";

#define PROCESS_CHILD_RESPONCE(action)                      \
switch (action)                                             \
{                                                           \
            case HandlerResponce::SuccessCapture:           \
                popActive();                                \
                applyQueueChanges();                          \
                return HandlerResponce::SuccessCapture;     \
                                                            \
                                                            \
            case HandlerResponce::SuccessYield:             \
                responce = HandlerResponce::SuccessYield;   \
                                                            \
                break;                                      \
                                                            \
            case HandlerResponce::Failure:                  \
                break;                                      \
                                                            \
            default:                                        \
                assert(!"Unprocessed responce");            \
}

Widget::HandlerResponce Widget::onClose(Event event)
{
    HandlerResponce responce = HandlerResponce::Failure;

    for (auto child : children)
    {
        PROCESS_CHILD_RESPONCE(child->onClose(event))
    }

    return responce;
}

Widget::~Widget()
{
    for (auto child : children) 
    {
        child->onDelete(Event::DeleteEvent());
        delete child;
    }
}

void Widget::addChild(Widget* child)
{
    if (child->noActive)
        children.insert(children.begin(), child);
    else
        children.push_back(child);
    window->sendEvent(Event::PaintEvent());
}

void Widget::setActive(Widget* widget)
{
    assert(widget);
    if (widget->noActive || activeChild == widget)
        return;

    if (activeChild) 
    {
        activeChild->isActive = false;
        Event event;
        event.type = Event::FocusLeft;
        activeChild->onFocusLeft(event);
    }

    activeChild = widget;
    widget->isActive = true;
    Event event;
    event.type = Event::FocusEntered;
    activeChild->onFocusEntered(event);
}

void Widget::popActive()
{    
    auto itActive = std::find(children.begin(), children.end(), activeChild);

    if (itActive == children.end())
    {
        activeChild = nullptr;
        return;
    }

    children.erase(itActive);
    children.push_back(activeChild);
}

void Widget::move(int deltaX, int deltaY)
{
    beginX += deltaX;
    beginY += deltaY;
    window->sendEvent(Event::PaintEvent());
}

void Widget::setPosition(int newX, int newY)
{
    beginX = newX;
    beginY = newY;
    window->sendEvent(Event::PaintEvent());
}

void Widget::pushToEraseQueue(Widget* widget)
{
    deletedChildren.push(widget);
}

void Widget::pushToPopQueue(Widget* widget)
{
    popChildren.push(widget);
}

void Widget::pushToAddQueue(Widget* widget)
{
    addChildren.push(widget);
}

void Widget::applyQueueChanges()
{
    while (!deletedChildren.empty())
    {
        auto itErase = std::find(children.begin(), children.end(), deletedChildren.front());

        if (itErase != children.end())
        {
            (*itErase)->onDelete(Event::DeleteEvent());
            delete *itErase;
            
            if (activeChild == *itErase) 
                activeChild = nullptr;

            children.erase(itErase);
        }
        
        deletedChildren.pop();
    }

    while (!popChildren.empty())
    {
        auto itErase = std::find(children.begin(), children.end(), popChildren.front());

        if (itErase != children.end())
        {
            if (activeChild == *itErase)
                activeChild = nullptr;

            children.erase(itErase);
        }

        popChildren.pop();
    }

    while (!addChildren.empty())
    {
        auto itErase = std::find(children.begin(), children.end(), addChildren.front());

        if (itErase == children.end())
        {
            children.push_back(addChildren.front());
        }

        addChildren.pop();
    }
}

Widget::HandlerResponce Widget::onKeyPressed(Event event)
{
    HandlerResponce responce = HandlerResponce::Failure;

    if (activeChild)
        PROCESS_CHILD_RESPONCE(activeChild->onKeyPressed(event));

    for (auto child : children)
    {
        if (child == activeChild)
            continue;

        PROCESS_CHILD_RESPONCE(child->onKeyPressed(event))
    }

    return responce;
}

Widget::HandlerResponce Widget::onKeyReleased(Event event)
{
    HandlerResponce responce = HandlerResponce::Failure;

    if (activeChild)
        PROCESS_CHILD_RESPONCE(activeChild->onKeyReleased(event));

    for (auto child : children)
    {
        if (child == activeChild)
            continue;

        PROCESS_CHILD_RESPONCE(child->onKeyReleased(event))
    }

    return responce;
}

Widget::HandlerResponce Widget::onMouseButtonPressed(Event event)
{
    mousePressed = true;
    HandlerResponce responce = HandlerResponce::Failure;

    bool activeSet = false;

    for (auto child = children.rbegin(); child != children.rend(); child++)
    {   
        Widget* renderable = *child;

        int virtualX = event.mouseButton.x - renderable->beginX,
            virtualY = event.mouseButton.y - renderable->beginY;

        if (renderable &&
            0 <= virtualX && virtualX < (int)renderable->width &&
            0 <= virtualY && virtualY < (int)renderable->height &&
            renderable->checkHover(virtualX, virtualY) &&
            !renderable->mousePressed)
        {
            if(!activeSet)
            {
                activeSet = true;
                setActive(renderable);
            }
            
            Event virtualEvent = event;
            virtualEvent.mouseButton = {.button = event.mouseButton.button,
                                        .x      = virtualX,
                                        .y      = virtualY};
            renderable->mousePressed = true;
            PROCESS_CHILD_RESPONCE(renderable->onMouseButtonPressed(virtualEvent))
        }
        
    }

    popActive();
    applyQueueChanges();
    return responce;
}

Widget::HandlerResponce Widget::onMouseButtonReleased(Event event)
{
    mousePressed = false;
    HandlerResponce responce = HandlerResponce::Failure;

    for (auto child = children.rbegin(); child != children.rend(); child++)
    {
        Widget* renderable = *child;

        int virtualX = event.mouseButton.x - renderable->beginX,
            virtualY = event.mouseButton.y - renderable->beginY;

        if (renderable &&
            renderable->mousePressed)
        {
            Event virtualEvent = event;
            virtualEvent.mouseButton = {.button = event.mouseButton.button,
                                        .x      = virtualX,
                                        .y      = virtualY};

            renderable->mousePressed = false;
            PROCESS_CHILD_RESPONCE(renderable->onMouseButtonReleased(virtualEvent))
        }
    }

    popActive();
    applyQueueChanges();
    return responce;
}

Widget::HandlerResponce Widget::onMouseScrolled(Event event)
{
    HandlerResponce responce = HandlerResponce::Failure;

    if (activeChild)
    {
        int virtualX = event.mouseWheel.x - activeChild->beginX,
            virtualY = event.mouseWheel.y - activeChild->beginY;

        Event virtualEvent = event;
        virtualEvent.mouseWheel = { .delta = event.mouseWheel.delta,
                                    .x = virtualX,
                                    .y = virtualY };

        PROCESS_CHILD_RESPONCE(activeChild->onMouseScrolled(event));
    }

    for (auto child : children)
    {
        if (child == activeChild)
            continue;

        int virtualX = event.mouseWheel.x - child->beginX,
            virtualY = event.mouseWheel.y - child->beginY;

        Event virtualEvent = event;
        virtualEvent.mouseWheel = { .delta = event.mouseWheel.delta,
                                    .x = virtualX,
                                    .y = virtualY };
        PROCESS_CHILD_RESPONCE(child->onMouseScrolled(virtualEvent))
    }

    return responce;
}

Widget::HandlerResponce Widget::onMouseMoved(Event event)
{
    HandlerResponce responce = HandlerResponce::Failure;

    for (auto child = children.rbegin(); child != children.rend(); child++)
    {
        Widget* renderable = *child;

        int virtualX = event.mouseMove.x - renderable->beginX,
            virtualY = event.mouseMove.y - renderable->beginY;

        /* MouseMoved send signals to hovered rectangles about mouse movement.
         * If it does not cover now renderable, but did, it signals about mouseLeft.
         * And vice versa.
         */

        Event virtualEvent = event;
        virtualEvent.mouseMove = { .x = virtualX,
                                   .y = virtualY,
                                   .dx = event.mouseMove.dx,
                                   .dy = event.mouseMove.dy };
        
        renderable->onMouseMoved(virtualEvent);

        if (renderable &&
            0 <= virtualX && virtualX < (int)renderable->width &&
            0 <= virtualY && virtualY < (int)renderable->height)
        {
            if (renderable->mouseOn)
                continue;
                
            renderable->mouseOn = true; // Need to do this if overrided handler does not set mouseOn
            renderable->onMouseEntered(virtualEvent);
        }

        else
        {
            if (!renderable->mouseOn)
                continue;
            
            renderable->mouseOn = false; // Need to do this if overrided handler does not set mouseOn
            renderable->onMouseLeft(virtualEvent);
        }
    }

    popActive();
    applyQueueChanges();
    return responce;
}

Widget::HandlerResponce Widget::onMouseEntered(Event event)
{
    // mouseOn = true;
    // HandlerResponce responce = HandlerResponce::Failure;

    // for (auto child = children.rbegin(); child != children.rend(); child++)
    // {
    //     Widget* renderable = *child;

    //     int virtualX = event.mouseButton.x - renderable->beginX,
    //         virtualY = event.mouseButton.y - renderable->beginY;

    //     if (renderable &&
    //         0 <= virtualX && virtualX < (int)renderable->width &&
    //         0 <= virtualY && virtualY < (int)renderable->height &&
    //         renderable->checkHover(virtualX, virtualY) &&
    //         !renderable->mouseOn)
    //     {
    //         Event virtualEvent = event;
    //         virtualEvent.mouseMove = { .x = virtualX,
    //                                    .y = virtualY,
    //                                    .dx = event.mouseMove.dx,
    //                                    .dy = event.mouseMove.dy };
    //         renderable->mouseOn = true; // Need to do this if overrided handler does not set mouseOn
    //         PROCESS_CHILD_RESPONCE(renderable->onMouseEntered(virtualEvent))
    //     }
    // }

    // popActive();
    // applyQueueChanges();
    (void)event;
    return HandlerResponce::Success;
}

Widget::HandlerResponce Widget::onMouseLeft(Event event)
{
    mouseOn = false;
    HandlerResponce responce = HandlerResponce::Failure;

    for (auto child = children.rbegin(); child != children.rend(); child++)
    {
        Widget* renderable = *child;

        int virtualX = event.mouseButton.x - renderable->beginX,
            virtualY = event.mouseButton.y - renderable->beginY;

        if (renderable &&
            0 <= virtualX && virtualX < (int)renderable->width &&
            0 <= virtualY && virtualY < (int)renderable->height &&
            renderable->checkHover(virtualX, virtualY) &&
            !renderable->mouseOn)
        {
            Event virtualEvent = event;
            virtualEvent.mouseMove = {.x = virtualX,
                                      .y = virtualY,
                                      .dx = event.mouseMove.dx,
                                      .dy = event.mouseMove.dy };

            renderable->mouseOn = false; // Need to do this if overrided handler does not set mouseOn
            PROCESS_CHILD_RESPONCE(renderable->onMouseLeft(virtualEvent))
        }
    }

    popActive();
    applyQueueChanges();
    return responce;
}

bool Widget::checkHover(int x, int y)
{
    (void)x;
    (void)y;
    return true;
}

Widget::HandlerResponce Widget::onTimer(Event event)
{
    for (auto child : children)
        child->onTimer(event);

    popActive();
    applyQueueChanges();
    return HandlerResponce::SuccessYield;
}

Widget::HandlerResponce Widget::onFocusEntered(Event event)
{
    (void)event;
    return HandlerResponce::Success;
}

Widget::HandlerResponce Widget::onFocusLeft(Event event)
{
    (void)event;
    return HandlerResponce::Success;
}

Widget::HandlerResponce Widget::onDelete(Event event)
{
    (void)event;
    return HandlerResponce::Success;
}

#pragma GCC diagnostic ignored "-Wunused-parameter"
void Widget::renderMyself(int shiftX, int shiftY)
{
    // pass by default: we have to do this and to ignore GCC
}


void Widget::render(int shiftX, int shiftY)
{
    renderMyself(shiftX, shiftY);

    for (auto child : children)
    {
        Widget* renderable = static_cast<Widget*>(child);
        renderable->render(shiftX + beginX, shiftY + beginY);
    }
}
