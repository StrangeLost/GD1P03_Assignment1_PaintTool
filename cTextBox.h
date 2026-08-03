#pragma once
#include <iostream>
#include <SFML/Graphics.hpp>

class cTextBox
{
public:
	cTextBox(
		sf::Font& font,
		sf::Vector2f boxPosition,
		sf::Vector2f boxSize,
		int textLimit,
		int textSize, 
		sf::Color textColor,
		sf::String texts
	);

	void HandleInput(const sf::Event::TextEntered& input);
	void Update();
	void Draw(sf::RenderTarget& target) const;

	void DeleteLastCharacter();
	void Type(int inputChar);
	bool IsSelected(sf::Vector2f mousePosition);
	void SelectTextBox();
	void DeselectTextBox();
		
	inline void SetSelected(bool value) { m_isSelected = value; }
	inline bool GetSelected() const { return m_isSelected; }

private:
	void RefreshDisplay();

	sf::Text m_textDisplay;
	sf::RectangleShape m_textBox;
	sf::String m_value;

	int m_textLimit;
	bool m_isSelected;
};

