#ifndef CTEXTEUP_H
#define CTEXTEUP_H

#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

class CTexteUp: public sf::Drawable
{
public:
	CTexteUp(	sf::Vector2f posRef,
				sf::Vector2f textUpOffset,
				std::string textToAppend);
	virtual ~CTexteUp();

	void 	setPosition(const sf::Vector2f& pos);
	void 	move(sf::Vector2f movement);
//	void 	update(sf::Time time);

protected:

private:
	sf::Vector2f 		m_posRef;		// Position de reference
	sf::Vector2f 		m_textUpOffset; // Offset du texte à afficher par rapport à la position de reference
	std::string 		m_textToAppend;	// Texte à ajouter au texte affiché
	sf::Text 			m_text;
	sf::Font 			m_textFont;
	sf::RectangleShape 	m_textBG;

private:
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

private:

	void setPositionInternal(const sf::Vector2f pos);

	std::string getTextToDisplay(void);

};

#endif // CTEXTEUP_H
