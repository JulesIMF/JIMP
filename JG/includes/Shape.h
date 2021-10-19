/*++

Copyright (c) 2021 JulesIMF, MIPT

Module Name:

    Shape.h

Abstract:

    

Author / Creation date:

    JulesIMF / 06.10.21

Revision History:

--*/

//
// Includes / usings
//

#ifndef SHAPE_JULESIMF
#define SHAPE_JULESIMF
#include <__JGDebug.h>

#ifdef SFML_WRAPPER
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#endif

//
// Defines
//

namespace JG
{
    struct Window;
    struct Canvas;

#ifdef SFML_WRAPPER
    using Color = sf::Color;
#else
    struct Color
    {

    };
#endif

    //
    // Shapes
    //

    struct Shape
    {
    public:
        Shape (int x, int y) : beginX(x), beginY(y), color( {0,0,0} ) {}
        virtual void draw(Window& window) const = 0;
        virtual void draw(Canvas& canvas) const = 0;
        virtual void setPosition(int newX, int newY);
        virtual void setColor(Color color);
        virtual void move(int deltaX, int deltaY);
        virtual Color getColor();
    
    protected:
        int beginX, beginY;
        Color color;
    };

    struct Circle : public Shape
    {
    public:
        Circle(int x, int y, int radius);
        virtual void draw(Window& window) const override;
        virtual void draw(Canvas& canvas) const override;
        virtual void setPosition(int newX, int newY) override;
        virtual void setColor(Color color) override;
        virtual void move(int deltaX, int deltaY);

    protected:
        int radius;

    #ifdef SFML_WRAPPER
        sf::CircleShape circle_;
    
    private:
        void validateSF();
    #endif
    };

    struct Rectangle : public Shape
    {
    public:
        Rectangle(int x, int y, int width, int heigth);
        virtual void draw(Window& window) const override;
        virtual void draw(Canvas& canvas) const override;
        virtual void setPosition(int newX, int newY) override;
        virtual void setColor(Color color) override;
        virtual void move(int deltaX, int deltaY);

    protected:
        int width, height;

    #ifdef SFML_WRAPPER
        sf::RectangleShape rectangle_;

    private:
        void validateSF();
    #endif
    };
}

#endif // !SHAPE_JULESIMF