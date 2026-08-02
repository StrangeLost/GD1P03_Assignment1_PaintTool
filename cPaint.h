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
#include <vector>
#include <optional>
#include <cstdio>
#include <SFML/Graphics.hpp>
#include "cButton.h"

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

	// Updates Handling
	void UpdateMousePosition();
	void UpdateButtons();

	// Draws handling
	void DrawButtons();
	void DrawStroke(sf::Vector2f from, sf::Vector2f to);

	// Helper Functions
	void SwitchTool(cButton& toolButton);
	sf::Vector2f WorldToCanvas(sf::Vector2f worldPosition) const;
	bool IsInsideCanvas(sf::Vector2f worldPosition) const;
	bool IsActiveButton(cButton& button) const;
	void StampBrush(sf::Vector2f canvasPosition);
	
	// Window
	sf::RenderWindow m_window;
	sf::Vector2f m_mousePosition;

	// Canvas
	sf::RenderTexture m_canvas;
	sf::RectangleShape m_canvasDisplay;
	sf::Vector2f m_canvasPosition;
	sf::Vector2f m_canvasSize;

	// Drawing Brush
	bool m_isDrawing = false;
	float m_brushRadius = 8.f;
	sf::Vector2f m_previousDrawPosition;
	sf::Color m_brushColor = sf::Color::Black;

	// Button base textures
	sf::Texture m_normalButtonTexture;
	sf::Texture m_hoveredButtonTexture;
	sf::Texture m_pressedButtonTexture;
	sf::Texture m_disabledButtonTexture;
	sf::Texture m_toggledButtonTexture;
	std::array<const sf::Texture*, 5> m_backgroundTextures;

	// Button icon textures
	sf::Texture m_pencilTexture;
	sf::Texture m_boxFillTexture;
	sf::Texture m_boxEmptyTexture;

	// Button base textures
	std::vector<cButton*> m_buttons;
	cButton* m_activeButton;
	cButton m_pencilButton;
	cButton m_boxFillButton;
	cButton m_boxEmptyButton;
};

