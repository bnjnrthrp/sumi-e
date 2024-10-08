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
	void updateBrush(Brush& brush);
	void renderBezier(std::vector<std::vector<ImVec2>>& curves,  std::vector<std::vector<Brush>>& strokes, Brush &brushState);
	void render();
	void shutdown();
	void render_conan_logo();
	static void HelpMarker(const char* desc);

private:
	ImVec2 calculateBezierPoints(const std::vector<ImVec2>& control_points, float t);
};

#endif // GUI_H

