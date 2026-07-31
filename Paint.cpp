#include "Paint.h"

// Handle window resizing.
void Paint::HandleWindowResize(
    std::optional<sf::Event> event,
    sf::RenderWindow& window)
{
    if (const auto* resized = event->getIf<sf::Event::Resized>())
    {
        const float width = static_cast<float>(resized->size.x);
        const float height = static_cast<float>(resized->size.y);

        sf::View view(sf::FloatRect(
            { 0.0f, 0.0f },
            { width, height }
        ));

        window.setView(view);
    }
}

// Handle all button input.
void Paint::HandleInput(std::optional<sf::Event> event)
{
	if (const auto* pressed =
		event->getIf<sf::Event::MouseButtonPressed>())
	{
		if (pressed->button == sf::Mouse::Button::Left)
		{
			printf("Left Click Pressed!\n");
		}
	}

	if (const auto* released =
		event->getIf<sf::Event::MouseButtonReleased>())
	{
		if (released->button == sf::Mouse::Button::Left)
		{
			printf("Left Click Released!\n");
		}
	}
}

void Paint::Update()
{
	m_MousePosition = sf::Mouse::getPosition();
}

void Paint::UpdateUI()
{

}

void Paint::Draw()
{
	DrawMouse();
}

void Paint::DrawMouse()
{

}