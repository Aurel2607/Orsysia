#ifndef CWARPDATA_H
#define CWARPDATA_H

#include "../main.h"

class CWarpData
{
public:
	CWarpData();
	virtual ~CWarpData();

	std::string getMapToLoad(void) const {return m_mapToLoad;}
	void setMapToLoad(const std::string& mapToLoad) {m_mapToLoad = mapToLoad;}

	std::string getWarpPointName(void) const {return m_warpPointName;}
	void setWarpPointName(const std::string& warpPointName) {m_warpPointName = warpPointName;}

protected:

private:
	std::string m_mapToLoad;
	std::string m_warpPointName;
};

#endif // CWARPDATA_H
