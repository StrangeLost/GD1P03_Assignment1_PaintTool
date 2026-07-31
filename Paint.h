#pragma once
#include <SFML/Graphics.hpp>

class Paint
{
public:
	void HandleWindowResize(std::optional<sf::Event> event, sf::RenderWindow& window);
	void HandleInput(std::optional<sf::Event> event);
	void Update();
	void Draw();

private:
	void UpdateUI();
	void DrawMouse();

	sf::Vector2i m_MousePosition;
};

