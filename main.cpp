#include "Paint.h"

int main()
{
    sf::RenderWindow window(sf::VideoMode({ 1280, 720 }), "Painter");
    sf::CircleShape shape(8.f);
    
    Paint* paint = new Paint();
    
    shape.setFillColor(sf::Color::Green);

    while (window.isOpen())
    {
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                window.close();

            paint->HandleWindowResize(event, window);
            paint->HandleInput(event);
        }

        sf::Vector2i mousePosition = sf::Mouse::getPosition(window);
        mousePosition.x -= shape.getRadius();
        mousePosition.y -= shape.getRadius();

        shape.setPosition(static_cast<sf::Vector2f>(mousePosition));

        window.clear();

        paint->Draw();

        // DEBUG DRAW
        window.draw(shape);
        
        window.display();
    }
}
