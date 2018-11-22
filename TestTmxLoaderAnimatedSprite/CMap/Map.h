#ifndef CMAP_H
#define CMAP_H

#include <tmx/MapLoader.hpp>
#include "Player.h"
#include "../main.h"
#include "WarpData.h"


class CMap : public sf::Drawable
{
public:
	enum errorCode_t{
		ok = 0,
		mapNotFound
	};

private:
	static const std::string collisionLayerName;
	static const std::string warpLayerName;
	static const std::string terrainModifLayerName;

public:
	enum interractionType_t{
		none = 0,
		collision,
		warp,
		terain
	};

	CMap(sf::Vector2f screenDimensions,std::string pathName, std::string startingMapFileName);
	virtual ~CMap();

	errorCode_t loadMap(std::string mapFileName);

	void 				displayLayerInfos(void);
	sf::Vector2u 		getMapSize() const;
	sf::Vector2f 		getWarpPointPosition(std::string warpPointName) const;
	const CWarpData 	getWarpData(void) const {return m_warpData;}
	sf::Vector2f 		getCameraInhibitionRectSize() const;
	void 				moveCameraInhibitionRect(sf::Vector2f movement);


	interractionType_t testInteraction(	CPlayer& player,
										sf::Vector2f& movt);

protected:

private:
	sf::Vector2f 		m_screenDimensions;
	std::string 		m_pathName;
	std::string 		m_actualMapFileName;
	tmx::MapLoader 		m_mapLoader;

	sf::RectangleShape 	m_cameraInhibitionRectShape;	// Camera limit Rectangle

	CWarpData			m_warpData;


private:
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

public:
	void drawCameraInhibitRect(sf::RenderTarget& rt);
	void drawLayer(sf::RenderTarget& rt, tmx::MapLayer::DrawType type);
};

#endif // CMAP_H
