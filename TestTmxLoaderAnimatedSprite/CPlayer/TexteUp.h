#ifndef CTEXTEUP_H
#define CTEXTEUP_H

#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

class CTexteUp: public sf::Drawable
{
public:
	CTexteUp(std::string textToAppend, sf::Vector2f position);
	virtual ~CTexteUp();

	const sf::Vector2f& 	getPosition() const;
	void 					setPosition(const sf::Vector2f& pos);
	void 					setPosition(const float x, const float y);

	const sf::Vector2f 	getCenter(void) const;
	void 				setCenter(const float x, const float y);
	void 				setCenter(sf::Vector2f center);


	const sf::Vector2f getSize() const;

	void toto(sf::Vector2f movement);
	void doMove(sf::Vector2f movement);
	void update(sf::Time time);

protected:

private:
	std::string m_textToAppend;
	sf::Text m_text;
	sf::Font m_textFont;
	sf::RectangleShape m_textBG;

private:
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

private:

	void setPositionInternal(const sf::Vector2f pos);
	void setTextInternal(const sf::Vector2f pos);

	static const float xOffset;
	static const float yOffset;

};

#endif // CTEXTEUP_H
