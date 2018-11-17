/*********************************************************************
Matt Marchant 2013 - 2015
SFML Tiled Map Loader - https://github.com/bjorn/tiled/wiki/TMX-Map-Format
						http://trederia.blogspot.com/2013/05/tiled-map-loader-for-sfml.html

The zlib license has been used to make this software fully compatible
with SFML. See http://www.sfml-dev.org/license.php

This software is provided 'as-is', without any express or
implied warranty. In no event will the authors be held
liable for any damages arising from the use of this software.

Permission is granted to anyone to use this software for any purpose,
including commercial applications, and to alter it and redistribute
it freely, subject to the following restrictions:

1. The origin of this software must not be misrepresented;
   you must not claim that you wrote the original software.
   If you use this software in a product, an acknowledgment
   in the product documentation would be appreciated but
   is not required.

2. Altered source versions must be plainly marked as such,
   and must not be misrepresented as being the original software.

3. This notice may not be removed or altered from any
   source distribution.
*********************************************************************/

#include "Game.h"
#include <tmx/MapLoader.hpp>
#include <tmx/Log.hpp>
#include "Player.h"
#include <sstream>
#include <cassert>
#include "main.h"

//-----------------------------------------------------------------------------
namespace
{

}

int main()
{
	CGame orsysiaGame("ORSYSIA", "maps/", "map4.tmx");

	orsysiaGame.run();

    while(renderWindow.isOpen())
    {



		// update Player
		nunPlayer.update(frameTime2);
        playerLimitRectangle.move(playerMovement  * frameTime.asSeconds());

        // Display HUD (fps, player position)
		sf::RectangleShape background(sf::Vector2f((fpsText.getGlobalBounds().width + 10), (fpsText.getGlobalBounds().height +10)));
		background.setFillColor(sf::Color::Black);
        float fpsCount = (1.f / frameTime2.asSeconds());
        fpsText.setString( 	"FPS:" + (std::to_string(fpsCount)).substr(0,5)
							+ " x:" + (std::to_string(nunPlayer.getPosition().x)).substr(0,5)
							+ " y:" + (std::to_string(nunPlayer.getPosition().y)).substr(0,5));
        fpsText.move(cameraMovement);


        ml.updateQuadTree(sf::FloatRect(0.f, 0.f, 800.f, 600.f));

        //draw
        renderWindow.clear();
        renderWindow.draw(ml);
		renderWindow.draw(nunPlayer);
		renderWindow.draw(background, fpsText.getTransform());
        renderWindow.draw(fpsText);
        renderWindow.draw(playerLimitRectangle);
        renderWindow.draw(cameraInhibitionRectShape);
        ml.drawLayer(renderWindow, tmx::MapLayer::Debug);//draw with debug information shown
        renderWindow.display();
    }

    return 0;
}

