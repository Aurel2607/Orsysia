#include "View.h"

CView::CView(sf::Vector2f screenDimensions):
	m_screenDimensions(screenDimensions)
{

	// Camera limit Rectangle
	//------------------------
	m_cameraInhibitionRectShape.setSize(sf::Vector2f(CAMERA_INHIBITION_WIDTH, CAMERA_INHIBITION_HEIGHT));
	m_cameraInhibitionRectShape.setFillColor(sf::Color::Transparent);
	m_cameraInhibitionRectShape.setOutlineColor(sf::Color::Red);
	m_cameraInhibitionRectShape.setOutlineThickness(2.f);
	m_cameraInhibitionRectShape.setPosition((m_screenDimensions - m_cameraInhibitionRectShape.getSize()) / 2.f);

}

CView::~CView()
{
	//dtor
}


sf::Vector2f CView::getSize() const
{
	return m_view.getSize();
}

sf::Vector2f CView::getCameraInhibitionRectSize() const
{
	return m_cameraInhibitionRectShape.getSize();
}

void CView::drawCameraInhibitRect(sf::RenderTarget& target)
{
	target.draw(m_cameraInhibitionRectShape);
}


