#include "cPaint.h"

cPaint::cPaint()
	: m_window(sf::VideoMode({ 1280, 720 }), "Painter")
{
}

cPaint::~cPaint()
{
}

void cPaint::Run()
{
	sf::Clock clock;
	
	while (m_window.isOpen())
	{
		float deltaTime = clock.restart().asSeconds();
		
		HandleEvents();
		Update(deltaTime);
		Draw();
	}
}

void cPaint::HandleEvents()
{
	while (const std::optional event = m_window.pollEvent())
	{
		// Handle software exit
		if (event->is<sf::Event::Closed>())
		{
			m_window.close();
		}

		HandleWindowResize(*event);
		HandleInput(*event);
	}
}

void cPaint::Update(float deltaTime)
{
	UpdateMousePosition();
}

void cPaint::Draw()
{
	m_window.clear();

	// Draw processes here...

	m_window.display();
}

// Handle window resizing.
void cPaint::HandleWindowResize(const sf::Event& event)
{
    if (const auto* resized = event.getIf<sf::Event::Resized>())
    {
        const float width = static_cast<float>(resized->size.x);
        const float height = static_cast<float>(resized->size.y);

        sf::View view(sf::FloatRect(
            { 0.0f, 0.0f },
            { width, height }
        ));

        m_window.setView(view);
    }
}

// Handle all button input.
void cPaint::HandleInput(const sf::Event& event)
{
	if (const auto* pressed = event.getIf<sf::Event::MouseButtonPressed>())
	{
		if (pressed->button == sf::Mouse::Button::Left)
		{
			printf("Left Click Pressed!\n");
		}
	}

	if (const auto* released = event.getIf<sf::Event::MouseButtonReleased>())
	{
		if (released->button == sf::Mouse::Button::Left)
		{
			printf("Left Click Released!\n");
		}
	}
}

void cPaint::UpdateMousePosition()
{
	const sf::Vector2i pixelPosition = sf::Mouse::getPosition(m_window);

	m_mousePosition = m_window.mapPixelToCoords(pixelPosition);
}
