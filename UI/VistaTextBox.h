/*++

Copyright (c) 2021 JulesIMF, MIPT

Module Name:

    VistaTextBox.h

Abstract:

    

Author / Creation date:

    JulesIMF / 16.11.21

Revision History:

--*/

#ifndef JIMP_VISTATEXTBOX
#define JIMP_VISTATEXTBOX

//
// Includes / usings
//

#include <JG.h>
#include <string>
#include "tnd.h"

//
// Defines
//

namespace JIMP
{
    namespace UI
    {
        struct VistaTextBox : public JG::Widget
        {
            static int const tbHeight = 21;
            static int const leftWidth  = 5,
                             rightWidth = 17;

            VistaTextBox(JG::Window* window, int beginX, int beginY, int width, char const* defaultText = "") : 
                JG::Widget(window, beginX, beginY, width = std::max(width, leftWidth + rightWidth), tbHeight),
                left(0, 0, leftWidth, tbHeight, "resources/images/TextBox/left.png"),
                mid(leftWidth, 0, realWidth = width - leftWidth - rightWidth, tbHeight, "resources/images/TextBox/mid.png"),
                right(width - rightWidth, 0, rightWidth, tbHeight, "resources/images/TextBox/right.png")
            {
                if (defaultText)
                    text = defaultText;

                font.loadFromFile(JG::defaultFont);
                sfText.setFont(font);
                sfText.setCharacterSize(charSize);
                sfText.setFillColor(JG::Color::Black);
                
                updateSfText();
            }

            char const* getText()
            {
                return text.c_str();
            }

            void setText(char const* newText)
            {
                text = newText;
            }

            void setText(std::string const& newText)
            {
                text = newText;
            }

            virtual void renderMyself(int shiftX, int shiftY)
            {
                int shift = 0;
                if (isActive)
                    shift += tbHeight;
                
                translateAndDraw(mid, window, shiftX + beginX, shiftY + beginY, 0, shift);

                sf::RenderTexture texture;
                texture.create(width - leftWidth - rightWidth, tbHeight);

                int verticalSpace = 3;

                sf::Transform selectionTransform;
                selectionTransform.translate({ (float)(-textShiftPx + positionToPx(std::min(selectionStart, cursorPosition))), (float)verticalSpace });
                int selectionSize = positionToPx(std::max(selectionStart, cursorPosition)) - positionToPx(std::min(selectionStart, cursorPosition));
                sf::RectangleShape selectionRect({ (float)selectionSize, (float)(height - 2 * verticalSpace) });
                sf::Color selectionColor = { 82, 178, 247 };
                selectionRect.setFillColor(selectionColor);
                texture.draw(selectionRect, selectionTransform);

                if (showCursor && isActive)
                {
                    int cursorSize = 1;
                    sf::Transform cursorTransform;
                    cursorTransform.translate({ (float)(-textShiftPx + positionToPx(cursorPosition)), (float)verticalSpace });
                    if (textAlign == TextAlign::Right) cursorTransform.translate({ (float)(-cursorSize), 0 });
                    sf::RectangleShape cursorRect({ (float)cursorSize, (float)(height - 2 * verticalSpace) });
                    sf::Color cursorColor = sf::Color::Black;
                    cursorRect.setFillColor(cursorColor);
                    texture.draw(cursorRect, cursorTransform);
                }

                sf::Transform textTransform;
                textTransform.translate({ (float)(-textShiftPx), 0 });
                texture.draw(sfText, textTransform);

                texture.display();
                sf::Transform transform;
                transform.translate({ (float)(beginX + shiftX + leftWidth), (float)(beginY + shiftY + 1) });
                sf::Sprite sprite(texture.getTexture());
                window->getSfWindow().draw(sprite, transform);

                translateAndDraw(left, window, shiftX + beginX, shiftY + beginY, 0, shift);
                translateAndDraw(right, window, shiftX + beginX, shiftY + beginY, 0, shift);
            }

            virtual JG::Widget::HandlerResponce onKeyPressed(JG::Event event)
            {
                switch (event.key.code)
                {
                case JG::Keyboard::Left:
                    inc(-1, event.key.shift);
                    break;
                
                case JG::Keyboard::Right:
                    inc(1, event.key.shift);
                    break;

                case JG::Keyboard::Backspace:
                    backspace();
                    break;

                case JG::Keyboard::Delete:
                    del();
                    break;

                case JG::Keyboard::Home:
                    home(event.key.shift);
                    break;

                case JG::Keyboard::End:
                    end(event.key.shift);
                    break;

                case JG::Keyboard::A:
                    if (event.key.control)
                    {
                        selectAll();
                        break;
                    }
                    // fallthrough
                default:
                    addCharByKeyCode(event.key.code, event.key.shift);
                    break;
                }

                return JG::Widget::HandlerResponce::Success;
            }

            virtual JG::Widget::HandlerResponce onTimer(JG::Event event) override
            {
                showCursor = ( (1 + event.timer.timeMs / blinkIntMs) ) & 1;
                return JG::Widget::HandlerResponce::SuccessYield;
            }

        protected:
            enum TextAlign
            {
                Left,
                Right,
            };

            TextAlign textAlign = TextAlign::Left;
            int edge  = 0;

            int cursorPosition = 0;
            int selectionStart = 0;
            int textShiftPx = 0;
            bool showCursor = true;
            int blinkIntMs = 500;
            int realWidth;

            int charSize = 15;
            std::string text;
            sf::Text sfText;
            sf::Font font;
            JG::Image left, mid, right;

            void addCharByKeyCode(int code, bool shift)
            {
                if (JG::Keyboard::A <= code && code <= JG::Keyboard::Z)
                {
                    addChar(shift ? ('A' + code) : ('a' + code));
                }

                if (JG::Keyboard::Num0 <= code && code <= JG::Keyboard::Num9)
                {
                    char c = 0;
                    switch (code - JG::Keyboard::Num0)
                    {
                    case 0:
                        c = shift ? ')' : '0';
                        break;
                    
                    case 1:
                        c = shift ? '!' : '1';
                        break;

                    case 2:
                        c = shift ? '@' : '2';
                        break;
                        
                    case 3:
                        c = shift ? '#' : '3';
                        break;

                    case 4:
                        c = shift ? '$' : '4';
                        break;

                    case 5:
                        c = shift ? '%' : '5';
                        break;

                    case 6:
                        c = shift ? '^' : '6';
                        break;

                    case 7:
                        c = shift ? '&' : '7';
                        break;

                    case 8:
                        c = shift ? '*' : '8';
                        break;                        

                    case 9:
                        c = shift ? '(' : '9';
                        break;
                    }

                    addChar(c);
                }

                switch (code)
                {
                case JG::Keyboard::Space:
                    addChar(' ');
                    break;
                
                case JG::Keyboard::LBracket:
                    addChar(shift ? '{' : '[');
                    break;

                case JG::Keyboard::RBracket:
                    addChar(shift ? '}' : ']');
                    break;

                case JG::Keyboard::SemiColon:
                    addChar(shift ? ':' : ';');
                    break;

                case JG::Keyboard::Comma:
                    addChar(shift ? '<' : ',');
                    break;
                
                case JG::Keyboard::Period:
                    addChar(shift ? '>' : '.');
                    break;
                
                case JG::Keyboard::Slash:
                    addChar(shift ? '?' : '/');
                    break;
                
                case JG::Keyboard::Backslash:
                    addChar(shift ? '|' : '\\');
                    break;

                case JG::Keyboard::Tilde:
                    addChar(shift ? '`' : '~');
                    break;
                
                case JG::Keyboard::Equal:
                    addChar(shift ? '+' : '=');
                    break;
                
                case JG::Keyboard::Hyphen:
                    addChar(shift ? '_' : '-');
                    break;
                
                case JG::Keyboard::Quote:
                    addChar(shift ? '"' : '\'');
                    break;
                
                case JG::Keyboard::Tab:
                    addChar('\t');
                    break;
                    
                default:
                    break;
                }
            }

            void addChar(char c)
            {
                removeSelected();
                text.insert(text.begin() + cursorPosition, c);
                updateSfText();
                inc(1);
            }

            void backspace()
            {
                if (removeSelected())
                    return;

                if (!cursorPosition)
                    return;
                
                text.erase(text.begin() + cursorPosition - 1);
                updateSfText();
                inc(-1);
            }

            void del()
            {
                if (removeSelected())
                    return;

                if (cursorPosition == text.size())
                    return;

                text.erase(text.begin() + cursorPosition);
                updateSfText();
                setAlignment(textAlign, edge);

                forceCursorAlign();
            }

            void home(bool select = false)
            {
                inc(std::min(-1, -cursorPosition), select);
            }

            void end(bool select = false)
            {
                inc(std::max(1, (int)text.size() - cursorPosition), select);
            }

            void inc(int n, bool select = false)
            {
                if (n > 0)
                    for (int i = 0; i != n; i++)
                        primitiveInc(select);
                
                else
                    for (int i = 0; i != -n; i++)
                        primitiveDec(select);
            }

            void selectAll()
            {
                home();
                end(true);
            }

            void forceCursorAlign()
            {
                if (/*textAlign == TextAlign::Left && */positionToPx(cursorPosition) - textShiftPx > realWidth)
                {
                    setAlignment(TextAlign::Right, cursorPosition);
                    return;
                }

                if (/*textAlign == TextAlign::Right && */positionToPx(cursorPosition) - textShiftPx < 0)
                {
                    setAlignment(TextAlign::Left, cursorPosition);
                    return;
                }
            }

            void primitiveInc(bool select = false)
            {
                if (!select && cursorPosition != selectionStart)
                    cursorPosition = std::max(cursorPosition, selectionStart);
                else
                    cursorPosition = std::min(cursorPosition + 1, (int)text.size());
                
                if (!select)
                    selectionStart = cursorPosition;

                setAlignment(textAlign, edge);

                forceCursorAlign();
            }

            void primitiveDec(bool select = false)
            {
                if (!select && cursorPosition != selectionStart)
                    cursorPosition = std::min(cursorPosition, selectionStart);
                else
                    cursorPosition = std::max(cursorPosition - 1, 0);

                if (!select)
                    selectionStart = cursorPosition;

                setAlignment(textAlign, edge);

                forceCursorAlign();
            }

            void updateSfText()
            {
                sfText.setString(text);
            }

            void updateTextShift()
            {
                if (textAlign == TextAlign::Left)
                    textShiftPx = positionToPx(edge);

                else
                    textShiftPx = positionToPx(edge) - realWidth;
            }

            int positionToPx(int position)
            {
                if (position < text.size())
                    return sfText.findCharacterPos(position).x;

                else
                    return sfText.getGlobalBounds().width;
            }

            bool removeSelected()
            {
                if (cursorPosition == selectionStart)
                    return false;
                
                if (cursorPosition > selectionStart)
                    std::swap(cursorPosition, selectionStart);
                
                text.erase(text.begin() + cursorPosition, text.begin() + selectionStart);
                selectionStart = cursorPosition;

                updateSfText();
                setAlignment(textAlign, edge);
                
                return true;
            }

            void setAlignment(TextAlign align, int position)
            {
                if (align == TextAlign::Right && sfText.getGlobalBounds().width < realWidth)
                {
                    textAlign = TextAlign::Left;
                    edge = 0;
                    updateTextShift();
                    return;
                }

                textAlign = align;
                edge = position;
                updateTextShift();
            }
        };
    }
}

#endif // !JIMP_VISTATEXTBOX