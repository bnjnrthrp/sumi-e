#ifndef BRUSH_H
#define BRUSH_H

#define IMGUI_DEFINE_MATH_OPERATORS
#include "./imgui/imgui_internal.h"
#include "./imgui/imgui.h"

enum component {
	BRUSH_BASE,
	BRUSH_MID,
	BRUSH_TIP
};

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
	void print();
	void calculateQuad(ImVec2& prev, ImVec2& curr, component component, ImVec2& q1, ImVec2& q2, ImVec2& q3, ImVec2& q4);
private:
	void addParametric(const ImVec2& pt, const double scale, const double t, const ImVec2& vector, ImVec2& dest);
	void normalize(ImVec2& vector);
};

#endif // BRUSH_H

