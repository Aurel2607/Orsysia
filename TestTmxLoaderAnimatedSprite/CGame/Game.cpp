#include "Game.h"

// Ctor
CGame::CGame(std::string gameName, std::string mapPathName, std::string mapFileName):
	m_gameName(gameName),
	m_screenDimensions(SCREEN_WIDTH, SCREEN_HEIGHT),
	m_renderWindow(sf::VideoMode(m_screenDimensions.x, m_screenDimensions.y), m_gameName),
	m_showDebug(false),
	m_mapPathName(mapPathName),
	m_mapFileName(mapFileName),
	m_mapLoader(m_mapPathName),
	m_layers(m_mapLoader.getLayers()),
	m_nunPlayer(48, 48, CPlayer::speedHero, "sprites/player.png", m_screenDimensions),
//TODO: Retrouver à qui ca appartient??
//	(sf::Vector2f((m_hudText.getGlobalBounds().width + 10), (m_hudText.getGlobalBounds().height +10))),
	m_playerMovement(0.f, 0.f)
{


    // Create map loader and load map
	//-------------------------------
	tmx::setLogLevel(tmx::Logger::Info | tmx::Logger::Error);//set the debugging output mode
    if (!m_mapLoader.load(m_mapFileName)){
		printf("CGame::CGame - map %s/%s not found\r\n",
				m_mapPathName.c_str(),
				m_mapFileName.c_str());
		throw;// TODO (Aurel#1#): Verifier throw comme erreur de retour CTOR
	}

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
//TODO: Retrouver à qui ca appartient??
//	(sf::Vector2f((m_hudText.getGlobalBounds().width + 10), (m_hudText.getGlobalBounds().height +10)))

	// Player limit Rectangle  // TODO (Aurel#1#): A bouger dans la classe CPlayer
	//------------------------
	m_playerLimitRectShape.setSize(m_nunPlayer.getSize());
	m_playerLimitRectShape.setFillColor(sf::Color::Transparent);
	m_playerLimitRectShape.setOutlineColor(sf::Color::Red);
	m_playerLimitRectShape.setOutlineThickness(2.f);
	m_playerLimitRectShape.setPosition(m_nunPlayer.getPosition().x, m_nunPlayer.getPosition().y);

	// Camera limit Rectangle // TODO (Aurel#1#): A bouger dans la classe CMap
	//------------------------
	m_cameraInhibitionRectShape.setSize(sf::Vector2f(CAMERA_INHIBITION_WIDTH, CAMERA_INHIBITION_HEIGHT));
	m_cameraInhibitionRectShape.setFillColor(sf::Color::Transparent);
	m_cameraInhibitionRectShape.setOutlineColor(sf::Color::Red);
	m_cameraInhibitionRectShape.setOutlineThickness(2.f);
	m_cameraInhibitionRectShape.setPosition((m_screenDimensions - m_cameraInhibitionRectShape.getSize()) / 2.f);

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
	testInteraction(m_layers);

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
			default: break;
			}
		}
	}

	m_playerMovement = sf::Vector2f(0.f,0.f);

	// if a key was pressed set the correct animation and move correctly
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
		if(m_nunPlayer.getPosition().y > 0){
			m_playerMovement.y -= m_nunPlayer.getSpeed();
		}
		m_noKeyWasPressed = false;
		m_nunPlayer.setDirection(direction::up);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))	{
		if((m_nunPlayer.getPosition().y + m_nunPlayer.getSize().y) < static_cast<float>(m_mapLoader.getMapSize().y)){
			m_playerMovement.y += m_nunPlayer.getSpeed();
		}
		m_noKeyWasPressed = false;
		m_nunPlayer.setDirection(direction::down);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))	{
		if(m_nunPlayer.getPosition().x > 0){
			m_playerMovement.x -= m_nunPlayer.getSpeed();
		}
		m_noKeyWasPressed = false;
		m_nunPlayer.setDirection(direction::left);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
		if((m_nunPlayer.getPosition().x + m_nunPlayer.getSize().x) < static_cast<float>(m_mapLoader.getMapSize().x)){
			m_playerMovement.x += m_nunPlayer.getSpeed();
		}
		m_noKeyWasPressed = false;
		m_nunPlayer.setDirection(direction::right);
	}


}


void CGame::update()
{
	sf::Time frameTime = m_frameClock.restart();

	m_nunPlayer.play();
	m_nunPlayer.move(m_playerMovement * frameTime.asSeconds());

	// update View
	sf::Vector2f cameraMovement(0.f, 0.f);
	// if no key was pressed stop the animation
	if (m_noKeyWasPressed) {
		m_nunPlayer.stop();
	}else{
		//On gère le scrolling
		sf::View view = m_renderWindow.getView();
		cameraMovement = centerScrolling(	m_mapLoader.getMapSize(),
											view,
											m_cameraInhibitionRectShape,
											m_nunPlayer);
		m_renderWindow.setView(view);
	}

	m_noKeyWasPressed = true;

	// update Player
	m_nunPlayer.update(frameTime);
	m_playerLimitRectShape.move(m_playerMovement  * frameTime.asSeconds());

	// update HUD (fps, player position)
	float fpsCount = (1.f / frameTime.asSeconds());
	m_hudText.setString( 	"FPS:" + (std::to_string(fpsCount)).substr(0,5)
						+ " x:" + (std::to_string(m_nunPlayer.getPosition().x)).substr(0,5)
						+ " y:" + (std::to_string(m_nunPlayer.getPosition().y)).substr(0,5));
	m_hudText.move(cameraMovement);
	m_hudBG.setSize(sf::Vector2f((m_hudText.getGlobalBounds().width + 10), (m_hudText.getGlobalBounds().height +10)));



}


void CGame::render()
{
	m_renderWindow.clear();
	m_renderWindow.draw(m_mapLoader);
	m_renderWindow.draw(m_nunPlayer);
	m_renderWindow.draw(m_hudBG, m_hudText.getTransform());
	m_renderWindow.draw(m_hudText);
	m_renderWindow.draw(m_playerLimitRectShape);
	m_renderWindow.draw(m_cameraInhibitionRectShape);
	m_mapLoader.drawLayer(m_renderWindow, tmx::MapLayer::Debug);//draw with debug information shown
	m_renderWindow.display();

}





// Test testInteraction
bool CGame::testInteraction(std::vector<tmx::MapLayer>& layersToCheck)
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


// TODO (Aurel#1#): A deplacer dans la classe CMap ou CCamera???

sf::Vector2f CGame::centerScrolling(const sf::Vector2u& actualMapSize,
									sf::View& actualView,
									sf::RectangleShape&  camInhibitRectShp,
									CPlayer& player)
{

	int cxperso = static_cast<int>(player.getPosition().x) + player.getWidth() / 2;
	int cyperso = static_cast<int>(player.getPosition().y) + player.getHeight() / 2;

	int xLimMin = static_cast<int>(actualView.getCenter().x - camInhibitRectShp.getSize().x / 2.f);
	int xLimMax = static_cast<int>(actualView.getCenter().x + camInhibitRectShp.getSize().x / 2.f);

	int yLimMin = static_cast<int>(actualView.getCenter().y - camInhibitRectShp.getSize().y / 2.f);
	int yLimMax = static_cast<int>(actualView.getCenter().y + camInhibitRectShp.getSize().y / 2.f);

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
	camInhibitRectShp.move(viewMoving);

	return viewMoving;
}

