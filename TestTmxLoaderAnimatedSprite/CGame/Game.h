#ifndef CGAME_H
#define CGAME_H

#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <tmx/MapLoader.hpp>
#include <tmx/Log.hpp>
#include "Player.h"

class CGame
{
public:
		CGame();
		virtual ~CGame();

		void run(void);

protected:

private:
	sf::Vector2f m_screenDimensions;
	sf::RenderWindow m_renderWindow;
	bool m_showDebug;

	CPlayer m_nunPlayer;

	void processEvents(void);
	void update(void);
	void render(void);
};

#endif // CGAME_H
