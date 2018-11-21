#include "TexteUp.h"



CTexteUp::CTexteUp(	sf::Vector2f posRef,
					sf::Vector2f textUpOffset,
					std::string textToAppend):
	m_posRef(posRef),
	m_textUpOffset(textUpOffset),
	m_textToAppend(textToAppend)
{
	// Initialisation du Texte
	if (!m_textFont.loadFromFile("fonts/Ubuntu-M.ttf")){
		printf("CTexteUp::CTexteUp - font not loaded\r\n");
	}
	m_text.setFont(m_textFont);
	m_text.setColor(sf::Color::White);
	m_text.setCharacterSize(14);
	m_text.setPosition(m_posRef + m_textUpOffset);
	m_text.setString(this->getTextToDisplay());

	// Initialisation du BackGround
	m_textBG.setFillColor(sf::Color::Black);
	m_textBG.setPosition(sf::Vector2f(m_text.getGlobalBounds().left, m_text.getGlobalBounds().top));
	m_textBG.setSize(sf::Vector2f(m_text.getGlobalBounds().width, m_text.getGlobalBounds().height));
}

CTexteUp::~CTexteUp()
{
	//dtor
}

void CTexteUp::setPosition(const sf::Vector2f& pos)
{
	m_posRef = pos;
	m_text.setPosition(m_posRef + m_textUpOffset);
	m_text.setString(this->getTextToDisplay());
	m_textBG.setPosition(sf::Vector2f(m_text.getGlobalBounds().left, m_text.getGlobalBounds().top));
}

void CTexteUp::move(sf::Vector2f movement)
{
	m_posRef += movement;
    m_text.move(movement);
	m_textBG.move(movement);
	m_text.setString(this->getTextToDisplay());
}

std::string CTexteUp::getTextToDisplay(void)
{
	return	(m_textToAppend
			+ " x:" + (std::to_string(m_posRef.x)).substr(0,5)
			+ " y:" + (std::to_string(m_posRef.y)).substr(0,5));
}


void CTexteUp::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(m_textBG);
	target.draw(m_text);
}

