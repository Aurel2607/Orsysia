#ifndef CMAP_H
#define CMAP_H

#include <tmx/MapLoader.hpp>
#include "Player.h"
#include "../main.h"


class CMap : public sf::Drawable
{
public:
	enum interractionType_t{
		none = 0,
		collision,
		warp,
		terain
	};

	CMap(std::string pathName, std::string startingMapFileName);
	virtual ~CMap();

	void displayLayerInfos(void);
	sf::Vector2u getMapSize() const;


	interractionType_t testInteraction(	CPlayer& player,
										sf::Vector2f& movt);

protected:

private:
	std::string 	m_pathName;
	std::string 	m_actualMapFileName;
	tmx::MapLoader 	m_mapLoader;

private:
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

public:
	void drawCameraInhibitRect(sf::RenderTarget& rt);
	void drawLayer(sf::RenderTarget& rt, tmx::MapLayer::DrawType type);
};

#endif // CMAP_H
