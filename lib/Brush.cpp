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

// Private Methods ------------------------------
