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
#include <cmath>

cPaint::cPaint()
// Initialize the software window
	: m_window(sf::VideoMode({ 1280, 720 }), "Painter"),
	m_canvas({ 1232, 600 }),
	m_tempCanvas({ 1232, 600 }),
	m_polygonVertex(sf::PrimitiveType::LineStrip),
	m_brushShape(nullptr),
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
	m_ellipseFillTexture("icon_ellipse_fill_32x32.png"),
	m_polygonTexture("icon_polygon_32x32.png"),
	m_stampTexture("icon_stamp_32x32.png"),
	m_saveTexture("icon_save_32x32.png"),
	m_loadTexture("icon_load_32x32.png"),

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

	m_ellipseFillButton(
		{ 76.f, 8.f },				// Button Position
		{ 32.f, 32.f },				// Button Size
		m_backgroundTextures,		// Button box textures
		m_ellipseFillTexture,		// Button box icon
		false,						// Button isDisabled
		true,						// Button isToggleable
		[this](cButton& button)		// Button onClick
		{
			SwitchTool(button);
		}
	),

	m_polygonButton(
		{ 110.f, 8.f },				// Button Position
		{ 32.f, 32.f },				// Button Size
		m_backgroundTextures,		// Button box textures
		m_polygonTexture,			// Button box icon
		false,						// Button isDisabled
		true,						// Button isToggleable
		[this](cButton& button)		// Button onClick
		{
			SwitchTool(button);
		}
	),

	m_stampButton(
		{ 144.f, 8.f },				// Button Position
		{ 32.f, 32.f },				// Button Size
		m_backgroundTextures,		// Button box textures
		m_stampTexture,				// Button box icon
		false,						// Button isDisabled
		true,						// Button isToggleable
		[this](cButton& button)		// Button onClick
		{
			SwitchTool(button);

			std::vector<std::string> loadPath = pfd::open_file(
				"Open image", ".",
				{ "Image files", "*.png *.jpg *.jpeg *.bmp *.tga" },
				pfd::opt::none
			).result();

			if (!loadPath.empty())
			{
				std::filesystem::path filePath = loadPath.front();

				LoadStampImage(filePath);
			}
		}
	),

	m_saveButton(
		{ 144.f, 42.f },			// Button Position
		{ 32.f, 32.f },				// Button Size
		m_backgroundTextures,		// Button box textures
		m_saveTexture,				// Button box icon
		false,						// Button isDisabled
		false,						// Button isToggleable
		[this](cButton& button)		// Button onClick
		{
			std::string savePath = pfd::save_file(
				"Save image", "Painting.png",
				{ "PNG image", "*.png" }
			).result();

			if (savePath != "")
			{
				std::filesystem::path filePath(savePath);

				filePath.replace_extension(".png");

				SaveCanvas(filePath);
			}
		}
	),

	m_loadButton(
		{ 178.f,42.f },				// Button Position
		{ 32.f, 32.f },				// Button Size
		m_backgroundTextures,		// Button box textures
		m_loadTexture,				// Button box icon
		false,						// Button isDisabled
		false,						// Button isToggleable
		[this](cButton& button)		// Button onClick
		{
			std::vector<std::string> loadPath = pfd::open_file(
				"Open image", ".",
				{ "Image files", "*.png *.jpg *.jpeg *.bmp *.tga" },
				pfd::opt::none
			).result();

			if (!loadPath.empty())
			{
				std::filesystem::path filePath = loadPath.front();
				
				LoadCanvas(filePath);
			}
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
		&m_ellipseFillButton,
		&m_polygonButton,
		&m_stampButton,
		&m_saveButton,
		&m_loadButton
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

	ImGui::SFML::Init(m_window);
}

void cPaint::Run()
{
	sf::Clock clock;
	
	while (m_window.isOpen())
	{
		sf::Time deltaTime = clock.restart();
		
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
			//ImGui::SFML::Shutdown();
			m_window.close();
		}

		ImGui::SFML::ProcessEvent(m_window, *event);
		HandleWindowResize(*event);
		HandleButtonInput(*event);
		HandleToolInput(*event);
		HandleTextBoxes(*event);
		HandleTextInput(*event);
	}
}

void cPaint::Update(sf::Time deltaTime)
{
	ImGui::SFML::Update(m_window, deltaTime);
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

	// Specifically for polygon stuff
	if (m_isPolygoning)
	{
		m_tempCanvas.draw(m_polygonVertex);
	}

	// Specifically for stamp stuff
	if (m_activeButton == &m_stampButton &&
		m_hasStampImage &&
		IsInsideCanvas(m_mousePosition))
	{
		sf::Sprite stampSprite(m_stampImage);

		const sf::Vector2u imageSize = m_stampImage.getSize();

		const sf::Vector2f scaledSize{
			static_cast<float>(imageSize.x) * m_stampScale,
			static_cast<float>(imageSize.y) * m_stampScale
		};

		sf::Vector2f position =
			WorldToCanvas(m_mousePosition) - scaledSize / 2.f;

		stampSprite.setPosition(position);

		stampSprite.setScale({ m_stampScale, m_stampScale });

		// Preview on the temporary canvas.
		m_tempCanvas.draw(stampSprite);
	}

	m_tempCanvas.display();

	m_window.draw(m_canvasDisplay);		// Permanent Draw
	m_window.draw(m_tempCanvasDisplay); // Temporary Draw
	DrawButtons();
	DrawTextBoxes();
	ImGui::SFML::Render(m_window);

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
				m_brushShape = new sf::RectangleShape();

			if (m_activeButton == &m_boxFillButton)
				m_brushShape = new sf::RectangleShape();

			if (m_activeButton == &m_ellipseFillButton)
				m_brushShape = new EllipseShape(
					{ 0.f, 0.f },
					m_pointCount
				);

			if (m_activeButton == &m_polygonButton)
			{
				const sf::Vector2f worldPosition = m_window.mapPixelToCoords(pressed->position);
				
				if (!IsInsideCanvas(worldPosition))
					return;
				
				const sf::Vector2f canvasPosition = WorldToCanvas(worldPosition);

				m_isPolygoning = true;
				m_polygonPoints.push_back(canvasPosition);
			}

			if (m_activeButton == &m_stampButton)
			{
				PlaceStamp();
				return;
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

	if (const auto* pressed = event.getIf<sf::Event::MouseButtonPressed>())
	{
		if (pressed->button == sf::Mouse::Button::Right)
		{
			if (m_activeButton == nullptr || m_activeButton != &m_polygonButton)
				return;

			m_isPolygoning = false;
			FinishPolygon();
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
					if (m_activeButton == &m_lineButton)
						textBox->SelectTextBox(1);

					if (m_activeButton == &m_ellipseFillButton)
						textBox->SelectTextBox(3);

					m_activeTextBox = textBox;
					return;
				}
			}
		}

		if (m_activeTextBox != nullptr)
		{
			SubmitTextValue();
		}
	}

	if (const auto* pressed = event.getIf<sf::Event::KeyPressed>())
	{
		if (pressed->code == sf::Keyboard::Key::Enter ||
			pressed->code == sf::Keyboard::Key::Escape)
		{
			SubmitTextValue();
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
	if (m_isPolygoning)
	{
		UsePolygonTool();
		return;
	}

	if (m_hasStampImage)
	{
		return;
	}

	if (m_activeButton == nullptr || !m_isDrawing || m_brushShape == nullptr)
		return;
	
	if (m_activeButton == &m_lineButton)
		UseLineTool();

	if (m_activeButton == &m_boxFillButton)
		UseBoxFillTool();

	if (m_activeButton == &m_ellipseFillButton)
		UseEllipseFillTool();
}

void cPaint::UpdateTextBoxes()
{
	for (cTextBox* textBox : m_textBoxes)
	{
		if (m_activeButton == nullptr)
			return;
		
		if (m_activeButton == &m_lineButton)
		{
			textBox->SetBoxPosition({ 8.f, 42.f });
			textBox->Update(static_cast<size_t>(m_brushRadius));
		}

		if (m_activeButton == &m_ellipseFillButton)
		{
			textBox->SetBoxPosition({ 76.f, 42.f });
			textBox->Update(m_pointCount);
		}
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
	for (cTextBox* textBox : m_textBoxes)
	{
		if (m_activeButton == &m_lineButton ||
			m_activeButton == &m_ellipseFillButton)
		{
			textBox->Show();
			textBox->Draw(m_window);
			return;
		}

		textBox->Hide();
	}
}

void cPaint::SwitchTool(cButton& toolButton)
{
	if (m_activeButton == &toolButton)
		return;

	m_isPolygoning = false;
	m_hasStampImage = false;
	m_polygonVertex.clear();
	m_polygonPoints.clear();

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

void cPaint::SubmitTextValue()
{
	if (m_activeTextBox == nullptr)
		return;

	m_activeTextBox->DeselectTextBox();

	if (m_activeButton == &m_lineButton)
		m_brushRadius = m_activeTextBox->GetValue();

	if (m_activeButton == &m_ellipseFillButton)
		m_pointCount = static_cast<size_t>(m_activeTextBox->GetValue());
	
	m_activeTextBox = nullptr;
}

void cPaint::FinishPolygon()
{
	if (m_polygonPoints.size() < 3)
	{
		m_polygonPoints.clear();
		m_polygonVertex.clear();
		return;
	}

	m_polygonVertex.clear();
	m_polygon.setPointCount(m_polygonPoints.size());

	for (std::size_t i = 0; i < m_polygonPoints.size(); ++i)
	{
		m_polygonVertex.append({ m_polygonPoints[i], m_brushColor });
		m_polygon.setPoint(i, m_polygonPoints[i]);
	}

	m_polygonVertex.append({ m_polygonPoints.front(), m_brushColor });
	m_polygon.setFillColor(m_brushColor);

	m_canvas.draw(m_polygonVertex);
	m_canvas.draw(m_polygon);
	m_canvas.display();

	m_polygonVertex.clear();
	m_polygonPoints.clear();
}

void cPaint::UseLineTool()
{
	if (auto* line = dynamic_cast<sf::RectangleShape*>(m_brushShape))
	{
		const sf::Vector2f mousePosition = WorldToCanvas(m_mousePosition);
		const sf::Vector2f direction = mousePosition - m_startDrawPosition;

		const float length = std::hypot(
			direction.x,
			direction.y
		);

		const float angle = std::atan2(
			direction.y,
			direction.x
		);

		line->setPosition(m_startDrawPosition);
		line->setSize({ length, m_brushRadius });
		line->setOrigin({ 0.f, m_brushRadius / 2.f });
		line->setRotation(sf::radians(angle));
		line->setFillColor(m_brushColor);
	}
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

void cPaint::UseEllipseFillTool()
{
	if (auto* ellipseFill = dynamic_cast<EllipseShape*>(m_brushShape))
	{
		const sf::Vector2f currentPosition = WorldToCanvas(m_mousePosition);

		// Support dragging in every direction.
		const sf::Vector2f topLeft{
			std::min(m_startDrawPosition.x, currentPosition.x),
			std::min(m_startDrawPosition.y, currentPosition.y)
		};

		const sf::Vector2f size{
			std::abs(currentPosition.x - m_startDrawPosition.x),
			std::abs(currentPosition.y - m_startDrawPosition.y)
		};

		ellipseFill->setPosition(topLeft);
		ellipseFill->setRadius(size / 2.f);
		ellipseFill->setFillColor(m_brushColor);
	}
}

void cPaint::UsePolygonTool()
{
	m_polygonVertex.clear();

	for (const sf::Vector2f& point : m_polygonPoints)
	{
		m_polygonVertex.append({
			point,
			m_brushColor
			});
	}

	if (!m_polygonPoints.empty())
	{
		m_polygonVertex.append({
			WorldToCanvas(m_mousePosition),
			m_brushColor
		});
	}
}

void cPaint::LoadStampImage(const std::filesystem::path& filePath)
{
	if (!m_stampImage.loadFromFile(filePath))
	{
		m_hasStampImage = false;
		return;
	}

	const sf::Vector2 imageSize = m_stampImage.getSize();

	const float widthScale =
		m_canvasSize.x / static_cast<float>(imageSize.x);

	const float heightScale =
		m_canvasSize.y / static_cast<float>(imageSize.y);

	m_stampScale = std::min({ 1.f, widthScale, heightScale });
	m_hasStampImage = true;
}

void cPaint::PlaceStamp()
{
	if (!m_hasStampImage || !IsInsideCanvas(m_mousePosition))
		return;

	sf::Sprite stampSprite(m_stampImage);

	const sf::Vector2u imageSize = m_stampImage.getSize();

	const sf::Vector2f scaledSize{
		static_cast<float>(imageSize.x) * m_stampScale,
		static_cast<float>(imageSize.y) * m_stampScale
	};

	sf::Vector2f position =
		WorldToCanvas(m_mousePosition) - scaledSize / 2.f;

	stampSprite.setPosition(position);
	stampSprite.setScale({ m_stampScale, m_stampScale });

	m_canvas.draw(stampSprite);
	m_canvas.display();
}

void cPaint::SaveCanvas(const std::filesystem::path& filePath)
{
	m_canvas.display();

	const sf::Image image = m_canvas.getTexture().copyToImage();

	image.saveToFile(filePath);
}

void cPaint::LoadCanvas(const std::filesystem::path& filePath)
{
	sf::Texture loadedTexture;

	if (!loadedTexture.loadFromFile(filePath))
		return;

	sf::Sprite loadedSprite(loadedTexture);

	m_canvas.clear(sf::Color::White);
	m_canvas.draw(loadedSprite);
	m_canvas.display();
}

