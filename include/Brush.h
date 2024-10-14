#ifndef BRUSH_H
#define BRUSH_H

#include "Bristle.h"


static const int DEFAULT_MAX_SIZE = 100;
class Brush

{
public:
	Brush(int size = 50, float pressure = 0.5f, float tipColor = 0.5f, float midColor = 0.5f, float baseColor = 0.5f, float wetness = 0.0f, float angle = 0.0f);
	~Brush();
	int m_brushSize;
	float m_pressure;
	float m_colors[3];
	float m_wetness;
	float m_angle;

	// Getters/Setters
	void lerpBrush(float a, Brush& prev, Brush& curr);
	int getBrushSize();
	void print();
private:
};

#endif // BRUSH_H

