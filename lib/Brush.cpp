#include "Brush.h"
#include "GUI.h"
#include <iostream>




// Constructor / Deconstructor --------------------------------
Brush::Brush(int size, float pressure, float tipColor, float midColor, float baseColor, float wetness, float angle)
	: m_brushSize(size),
	  m_pressure(pressure),
	  m_colors{tipColor, midColor, baseColor},
	  m_wetness(wetness),
	  m_angle(angle)
{}
Brush::~Brush(){}

// Public Methods ----------------------------

void Brush::print()
{
	std::cout << "# Bristles: " << m_brushSize << std::endl
		<< "Pressure: " << m_pressure << std::endl
		<< "Colors: " << std::endl
		<< "tip \t mid \t base" << std::endl
		<< m_colors[0] << "\t" << m_colors[1] << "\t" << m_colors[2] << std::endl
		<< "Wetness: " << m_wetness << std::endl
		<< "Angle: " << m_angle << std::endl;
}

void Brush::lerpBrush(float a, Brush& prev, Brush& curr)
{	

	// Does not adjust size
	float lerpColor1 = (1.0f - a) * (prev.m_colors[0]) + (a) * (curr.m_colors[0]);
	float lerpColor2 = (1.0f - a) * (prev.m_colors[1]) + (a) * (curr.m_colors[1]);
	float lerpColor3 = (1.0f - a) * (prev.m_colors[2]) + (a) * (curr.m_colors[2]);



	m_brushSize = prev.m_brushSize;
	m_pressure = (1.0f - a) * (prev.m_pressure) + (a) * (curr.m_pressure);
	m_colors[0] = lerpColor1;
	m_colors[1] = lerpColor2;
	m_colors[2] = lerpColor3;
	m_wetness = (1.0f - a) * (prev.m_wetness) + (a) * (curr.m_wetness);
	m_angle = (1.0f - a) * (prev.m_angle) + (a) * (curr.m_angle);
}

/*
* Finds the 4 quad points for a line given a start and stop point. Will do a CCW winding.
*/
void Brush::calculateQuad(ImVec2& prev, ImVec2& curr, component component, ImVec2& q1, ImVec2& q2, ImVec2& q3, ImVec2& q4)
{
	float dx, dy, tip, mid, base;
	
	// Relative size of quad
	tip = .33f;
	mid = .67f;
	base = 1.0f;
	float t = (float)m_brushSize / 2.0f;
	float scale =	(component == BRUSH_TIP) ? tip :
					(component == BRUSH_MID) ? mid :
					base;

	dy = curr.y - prev.y;
	dx = curr.x - prev.x;
	// Calculate the slope and its inverse
	// Normalize them both

	// Determine each point as a function of p + (componentFactor * t) * v, were
	// Component factor is the t multiplier for base (1.0), mid (.67), and tip (.33)
	// t is scalar, which is maxed at the size of half the brush (brushSize / 2)
	// v is the inverse slope at that point, giving us a perpendicular position
	ImVec2 m = curr - prev;
	ImVec2 inverseM(-m.y, m.x);
	normalize(inverseM);

	addParametric(prev, scale, -t, inverseM, q1);
	addParametric(curr, scale, -t, inverseM, q2);
	addParametric(curr, scale, t, inverseM, q3);
	addParametric(prev, scale, t, inverseM, q4);

	
	
	
	#if DEBUG
	std::cout << "slope: (" << m.x << ", " << m.y << ")" << std::endl;
	std::cout << "inverse: (" << inverseM.x << ", " << inverseM.y << ")" << std::endl;
	std::cout << "points: " << std::endl;
	std::cout << "prev: (" << prev.x << ", " << prev.y << ")" << std::endl;
	std::cout << "curr: (" << curr.x << ", " << curr.y << ")" << std::endl;
	std::cout << "Quad points: " << std::endl;
	std::cout << "(" << q1.x << ", " << q1.y << ")" << std::endl;
	std::cout << "(" << q2.x << ", " << q2.y << ")" << std::endl;
	std::cout << "(" << q3.x << ", " << q3.y << ")" << std::endl;
	std::cout << "(" << q4.x << ", " << q4.y << ")" << std::endl;
	#endif
	// Find the two edge points of the base, mid, and tip, respectively based on the brush pressure, tip, and size

	
}
// Private Methods ------------------------------
void Brush::addParametric(const ImVec2& pt, const double scale, const double t, const ImVec2& vector, ImVec2& dest)
{
	dest.x = pt.x + scale * t * vector.x;
	dest.y = pt.y + scale * t * vector.y;
}

void Brush::normalize(ImVec2& vector)
{
	double length = sqrt(vector.x * vector.x + vector.y * vector.y);
	vector.x /= length;
	vector.y /= length;
}