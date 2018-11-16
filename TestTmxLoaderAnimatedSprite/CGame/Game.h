#ifndef CGAME_H
#define CGAME_H

#include <tmx/MapLoader.hpp>
#include <tmx/Log.hpp>
#include "Player.h"

class CGame
{
public:
		CGame();
		virtual ~CGame();

		void loopFn(void);

protected:

private:
	bool m_showDebug;
	bool handleWindowEvent(sf::RenderWindow& renderWindow, CPlayer& player);

};

#endif // CGAME_H
