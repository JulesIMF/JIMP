/*++

Copyright (c) 2021 JulesIMF, MIPT

Module Name:

    VistaPanel.h

Abstract:

    JIMP::UI::Spline is a square-shaped widget
    that has 3splines: red, green and blue. 
    It contains points for each color and
    calculates plot linearly between them.

    Points are not JG::Widgets, Spline proceeds
    all events by itself.

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

            enum Chanel
            {
                None = 0,
                R = 1,
                G = 2,
                B = 3,


                Min = R,
                Lim = B + 1
            };

            struct SplinePoint
            {
                int x, y;
            };

            Spline(JG::Window* window, int beginX, int beginY) : 
                JG::Widget(window, beginX, beginY, fullSize, fullSize),
                fullImage(0, 0, fullSize, fullSize, "resources/images/Spline/back_grad.png")
            {
                for (int chanel = Min; chanel < Lim; chanel++)
                {
                    chanelToPoints[chanel]->push_back({ 0, 0 });
                    chanelToPoints[chanel]->push_back({ backSize - 1, backSize - 1 });
                    recalcPlot(*chanelToPoints[chanel], chanelToPlot[chanel]);
                }
            }

            int* getPlot(Chanel chanel)
            {   
                return chanelToPlot[chanel];
            }

            void flush()
            {
                changed = false;
            }

            bool isChanged()
            {
                return changed;
            }

            virtual void renderMyself(int shiftX, int shiftY) override
            {
                translateAndDraw(fullImage, window, shiftX + beginX, shiftY + beginY);
                sf::Color const chanelToColor[nChanels + 1] = 
                {
                    sf::Color::Black,
                    sf::Color::Red,
                    sf::Color::Green,
                    sf::Color::Blue
                };

                for (int chanel = Lim - 1; chanel >= Min; chanel--)
                {
                    auto plot = chanelToPlot[chanel];
                    auto color = chanelToColor[chanel];

                    for (int i = 0; i != backSize - 1; i++)
                    {
                        sf::Vertex line[] =
                        {
                            sf::Vertex(sf::Vector2f(shiftX + beginX + gradSize + i, shiftY + beginY + backSize - 1 - plot[i]), color),
                            sf::Vertex(sf::Vector2f(shiftX + beginX + gradSize + i + 1, shiftY + beginY + backSize - 1 - plot[i + 1]), color)
                        };

                        window->getSfWindow().draw(line, 2, sf::Lines);
                    }
                }

                sf::Color pColor = sf::Color::Black;
                sf::Color grabColor = sf::Color::Magenta;

                for (int chanel = Lim - 1; chanel >= Min; chanel--)
                {
                    auto& points = *chanelToPoints[chanel];

                    for (int i = 0; i != points.size(); i++)
                    {
                        auto point = points[i];

                        int const pSize = 9;
                        sf::RectangleShape rect({ pSize, pSize });
                        rect.setPosition(sf::Vector2f(-(pSize + 1) / 2 + shiftX + beginX + gradSize + point.x, -(pSize + 1) / 2 + shiftY + beginY + backSize - 1 - point.y));
                        rect.setFillColor((i == grabbedPoint.index) && (chanel == grabbedPoint.chanel) ? grabColor : pColor);
                        window->getSfWindow().draw(rect);
                    }
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
            bool changed = false;
            static int const nChanels = 3;

            std::vector<SplinePoint> rPoints,
                gPoints,
                bPoints;

            int rPlot[backSize],
                gPlot[backSize],
                bPlot[backSize];

            int* chanelToPlot[nChanels + 1] =
            {
                nullptr,
                rPlot,
                gPlot,
                bPlot
            };

            std::vector<SplinePoint>* chanelToPoints[nChanels + 1] =
            {
                nullptr,
                &rPoints,
                &gPoints,
                &bPoints
            };

            JG::Image fullImage;

            struct GrabPoint
            {
                Chanel chanel;
                std::vector<SplinePoint>* points;
                int* plot;
                int index;
            };

            GrabPoint grabbedPoint = {};

            void recalcPlot(std::vector<SplinePoint>& points, int* plot)
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

                changed = true;
            }

            void deletePoint()
            {
                if (grabbedPoint.chanel == None ||
                    grabbedPoint.index == 0 ||
                    grabbedPoint.index == grabbedPoint.points->size() - 1)
                    return;

                grabbedPoint.points->erase(grabbedPoint.points->begin() + grabbedPoint.index);
                recalcPlot(*grabbedPoint.points, grabbedPoint.plot);
                releasePoint();
            }

            void releasePoint()
            {
                grabbedPoint = {};
            }

            void movePoint(int shiftX, int shiftY)
            {
                if (!grabbedPoint.chanel)
                    return;

                auto& points = *grabbedPoint.points;
                auto plot = grabbedPoint.plot;

                points[grabbedPoint.index].y += shiftY;
                points[grabbedPoint.index].y = std::min(points[grabbedPoint.index].y, backSize - 1);
                points[grabbedPoint.index].y = std::max(points[grabbedPoint.index].y, 0);

                if (grabbedPoint.index != 0 && grabbedPoint.index != points.size() - 1)
                {
                    points[grabbedPoint.index].x += shiftX;
                    points[grabbedPoint.index].x = std::min(points[grabbedPoint.index].x, backSize - 1);
                    points[grabbedPoint.index].x = std::max(points[grabbedPoint.index].x, 0);

                    if (points[grabbedPoint.index].x <= points[grabbedPoint.index - 1].x || points[grabbedPoint.index].x >= points[grabbedPoint.index + 1].x)
                        deletePoint();
                }

                recalcPlot(points, plot);
            }

            void grabPoint(int x, int y)
            {
                for (int chanel = Min; chanel < Lim; chanel++)
                {
                    auto plot = chanelToPlot[chanel];
                    auto& points = *chanelToPoints[chanel];

                    x = std::min(x, backSize - 1);
                    x = std::max(x, 0);

                    y = std::min(y, backSize - 1);
                    y = std::max(y, 0);

                    int dist = 9;
                    int i = 0;

                    if ((int)abs(y - plot[x]) > dist)
                    {
                        releasePoint();
                        goto nextChanel;
                    }


                    for (; i != points.size(); i++)
                    {
                        if ((int)abs(y - points[i].y) <= dist && (int)abs(x - points[i].x) <= dist)
                        {
                            grabbedPoint = { (Chanel)chanel, &points, plot, i };
                            return;
                        }

                        if (points[i].x > x)
                            break;
                    }

                    points.insert(points.begin() + i, { x, y });
                    grabbedPoint = { (Chanel)chanel, &points, plot, i };
                    recalcPlot(*grabbedPoint.points, grabbedPoint.plot);
                    return;

                nextChanel:
                    ;
                }
            }

            void nextPoint()
            {
                if (!grabbedPoint.chanel)
                    grabbedPoint =
                {
                    .chanel = R,
                    .points = &rPoints,
                    .plot = rPlot,
                    .index = -1,
                };



                grabbedPoint.index = grabbedPoint.index + 1;

                if (grabbedPoint.index == grabbedPoint.points->size())
                {
                    grabbedPoint.chanel = (Chanel)(1 + (grabbedPoint.chanel + 1) % (Lim - Min));
                    grabbedPoint.points = chanelToPoints[grabbedPoint.chanel];
                    grabbedPoint.plot = chanelToPlot[grabbedPoint.chanel];
                    grabbedPoint.index = 0;
                }
            }
                
        };
    }
}

#endif // spline