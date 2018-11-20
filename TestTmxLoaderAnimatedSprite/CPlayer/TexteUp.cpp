#include "TexteUp.h"

CTexteUp::CTexteUp(std::string text, sf::Vector2f position)
{
	// Initialisation du Texte Up
	//------------------------
	if (!m_textFont.loadFromFile("fonts/Ubuntu-M.ttf")){
		printf("CTexteUp::CTexteUp - font not loaded\r\n");
	}
	m_text.setFont(m_textFont);
	m_text.setColor(sf::Color::White);
	m_text.setCharacterSize(14);
	m_text.setPosition(getPosition().x - 25.f, getPosition().y - 15.f);
	m_text.setString(	" x:" + (std::to_string(getPosition().x)).substr(0,5)
						+ " y:" + (std::to_string(getPosition().y)).substr(0,5));
	m_textBG.setSize(sf::Vector2f(m_text.getLocalBounds().width + 5, m_text.getGlobalBounds().height + 5));
	m_textBG.setPosition(m_text.getPosition());
	m_textBG.setFillColor(sf::Color::Black);
}

CTexteUp::~CTexteUp()
{
	//dtor
}

void CTexteUp::setString(std::string text)
{
	m_text.setString(text);
	m_textBG.setSize(sf::Vector2f(	m_text.getGlobalBounds().width + 5,
									m_text.getGlobalBounds().height + 5));
)

void CTexteUp::move(sf::Vector2f movement)
{
    m_text.move(movement);
	m_textBG.move(movement);
)


void CTexteUp::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(m_textBG);
	target.draw(m_text);
}


