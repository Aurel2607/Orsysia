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
		CGame(std::string gameName, std::string mapPathName, std::string mapFileName);
		virtual ~CGame();

		void run(void);

protected:

private:
	std::string m_gameName;

	sf::Vector2f m_screenDimensions;
	sf::RenderWindow m_renderWindow;
	bool m_showDebug;

	// TODO (Aurel#1#): Ajouter à la Classe CMap
	std::string m_mapPathName;
	std::string m_mapFileName;
	tmx::MapLoader m_mapLoader;

	CPlayer m_nunPlayer;

	// TODO (Aurel#1#): Ajouter à la Classe CHud
	// HUD Text
	//-----------
	sf::Font m_hudFont;
	sf::Text m_hudText;
	sf::RectangleShape m_hudBG;

	// Camera limit Rectangle // TODO (Aurel#1#): A bouger dans la classe CMap
	//------------------------
	sf::RectangleShape m_cameraInhibitionRectShape;


	// Input
	bool m_noKeyWasPressed;

	sf::Clock m_frameClock;

	sf::Vector2f m_playerMovement;

private:
	void processEvents(void);
	void update(void);
	void render(void);

private:
	enum interractionType_t{
		collision = 0,
		warp,
		terain
	};

private:
	bool testInteraction(tmx::MapLoader & ml);

	interractionType_t testInteraction2(tmx::MapLoader & ml,
										CPlayer& player,
										sf::Vector2f& movt);

	sf::Vector2f centerScrolling(const sf::Vector2u& actualMapSize,
								sf::View& actualView,
								sf::RectangleShape&  cameraInhibitionRectangleShape,
								CPlayer& player);

private:
	sf::Vector2f mapGetWarpStartPosition(void);
};

#endif // CGAME_H
