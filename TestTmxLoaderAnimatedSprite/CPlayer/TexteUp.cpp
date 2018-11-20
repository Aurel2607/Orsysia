#include "TexteUp.h"

const float CTexteUp::xOffset = 25.f;
const float CTexteUp::yOffset = 15.f;


CTexteUp::CTexteUp(std::string textToAppend, sf::Vector2f position):
	m_textToAppend(textToAppend)
{
	// Initialisation du Texte Up
	//------------------------
	if (!m_textFont.loadFromFile("fonts/Ubuntu-M.ttf")){
		printf("CTexteUp::CTexteUp - font not loaded\r\n");
	}
	m_text.setFont(m_textFont);
	m_text.setColor(sf::Color::White);
	m_text.setCharacterSize(14);
	m_textBG.setFillColor(sf::Color::Black);

	setPositionInternal(getPosition());
}

CTexteUp::~CTexteUp()
{
	//dtor
}

const sf::Vector2f& CTexteUp::getPosition() const
{
	return m_text.getPosition();
}

void CTexteUp::setPosition(const sf::Vector2f& pos)
{
	setPositionInternal(pos);
}

void CTexteUp::setPosition(const float x, const float y)
{
	sf::Vector2f pos(x,y);
	setPositionInternal(pos);
}

const sf::Vector2f CTexteUp::getCenter(void) const
{
	sf::Vector2f toReturn(	getPosition().x - m_text.getGlobalBounds().left / 2.f,
							getPosition().y - m_text.getGlobalBounds().top / 2.f);
	return toReturn;
}

void CTexteUp::setCenter(const float x, const float y)
{
	sf::Vector2f center(	x + m_text.getGlobalBounds().left / 2.f,
							y + m_text.getGlobalBounds().top / 2.f);
	setPositionInternal(center);
}

void CTexteUp::setCenter(sf::Vector2f center)
{
	setCenter(center.x, center.y);
}

const sf::Vector2f CTexteUp::getSize() const
{
	return getSize();
}

void CTexteUp::toto(sf::Vector2f movement)
{
	setCenter(0.f, 0.f);
//    m_text.move(movement);
//	m_textBG.move(movement);
)

void CTexteUp::doMove(sf::Vector2f movement)
{
    m_text.move(movement);
	m_textBG.move(movement);
)



void CTexteUp::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(m_textBG);
	target.draw(m_text);
}


void CTexteUp::setPositionInternal(const sf::Vector2f pos)
{
	sf::Vector2f posToSet(pos.x - xOffset, pos.y - yOffset);
	m_text.setPosition(posToSet);
	setTextInternal(posToSet);

	m_textBG.setSize(sf::Vector2f(m_text.getLocalBounds().width + 5, m_text.getGlobalBounds().height + 5));
	m_textBG.setPosition(m_text.getPosition());
}

void CTexteUp::setTextInternal(const sf::Vector2f pos)
{
	m_text.setString(	m_textToAppend
						+ " x:" + (std::to_string(pos.x)).substr(0,5)
						+ " y:" + (std::to_string(pos.y)).substr(0,5));
}

