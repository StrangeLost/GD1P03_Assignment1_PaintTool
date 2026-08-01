/***********************************************************************
Bachelor of Software Engineering
Media Design School
Auckland
New Zealand
(c) 2026 Media Design School
File Name : cButton.h
Description : The cButton class helps with tool selection for the paint software.
Author : Rony Day Song
Mail : rony.song@mds.ac.nz
**************************************************************************/

#pragma once
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/System/Vector2.hpp>

#include <functional>
#include <array>

namespace sf
{
	class RenderTarget;
	class Texture;
}

enum class ButtonState
{
	Normal   = 0,
	Hovered  = 1,
	Pressed  = 2,
	Disabled = 3,
	Toggled  = 4,
};

class cButton
{
public:
	cButton(
		sf::Vector2f position,
		sf::Vector2f size,
		std::array<const sf::Texture*, 5> backgroundTextures,
		const sf::Texture& iconTexture,
		bool isDisabled = false,
		bool isToggleable = false
	);

	void Update(sf::Vector2f& mousePosition);
	void Draw(sf::RenderTarget& target) const;

	void HandleMousePressed(sf::Vector2f mousePosition);
	void HandleMouseReleased(sf::Vector2f mousePosition);
	void SetOnClick(std::function<void()> function);

	bool IsToggled() const;

private:
	bool Contains(sf::Vector2f point) const;
	void UpdateAppearance();

	sf::RectangleShape m_shape;
	sf::RectangleShape m_icon;

	ButtonState m_state = ButtonState::Normal;

	std::array<const sf::Texture*, 5> m_backgroundTextures;
	std::function<void()> m_onClick;

	bool m_isDisabled = false;
	bool m_isToggleable = false;
	bool m_isToggled = false;
	bool m_isPressedInside = false;
};
