/***********************************************************************
Bachelor of Software Engineering
Media Design School
Auckland
New Zealand
(c) 2026 Media Design School
File Name : cPaint.h
Description : This is the core of the software.
Author : Rony Day Song
Mail : rony.song@mds.ac.nz
**************************************************************************/

#pragma once
#include "cButton.h"
#include "cTextBox.h"

class cPaint
{
public:
	cPaint();
	~cPaint() = default;
	void Run();

private:
	// Core functions
	void HandleEvents();
	void Update(float deltaTime);
	void Draw();

	// Events Handling
	void HandleWindowResize(const sf::Event& event);
	void HandleButtonInput(const sf::Event& event);
	void HandleToolInput(const sf::Event& event);
	void HandleTextBoxes(const sf::Event& event);
	void HandleTextInput(const sf::Event& event);

	// Updates Handling
	void UpdateMousePosition();
	void UpdateButtons();
	void UpdateToolUse();
	void UpdateTextBoxes();

	// Draws handling
	void DrawButtons();
	void DrawTextBoxes();

	// Helper Functions
	void SwitchTool(cButton& toolButton);
	sf::Vector2f WorldToCanvas(sf::Vector2f worldPosition) const;
	bool IsInsideCanvas(sf::Vector2f worldPosition) const;
	bool IsActiveButton(cButton& button) const;
	void SubmitTextValue();

	// Use Drawing Tools
	void UseLineTool();
	void UseBoxFillTool();
	void UseBoxEmptyTool();
	
	// Window
	sf::RenderWindow m_window;
	sf::Vector2f m_mousePosition;

	// Canvas
	sf::RenderTexture m_canvas;
	sf::RenderTexture m_tempCanvas;
	sf::RectangleShape m_canvasDisplay;
	sf::RectangleShape m_tempCanvasDisplay;
	sf::Vector2f m_canvasPosition;
	sf::Vector2f m_canvasSize;

	// Drawing Brush
	bool m_isDrawing = false;
	float m_brushRadius = 8.f;
	sf::Shape* m_brushShape = nullptr;
	sf::Vector2f m_startDrawPosition;
	sf::Color m_brushColor = sf::Color::Black;

	// Button base textures
	sf::Texture m_normalButtonTexture;
	sf::Texture m_hoveredButtonTexture;
	sf::Texture m_pressedButtonTexture;
	sf::Texture m_disabledButtonTexture;
	sf::Texture m_toggledButtonTexture;
	std::array<const sf::Texture*, 5> m_backgroundTextures;

	// Button icon textures
	sf::Texture m_lineTexture;
	sf::Texture m_boxFillTexture;
	sf::Texture m_boxEmptyTexture;

	// Button base textures
	std::vector<cButton*> m_buttons;
	cButton* m_activeButton;
	cButton m_lineButton;
	cButton m_boxFillButton;
	cButton m_boxEmptyButton;

	// Text boxes
	sf::Font m_font;
	std::vector<cTextBox*> m_textBoxes;
	cTextBox* m_activeTextBox;
	cTextBox m_brushTextBox;
};

