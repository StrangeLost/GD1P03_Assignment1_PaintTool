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
	m_canvas({ 1232, 600 }),
	m_tempCanvas({ 1232, 600 }),
	m_activeButton(nullptr),
	m_activeTextBox(nullptr),

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
	m_lineTexture("icon_line_32x32.png"),
	m_boxFillTexture("icon_box_fill_32x32.png"),
	m_boxEmptyTexture("icon_box_empty_32x32.png"),

	// Initialize all buttons
	m_lineButton(
		{ 8.f, 8.f },				// Button Position
		{ 32.f, 32.f },				// Button Size
		m_backgroundTextures,		// Button box textures
		m_lineTexture,				// Button box icon
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
	),

	// Initialize all textBoxes
	m_font("Roboto.ttf"),
	m_brushTextBox(
		m_font,				// Text Font
		{ 8.f, 42.f },		// TextBox Position
		{ 32.f, 32.f },		// TextBox Size
		3,					// Text Limit
		16,					// Font Size 
		sf::Color::Black,	// Font Color
		std::to_string(static_cast<int>(m_brushRadius))
	)
{
	// Store all the buttons into m_buttons
	m_buttons =
	{
		&m_lineButton,
		&m_boxFillButton,
		&m_boxEmptyButton,
	};

	// Store all the textboxes into m_textBoxes
	m_textBoxes =
	{
		&m_brushTextBox,
	};

	// Initialise the canvas
	m_canvasPosition = { 24.f, 96.f };
	m_canvasSize = { 1232.f, 600.f };

	m_canvas.clear(sf::Color::White);
	m_canvas.display();

	m_tempCanvas.clear(sf::Color::Transparent);
	m_tempCanvas.display();

	m_canvasDisplay.setPosition(m_canvasPosition);
	m_canvasDisplay.setSize(m_canvasSize);
	m_canvasDisplay.setTexture(&m_canvas.getTexture(), true);

	m_tempCanvasDisplay.setPosition(m_canvasPosition);
	m_tempCanvasDisplay.setSize(m_canvasSize);
	m_tempCanvasDisplay.setTexture(&m_tempCanvas.getTexture(), true);
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
		HandleTextBoxes(*event);
		HandleTextInput(*event);
	}
}

void cPaint::Update(float deltaTime)
{
	UpdateMousePosition();
	UpdateButtons();
	UpdateToolUse();
	UpdateTextBoxes();
}

void cPaint::Draw()
{
	m_window.clear(sf::Color(0x2E1F9EFF));
	m_tempCanvas.clear(sf::Color::Transparent);

	// Temporary Drawing if there's shape to draw
	if (m_isDrawing && m_brushShape != nullptr)
	{
		m_tempCanvas.draw(*m_brushShape);
	}

	m_tempCanvas.display();

	m_window.draw(m_canvasDisplay);		// Permanent Draw
	m_window.draw(m_tempCanvasDisplay); // Temporary Draw
	DrawButtons();
	DrawTextBoxes();

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
		// On Mouse Left Button Press
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
		// On Mouse Left Button Release
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

			m_isDrawing = true;
			m_startDrawPosition = WorldToCanvas(m_mousePosition);

			if (m_activeButton == &m_lineButton)
			{
				
			}

			if (m_activeButton == &m_boxFillButton)
			{
				m_brushShape = new sf::RectangleShape(
					{ 0.f, 0.f }
				);
			}

			if (m_activeButton == &m_boxEmptyButton)
			{

			}
		}
	}

	if (const auto* released = event.getIf<sf::Event::MouseButtonReleased>())
	{
		if (released->button == sf::Mouse::Button::Left)
		{
			if (m_brushShape != nullptr)
			{
				m_canvas.draw(*m_brushShape);
				m_canvas.display();
			}

			m_isDrawing = false;
			delete m_brushShape;
			m_brushShape = nullptr;
		}
	}
}

void cPaint::HandleTextBoxes(const sf::Event& event)
{
	if (const auto* pressed = event.getIf<sf::Event::MouseButtonPressed>())
	{
		if (pressed->button == sf::Mouse::Button::Left)
		{
			for (cTextBox* textBox : m_textBoxes)
			{
				if (textBox->IsSelected(m_mousePosition))
				{
					textBox->SelectTextBox();
					m_activeTextBox = textBox;
					return;
				}
			}
		}

		if (m_activeTextBox != nullptr)
		{
			m_activeTextBox->DeselectTextBox();
			m_activeTextBox = nullptr;
		}
	}
}

void cPaint::HandleTextInput(const sf::Event& event)
{
	if (const auto* typedCharacter = event.getIf<sf::Event::TextEntered>())
	{
		if (m_activeTextBox == nullptr)
			return;

		m_activeTextBox->HandleInput(*typedCharacter);
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

void cPaint::UpdateToolUse()
{
	if (m_activeButton == nullptr || !m_isDrawing || m_brushShape == nullptr)
		return;
	
	if (m_activeButton == &m_lineButton)
		UseLineTool();

	if (m_activeButton == &m_boxFillButton)
		UseBoxFillTool();

	if (m_activeButton == &m_boxEmptyButton)
		UseBoxEmptyTool();
}

void cPaint::UpdateTextBoxes()
{
	for (cTextBox* textBox : m_textBoxes)
	{
		textBox->Update();
	}
}

void cPaint::DrawButtons()
{
	for (const cButton* button : m_buttons)
	{
		button->Draw(m_window);
	}
}

void cPaint::DrawTextBoxes()
{
	for (const cTextBox* textBox : m_textBoxes)
	{
		textBox->Draw(m_window);
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

void cPaint::SpawnLineTool()
{

}

void cPaint::SpawnBoxFillTool()
{
	m_brushShape = new sf::RectangleShape();
}

void cPaint::SpawnBoxEmptyTool()
{

}

void cPaint::UseLineTool()
{
	
}

void cPaint::UseBoxFillTool()
{
	if (auto* boxFill = dynamic_cast<sf::RectangleShape*>(m_brushShape))
	{
		const sf::Vector2f boxWidth = WorldToCanvas(m_mousePosition) - m_startDrawPosition;

		boxFill->setPosition(m_startDrawPosition);
		boxFill->setSize(boxWidth);
		boxFill->setFillColor(m_brushColor);
	}
}

void cPaint::UseBoxEmptyTool()
{

}

