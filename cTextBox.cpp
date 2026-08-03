#include "cTextBox.h"

#define DELETE_KEY 8
#define ENTER_KEY 13
#define ESCAPE_KEY 27

cTextBox::cTextBox(
	sf::Font& font,
	sf::Vector2f boxPosition,
	sf::Vector2f boxSize,
	int textLimit,
	int textSize,
	sf::Color textColor,
	sf::String texts
)
	: m_textDisplay(font),
	  m_textLimit(textLimit),
	  m_value(texts),
	  m_isSelected(false)
{
	m_textBox.setPosition(boxPosition);
	m_textBox.setSize(boxSize);

	m_textDisplay.setCharacterSize(textSize);
	m_textDisplay.setFillColor(textColor);

	RefreshDisplay();
}

void cTextBox::HandleInput(const sf::Event::TextEntered& typedText)
{
	if (!m_isSelected)
		return;

	const auto character = typedText.unicode;

	if (character == DELETE_KEY)
	{
		DeleteLastCharacter();
		return;
	}

	if (character == ENTER_KEY || character == ESCAPE_KEY)
	{
		DeselectTextBox();
		return;
	}

	// Only accept digits
	if (character < '0' || character > '9')
		return;

	if (m_value.getSize() >= static_cast<std::size_t>(m_textLimit))
		return;

	Type(character);
}

void cTextBox::Update()
{
	const sf::Vector2f boxCenter = m_textBox.getPosition() + m_textBox.getSize() / 2.f;
	const sf::FloatRect textBounds = m_textDisplay.getLocalBounds();
	
	m_textDisplay.setOrigin({
		textBounds.position.x + textBounds.size.x / 2.f,
		textBounds.position.y + textBounds.size.y / 2.f
		});

	m_textDisplay.setPosition(boxCenter);
}

void cTextBox::Draw(sf::RenderTarget& target) const
{
	target.draw(m_textBox);
	target.draw(m_textDisplay);
}

void cTextBox::DeleteLastCharacter()
{
	if (m_value.getSize() > 0)
	{
		m_value.erase(m_value.getSize() - 1);
	}

	RefreshDisplay();
}

void cTextBox::Type(int inputChar)
{
	m_value += static_cast<char32_t>(inputChar);
	RefreshDisplay();
}

bool cTextBox::IsSelected(sf::Vector2f mousePosition)
{
	return m_textBox.getGlobalBounds().contains(mousePosition);
}

void cTextBox::SelectTextBox()
{
	if (m_isSelected)
		return;

	m_isSelected = true;
	RefreshDisplay();
}

void cTextBox::DeselectTextBox()
{
	m_isSelected = false;
	RefreshDisplay();
}

void cTextBox::RefreshDisplay()
{
	sf::String displayedText = m_value;

	if (m_isSelected)
	{
		displayedText += "|";
	}

	m_textDisplay.setString(displayedText);
}
