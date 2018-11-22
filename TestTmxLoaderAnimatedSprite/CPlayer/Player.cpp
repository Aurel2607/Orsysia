
#include "Player.h"

const float CPlayer::speedHero = 150.f;
const float CPlayer::xOffsetTextUp = -30.f;
const float CPlayer::yOffsetTextUp = -20.f;

CPlayer::CPlayer(int playerWidth, int playerHeight, float speed, std::string spriteSheet, sf::Vector2f position):
    m_playerWidth(playerWidth),
    m_playerHeight(playerHeight),
    m_speed(speed),
    m_isEyesOpened(true),
    m_animatedSprite(sf::seconds(0.2), false, true),
    m_direction(direction_t::down),
    m_directionHasChanged(false),
    m_textUp(	position,
				sf::Vector2f(xOffsetTextUp, yOffsetTextUp),
				"Nun")
{
	// Load Texture
	if (!m_texture1.loadFromFile(spriteSheet)){
		printf("Failed to load player spritesheet %s!\r\n", spriteSheet.c_str() );
		// TODO (Aurel#1#): Throw???
	}

	// Set current animation
	m_pCurrentAnimation = &m_walkingAnimationDown;

	setEyesOpened(m_isEyesOpened);

	// set up AnimatedSprite position
	setPosition((position.x - (float)playerWidth) / 2.f,
				(position.y - (float)playerHeight) / 2.f);


	// Player limit Rectangle
	//------------------------
	m_playerLimitRectShape.setSize(getSize());
	m_playerLimitRectShape.setFillColor(sf::Color::Transparent);
	m_playerLimitRectShape.setOutlineColor(sf::Color::Red);
	m_playerLimitRectShape.setOutlineThickness(2.f);
	m_playerLimitRectShape.setPosition(getPosition().x, getPosition().y);

}

CPlayer::~CPlayer()
{
    //dtor
}

void CPlayer::setUpAnimation(   int playerWidth,    int playerWidthOffset,
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
	m_walkingAnimationDown.addFrame(sf::IntRect((playerWidth + playerWidthOffset),     	playerHeightOffset, playerWidth, playerHeight));
	m_walkingAnimationDown.addFrame(sf::IntRect(((2* playerWidth) + playerWidthOffset),	playerHeightOffset, playerWidth, playerHeight));
	m_walkingAnimationDown.addFrame(sf::IntRect((playerWidth + playerWidthOffset),		playerHeightOffset, playerWidth, playerHeight));
	m_walkingAnimationDown.addFrame(sf::IntRect(playerWidthOffset,                      playerHeightOffset, playerWidth, playerHeight));

	m_walkingAnimationLeft.setSpriteSheet(texture);
	m_walkingAnimationLeft.addFrame(sf::IntRect((playerWidth + playerWidthOffset),         	(playerHeight + playerHeightOffset), playerWidth, playerHeight));
	m_walkingAnimationLeft.addFrame(sf::IntRect(((2 * playerWidth) + playerWidthOffset),   	(playerHeight + playerHeightOffset), playerWidth, playerHeight));
	m_walkingAnimationLeft.addFrame(sf::IntRect((playerWidth + playerWidthOffset),         	(playerHeight + playerHeightOffset), playerWidth, playerHeight));
	m_walkingAnimationLeft.addFrame(sf::IntRect(playerWidthOffset,                   		(playerHeight + playerHeightOffset), playerWidth, playerHeight));

	m_walkingAnimationRight.setSpriteSheet(texture);
	m_walkingAnimationRight.addFrame(sf::IntRect((playerWidth + playerWidthOffset),        	((2 * playerHeight) + playerHeightOffset), playerWidth, playerHeight));
	m_walkingAnimationRight.addFrame(sf::IntRect(((2 * playerWidth) + playerWidthOffset),  	((2 * playerHeight) + playerHeightOffset), playerWidth, playerHeight));
	m_walkingAnimationRight.addFrame(sf::IntRect((playerWidth + playerWidthOffset),			((2 * playerHeight) + playerHeightOffset), playerWidth, playerHeight));
	m_walkingAnimationRight.addFrame(sf::IntRect(playerWidthOffset,                  		((2 * playerHeight) + playerHeightOffset), playerWidth, playerHeight));

	m_walkingAnimationUp.setSpriteSheet(texture);
	m_walkingAnimationUp.addFrame(sf::IntRect((playerWidth + playerWidthOffset),      	((3 * playerHeight) + playerHeightOffset), playerWidth, playerHeight));
	m_walkingAnimationUp.addFrame(sf::IntRect(((2 * playerWidth) + playerWidthOffset),	((3 * playerHeight) + playerHeightOffset), playerWidth, playerHeight));
	m_walkingAnimationUp.addFrame(sf::IntRect((playerWidth + playerWidthOffset),    	((3 * playerHeight) + playerHeightOffset), playerWidth, playerHeight));
	m_walkingAnimationUp.addFrame(sf::IntRect(playerWidthOffset,                     	((3 * playerHeight) + playerHeightOffset), playerWidth, playerHeight));
}



void CPlayer::setUpAnimationEyesClosed(void)
{
    // set up the initial animation
	setUpAnimation(	m_playerWidth, 0,
					m_playerHeight, 0,
					m_texture1);
}

void CPlayer::setUpAnimationEyesOpen(void)
{
    // set up the initial animation
	setUpAnimation(	m_playerWidth, 0,
					m_playerHeight, (4*m_playerHeight),
					m_texture1);
}

void CPlayer::setUpAnimationBlackEyesOpen(void)
{
    // set up the initial animation
	setUpAnimation(	m_playerWidth, (3 * m_playerWidth),
					m_playerHeight, (4*m_playerHeight),
					m_texture1);
}

void CPlayer::play()
{
	m_animatedSprite.play(*m_pCurrentAnimation);
}

void CPlayer::stop()
{
	m_animatedSprite.stop();
}

void CPlayer::move(sf::Vector2f movement)
{
	// Move Sprite
	m_animatedSprite.move(movement);

	// Move LimitRectShape
	m_playerLimitRectShape.move(movement);

	// Move Texte Up
    m_textUp.move(movement);
}

void CPlayer::update(sf::Time time)
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

void CPlayer::setDirection(direction_t dir)
{
	// Si la direction actuelle est différente de l'ancienne,
	if(m_direction != dir)
		m_directionHasChanged = true; // Alors on a eu un changement de direction

	m_direction = dir;
}

direction_t CPlayer::getDirection(void)
{
	return m_direction;
}

void CPlayer::setEyesOpened(bool eyesOpen)
{
	m_isEyesOpened = eyesOpen;
	printf("CPlayer::setEyesOpened: m_isEyesOpened=%d\r\n", m_isEyesOpened );
	if(m_isEyesOpened == true){
		setUpAnimationEyesOpen();
	}else{
		setUpAnimationEyesClosed();
	}
}

void CPlayer::toggleEyes(void)
{
	m_isEyesOpened = !m_isEyesOpened;
	printf("CPlayer::toggleEyes: m_isEyesOpened=%d\r\n", m_isEyesOpened );
	setEyesOpened(m_isEyesOpened);
}

bool CPlayer::isEyesOpened(void)
{
	return m_isEyesOpened;
}



void CPlayer::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(m_textUp);
	target.draw(m_playerLimitRectShape);
	target.draw(m_animatedSprite, states);
}


const sf::Vector2f& CPlayer::getPosition() const
{
	return m_animatedSprite.getPosition();
}

void CPlayer::setPosition(const sf::Vector2f& pos)
{
	m_animatedSprite.setPosition(pos);

	// LimitRectShape
	m_playerLimitRectShape.setPosition(pos);

	// Texte Up
	m_textUp.setPosition(pos);

}

void CPlayer::setPosition(float x, float y)
{
	sf::Vector2f pos(x,y);
	setPosition(pos);
}

const sf::Vector2f CPlayer::getCenter(void) const
{
	sf::Vector2f center;
	center.x = m_animatedSprite.getPosition().x + getWidth() / 2.f;
	center.y = m_animatedSprite.getPosition().y + getHeight() / 2.f;
	return center;
}

void CPlayer::setCenter(const sf::Vector2f& center)
{
	sf::Vector2f posToSet;
	posToSet.x = center.x - getWidth() / 2.f;
	posToSet.y = center.y - getHeight() / 2.f;
	return this->setPosition(posToSet);
}

const sf::Vector2f CPlayer::getSize() const
{
	sf::Vector2f sizePlayer(m_playerWidth, m_playerHeight);
	return sizePlayer;
}

sf::Rect<float> CPlayer::getRect() const
{
	sf::Rect<float> rectToReturn(getPosition(), getSize());
	return rectToReturn;
}




