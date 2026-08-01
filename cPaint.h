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
	void HandleInput(const sf::Event& event);

	// Updates Handling
	void UpdateMousePosition();
	void UpdateButtons();

	// Draws handling
	void DrawButtons();

	sf::RenderWindow m_window;
	sf::Vector2f m_mousePosition;

	// Button base textures
	sf::Texture m_normalButtonTexture;
	sf::Texture m_hoveredButtonTexture;
	sf::Texture m_pressedButtonTexture;
	sf::Texture m_disabledButtonTexture;
	sf::Texture m_toggledButtonTexture;
	std::array<const sf::Texture*, 5> m_backgroundTextures;

	// Button icon textures
	sf::Texture m_pencilIconTexture;

	std::vector<cButton*> m_buttons;
	cButton m_pencilButton;
};

