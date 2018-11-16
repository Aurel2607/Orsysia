#ifndef CMAP_H
#define CMAP_H

#include <tmx/MapLoader.hpp>


class CMap
{
	public:
		CMap(std::string pathName, std::string fileName);
		virtual ~CMap();

	protected:

	private:
	std::string 				m_pathName;
	std::string 				m_fileName;

	tmx::MapLoader 				m_mapLoader;
	sf::Vector2u 				m_size;
	std::vector<tmx::MapLayer>& m_layers;
};

#endif // CMAP_H
