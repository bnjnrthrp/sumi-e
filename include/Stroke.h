#ifndef STROKE_H
#define STROKE_H
#include <vector>
#include "Brush.h"

struct strokeData
{
	float stroke_position[2];
	float stroke_pressure;
};

class Stroke
{
public:
	Stroke();
	~Stroke();

	void addStroke(const Brush& brush, const float pressure);

private:
	std::vector<strokeData> m_stroke;
};

#endif // STROKE_H