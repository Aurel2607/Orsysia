#include "Monster.h"


const float CMonster::speedSlow = 50.f;
const float CMonster::speedNormal = 100.f;
const float CMonster::speedFast = 150.f;
const float CMonster::xOffsetTextUp = -30.f;
const float CMonster::yOffsetTextUp = -20.f;

CMonster::CMonster(	std::string name,
					int playerWidth,
					int playerHeight,
					int animatedFrameQty,
					sf::Vector2f initialOffset,
					sf::Vector2f offset,
					float speed,
					std::string spriteSheet,
					sf::Vector2f center):
    m_width(playerWidth),
    m_height(playerHeight),
    m_animatedFrameQty(animatedFrameQty),
    m_initialOffset(initialOffset),
    m_offset(offset),
    m_speed(speed),
    m_animatedSprite(sf::seconds(0.2), false, true),
    m_direction(direction_t::down),
    m_directionHasChanged(false),
    m_textUp(	center,
				sf::Vector2f(xOffsetTextUp, yOffsetTextUp),
				name)
{
	// Load Texture
	if (!m_texture1.loadFromFile(spriteSheet)){
		printf("Failed to load player spritesheet %s!\r\n", spriteSheet.c_str() );
		// TODO (Aurel#1#): Throw???
	}

	// Set current animation
	m_pCurrentAnimation = &m_walkingAnimationDown;

	// set up AnimatedSprite position
	setCenter(center);

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


void CMonster::setUpAnimation(   int playerWidth,    int playerWidthOffset,
                                int playerHeight,   int playerHeightOffset,
                                sf::Texture& texture)
{
	// Remove old frame
    if(m_walkingAnimationDown.getSize() != 0)
		m_walkingAnimationDown.resetFrames();
    if(m_walkingAnimationLeft.getSize() != 0)
		m_walkingAnimationLeft.resetFrames();
    if(m_walkingAnimationRight.getSize() != 0)
		m_walkingAnimationRight.resetFrames();
    if(m_walkingAnimationUp.getSize() != 0)
		m_walkingAnimationUp.resetFrames();

    // Set up the animations for all four directions
	m_walkingAnimationDown.setSpriteSheet(texture);
	m_walkingAnimationLeft.setSpriteSheet(texture);
	m_walkingAnimationRight.setSpriteSheet(texture);
	m_walkingAnimationUp.setSpriteSheet(texture);
	for(int i = 0; i < m_animatedFrameQty; i++){
		m_walkingAnimationDown.addFrame(sf::IntRect((i * playerWidth + playerWidthOffset),  playerHeightOffset, playerWidth, playerHeight));
		m_walkingAnimationLeft.addFrame(sf::IntRect((i * playerWidth + playerWidthOffset),	(playerHeight + playerHeightOffset), playerWidth, playerHeight));
		m_walkingAnimationRight.addFrame(sf::IntRect((i * playerWidth + playerWidthOffset),	((2 * playerHeight) + playerHeightOffset), playerWidth, playerHeight));
		m_walkingAnimationUp.addFrame(sf::IntRect((i * playerWidth + playerWidthOffset),    ((3 * playerHeight) + playerHeightOffset), playerWidth, playerHeight));
	}
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

	// Move Texte Up
    m_textUp.move(movement);
}

void CMonster::update(sf::Time time)
{
	// Si on a eu une mise à jour de la direction
	if(m_directionHasChanged == true){
		switch(m_direction){
		case direction_t::down:
			m_pCurrentAnimation = &m_walkingAnimationDown;
			break;
		case direction_t::up:
			m_pCurrentAnimation = &m_walkingAnimationUp;
			break;
		case direction_t::left:
			m_pCurrentAnimation = &m_walkingAnimationLeft;
			break;
		case direction_t::right:
			m_pCurrentAnimation = &m_walkingAnimationRight;
			break;
		default:
			break;
		}
		// Mise à jour de la direction effectuée
		m_directionHasChanged = false;
	}
	m_animatedSprite.update(time);
}

void CMonster::setDirection(direction_t dir)
{
	// Si la direction actuelle est différente de l'ancienne,
	if(m_direction != dir)
		m_directionHasChanged = true; // Alors on a eu un changement de direction

	m_direction = dir;
}

void CMonster::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(m_textUp);
	target.draw(m_limitRectShape);
	target.draw(m_animatedSprite, states);
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
	m_textUp.setPosition(posToSet);
}
