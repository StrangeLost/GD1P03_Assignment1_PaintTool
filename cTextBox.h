/***********************************************************************
Bachelor of Software Engineering
Media Design School
Auckland
New Zealand
(c) 2026 Media Design School
File Name : cTextBox.h
Description : This class helps with changing the brush size
Author : Rony Day Song
Mail : rony.song@mds.ac.nz
**************************************************************************/

#pragma once
#include <iostream>
#include "cButton.h"
#include "portable-file-dialogs.h"

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
	void Update(size_t value);
	void Draw(sf::RenderTarget& target) const;

	void DeleteLastCharacter();
	void Type(int inputChar);
	bool IsSelected(sf::Vector2f mousePosition);
	void SelectTextBox(int minValue);
	void DeselectTextBox();
	void Show();
	void Hide();
	
	inline void SetBoxPosition(sf::Vector2f position) { m_textBox.setPosition(position); }

	inline void SetSelected(bool value) { m_isSelected = value; }
	inline bool GetSelected() const { return m_isSelected; }

	inline void SetValue(sf::String value) { m_value = value; }
	inline float GetValue() { return std::stof(m_value.toAnsiString()); }

private:
	void RefreshDisplay();

	sf::Text m_textDisplay;
	sf::RectangleShape m_textBox;
	sf::String m_value;

	int m_textLimit;
	int m_minValue;
	bool m_isSelected;
	bool m_isVisible;
};

