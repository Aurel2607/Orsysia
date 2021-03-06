#ifndef CGAME_H
#define CGAME_H

#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <tmx/MapLoader.hpp>
#include <tmx/Log.hpp>
#include "Player.h"
#include "Map.h"
#include "EvilSchtroumpf.h"
#include <vector>

class CGame
{
public:
		CGame(std::string gameName, std::string mapPathName, std::string startingMapFileName);
		virtual ~CGame();

		void run(void);

protected:

private:
	std::string m_gameName;

	sf::Vector2f m_screenDimensions;
	sf::RenderWindow m_renderWindow;
	bool m_showDebug;

	CMap 					m_map;
	CPlayer 				m_nunPlayer;
//	std::vector<CMonster> 	m_actualMonstersList;
	CEvilSchtroumpf			m_evilSchtroumpf;

	// TODO (Aurel#1#): Ajouter � la Classe CHud
	// HUD Text
	//-----------
	sf::Font m_hudFont;
	sf::Text m_hudText;
	sf::RectangleShape m_hudBG;


	// Input
	bool m_noKeyWasPressed;

	sf::Clock m_frameClock;

	sf::Vector2f m_playerMovement;

	bool m_comingFromWrap;
	bool m_disableInput;

	sf::View m_minimapView;
//	sf::view m_fixedView;
//	sf::View m_standardView

private:
	void processEvents(void);
	void updatePlayer(sf::Time& frameTime);
	void updateMonsters(sf::Time& frameTime);
	void render(void);

private:
	sf::Vector2f centerScrolling(	const sf::Vector2u& actualMapSize,
									sf::View& actualView,
									CPlayer& player);

};

#endif // CGAME_H
