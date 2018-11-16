#include "Map.h"

CMap::CMap(std::string pathName, std::string fileName):
	m_pathName(pathName),
	m_fileName(fileName),
	m_mapLoader(pathName)
{
	m_mapLoader.load(m_fileName);
    m_size = m_mapLoader.getMapSize();
    m_layers = m_mapLoader.getLayers();
}

CMap::~CMap()
{
	//dtor
}


