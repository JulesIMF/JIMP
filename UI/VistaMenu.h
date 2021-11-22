/*++

Copyright (c) 2021 JulesIMF, MIPT

Module Name:

    VistaMenu.h

Abstract:

    Vista-style menu.

Author / Creation date:

    JulesIMF / 27.10.21

Revision History:

--*/

#ifndef JIMP_VISTAMENU
#define JIMP_VISTAMENU

//
// Includes / usings
//

#include <JG.h>
#include <cassert>
#include "tnd.h"
#include <common/messages.h>

//
// Defines
//

namespace JIMP
{
    namespace UI
    {
        struct VistaMenu;

        struct VistaMenuItem : public JG::Widget
        {
            static int const itemWidth = 500, itemHeight = 24;

            VistaMenuItem(JG::Window* window, char const* caption = "", bool bridge = false) :
                JG::Widget(window, 0, 0, itemWidth, itemHeight),
                caption(caption),
                itemStrip(0, 0, itemWidth, itemHeight, /*bridge ? "resources/images/Menu/item_strip_bridge.png" :*/ "resources/images/Menu/item_strip.png")
            {

            }

            virtual JG::Widget::HandlerResponce onClick(JG::Event event)
            {
                return JG::Widget::HandlerResponce::Success;
            }

            virtual void renderMyself(int shiftX, int shiftY) override
            {
                int shiftTexture = 0;
                if (mouseOn)
                    shiftTexture += itemHeight;
                
                translateAndDraw(itemStrip, window, shiftX + beginX, shiftY + beginY, 0, shiftTexture);

                float const spaceLeft = 35.0;

                if (caption == nullptr)
                    return;

                sf::Font font;

                if (!font.loadFromFile(JG::defaultFont))
                {
                    printf("Can`t load %s\n", JG::defaultFont);
                    return;
                }

                int charSize = 15;

                sf::Text text;
                text.setFont(font);
                text.setString(caption);
                text.setCharacterSize(charSize);
                text.setFillColor(JG::Color::Black);

                text.move({ (float)beginX + (float)shiftX + spaceLeft, (float)beginY + (float)shiftY + (float)1.0 });
                window->getSfWindow().draw(text);
            }

            virtual JG::Widget::HandlerResponce onMouseButtonPressed(JG::Event event)
            {
                onClick(event);
                return JG::Widget::HandlerResponce::Success;
            }

            // void linkBridgeTo(Menu* bridgeTo)
            // {
            //     if (!bridge)
            //         throw std::logic_error("Tried to link menu to a non-bridge item");
                
            //     this->bridgeTo = bridgeTo;
            // }

            // bool displaysBridge()
            // {
            //     return mouseOn;
            // }

            // virtual JG::Widget::HandlerResponce onMouseEntered(JG::Event) override
            // {
            //     if (bridge)
            //     {

            //     }
            // }

        protected:
            char const* caption = "";
            // bool const bridge = nullptr;
            // Menu* bridgeTo;
            JG::Image itemStrip;
        };

        struct VistaMenuButton;

        struct VistaMenu : public JG::Widget
        {
            VistaMenu(JG::Window* window, VistaMenuButton* menuButton) :
                JG::Widget(window, beginX, beginY, VistaMenuItem::itemWidth, 0),
                menuButton(menuButton)
            {

            }

            virtual void addChild(JG::Widget* item) override
            {
                item->beginY += height;
                height += item->height;
                JG::Widget::addChild(item);
            }

            virtual JG::Widget::HandlerResponce onMouseButtonPressed(JG::Event event);
        
        protected:
            VistaMenuButton* menuButton;
        };

        struct VistaMenuButton;

        struct VistaMenuStrip : public JG::Widget
        {
            static int const stripHeight = 20;
            VistaMenuStrip(JG::Window* window) :
                JG::Widget(window, 0, 0, window->width, stripHeight),
                strip(0, 0, width, stripHeight, "resources/images/Menu/strip.png")
            {
                noActive = true;
            }

            virtual void renderMyself(int shiftX, int shiftY)
            {
                translateAndDraw(strip, window, 0, 0);
            }

            virtual void addChild(JG::Widget* menuButton)
            {
                menuButton->beginX += buttonsOccupedOnLeft;
                buttonsOccupedOnLeft += menuButton->width;
                JG::Widget::addChild(menuButton);
            }

        protected:
            friend VistaMenuButton;
            bool checked = false;
            void hideAll();
            JG::Image strip;        
            int buttonsOccupedOnLeft = 0;
        };

        struct VistaMenuButton : public JG::Widget
        {
            static int const buttonHeight = 20;
            static int const leftRightSize = 3;
            static int const space = 10;

            static int getTextSize(char const* str)
            {
                assert(str);

                sf::Font font;

                if (!font.loadFromFile(JG::defaultFont))
                {
                    printf("Can`t load %s\n", JG::defaultFont);
                    return 0;
                }

                sf::Text text;
                text.setFont(font);
                text.setString(str);
                text.setCharacterSize(charSize);
                text.setFillColor(JG::Color::Black);
                return (int)text.getGlobalBounds().width;
            }

            void setMenu(VistaMenu* menu)
            {
                this->menu = menu;
                menu->beginX = beginX;
                menu->beginY = buttonHeight;
            }

            VistaMenuButton(JG::Window* window, VistaMenuStrip* strip, char const* caption) :
                JG::Widget(window, 0, 0, 2 * space + getTextSize(caption), buttonHeight),
                strip(strip),
                caption(caption),
                left(0, 0, leftRightSize, buttonHeight, "resources/images/Menu/left_button.png"),
                mid(leftRightSize, 0, width - 2 * leftRightSize, buttonHeight, "resources/images/Menu/mid_button.png"),
                right(width - leftRightSize, 0, leftRightSize, buttonHeight, "resources/images/Menu/right_button.png")
            {

            }

            virtual void renderMyself(int shiftX, int shiftY)
            {
                int shiftTexture = 0;
                if (mouseOn)
                    shiftTexture += buttonHeight;

                if (mousePressed)
                    shiftTexture += buttonHeight;

                if (checked)
                    shiftTexture = 2 * buttonHeight;

                translateAndDraw(left, window, shiftX + beginX, shiftY + beginY, 0, shiftTexture);
                translateAndDraw(mid, window, shiftX + beginX, shiftY + beginY, 0, shiftTexture);
                translateAndDraw(right, window, shiftX + beginX, shiftY + beginY, 0, shiftTexture);

                if (caption == nullptr)
                    return;

                sf::Font font;

                if (!font.loadFromFile(JG::defaultFont))
                {
                    printf("Can`t load %s\n", JG::defaultFont);
                    return;
                }

                sf::Text text;
                text.setFont(font);
                text.setString(caption);
                text.setCharacterSize(charSize);
                text.setFillColor(JG::Color::Black);

                text.move({ (float)beginX + (float)shiftX + (float)space, (float)beginY + (float)shiftY + (float)1.0 });
                window->getSfWindow().draw(text);
            }

            virtual JG::Widget::HandlerResponce onMouseButtonPressed(JG::Event event) override
            {
                setChecked(!checked);

                if (checked)
                    displayMenu();

                if (!checked)
                    hideMenu();

                return JG::Widget::HandlerResponce::Success;
            }

            virtual JG::Widget::HandlerResponce onMouseEntered(JG::Event event) override
            {
                if (strip->checked && !checked)
                {
                    displayMenu();
                }
                return JG::Widget::HandlerResponce::Success;
            }

        protected:
            friend VistaMenuStrip;
            friend VistaMenu;

            void displayMenu()
            {
                strip->hideAll();
                setChecked(true);
                if (menu)
                {
                    window->pushToAddQueue(menu);
                    window->setActive(menu);
                }
            }

            void hideMenu()
            {
                window->pushToPopQueue(menu);
                setChecked(false);
            }

            void setChecked(bool newChecked)
            {
                checked = newChecked;
                strip->checked = newChecked;
            }

            JG::Image left, mid, right;
            VistaMenuStrip* strip;
            static int const charSize = 14;
            bool pressed = false;
            char const* caption;
            VistaMenu* menu;
            bool checked = false;
        };

        void VistaMenuStrip::hideAll()
        {
            for (auto child : children)
            {
                VistaMenuButton* thisButton = (VistaMenuButton*)child;
                thisButton->hideMenu();
            }
        }

        JG::Widget::HandlerResponce VistaMenu::onMouseButtonPressed(JG::Event event)
        {
            JG::Widget::onMouseButtonPressed(event);
            menuButton->hideMenu();
            return JG::Widget::HandlerResponce::Success;
        }
    }
}

#endif // vista