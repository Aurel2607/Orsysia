#include "Monster.h"


const float CMonster::speedSlow = 50.f;
const float CMonster::speedNormal = 100.f;
const float CMonster::speedFast = 150.f;
const float CMonster::xOffsetTextUp = -30.f;
const float CMonster::yOffsetTextUp = -20.f;

CMonster::CMonster(	const std::string& name,
					int playerWidth,
					int playerHeight,
					float speed,
					const std::string & spriteSheet):
	m_name(name),
    m_width(playerWidth),
    m_height(playerHeight),
    m_animatedSprite(sf::seconds(0.2f))
{
	// Load Texture
	if (!m_texture.loadFromFile(spriteSheet)){
		printf("CMonster::setUpAnimation - Failed to load player spritesheet %s!\r\n", spriteSheet.c_str() );
	}



	// Player limit Rectangle
	//------------------------

	m_limitRectShape.setSize(sf::Vector2f(m_width, m_height));
	m_limitRectShape.setFillColor(sf::Color::Transparent);
	m_limitRectShape.setOutlineColor(sf::Color::Red);
	m_limitRectShape.setOutlineThickness(2.f);
	m_limitRectShape.setPosition(getCenter().x - getWidth()/2.f,
								 getCenter().y - getHeight()/2.f);

}


CMonster::~CMonster()
{
	//dtor
}


void CMonster::setUpAnimation(	AnimatedSprite::CAnimation& anim,
								int frameQty,
								AnimatedSprite::CAnimation::type_t animType,
								float XLength, float XOffset, float XGap,
								float YLength, float YOffset)
{
	// Remove old frame
    if(anim.getSize() != 0)
		anim.resetFrames();

    // Set up the animation
	anim.setSpriteSheet(m_texture);
	for(int i = 0; i < frameQty; i++){
		anim.addFrame(sf::IntRect(	(i * (XLength + XGap) + XOffset), YOffset, XLength, YLength));
	}
	anim.setAnimType(animType);

}

void CMonster::play()
{
	m_animatedSprite.play(*m_pCurrentAnimation);
}

void CMonster::stop()
{
	m_animatedSprite.stop();
}

void CMonster::move(sf::Vector2f movement)
{
	// Move Sprite
	m_animatedSprite.move(movement);

	// Move LimitRectShape
	m_limitRectShape.move(movement);
}

void CMonster::update(sf::Time time)
{
	m_animatedSprite.update(time);
}

void CMonster::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(m_animatedSprite, states);
	target.draw(m_limitRectShape);
}


const sf::Vector2f CMonster::getCenter(void) const
{
	sf::Vector2f center;
	center.x = m_animatedSprite.getPosition().x + getWidth() / 2.f;
	center.y = m_animatedSprite.getPosition().y + getHeight() / 2.f;
	return center;
}

void CMonster::setCenter(const sf::Vector2f& center)
{
	sf::Vector2f posToSet;
	posToSet.x = center.x - getWidth() / 2.f;
	posToSet.y = center.y - getHeight() / 2.f;

	m_animatedSprite.setPosition(posToSet);

	// LimitRectShape
	m_limitRectShape.setPosition(posToSet);

	// Texte Up
//	m_textUp.setPosition(posToSet);
}

