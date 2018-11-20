#ifndef CPLAYER_H
#define CPLAYER_H

#include <string>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include "../AnimatedSprite/AnimatedSprite.h"
#include "../main.h"

class CPlayer: public sf::Drawable
{
public:
    static const float speedHero;


public:
    CPlayer(int playerWidth, int playerHeight, float speed, std::string spriteSheet, sf::Vector2f screenDimensions);
    virtual ~CPlayer();

    float   getSpeed(void) const {return m_speed;};
    int     getWidth(void) const {return m_playerWidth;};
    int     getHeight(void) const {return m_playerHeight;};

    void play();
    void stop();
    void move(sf::Vector2f movement);
    void update(sf::Time time);
 	void setDirection(direction_t);
 	direction_t getDirection(void);

protected:
    int m_playerWidth;
    int m_playerHeight;
    float m_speed;
	bool m_isEyesOpened;

   	// Animation
    sf::Texture m_texture1;
    AnimatedSprite::CAnimatedSprite m_animatedSprite;
   	AnimatedSprite::CAnimation m_walkingAnimationDown;
    AnimatedSprite::CAnimation m_walkingAnimationUp;
    AnimatedSprite::CAnimation m_walkingAnimationRight;
    AnimatedSprite::CAnimation m_walkingAnimationLeft;
    AnimatedSprite::CAnimation* m_pCurrentAnimation;

//-----------------------------------------------------------------------------
//							Player Apparence
//-----------------------------------------------------------------------------
private:
	direction_t 	m_direction;
	bool 			m_directionHasChanged;

    void setUpAnimation(int playerWidth,    int playerWidthOffset,
                        int playerHeight,   int playerHeightOffset,
                        sf::Texture& texture);
    void setUpAnimationEyesClosed(void);
    void setUpAnimationEyesOpen(void);
    void setUpAnimationBlackEyesOpen(void);

public:
	void setEyesOpened(bool eyesOpen);
	void toggleEyes(void);
	bool isEyesOpened(void);


//-----------------------------------------------------------------------------
//							Player Drawing - HUD
//-----------------------------------------------------------------------------
private:
	// Texte Up
	sf::Text m_textUp;
	sf::Font m_textUpFont;
	sf::RectangleShape m_textUpBGRectShape;

//-----------------------------------------------------------------------------
//							Player Drawing - Player limit Rectangle
//-----------------------------------------------------------------------------
private:
	sf::RectangleShape m_playerLimitRectShape;

//-----------------------------------------------------------------------------
//							Player Drawing
//-----------------------------------------------------------------------------
private:
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;


//-----------------------------------------------------------------------------
//							Player position
//-----------------------------------------------------------------------------
public:
	const sf::Vector2f& getPosition() const;
	void setPosition(sf::Vector2f& pos);
	const sf::Vector2f getCenter(void) const;
	void setCenter(sf::Vector2f& center);

	const sf::Vector2f getSize() const;

	sf::Rect<float> getRect() const;

	void centerScrolling(void);

};

#endif // CPLAYER_H
