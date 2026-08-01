/***********************************************************************
Bachelor of Software Engineering
Media Design School
Auckland
New Zealand
(c) 2026 Media Design School
File Name : cPaint.cpp
Description : Implementations of cPaint.h.
Author : Rony Day Song
Mail : rony.song@mds.ac.nz
**************************************************************************/

#include "cPaint.h"

cPaint::cPaint()
	// Initialize the software window
	: m_window(sf::VideoMode({ 1280, 720 }), "Painter"),

	// Load the base button textures
	m_normalButtonTexture("icon_box_normal_32x32.png"),
	m_hoveredButtonTexture("icon_box_hovered_32x32.png"),
	m_pressedButtonTexture("icon_box_pressed_32x32.png"),
	m_disabledButtonTexture("icon_box_disabled_32x32.png"),
	m_toggledButtonTexture("icon_box_toggled_32x32.png"),
	
	// Store them in one array to reduce repetition
	m_backgroundTextures{
		&m_normalButtonTexture,
		&m_hoveredButtonTexture,
		&m_pressedButtonTexture,
		&m_disabledButtonTexture,
		&m_toggledButtonTexture
	},

	// Load the icon button textures
	m_pencilIconTexture("icon_pencil_32x32.png"),

	// Initialize all buttons
	m_pencilButton(
		{ 8.f, 8.f },			// Button Position
		{ 32.f, 32.f },			// Button Size
		m_backgroundTextures,	// Button box textures
		m_pencilIconTexture,	// Button box icon
		false,					// Button isDisabled
		true					// Button isToggleable
	)
{
	// Store all the buttons into m_buttons
	m_buttons =
	{
		&m_pencilButton
	};
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
	UpdateButtons();
}

void cPaint::Draw()
{
	m_window.clear();

	// Draw processes here...
	DrawButtons();

	m_window.display();
}

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

// Handle all keyboard & mouse input.
void cPaint::HandleInput(const sf::Event& event)
{
	if (const auto* pressed = event.getIf<sf::Event::MouseButtonPressed>())
	{
		if (pressed->button == sf::Mouse::Button::Left)
		{
			for (cButton* button : m_buttons)
			{
				button->HandleMousePressed(m_mousePosition);
			}
		}
	}

	if (const auto* released = event.getIf<sf::Event::MouseButtonReleased>())
	{
		if (released->button == sf::Mouse::Button::Left)
		{
			for (cButton* button : m_buttons)
			{
				button->HandleMouseReleased(m_mousePosition);
			}
		}
	}
}

void cPaint::UpdateMousePosition()
{
	const sf::Vector2i pixelPosition = sf::Mouse::getPosition(m_window);

	m_mousePosition = m_window.mapPixelToCoords(pixelPosition);
}

void cPaint::UpdateButtons()
{
	for (cButton* button : m_buttons)
	{
		button->Update(m_mousePosition);
	}
}

void cPaint::DrawButtons()
{
	for (const cButton* button : m_buttons)
	{
		button->Draw(m_window);
	}
}
