#ifndef GUI_H
#define GUI_H

#include <vector>
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#include "Brush.h"

class GUI
{
public: 
	void init(GLFWwindow* window, const char* glsl_version);
	void newFrame();
	void mainMenu(Brush& brush, bool& showControlPoints);
	void renderBezier(std::vector<std::vector<ImVec2>>& curves,  std::vector<std::vector<Brush>>& strokes, Brush &brushState, bool& showControlPoints);
	void render();
	void shutdown();
	static void HelpMarker(const char* desc);

private:
	ImVec2 calculateBezierPoints(const std::vector<ImVec2>& controlPoints, float t);
	void drawControlPointsAndLines(std::vector<ImVec2>& curvePt, ImVec2& canvas_topLeft, ImDrawList* draw_list, bool& show);
	void drawBezierCurve(std::vector<ImVec2>& curve, ImVec2& canvas_topLeft, std::vector<Brush>& stroke, ImDrawList* draw_list);
};

#endif // GUI_H

