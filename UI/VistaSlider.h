/*++

Copyright (c) 2021 JulesIMF, MIPT

Module Name:

    VistaSlider.h

Abstract:

    Vista-style slider.

Author / Creation date:

    JulesIMF / 27.10.21

Revision History:

--*/

#ifndef JIMP_VISTASLIDER
#define JIMP_VISTASLIDER

//
// Includes / usings
//

#include <JG.h>
#include "tnd.h"
#include <common/messages.h>

//
// Defines
//

namespace JIMP
{
    namespace UI
    {
        struct VistaSlider : public JG::Widget
        {
            static int const backHeight = 4;
            VistaSlider(JG::Window* window, int beginX, int beginY, int width, float to = 100, float from = 0) :
                JG::Widget(window, beginX, beginY, width, VistaThumb::thumbHeight),
                back(0, (VistaThumb::thumbHeight - backHeight) / 2, width, backHeight, "resources/images/Slider/back.png"),
                to(to), from(from)
            {
                addChild(thumb = new VistaThumb(window, this));
            }

            struct VistaThumb : public JG::Widget
            {
                static int const thumbHeight = 14,
                    thumbWidth = 8;

                VistaThumb(JG::Window* window, VistaSlider* slider) :
                    JG::Widget(window, 0, 0, thumbHeight, thumbWidth),
                    thumbImage(0, 0, thumbWidth, thumbHeight, "resources/images/Slider/toggler_hor.png"),
                    slider(slider)
                {

                }

                virtual void renderMyself(int shiftX, int shiftY)
                {
                    int moveTexture = 0;

                    if (mouseOn)
                        moveTexture += thumbWidth;

                    if (mousePressed)
                        moveTexture += thumbWidth;

                    translateAndDraw(thumbImage, window, beginX + shiftX, beginY + shiftY, moveTexture, 0);
                }

                virtual JG::Widget::HandlerResponce onMouseMoved(JG::Event event)
                {
                    if (mousePressed)
                    {
                        auto delta = -beginX;
                        beginX += event.mouseMove.dx;
                        beginX = std::max(beginX, 0);
                        beginX = std::min(beginX, slider->width - thumbWidth);
                        delta += beginX;
                        slider->moveSliderInPx(delta);
                    }

                    return JG::Widget::HandlerResponce::Success;
                }

            protected:
                friend VistaSlider;
                VistaSlider* slider;
                JG::Image thumbImage;
            };

            virtual JG::Widget::HandlerResponce onMouseButtonPressed(JG::Event event)
            {
                JG::Widget::onMouseButtonPressed(event);
                if (thumb->mousePressed)
                    return JG::Widget::HandlerResponce::Success;

                auto delta = -thumb->beginX;
                thumb->beginX = event.mouseButton.x;
                thumb->beginX = std::min(thumb->beginX, width - VistaThumb::thumbWidth);
                delta += thumb->beginX;
                moveSliderInPx(delta);

                return JG::Widget::HandlerResponce::Success;
            }

            virtual void renderMyself(int shiftX, int shiftY)
            {
                translateAndDraw(back, window, beginX + shiftX, beginY + shiftY);
            }

            virtual void onMove(float dx)
            {
                debugMessage("moved on %f", dx);
            }

            virtual void move(float dx)
            {
                position += dx;
                onMove(dx);
            }

            virtual void setPosition(float position)
            {
                move(position - this->position);
            }

            void moveSliderInPx(int dx)
            {
                pxPosition += dx;
                float percent = (float)pxPosition / (float)width;

                float newPosition = from + (to - from) * percent;
                setPosition(newPosition);
            }

        protected:
            float from, to;
            int pxPosition = 0;
            float position = 0.0;
            friend VistaThumb;
            VistaThumb* thumb;
            JG::Image back;
        };
    }
}

#endif // vista