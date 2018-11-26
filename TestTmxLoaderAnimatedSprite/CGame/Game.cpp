//
// https://github.com/SFML/SFML/wiki/Tutorial:-Using-View
//


#include "Game.h"



// Ctor
CGame::CGame(std::string gameName, std::string mapPathName, std::string startingMapFileName):
	m_gameName(gameName),
	m_screenDimensions(SCREEN_WIDTH, SCREEN_HEIGHT),
	m_renderWindow(sf::VideoMode(m_screenDimensions.x, m_screenDimensions.y), m_gameName),
	m_showDebug(false),
	m_map(m_screenDimensions, mapPathName, startingMapFileName),
//	m_nunPlayer(80, 64, CPlayer::speedHero, "sprites/angel.png", m_screenDimensions / 2.f),
//	m_nunPlayer(48, 48, CPlayer::speedHero, "sprites/citizen1.png", m_screenDimensions / 2.f),
//	m_nunPlayer(24, 32, CPlayer::speedHero, "sprites/Nun/Char/Nun Sex A.png", m_screenDimensions / 2.f),
	m_nunPlayer(48, 48, CPlayer::speedHero, "sprites/player.png", m_screenDimensions / 2.f),
	m_sucub("Sucub", 56, 48, CMonster::speedNormal, "sprites/evilSchtroumpf.png"),
	m_playerMovement(0.f, 0.f),
	m_comingFromWrap(true),
	m_disableInput(false),
	m_minimapView(sf::FloatRect(m_renderWindow.getView().getCenter().x,
								m_renderWindow.getView().getCenter().y,
								200.f,
								static_cast<float>(m_renderWindow.getSize().y*200.f/m_renderWindow.getSize().x)))

{

	tmx::setLogLevel(tmx::Logger::Info | tmx::Logger::Error);//set the debugging output mode

	// player late Init
	//------------------
	sf::Vector2f startPosition = m_map.getWarpPointPosition("start1");
	m_nunPlayer.setCenter(startPosition);

	// monster1 late Init
	//------------------
	sf::Vector2f monster1StartPosition = m_map.getWarpPointPosition("monster1");
	m_sucub.setCenter(monster1StartPosition);
//	m_actualMonstersList.push_back(CMonster(	"Sucub",
//												48, 48,
//												4,
//												sf::Vector2f(0,0),
//												sf::Vector2f(0,0),
//												CMonster::speedNormal,
//												"sprites/rcrAmrF.png",
//												monster1StartPosition));
//	printf("CGame::CGame - m_actualMonstersList.size: %d\r\n", m_actualMonstersList.size());
//
//	for (std::vector<CMonster>::iterator it = m_actualMonstersList.begin() ; it != m_actualMonstersList.end(); ++it)
//	{
//			printf("CGame::CGame - Monster '%s' : \r\n", it->getName().c_str());
//	}

	// m_renderWindow late init
	//--------------------------
	m_renderWindow.setFramerateLimit(60);

	// HUD Initialisation  //TODO (Aurel#1#): Ajouter à la Classe CHud
	//--------------------------
	if (!m_hudFont.loadFromFile("fonts/Ubuntu-M.ttf")){
		printf("CGame::CGame - font not loaded\r\n");
		throw;// TODO (Aurel#1#): Verifier throw comme erreur de retour CTOR
	}
	m_hudText.setFont(m_hudFont);
	m_hudText.setColor(sf::Color::White);
	m_hudText.setCharacterSize(14);
	m_hudBG.setSize(sf::Vector2f((m_hudText.getGlobalBounds().width + 10), (m_hudText.getGlobalBounds().height +10)));
	m_hudBG.setFillColor(sf::Color::Black);





//	m_fixedView = m_renderWindow.getView(); // The 'fixed' view will never change
//
//	m_standardView = m_fixedView; // The 'standard' view will be the one that gets always displayed
//
//	unsigned int size = 200; // The 'minimap' view will show a smaller picture of the map
//	m_minimap(sf::FloatRect(	standard.getCenter().x,
//									standard.getCenter().y,
//									static_cast<float>(size),
//									static_cast<float>(m_renderWindow.getSize().y*size/m_renderWindow.getSize().x)));
//	minimap.setViewport(sf::FloatRect(	1.f-static_cast<float>(minimap.getSize().x)/m_renderWindow.getSize().x-0.02f,
//										1.f-static_cast<float>(minimap.getSize().y)/m_renderWindow.getSize().y-0.02f,
//										static_cast<float>(minimap.getSize().x)/m_renderWindow.getSize().x,
//										static_cast<float>(minimap.getSize().y)/m_renderWindow.getSize().y));
//	minimap.zoom(4.f);
//
//	// The 'left' and the 'right' view will be used for splitscreen displays
//	sf::View left(sf::FloatRect(0.f, 0.f, static_cast<float>(m_renderWindow.getSize().x/2), static_cast<float>(m_renderWindow.getSize().y)));
//	left.setViewport(sf::FloatRect(0.f, 0.f, 0.5, 1.f));
//	sf::View right(sf::FloatRect(0.f, 0.f, static_cast<float>(m_renderWindow.getSize().x/2), static_cast<float>(m_renderWindow.getSize().y)));
//	right.setViewport(sf::FloatRect(0.5, 0.f, 0.5, 1.f));
//	right.move(100.f, 100.f); // The 'right' view should be set a bit diffrent to notice the difference
//
//	sf::RectangleShape miniback; // We want to draw a rectangle behind the minimap
//	miniback.setPosition(minimap.getViewport().left*m_renderWindow.getSize().x-5, minimap.getViewport().top*m_renderWindow.getSize().y-5);
//	miniback.setSize(sf::Vector2f(minimap.getViewport().width*m_renderWindow.getSize().x+10, minimap.getViewport().height*m_renderWindow.getSize().y+10));
//	miniback.setFillColor(sf::Color(160, 8, 8));
//
//	sf::Text position;
//	position.setString("<0, 0> - <0, 0>"); // The text will contain the position of the cursor
//	position.setPosition(10.f, 10.f);
//	position.setColor(sf::Color::White);

}

// Dtor
CGame::~CGame()
{
}


//-------------------------
// 		Main Function
//-------------------------
void CGame::run(void)
{
	//TEMP
	m_map.displayLayerInfos();

	while(m_renderWindow.isOpen() == true)
	{
		processEvents();

		sf::Time frameTime = m_frameClock.restart();
		updatePlayer(frameTime);
		updateMonsters(frameTime);
		render();
	}
}



void CGame::processEvents(void)
{
	static bool key_O_StilPressed = false;
	sf::Event event;
	// TODO (Aurel#1#): Voir le traitement des inputs (event, Keyboard etc...)
	while(m_renderWindow.pollEvent(event))
	{
		if(event.type == sf::Event::Closed)
			m_renderWindow.close();

		// Key pressed
		if(event.type == sf::Event::KeyPressed){
			switch (event.key.code)
			{
			case sf::Keyboard::Escape:
				m_renderWindow.close();
				break;

			default:
				break;
			}
		}

		// Key released
		if(event.type == sf::Event::KeyReleased){
			switch(event.key.code){
			case sf::Keyboard::D:
				m_showDebug = !m_showDebug;
				break;
			case sf::Keyboard::O:
				m_nunPlayer.toggleEyes();
				break;
			case sf::Keyboard::S:
				m_sucub.nextAnim();
				break;
			default: break;
			}

			// The key is released, the Disable input when comming from a warp position is not needed anymore
			m_disableInput = false;
		}
	}

	m_playerMovement = sf::Vector2f(0.f,0.f);

	// Disable input when comming from a warp position
	if(m_disableInput == false)
	{
		// if a key was pressed set the correct animation and move correctly
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
			if(m_nunPlayer.getPosition().y > 0){
				m_playerMovement.y -= m_nunPlayer.getSpeed();
			}
			m_noKeyWasPressed = false;
			m_nunPlayer.setDirection(direction_t::up);
		}else
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))	{
			if((m_nunPlayer.getPosition().y + m_nunPlayer.getSize().y) < static_cast<float>(m_map.getMapSize().y)){
				m_playerMovement.y += m_nunPlayer.getSpeed();
			}
			m_noKeyWasPressed = false;
			m_nunPlayer.setDirection(direction_t::down);
		}else
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))	{
			if(m_nunPlayer.getPosition().x > 0){
				m_playerMovement.x -= m_nunPlayer.getSpeed();
			}
			m_noKeyWasPressed = false;
			m_nunPlayer.setDirection(direction_t::left);
		}else
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
			if((m_nunPlayer.getPosition().x + m_nunPlayer.getSize().x) < static_cast<float>(m_map.getMapSize().x)){
				m_playerMovement.x += m_nunPlayer.getSpeed();
			}
			m_noKeyWasPressed = false;
			m_nunPlayer.setDirection(direction_t::right);
		}
	}

}


void CGame::updatePlayer(sf::Time& frameTime)
{

	m_nunPlayer.play();


	// update View
	sf::Vector2f cameraMovement(0.f, 0.f);
	sf::View view = m_renderWindow.getView();
	// if no key was pressed stop the animation
	if (m_noKeyWasPressed) {
		m_nunPlayer.stop();
	}else{
		CMap::interractionType_t inter = m_map.testInteraction(m_nunPlayer, m_playerMovement * frameTime.asSeconds());
		if(inter == CMap::interractionType_t::collision)
			{
			printf(" Collision\r\n");
			m_playerMovement = sf::Vector2f(0.f,0.f);
		}
		else if(inter == CMap::interractionType_t::warp)
		{
			if(m_comingFromWrap == false){
				CWarpData warpData = m_map.getWarpData();
				if(warpData.getMapToLoad() != ""){
					m_map.loadMap(warpData.getMapToLoad());
					m_nunPlayer.setCenter(m_map.getWarpPointPosition(warpData.getWarpPointName()));
					m_playerMovement = sf::Vector2f(0.f,0.f);
					printf(" Warping\r\n");
					m_comingFromWrap = true;
					m_disableInput = true;
				}
			}
		}
		else
		{
//			printf(" None\r\n");
			m_comingFromWrap = false;
		}
		m_nunPlayer.move(m_playerMovement * frameTime.asSeconds());
		//On gère le scrolling
	}
	cameraMovement = centerScrolling(	m_map.getMapSize(),
										view,
										m_nunPlayer);
	m_renderWindow.setView(view);

	m_noKeyWasPressed = true;

	// update Player
	m_nunPlayer.update(frameTime);

	// update HUD (fps, player position)
	float fpsCount = (1.f / frameTime.asSeconds());
	m_hudText.setString( 	"FPS:" + (std::to_string(fpsCount)).substr(0,5)
						+ " x:" + (std::to_string(m_nunPlayer.getPosition().x)).substr(0,5)
						+ " y:" + (std::to_string(m_nunPlayer.getPosition().y)).substr(0,5));
	m_hudText.move(cameraMovement);
	m_hudBG.setSize(sf::Vector2f((m_hudText.getGlobalBounds().width + 10), (m_hudText.getGlobalBounds().height +10)));
}


void CGame::updateMonsters(sf::Time& frameTime)
{
	m_sucub.play();
	m_sucub.update(frameTime);
}


void CGame::render()
{
	m_renderWindow.clear();
	m_renderWindow.draw(m_map);
	m_renderWindow.draw(m_nunPlayer);
//	for (std::vector<CMonster>::iterator it = m_actualMonstersList.begin() ; it != m_actualMonstersList.end(); ++it)
//	{
		m_renderWindow.draw(m_sucub);
//	}
	m_renderWindow.draw(m_hudBG, m_hudText.getTransform());
	m_renderWindow.draw(m_hudText);
//	m_renderWindow.setView(fixed); // Draw 'GUI' elements with fixed positions
//    m_renderWindow.draw(position);
//    m_renderWindow.draw(miniback);

//    m_renderWindow.setView(m_minimapView); // Draw minimap
	m_map.drawCameraInhibitRect(m_renderWindow);
	m_map.drawLayer(m_renderWindow, tmx::MapLayer::Debug);//draw with debug information shown
	m_renderWindow.display();

}







// TODO (Aurel#1#): A deplacer dans la classe CMap ou CCamera???
sf::Vector2f CGame::centerScrolling(const sf::Vector2u& actualMapSize,
									sf::View& actualView,
									CPlayer& player)
{
	float xLimMin = actualView.getCenter().x - m_map.getCameraInhibitionRectSize().x / 2.f;
	float xLimMax = actualView.getCenter().x + m_map.getCameraInhibitionRectSize().x / 2.f;

	float yLimMin = actualView.getCenter().y - m_map.getCameraInhibitionRectSize().y / 2.f;
	float yLimMax = actualView.getCenter().y + m_map.getCameraInhibitionRectSize().y / 2.f;

	sf::Vector2f viewMoving(0.f, 0.f);

	// 	Moving camera within inhibit Rectangle
	//------------------------------------------
	if (player.getCenter().x < (actualView.getCenter().x - actualView.getSize().x /4.f)){
		viewMoving.x = -15.f;
	}
	else if (player.getCenter().x < xLimMin){
		viewMoving.x = - 3.f;
	}

	if (player.getCenter().x > (actualView.getCenter().x + actualView.getSize().x /4.f)){
		viewMoving.x = 15.f;
	}
	else if (player.getCenter().x > xLimMax){
		viewMoving.x = 3.f;
	}

	if (player.getCenter().y < (actualView.getCenter().y - actualView.getSize().y /4.f)){
		viewMoving.y = - 15.f;
	}
	else if (player.getCenter().y < yLimMin){
		viewMoving.y = - 3.f;
	}

	if (player.getCenter().y > (actualView.getCenter().y + actualView.getSize().y /4.f)){
		viewMoving.y = 15.f;
	}
	else if (player.getCenter().y > yLimMax){
		viewMoving.y = 3.f;
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
	m_map.moveCameraInhibitionRect(viewMoving);

	return viewMoving;
}



