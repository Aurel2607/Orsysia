#include "Game.h"

// Ctor
CGame::CGame():
	m_screenDimensions(SCREEN_WIDTH, SCREEN_HEIGHT),
	m_renderWindow(sf::VideoMode(m_screenDimensions.x, m_screenDimensions.y), "ORSYSIA"),
	m_showDebug(false),
	m_nunPlayer(48, 48, CPlayer::speedHero, "sprites/player.png", m_screenDimensions)
{
}

// Dtor
CGame::~CGame()
{
}


void CGame::run(void)
{
	while(m_renderWindow.isOpen() == true)
	{
		processEvents();
		update();
		render();
	}
}



void CGame::processEvents(void)
{
	static bool key_O_StilPressed = false;
	sf::Event event;
	while(m_renderWindow.pollEvent(event))
	{
		if(event.type == sf::Event::Closed)
			m_renderWindow.close();

		if(event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
			m_renderWindow.close();

		// Gestion de l'appuie touche O (bascule keyPressed/keyReleased)
		if ((event.type == sf::Event::KeyPressed) &&
			(event.key.code == sf::Keyboard::O) &&
			(key_O_StilPressed == false)){
			printf(" sf::Event::KeyPressed::O \r\n");
			m_nunPlayer.toggleEyes();
			key_O_StilPressed = true;
		}

		// Gestion de l'appuie touche O (bascule keyPressed/keyReleased)
		if ((event.type == sf::Event::KeyReleased) &&
			(event.key.code == sf::Keyboard::O) &&
			(key_O_StilPressed == true)) {
			key_O_StilPressed = false;
		}

		if(event.type == sf::Event::KeyReleased){
			switch(event.key.code){
				case sf::Keyboard::D:
					m_showDebug = !m_showDebug;
					break;
				default: break;
			}
		}
   }
}


void CGame::update()
{


}


void CGame::render()
{

}



