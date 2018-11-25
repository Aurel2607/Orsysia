#ifndef CMONSTER_H
#define CMONSTER_H

#include <string>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include "../AnimatedSprite/AnimatedSprite.h"
#include "../main.h"
#include "TexteUp.h"


class CMonster : public sf::Drawable
{
private:
	static const float xOffsetTextUp;
	static const float yOffsetTextUp;

public:
	static const float speedSlow;
	static const float speedNormal;
	static const float speedFast;


public:
	CMonster(	std::string name,
				int playerWidth,
				int playerHeight,
				int animatedFrameQty,
				sf::Vector2f initialOffset,
				sf::Vector2f offset,
				float speed,
				std::string spriteSheet,
				sf::Vector2f center);
	virtual ~CMonster();

	float   getSpeed(void) const {return m_speed;};
    int     getWidth(void) const {return m_width;};
    int     getHeight(void) const {return m_height;};

    void play();
    void stop();
    void move(sf::Vector2f movement);
    void update(sf::Time time);
 	void setDirection(direction_t);
 	direction_t getDirection(void);

protected:
	int m_width;
    int m_height;
    int	m_animatedFrameQty;
	sf::Vector2f m_initialOffset;
	sf::Vector2f m_offset;
    float m_speed;

private:
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

    void setUpAnimation(int width,    int widthOffset,
                        int height,   int heightOffset,
                        sf::Texture& texture);


//-----------------------------------------------------------------------------
//							Player Drawing - Player limit Rectangle
//-----------------------------------------------------------------------------
private:
	sf::RectangleShape m_limitRectShape;

// TextUp
private:
	CTexteUp m_textUp;

//-----------------------------------------------------------------------------
//							Player Drawing
//-----------------------------------------------------------------------------
private:
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;


//-----------------------------------------------------------------------------
//							Player position
//-----------------------------------------------------------------------------
public:
	const sf::Vector2f getCenter(void) const;
	void setCenter(const sf::Vector2f& center);


	sf::Rect<float> getRect() const;
};

#endif // CMONSTER_H
