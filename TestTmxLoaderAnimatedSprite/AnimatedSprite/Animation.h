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
//
// The sources were altered:
//	- for Naming Purpose
// 	- to add a resetFrames method
//	- to store some animation information like:
//		- is the animation running at the setting
//		- is the animation a loop animation

#ifndef ANIMATION_INCLUDE
#define ANIMATION_INCLUDE

#include <vector>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/Texture.hpp>

namespace AnimatedSprite
{
    class CAnimation
    {
	public:
		enum type_t{
			once = 0,
			loop,
			reverse
		};

    public:
        CAnimation();

        void addFrame(sf::IntRect rect);
        void setSpriteSheet(const sf::Texture& texture);
        const sf::Texture* getSpriteSheet() const;
        std::size_t getSize() const;
        const sf::IntRect& getFrame(std::size_t n) const;
        void resetFrames(void);

        void 			setAnimType(const type_t type);
        const type_t 	getAnimType(void) const;

     private:
        std::vector<sf::IntRect> m_frames;
        const sf::Texture* m_texture;
		type_t m_animType;
    };
}
#endif // ANIMATION_INCLUDE
