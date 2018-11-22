#include "Map.h"

const std::string CMap::collisionLayerName = "solidObject";
const std::string CMap::warpLayerName = "warpObject";
const std::string CMap::terrainModifLayerName = "terrainModif";


CMap::CMap(sf::Vector2f screenDimensions, std::string pathName, std::string startingMapFileName):
	m_screenDimensions(screenDimensions),
	m_pathName(pathName),
	m_actualMapFileName(startingMapFileName),
	m_mapLoader(pathName),
	m_cameraInhibitionRectShape(sf::Vector2f(CAMERA_INHIBITION_WIDTH, CAMERA_INHIBITION_HEIGHT)),
	m_warpData()
{
    // Load Map
    if (!m_mapLoader.load(m_actualMapFileName)){
		printf("CMap::CMap - map %s/%s not found\r\n",
				m_pathName.c_str(),
				m_actualMapFileName.c_str());
		throw;// TODO (Aurel#1#): Verifier throw comme erreur de retour CTOR
	}else{
		printf("CMap::CMap - map %s/%s loaded\r\n",
				m_pathName.c_str(),
				m_actualMapFileName.c_str());
	}

	// Camera limit Rectangle
	//------------------------
	m_cameraInhibitionRectShape.setFillColor(sf::Color::Transparent);
	m_cameraInhibitionRectShape.setOutlineColor(sf::Color::Red);
	m_cameraInhibitionRectShape.setOutlineThickness(2.f);
	m_cameraInhibitionRectShape.setPosition((m_screenDimensions - m_cameraInhibitionRectShape.getSize()) / 2.f);

}

CMap::~CMap()
{
	//dtor
}

CMap::errorCode_t CMap::loadMap(std::string mapFileName)
{
    // Load Map
    if (!m_mapLoader.load(mapFileName)){
		printf("CMap::loadMap - map %s/%s not found\r\n",
				m_pathName.c_str(),
				mapFileName.c_str());
		return errorCode_t::mapNotFound;
	}else{
		printf("CMap::CMap - map %s/%s loaded\r\n",
				m_pathName.c_str(),
				mapFileName.c_str());

		displayLayerInfos();

		return errorCode_t::ok;
	}
}

// display Layer Info
void CMap::displayLayerInfos(void)
{
	auto& layersToCheck = m_mapLoader.getLayers();
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
}

sf::Vector2u CMap::getMapSize() const
{
	return m_mapLoader.getMapSize();
}

sf::Vector2f CMap::getCameraInhibitionRectSize() const
{
	return m_cameraInhibitionRectShape.getSize();
}

void CMap::moveCameraInhibitionRect(sf::Vector2f movement)
{
	m_cameraInhibitionRectShape.move(movement);
}


sf::Vector2f CMap::getWarpPointPosition(std::string warpPointName) const
{
	auto& layersToCheck = m_mapLoader.getLayers();
	for(auto& layerInd : layersToCheck)
	{
		if(layerInd.type == tmx::ObjectGroup)
		{
			// Collisions
			if(layerInd.name == "warpObject")
			{
				for(const auto& obj : layerInd.objects)
				{
					if(obj.getName() == warpPointName){
						//handle collision
						printf("CMap::getWarpPointPosition %0.2f  %0.2f\r\n",
								obj.getCentre().x,
								obj.getCentre().y);
						return obj.getCentre();
					}
				}
			}
		}
	}
}

// Test testInteraction
CMap::interractionType_t CMap::testInteraction(	CPlayer& player,
												const sf::Vector2f& movt)
{
	sf::Rect<float> futurRect(	player.getRect().left,
								player.getRect().top,
								player.getRect().width,
								player.getRect().height);
//	printf(" CMap::testInteraction: Mvt %0.2f, %0.2f\r\n", movt.x, movt.y);

	if(player.getDirection() == direction_t::down){
		futurRect.top = player.getRect().top + movt.x + 3;
	}
	if(player.getDirection() == direction_t::up){
		futurRect.top = player.getRect().top + movt.x - 3;
	}

	if(player.getDirection() == direction_t::left){
		futurRect.left = player.getRect().left + movt.y - 3;
	}
	if(player.getDirection() == direction_t::right){
		futurRect.left = player.getRect().left + movt.y + 3;
	}


	auto& layersToCheck = m_mapLoader.getLayers();
	for(auto& layerInd : layersToCheck)
	{
		if(layerInd.type == tmx::ObjectGroup)
		{
			// Collisions
			if(layerInd.name == collisionLayerName)
			{
				for(auto& obj : layerInd.objects)
				{
					if(futurRect.intersects(obj.getAABB())){
						//handle collision
						return interractionType_t::collision;
					}
				}

			}

			// warpObject
			if(layerInd.name == warpLayerName){
				for(auto& obj : layerInd.objects)
				{
					if(futurRect.contains(obj.getCentre())){
						// Define WarpPoint
						m_warpData.setMapToLoad(obj.getPropertyString("mapToLoad"));
						m_warpData.setWarpPointName(obj.getPropertyString("warpPoint"));

						printf("warpObject: %s (mapToLoad '%s', warpPoint '%s'\r\n",
								obj.getName().c_str(),
								m_warpData.getMapToLoad().c_str(),
								m_warpData.getWarpPointName().c_str());


						return interractionType_t::warp;
					}
				}
			}

			// Terrain Modification
			if(layerInd.name == terrainModifLayerName){
				return interractionType_t::terain;
			}
		}
	}
	return interractionType_t::none;
}

void CMap::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(m_mapLoader);
}

void CMap::drawLayer(sf::RenderTarget& rt, tmx::MapLayer::DrawType type)
{
	m_mapLoader.drawLayer(rt, type, true);
}

void CMap::drawCameraInhibitRect(sf::RenderTarget& target)
{
	target.draw(m_cameraInhibitionRectShape);
}
