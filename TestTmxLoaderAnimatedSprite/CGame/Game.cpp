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
	m_nunPlayer(48, 48, CPlayer::speedHero, "sprites/player.png", m_screenDimensions / 2.f),
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

	// player late Init
	//------------------
	sf::Vector2f startPosition = mapGetWarpStartPosition();
	m_nunPlayer.setCenter(startPosition);

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
	testInteraction(m_mapLoader);

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
		m_nunPlayer.setDirection(direction_t::up);
	}else
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))	{
		if((m_nunPlayer.getPosition().y + m_nunPlayer.getSize().y) < static_cast<float>(m_mapLoader.getMapSize().y)){
			m_playerMovement.y += m_nunPlayer.getSpeed();
		}
		m_noKeyWasPressed = false;
		m_nunPlayer.setDirection(direction_t::down);
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))	{
		if(m_nunPlayer.getPosition().x > 0){
			m_playerMovement.x -= m_nunPlayer.getSpeed();
		}
		m_noKeyWasPressed = false;
		m_nunPlayer.setDirection(direction_t::left);
	}else
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
		if((m_nunPlayer.getPosition().x + m_nunPlayer.getSize().x) < static_cast<float>(m_mapLoader.getMapSize().x)){
			m_playerMovement.x += m_nunPlayer.getSpeed();
		}
		m_noKeyWasPressed = false;
		m_nunPlayer.setDirection(direction_t::right);
	}


}


void CGame::update()
{
	sf::Time frameTime = m_frameClock.restart();

	m_nunPlayer.play();


	// update View
	sf::Vector2f cameraMovement(0.f, 0.f);
	// if no key was pressed stop the animation
	if (m_noKeyWasPressed) {
		m_nunPlayer.stop();
	}else{
		sf::View view = m_renderWindow.getView();
//		if(testInteraction2(m_mapLoader, m_playerMovement + m_nunPlayer.getCenter()) == interractionType_t::colision){
		if(testInteraction2(m_mapLoader,
							m_nunPlayer,
							m_playerMovement) == interractionType_t::collision){
			printf(" Collision\r\n");
			m_playerMovement = sf::Vector2f(0.f,0.f);
		}
		m_nunPlayer.move(m_playerMovement * frameTime.asSeconds());
		//On gère le scrolling
		cameraMovement = centerScrolling(	m_mapLoader.getMapSize(),
											view,
											m_cameraInhibitionRectShape,
											m_nunPlayer);
		m_renderWindow.setView(view);
	}

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


void CGame::render()
{
	m_renderWindow.clear();
	m_renderWindow.draw(m_mapLoader);
	m_renderWindow.draw(m_nunPlayer);
	m_renderWindow.draw(m_hudBG, m_hudText.getTransform());
	m_renderWindow.draw(m_hudText);
	m_renderWindow.draw(m_cameraInhibitionRectShape);
	m_mapLoader.drawLayer(m_renderWindow, tmx::MapLayer::Debug);//draw with debug information shown
	m_renderWindow.display();

}





// Test testInteraction
bool CGame::testInteraction(tmx::MapLoader & ml)
{
	const auto& layersToCheck = ml.getLayers();
	for(auto& layerInd : layersToCheck)
	{
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
	}
	return true;
}

// Test testInteraction
CGame::interractionType_t CGame::testInteraction2(	tmx::MapLoader & ml,
													CPlayer& player,
													sf::Vector2f& movt)
{
	sf::Rect<float> futurRect(	player.getRect().left,
								player.getRect().top,
								player.getRect().width,
								player.getRect().height);

//	if(player.getDirection() == direction_t::down){
//		futurRect.top = player.getRect().top + 2;
////		futurRect.top = player.getRect().top + movt.x;
//	}
//	if(player.getDirection() == direction_t::up){
//		futurRect.top = player.getRect().top - 1;
////		futurRect.top = player.getRect().top + movt.x;
//	}
//
//	if(player.getDirection() == direction_t::left){
//		futurRect.left = player.getRect().left - 2;
////		futurRect.left = player.getRect().left + movt.y;
//	}
//	if(player.getDirection() == direction_t::right){
//		futurRect.left = player.getRect().left + 1;
////		futurRect.left = player.getRect().left + movt.y;
//	}
	if(movt.x > 0){
		futurRect.top = player.getRect().top + 2;
	}else if(movt.x < 0){
		futurRect.top = player.getRect().top - 2;
	}

	if(movt.y > 0){
		futurRect.left = player.getRect().left + 2;
	}else if(movt.y < 0){
		futurRect.left = player.getRect().left - 2;
	}


	const auto& layersToCheck = ml.getLayers();
	for(const auto& layerInd : layersToCheck)
	{
		if(layerInd.type == tmx::ObjectGroup)
		{
			// Collisions
			if(layerInd.name == "solidObject")
			{
				for(const auto& obj : layerInd.objects)
				{
					if(futurRect.intersects(obj.getAABB())){
						//handle collision
						return interractionType_t::collision;
					}
				}

				// Trigger
				if(layerInd.name == "trigger"){
					//send trigger command to queue
					return interractionType_t::warp;
				}

				// Terrain Modification
				if(layerInd.name == "terrainModif"){
					return interractionType_t::terain;
				}
			}
		}
	}
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



sf::Vector2f CGame::mapGetWarpStartPosition(void)
{
	const auto& layersToCheck = m_mapLoader.getLayers();
	for(const auto& layerInd : layersToCheck)
	{
		if(layerInd.type == tmx::ObjectGroup)
		{
			// Collisions
			if(layerInd.name == "warpObject")
			{
				for(const auto& obj : layerInd.objects)
				{
					if(obj.getName() == "start1"){
						//handle collision
						return obj.getCentre();
					}
				}
			}
		}
	}
}
