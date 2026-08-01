#pragma once
#include <SFML/Graphics.hpp>

class cPaint
{
public:
	cPaint();
	~cPaint();
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

	// Draws handling

	sf::RenderWindow m_window;
	sf::Vector2f m_mousePosition;
};

