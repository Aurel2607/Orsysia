#include "Map.h"

CMap::CMap(std::string pathName, std::string startingMapFileName):
	m_pathName(pathName),
	m_actualMapFileName(startingMapFileName),
	m_mapLoader(pathName)
{
    // Load Map
    if (!m_mapLoader.load(m_actualMapFileName)){
		printf("CMap::CMap - map %s/%s not found\r\n",
				m_pathName.c_str(),
				m_actualMapFileName.c_str());
		throw;// TODO (Aurel#1#): Verifier throw comme erreur de retour CTOR
	}


}

CMap::~CMap()
{
	//dtor
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


// Test testInteraction
CMap::interractionType_t CMap::testInteraction(	CPlayer& player,
												sf::Vector2f& movt)
{
	sf::Rect<float> futurRect(	player.getRect().left,
								player.getRect().top,
								player.getRect().width,
								player.getRect().height);

	if(player.getDirection() == direction_t::down){
		futurRect.top = player.getRect().top + 2;
//		futurRect.top = player.getRect().top + movt.x;
	}
	if(player.getDirection() == direction_t::up){
		futurRect.top = player.getRect().top - 1;
//		futurRect.top = player.getRect().top + movt.x;
	}

	if(player.getDirection() == direction_t::left){
		futurRect.left = player.getRect().left - 2;
//		futurRect.left = player.getRect().left + movt.y;
	}
	if(player.getDirection() == direction_t::right){
		futurRect.left = player.getRect().left + 1;
//		futurRect.left = player.getRect().left + movt.y;
	}
//	if(movt.x > 0){
//		futurRect.top = player.getRect().top + 2;
//	}else if(movt.x < 0){
//		futurRect.top = player.getRect().top - 2;
//	}
//
//	if(movt.y > 0){
//		futurRect.left = player.getRect().left + 2;
//	}else if(movt.y < 0){
//		futurRect.left = player.getRect().left - 2;
//	}


	auto& layersToCheck = m_mapLoader.getLayers();
	for(auto& layerInd : layersToCheck)
	{
		if(layerInd.type == tmx::ObjectGroup)
		{
			// Collisions
			if(layerInd.name == "solidObject")
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
			if(layerInd.name == "warpObject"){
				// TODO:
				for(auto& obj : layerInd.objects)
				{
					if(futurRect.intersects(obj.getAABB())){
						printf("warpObject: %s (mapToLoad '%s', warpPoint '%s')\r\n",
								obj.getName().c_str(),
//								(obj.getPropertyString(static_cast<const std::string>"mapToLoad")).c_str(),
								obj.getPropertyString("mapToLoad").c_str(),
								obj.getPropertyString("warpPoint").c_str());
						//handle warp
						return interractionType_t::warp;
					}
				}
			}

			// Terrain Modification
			if(layerInd.name == "terrainModif"){
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

