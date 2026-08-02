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
	  m_activeButton(nullptr),
	  m_canvas({1232u, 600u}),

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
	m_pencilTexture("icon_pencil_32x32.png"),
	m_boxFillTexture("icon_box_fill_32x32.png"),
	m_boxEmptyTexture("icon_box_empty_32x32.png"),

	// Initialize all buttons
	m_pencilButton(
		{ 8.f, 8.f },				// Button Position
		{ 32.f, 32.f },				// Button Size
		m_backgroundTextures,		// Button box textures
		m_pencilTexture,		// Button box icon
		false,						// Button isDisabled
		true,						// Button isToggleable
		[this](cButton& button)		// Button onClick
		{
			SwitchTool(button);
		}
	),

	m_boxFillButton(
		{ 42.f, 8.f },				// Button Position
		{ 32.f, 32.f },				// Button Size
		m_backgroundTextures,		// Button box textures
		m_boxFillTexture,			// Button box icon
		false,						// Button isDisabled
		true,						// Button isToggleable
		[this](cButton& button)		// Button onClick
		{
			SwitchTool(button);
		}
	),

	m_boxEmptyButton(
		{ 76.f, 8.f },				// Button Position
		{ 32.f, 32.f },				// Button Size
		m_backgroundTextures,		// Button box textures
		m_boxEmptyTexture,			// Button box icon
		false,						// Button isDisabled
		true,						// Button isToggleable
		[this](cButton& button)		// Button onClick
		{
			SwitchTool(button);
		}
	)
{
	// Store all the buttons into m_buttons
	m_buttons =
	{
		&m_pencilButton,
		&m_boxFillButton,
		&m_boxEmptyButton
	};

	// Initialise the canvas
	m_canvasPosition = { 24.f, 96.f };
	m_canvasSize = { 1232.f, 600.f };

	m_canvas.clear(sf::Color::White);
	m_canvas.display();

	m_canvasDisplay.setPosition(m_canvasPosition);
	m_canvasDisplay.setSize(m_canvasSize);
	m_canvasDisplay.setTexture(&m_canvas.getTexture(), true);
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
		HandleButtonInput(*event);
		HandleToolInput(*event);
	}
}

void cPaint::Update(float deltaTime)
{
	UpdateMousePosition();
	UpdateButtons();

	// Check brush stroke
	if (m_isDrawing)
	{
		const sf::Vector2f currentPosition = WorldToCanvas(m_mousePosition);

		DrawStroke(
			m_previousDrawPosition,
			currentPosition
		);

		m_previousDrawPosition = currentPosition;

		m_canvas.display();
	}
}

void cPaint::Draw()
{
	m_window.clear();

	// Draw processes here...
	m_window.draw(m_canvasDisplay);
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
void cPaint::HandleButtonInput(const sf::Event& event)
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

void cPaint::HandleToolInput(const sf::Event& event)
{
	if (const auto* pressed = event.getIf<sf::Event::MouseButtonPressed>())
	{
		if (pressed->button == sf::Mouse::Button::Left)
		{
			if (m_activeButton == nullptr)
				return;

			const sf::Vector2f worldPosition = m_window.mapPixelToCoords(pressed->position);

			m_isDrawing = true;
			m_previousDrawPosition = WorldToCanvas(worldPosition);

			StampBrush(m_previousDrawPosition);
			m_canvas.display();
			
		}
	}

	if (const auto* released = event.getIf<sf::Event::MouseButtonReleased>())
	{
		if (released->button == sf::Mouse::Button::Left)
		{
			m_isDrawing = false;
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

void cPaint::DrawStroke(sf::Vector2f from, sf::Vector2f to)
{
	if (m_activeButton == nullptr || !m_isDrawing)
		return;

	const sf::Vector2f difference = to - from;

	const float distance = std::sqrt(
		difference.x * difference.x +
		difference.y * difference.y
	);

	const float spacing = std::max(1.f, m_brushRadius * 0.5f);
	const int steps = std::max(
		1,
		static_cast<int>(std::ceil(distance / spacing))
	);

	for (int i = 1; i <= steps; ++i)
	{
		const float amount =
			static_cast<float>(i) /
			static_cast<float>(steps);

		const sf::Vector2f position =
			from + difference * amount;

		StampBrush(position);
	}
}

void cPaint::SwitchTool(cButton& toolButton)
{
	if (m_activeButton == &toolButton)
		return;

	if (m_activeButton != nullptr)
	{
		m_activeButton->SetIsToggled(false);
	}

	m_activeButton = &toolButton;
}

sf::Vector2f cPaint::WorldToCanvas(sf::Vector2f worldPosition) const
{
	return worldPosition - m_canvasPosition;
}

bool cPaint::IsInsideCanvas(sf::Vector2f worldPosition) const
{
	const sf::FloatRect bounds(m_canvasPosition, m_canvasSize);
	return bounds.contains(worldPosition);
}

bool cPaint::IsActiveButton(cButton& button) const
{
	return m_activeButton == &button;
}

void cPaint::StampBrush(sf::Vector2f canvasPosition)
{
	sf::CircleShape brush(m_brushRadius);

	brush.setOrigin({
		m_brushRadius,
		m_brushRadius
	});

	brush.setPosition(canvasPosition);
	brush.setFillColor(m_brushColor);

	m_canvas.draw(brush);
}

