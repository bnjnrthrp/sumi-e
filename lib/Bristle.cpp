#include "Bristle.h"
#include <algorithm>

Bristle::Bristle()
	:m_color(0.0f), 
	 m_quantity(0.0f), 
	 m_relPosition{ 0, 0 }
{}
Bristle::~Bristle() {};

// Public functions
void Bristle::updateState() {}
void Bristle::colorPaper(Paper& dest) {}

// Getters/Setters
void Bristle::setColor(const float value)
{
	m_color = std::clamp(value, 0.0f, 1.0f);
}
void Bristle::setQuantity(const float value)
{
	m_quantity = std::clamp(value, 0.0f, 1.0f);
}
void Bristle::setPosition(float x, float y)
{
	m_relPosition[0] = x;
	m_relPosition[1] = y;
}
float Bristle::getColor() const { return m_color; }
float Bristle::getQuantity() const { return m_quantity; }
float Bristle::getPosition() const { return m_relPosition[0]; }
