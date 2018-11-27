////////////////////////////////////////////////////////////
//
// Copyright (C) 2014 Maximilian Wagenbach (aka. Foaly) (foaly.f@web.de)
//
// This software is provided 'as-is', without any express or implied warranty.
// In no event will the authors be held liable for any damages arising from the use of this software.
//
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it freely,
// subject to the following restrictions:
//
// 1. The origin of this software must not be misrepresented;
// you must not claim that you wrote the original software.
// If you use this software in a product, an acknowledgment
// in the product documentation would be appreciated but is not required.
//
// 2. Altered source versions must be plainly marked as such,
// and must not be misrepresented as being the original software.
//
// 3. This notice may not be removed or altered from any source distribution.
//
////////////////////////////////////////////////////////////

#include "AnimatedSprite.h"

namespace AnimatedSprite
{
    CAnimatedSprite::CAnimatedSprite(sf::Time frameTime) :
        m_animation(NULL),
        m_frameTime(frameTime),
        m_currentFrame(0),
        m_isPaused(true),
        m_reversedPart(false),
         m_texture(NULL)
    {

    }

    void CAnimatedSprite::setAnimation(const CAnimation& animation)
    {
        m_animation = &animation;
        m_texture = m_animation->getSpriteSheet();
        m_currentFrame = 0;
        m_reversedPart = false;
        setFrame(m_currentFrame);
    }

    void CAnimatedSprite::setFrameTime(sf::Time time)
    {
        m_frameTime = time;
    }

    void CAnimatedSprite::play()
    {
        m_isPaused = false;
    }

    void CAnimatedSprite::play(const CAnimation& animation)
    {
        if (getAnimation() != &animation){
            setAnimation(animation);
        }
        play();
    }

    void CAnimatedSprite::pause()
    {
        m_isPaused = true;
    }

    void CAnimatedSprite::stop()
    {
        m_isPaused = true;
        m_currentFrame = 0;
        setFrame(m_currentFrame);
    }

    void CAnimatedSprite::setColor(const sf::Color& color)
    {
        // Update the vertices' color
        m_vertices[0].color = color;
        m_vertices[1].color = color;
        m_vertices[2].color = color;
        m_vertices[3].color = color;
    }

    const CAnimation* CAnimatedSprite::getAnimation() const
    {
        return m_animation;
    }

    sf::FloatRect CAnimatedSprite::getLocalBounds() const
    {
        sf::IntRect rect = m_animation->getFrame(m_currentFrame);

        float width = static_cast<float>(std::abs(rect.width));
        float height = static_cast<float>(std::abs(rect.height));

        return sf::FloatRect(0.f, 0.f, width, height);
    }

    sf::FloatRect CAnimatedSprite::getGlobalBounds() const
    {
        return getTransform().transformRect(getLocalBounds());
    }

    bool CAnimatedSprite::isPlaying() const
    {
        return !m_isPaused;
    }

    sf::Time CAnimatedSprite::getFrameTime() const
    {
        return m_frameTime;
    }

    void CAnimatedSprite::setFrame(std::size_t newFrame, bool resetTime)
    {
        if (m_animation)
        {
            //calculate new vertex positions and texture coordiantes
            sf::IntRect rect = m_animation->getFrame(newFrame);

            m_vertices[0].position = sf::Vector2f(0.f, 0.f);
            m_vertices[1].position = sf::Vector2f(0.f, static_cast<float>(rect.height));
            m_vertices[2].position = sf::Vector2f(static_cast<float>(rect.width), static_cast<float>(rect.height));
            m_vertices[3].position = sf::Vector2f(static_cast<float>(rect.width), 0.f);

            float left = static_cast<float>(rect.left) + 0.0001f;
            float right = left + static_cast<float>(rect.width);
            float top = static_cast<float>(rect.top);
            float bottom = top + static_cast<float>(rect.height);

            m_vertices[0].texCoords = sf::Vector2f(left, top);
            m_vertices[1].texCoords = sf::Vector2f(left, bottom);
            m_vertices[2].texCoords = sf::Vector2f(right, bottom);
            m_vertices[3].texCoords = sf::Vector2f(right, top);
        }

        if (resetTime)
            m_currentTime = sf::Time::Zero;
    }

    void CAnimatedSprite::update(sf::Time deltaTime)
    {
        // if not paused and we have a valid animation
        if (!m_isPaused && m_animation)
        {
            // add delta time
            m_currentTime += deltaTime;

            // if current time is bigger then the frame time advance one frame
            if (m_currentTime >= m_frameTime)
            {
                // reset time, but keep the remainder
                m_currentTime = sf::microseconds(m_currentTime.asMicroseconds() % m_frameTime.asMicroseconds());

				switch(m_animation->getAnimType())
				{
					case CAnimation::type_t::once:
						printf("CAnimatedSprite::update - Once\r\n");
						update_Once();
						break;
					case CAnimation::type_t::loop:
						printf("CAnimatedSprite::update - loop\r\n");
						update_Loop();
						break;
					case CAnimation::type_t::reverse:
						printf("CAnimatedSprite::update - reverse\r\n");
						update_Reverse();
						break;
					 default:
						break;
				}

                // set the current frame, not reseting the time
				printf("m_currentFrame - %d\r\n", m_currentFrame);
                setFrame(m_currentFrame, false);
            }
        }
    }

	void CAnimatedSprite::update_Once(void)
	{
		// get next Frame index
		if (m_currentFrame + 1 < m_animation->getSize()){
			m_currentFrame++;
		}else{
			m_isPaused = true;
		}
	}

	void CAnimatedSprite::update_Loop(void)
	{
		// get next Frame index
		if (m_currentFrame + 1 < m_animation->getSize()){
			m_currentFrame++;
		}else{
			// animation has ended
			m_currentFrame = 0; // reset to start
		}
	}

	void CAnimatedSprite::update_Reverse(void)
	{
		printf("CAnimatedSprite::update_Reverse - %d\r\n", m_currentFrame);
	   // Partie de l'anim pas en reverse
		if(m_reversedPart == false){
			m_currentFrame++;
			if(m_currentFrame+1 >= m_animation->getSize()){
				m_reversedPart = true;
			}
		}
		// Partie de l'anim en reverse
		else{
			m_currentFrame--;
			if(m_currentFrame <= 0){
				m_reversedPart = false;
			}
		}
	}



    void CAnimatedSprite::draw(sf::RenderTarget& target, sf::RenderStates states) const
    {
        if (m_animation && m_texture)
        {
            states.transform *= getTransform();
            states.texture = m_texture;
            target.draw(m_vertices, 4, sf::Quads, states);
        }
    }
}
