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

int Brush::getBrushSize() { return m_brushSize; }
// Private Methods ------------------------------
