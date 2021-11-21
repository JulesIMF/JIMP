/*++

Copyright (c) 2021 JulesIMF, MIPT

Module Name:

    VistaPanel.h

Abstract:



Author / Creation date:

    JulesIMF / 27.10.21

Revision History:

--*/

#ifndef JIMP_SPLINE
#define JIMP_SPLINE

//
// Includes / usings
//

#include <JG.h>
#include "tnd.h"
#include <common/messages.h>
#include <cassert>
#include <cmath>

//
// Defines
//

namespace JIMP
{
    namespace UI
    {
        struct Spline : public JG::Widget
        {
            static int const backSize = 256;
            static int const fullSize = 270;
            static int const gradSize = fullSize - backSize;

            struct SplinePoint
            {
                int x, y;
            };

            Spline(JG::Window* window, int beginX, int beginY) : 
                JG::Widget(window, beginX, beginY, fullSize, fullSize),
                fullImage(0, 0, fullSize, fullSize, "resources/images/Spline/back_grad.png")
            {
                points.push_back({ 0, 0 });
                points.push_back({ 134, 169 });
                points.push_back({ backSize - 1, backSize - 1 });
                recalcPlot();
            }

            virtual void renderMyself(int shiftX, int shiftY) override
            {
                translateAndDraw(fullImage, window, shiftX + beginX, shiftY + beginY);
                sf::Color const color = sf::Color::Blue;

                for (int i = 0; i != backSize - 1; i++)
                {
                    sf::Vertex line[] =
                    {
                        sf::Vertex(sf::Vector2f(shiftX + beginX + gradSize + i, shiftY + beginY + backSize - 1 - plot[i]), color),
                        sf::Vertex(sf::Vector2f(shiftX + beginX + gradSize + i + 1, shiftY + beginY + backSize - 1 - plot[i + 1]), color)
                    };

                    window->getSfWindow().draw(line, 2, sf::Lines);
                }

                for (int i = 0; i != points.size(); i++)
                {
                    auto point = points[i];

                    sf::Color pColor = { 0, 0, 160 };
                    sf::Color grabColor = { 160, 0, 0 };

                    int const pSize = 9;
                    sf::RectangleShape rect({ pSize, pSize });
                    rect.setPosition(sf::Vector2f(-(pSize + 1) / 2 + shiftX + beginX + gradSize + point.x, -(pSize + 1) / 2 + shiftY + beginY + backSize - 1 - point.y));
                    rect.setFillColor((i == grabbedPoint) ? grabColor : pColor);
                    window->getSfWindow().draw(rect);
                }
            }

            virtual JG::Widget::HandlerResponce onKeyPressed(JG::Event event)
            {
                int const smallStep = 1, bigStep = 10;
                bool shift = event.key.shift;
                int step = shift ? bigStep : smallStep;
                switch (event.key.code)
                {
                case JG::Keyboard::Delete:
                    deletePoint();
                    break;

                case JG::Keyboard::Escape:
                    releasePoint();
                    break;

                case JG::Keyboard::Down:
                    movePoint(0, -step);
                    break;

                case JG::Keyboard::Up:
                    movePoint(0, step);
                    break;

                case JG::Keyboard::Left:
                    movePoint(-step, 0);
                    break;

                case JG::Keyboard::Right:
                    movePoint(step, 0);
                    break;

                case JG::Keyboard::Tab:
                    nextPoint();
                    break;
                    
                default:
                    break;                    
                }

                return JG::Widget::HandlerResponce::Success;
            }

            virtual JG::Widget::HandlerResponce onMouseButtonPressed(JG::Event event)
            {
                grabPoint(event.mouseButton.x - gradSize, backSize - 1 - event.mouseButton.y);
                return JG::Widget::HandlerResponce::Success;
            }

            virtual JG::Widget::HandlerResponce onMouseMoved(JG::Event event)
            {
                if (mousePressed)
                    movePoint(event.mouseMove.dx, -event.mouseMove.dy);

                return JG::Widget::HandlerResponce::Success;
            }

            protected:
                std::vector<SplinePoint> points;
                int plot[backSize];
                JG::Image fullImage;

                int grabbedPoint = 1;

                void recalcPlot()
                {
                    assert(points.size() >= 2);
                    assert(points.front().x == 0);
                    assert(points.back().x == backSize - 1);

                    int index = 0;
                    for (int i = 0; i != backSize; i++)
                    {
                        assert(points[index].x != points[index + 1].x);

                        if (points[index + 1].x < i)
                            index++;
                        
                        double coef = ((double)points[index].y - (double)points[index + 1].y) / ((double)points[index].x - (double)points[index + 1].x);                        
                        plot[i] = (double)points[index].y + (int)(coef * (i - (double)points[index].x));
                    }
                }

                void deletePoint()
                {
                    if (grabbedPoint == -1 || grabbedPoint == 0 || grabbedPoint == points.size() - 1)
                        return;

                    points.erase(points.begin() + grabbedPoint);
                    // releasePoint();
                    recalcPlot();
                }

                void releasePoint()
                {
                    grabbedPoint = -1;
                }

                void movePoint(int shiftX, int shiftY)
                {
                    if (grabbedPoint == -1)
                        return;

                    points[grabbedPoint].y += shiftY;
                    points[grabbedPoint].y = std::min(points[grabbedPoint].y, backSize - 1);
                    points[grabbedPoint].y = std::max(points[grabbedPoint].y, 0);

                    if (grabbedPoint != 0 && grabbedPoint != points.size() - 1)
                    {
                        points[grabbedPoint].x += shiftX;
                        points[grabbedPoint].x = std::min(points[grabbedPoint].x, backSize - 1);
                        points[grabbedPoint].x = std::max(points[grabbedPoint].x, 0);

                        if (points[grabbedPoint].x <= points[grabbedPoint - 1].x || points[grabbedPoint].x >= points[grabbedPoint + 1].x)
                            deletePoint();
                    }

                    recalcPlot();
                }

                void grabPoint(int x, int y)
                {
                    x = std::min(x, backSize - 1);
                    x = std::max(x, 0);

                    y = std::min(y, backSize - 1);
                    y = std::max(y, 0);

                    int dist = 9;
                    if ((int)abs(y - plot[x]) > dist)
                    {
                        releasePoint();
                        return;
                    }
                    
                    int i = 0;

                    for(; i != points.size(); i++)
                    {
                        if ((int)abs(y - points[i].y) <= dist && (int)abs(x - points[i].x) <= dist)
                        {
                            grabbedPoint = i;
                            return;
                        }

                        if (points[i].x > x)
                            break;                        
                    }

                    points.insert(points.begin() + i, { x, y });
                    grabbedPoint = i;
                    recalcPlot();
                }

                void nextPoint()
                {
                    grabbedPoint = (grabbedPoint + 1) % points.size();
                }
        };
    }
}

#endif // spline