/***********************************************************************
Bachelor of Software Engineering
Media Design School
Auckland
New Zealand
(c) 2026 Media Design School
File Name : cButton.cpp
Description : The implementation of cButton class.
Author : Rony Day Song
Mail : rony.song@mds.ac.nz
**************************************************************************/

#include <SFML/Graphics/RenderTarget.hpp>
#include "cButton.h"

cButton::cButton(
	sf::Vector2f position,
	sf::Vector2f size,
	std::array<const sf::Texture*, 5> backgroundTextures,
	const sf::Texture& iconTexture,
	bool isDisabled,
	bool isToggleable,
	std::function<void(cButton&)> onClickFunction
)
	: m_backgroundTextures(backgroundTextures),
	  m_isDisabled(isDisabled),
	  m_isToggleable(isToggleable)
{
	m_shape.setPosition(position);
	m_shape.setSize(size);
	m_shape.setTexture(m_backgroundTextures[0], true);

	m_icon.setPosition(position);
	m_icon.setSize(size);
	m_icon.setTexture(&iconTexture, true);

	SetOnClick(onClickFunction);
}

void cButton::Update(sf::Vector2f& mousePosition)
{
	if (m_isDisabled)
	{
		m_state = ButtonState::Disabled;
	}
	else if (m_isPressedInside && Contains(mousePosition))
	{
		m_state = ButtonState::Pressed;
	}
	else if (Contains(mousePosition))
	{
		m_state = ButtonState::Hovered;
	}
	else if (m_isToggled)
	{
		m_state = ButtonState::Toggled;
	}
	else
	{
		m_state = ButtonState::Normal;
	}

	UpdateAppearance();
}

void cButton::Draw(sf::RenderTarget& target) const
{
	target.draw(m_shape);
	target.draw(m_icon);
}

void cButton::HandleMousePressed(sf::Vector2f mousePosition)
{
	if (m_isDisabled)
		return;

	if (Contains(mousePosition))
	{
		m_isPressedInside = true;	
	}
}

void cButton::HandleMouseReleased(sf::Vector2f mousePosition)
{
	if (m_isDisabled)
	{
		m_isPressedInside = false;
		return;
	}

	const bool clicked = Contains(mousePosition) && m_isPressedInside;
	
	m_isPressedInside = false;

	if (!clicked)
	{
		return;
	}

	if (m_isToggleable)
	{
		m_isToggled = true;
	}

	if (m_onClick)
		m_onClick(*this);
}

void cButton::SetOnClick(std::function<void(cButton&)> function)
{
	m_onClick = std::move(function);
}

bool cButton::IsToggled() const
{
	return m_isToggled;
}

bool cButton::Contains(sf::Vector2f point) const
{
	return m_shape.getGlobalBounds().contains(point);
}

void cButton::UpdateAppearance()
{
	switch (m_state)
	{
	case ButtonState::Normal:
		m_shape.setTexture(m_backgroundTextures[0], true);
		break;
	case ButtonState::Hovered:
		m_shape.setTexture(m_backgroundTextures[1], true);
		break;
	case ButtonState::Pressed:
		m_shape.setTexture(m_backgroundTextures[2], true);
		break;
	case ButtonState::Disabled:
		m_shape.setTexture(m_backgroundTextures[3], true);
		break;
	case ButtonState::Toggled:
		m_shape.setTexture(m_backgroundTextures[4], true);
		break;
	}
}
