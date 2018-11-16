#include "Game.h"

CGame::CGame():
	m_showDebug(false)
{
	//ctor
}

CGame::~CGame()
{
	//dtor
}


void CGame::loopFn(void)
{


}



bool CGame::handleWindowEvent(sf::RenderWindow& renderWindow, CPlayer& player)
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

		if(event.type == sf::Event::KeyReleased){
			switch(event.key.code)
			{
			case sf::Keyboard::D:
				m_showDebug = !m_showDebug;
				break;
			default: break;
			}
		}
   }
}
