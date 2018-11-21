#ifndef CWARPDATA_H
#define CWARPDATA_H

#include "../main.h"

class CWarpData
{
public:
	CWarpData(std::string mapToLoad, sf::Vector2f warpPoint);
	virtual ~CWarpData();

	std::string getMapToLoad(void) const {return m_mapToLoad;}
	void setMapToLoad(std::string mapToLoad) {m_mapToLoad = mapToLoad;}
	sf::Vector2f getWarpPoint(void) const {return m_warpPoint;}
	void setWarpPoint(sf::Vector2f warpPoint) {m_warpPoint = warpPoint;}

protected:

private:
	std::string m_mapToLoad;
	sf::Vector2f m_warpPoint;
};

#endif // CWARPDATA_H
