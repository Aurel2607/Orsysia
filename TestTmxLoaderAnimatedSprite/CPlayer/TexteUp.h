#ifndef CTEXTEUP_H
#define CTEXTEUP_H

#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

class CTexteUp
{
public:
	CTexteUp(std::string text, sf::Vector2f position);
	virtual ~CTexteUp();

	const sf::Vector2f& getPosition() const;
	void setPosition(sf::Vector2f& pos);

	const sf::Vector2f getCenter(void) const;
	void setCenter(sf::Vector2f& center);

	const sf::Vector2f getSize() const;
	void setString(std::string text);

	void move(sf::Vector2f movement);
	void update(sf::Time time);

protected:

private:
	sf::Text m_text;
	sf::Font m_textFont;
	sf::RectangleShape m_textBG;

private:
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

};

#endif // CTEXTEUP_H
