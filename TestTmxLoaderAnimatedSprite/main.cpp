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
	bool handleWindowEvent(sf::RenderWindow& renderWindow, CPlayer& player)
	{
		static bool key_O_StilPressed = false;
        sf::Event event;
        while(renderWindow.pollEvent(event)){
            if(event.type == sf::Event::Closed)
                renderWindow.close();
            if(event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
                renderWindow.close();

			// Gestion de l'appuie touche O (bascule keyPressed/keyReleased)
			if ((event.type == sf::Event::KeyPressed) &&
				(event.key.code == sf::Keyboard::O) &&
				(key_O_StilPressed == false))
			{
				printf(" sf::Event::KeyPressed::O \r\n");
//				player.toggleEyes();
				if(player.isEyesOpened() == true)
					player.setEyesOpened(false);
				else
					player.setEyesOpened(true);

				key_O_StilPressed = true;
				return true;
			}

			// Gestion de l'appuie touche O (bascule keyPressed/keyReleased)
            if ((event.type == sf::Event::KeyReleased) &&
                (event.key.code == sf::Keyboard::O) &&
                (key_O_StilPressed == true)) {
                key_O_StilPressed = false;
            }
		}
	}

	// Test testInteraction
	bool testInteraction(std::vector<tmx::MapLayer>& layersToCheck)
	{
		for(auto& layerInd : layersToCheck){
			printf(" Layer: %s  (", layerInd.name.c_str());
			switch (layerInd.type)
			{
			case tmx::Layer:
				printf("Layer)\r\n");
				break;

			case tmx::ObjectGroup:
				printf("ObjectGroup)\r\n");
				for(auto& obj : layerInd.objects){
					printf("Object : %s (%0.2f,%0.2f)\r\n",
								obj.getName().c_str(),
								obj.getCentre().x,
								obj.getCentre().y);
				}
				break;

			case tmx::ImageLayer:
				printf("ImageLayer)\r\n");
				break;

			default:
				printf("Default)\r\n");
				break;
			}

//			if(layerInd.type == tmx::ObjectGroup){
//				for(auto& obj : layerInd.objects){

//					// Collisions
//					if(layerInd.name == "solidObject"){
//						for(auto& point : player.collisionPoints){
//							if(obj.contains(player.position + point)){
//								//handle collision
//							   break; //don't test more points than you need
//							}
//						}
//					}
//
//					// Action
//					else if(layerInd.name == "trigger"){
//						//send trigger command to queue
//					}
//
//					// Terrain Modification
//					else if(layerInd.name == "terrainModif"){
//						//send trigger command to queue
//					}
//
//				}
//			}

		}
		return true;
	}

	sf::Vector2f centerScrolling(sf::Vector2u& actualMapSize,
								sf::View& actualView,
								sf::Vector2f cameraInhibition,
								sf::RectangleShape&  cameraInhibitionRectangleShape,
								CPlayer& player)
	{

		int cxperso = static_cast<int>(player.getPosition().x) + player.getWidth() / 2;
		int cyperso = static_cast<int>(player.getPosition().y) + player.getHeight() / 2;

		int xLimMin = static_cast<int>(actualView.getCenter().x - cameraInhibition.x / 2.f);
		int xLimMax = static_cast<int>(actualView.getCenter().x + cameraInhibition.x / 2.f);

		int yLimMin = static_cast<int>(actualView.getCenter().y - cameraInhibition.y / 2.f);
		int yLimMax = static_cast<int>(actualView.getCenter().y + cameraInhibition.y / 2.f);

		sf::Vector2f viewMoving(0.f, 0.f);

		// 	Moving camera within inhibit Rectangle
		//------------------------------------------
		if (cxperso < xLimMin){
			viewMoving.x = - 3.f;
		}
		else if (cxperso < (actualView.getCenter().x - actualView.getSize().x /2.f)){
			viewMoving.x = -30.f;
		}

		if (cxperso > xLimMax){
			viewMoving.x = 3.f;
		}
		else if (cxperso > (actualView.getCenter().x + actualView.getSize().x /2.f)){
			viewMoving.x = 30.f;
		}

		if (cyperso < yLimMin){
			viewMoving.y = - 3.f;
		}
		else if (cyperso < (actualView.getCenter().y - actualView.getSize().y /2.f)){
			viewMoving.y = - 30.f;
		}

		if (cyperso > yLimMax){
			viewMoving.y = 3.f;
		}
		else if (cyperso > (actualView.getCenter().y + actualView.getSize().y /2.f)){
			viewMoving.y = 30.f;
		}

		// Blocking Camera with the end of the map
		//-----------------------------------------
		if (actualView.getCenter().x + viewMoving.x - actualView.getSize().x / 2.f < 0){
			viewMoving.x = 0;
		}
		else if ((actualView.getCenter().x + viewMoving.x + actualView.getSize().x / 2.f) >= actualMapSize.x){
			viewMoving.x = 0;
		}

		if (actualView.getCenter().y + viewMoving.y - actualView.getSize().y / 2.f < 0){
			viewMoving.y = 0;
		}
		else if ((actualView.getCenter().y + viewMoving.y + actualView.getSize().y / 2.f) >= actualMapSize.y){
			viewMoving.y = 0;
		}

		actualView.move(viewMoving);
		cameraInhibitionRectangleShape.move(viewMoving);

		return viewMoving;
	}
}

int main()
{
	CGame orsysiaGame;

	int cpt_intersection = 0;
	int cpt_mapGetLimitReachedStatus = 0;

	sf::Vector2f playerPos;


    sf::Vector2f screenDimensions(SCREEN_WIDTH, SCREEN_HEIGHT);
    sf::RenderWindow renderWindow(sf::VideoMode(screenDimensions.x, screenDimensions.y), "TMX Loader + Animated Sprite");
	renderWindow.setFramerateLimit(60);

	// HUD Text
	//-----------
	sf::Font font;
	if (!font.loadFromFile("fonts/Ubuntu-M.ttf")){
		printf("main - font not loaded\r\n");
	}
	sf::Text fpsText;
	fpsText.setFont(font);
	fpsText.setColor(sf::Color::White);
	fpsText.setCharacterSize(14);

    // Player
	//-----------
    CPlayer nunPlayer(48, 48, CPlayer::speedHero, "sprites/player.png", screenDimensions);

	// Player limit Rectangle
	//------------------------
	sf::RectangleShape playerLimitRectangle(nunPlayer.getSize());
	playerLimitRectangle.setFillColor(sf::Color::Transparent);
	playerLimitRectangle.setOutlineColor(sf::Color::Red);
	playerLimitRectangle.setOutlineThickness(2.f);
	playerLimitRectangle.setPosition(nunPlayer.getPosition().x, nunPlayer.getPosition().y);

	// Camera limit Rectangle
	//------------------------
	sf::Vector2f cameraInhibitionSize(CAMERA_INHIBITION_WIDTH, CAMERA_INHIBITION_HEIGHT);
	sf::RectangleShape cameraInhibitionRectShape(cameraInhibitionSize);
	cameraInhibitionRectShape.setFillColor(sf::Color::Transparent);
	cameraInhibitionRectShape.setOutlineColor(sf::Color::Red);
	cameraInhibitionRectShape.setOutlineThickness(2.f);
	cameraInhibitionRectShape.setPosition((screenDimensions - cameraInhibitionSize) / 2.f);


	//set the debugging output mode
	tmx::setLogLevel(tmx::Logger::Info | tmx::Logger::Error);

    // Create map loader and load map
	//-------------------------------
    tmx::MapLoader ml("maps/");
    ml.load("map4.tmx");
    sf::Vector2u mapSize = ml.getMapSize();
    std::vector<tmx::MapLayer>& layers = ml.getLayers();



	// Temp
	testInteraction(layers);

    sf::Clock deltaClock, frameClock;

//    const float dt = 0.01f;

//    float previousUpdateTime = deltaClock.getElapsedTime().asSeconds();
//    float accumulator = 0.f;

    bool noKeyWasPressed = true;
    while(renderWindow.isOpen())
    {
        orsysiaGame.loopFn();
        handleWindowEvent(renderWindow, nunPlayer);

		sf::Event event;

        sf::Time frameTime2 = frameClock.restart();
		sf::Vector2f playerMovement(0.f, 0.f);

		// if a key was pressed set the correct animation and move correctly
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
			if(nunPlayer.getPosition().y > 0){
				playerMovement.y -= nunPlayer.getSpeed();
			}
			noKeyWasPressed = false;
			nunPlayer.setDirection(direction::up);
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))	{
			if((nunPlayer.getPosition().y + nunPlayer.getSize().y) < static_cast<float>(ml.getMapSize().y)){
				playerMovement.y += nunPlayer.getSpeed();
			}
			noKeyWasPressed = false;
			nunPlayer.setDirection(direction::down);
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))	{
			if(nunPlayer.getPosition().x > 0){
				playerMovement.x -= nunPlayer.getSpeed();
			}
			noKeyWasPressed = false;
			nunPlayer.setDirection(direction::left);
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
			if((nunPlayer.getPosition().x + nunPlayer.getSize().x) < static_cast<float>(ml.getMapSize().x)){
				playerMovement.x += nunPlayer.getSpeed();
			}
			noKeyWasPressed = false;
			nunPlayer.setDirection(direction::right);
		}
		nunPlayer.play();
		nunPlayer.move(playerMovement * frameTime2.asSeconds());


		sf::Vector2f cameraMovement(0.f, 0.f);
		// if no key was pressed stop the animation
		if (noKeyWasPressed) {
			nunPlayer.stop();
		}else{
			//On gère le scrolling
			sf::View view = renderWindow.getView();
			cameraMovement = centerScrolling(	mapSize,
												view,
												cameraInhibitionSize,
												cameraInhibitionRectShape,
												nunPlayer);
			renderWindow.setView(view);
		}

		noKeyWasPressed = true;

		// update Player
		nunPlayer.update(frameTime2);
        playerLimitRectangle.move(playerMovement  * frameTime2.asSeconds());

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

