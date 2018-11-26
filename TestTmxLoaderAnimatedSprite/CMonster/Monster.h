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
				sf::Vector2f gap,
				float speed,
				std::string spriteSheet,
				sf::Vector2f center);
	virtual ~CMonster();

	std::string getName(void) const {return m_name;};
	float   	getSpeed(void) const {return m_speed;};
    int     	getWidth(void) const {return m_width;};
    int     	getHeight(void) const {return m_height;};

    void play();
    void stop();
    void move(sf::Vector2f movement);
    void update(sf::Time time);
 	void setDirection(direction_t);
 	direction_t getDirection(void);

protected:
	std::string m_name;
	int m_width;
    int m_height;
    int	m_animatedFrameQty;
	sf::Vector2f m_offset;
	sf::Vector2f m_gap;
    float m_speed;

private:
	   	// Animation
    sf::Texture m_texture1;
    AnimatedSprite::CAnimatedSprite m_animatedSprite;
   	AnimatedSprite::CAnimation m_oralWithSuccubus;
   	AnimatedSprite::CAnimation m_oralClimaxWithSuccubus;
    AnimatedSprite::CAnimation m_sexWithSuccubus;
   	AnimatedSprite::CAnimation m_rougherSexWithSuccubus;
   	AnimatedSprite::CAnimation m_rougherSexClimaxWithSuccubus;
	AnimatedSprite::CAnimation* m_pCurrentAnimation;

 //-----------------------------------------------------------------------------
//							Player Apparence
//-----------------------------------------------------------------------------
private:
    void setUpAnimation(AnimatedSprite::CAnimation& anim,
						int frameQty,
						bool frameReverse,
						float XLength, float XOffset, float XGap,
						float YLength, float YOffset,
						sf::Texture& texture);


//-----------------------------------------------------------------------------
//							Player Drawing - Player limit Rectangle
//-----------------------------------------------------------------------------
private:
//	sf::RectangleShape m_limitRectShape;

// TextUp
private:
//	CTexteUp m_textUp;

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

	void nextAnim(void);
	sf::Rect<float> getRect() const;
};

#endif // CMONSTER_H
