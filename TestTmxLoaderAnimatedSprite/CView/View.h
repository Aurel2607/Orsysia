#ifndef CVIEW_H
#define CVIEW_H

#include "../main.h"

class CView : public sf::Drawable
{
public:
	CView(sf::Vector2f screenDimensions);
	virtual ~CView();

	sf::Vector2f getCenter() const;
	sf::Vector2f getSize() const;
	sf::Vector2f getCameraInhibitionRectSize() const;

	void drawCameraInhibitRect(sf::RenderTarget& target);
protected:

private:
	sf::Vector2f 	m_screenDimensions;
	sf::View 		m_view;

	// Camera limit Rectangle
	sf::RectangleShape 	m_cameraInhibitionRectShape;

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

};

#endif // CVIEW_H
