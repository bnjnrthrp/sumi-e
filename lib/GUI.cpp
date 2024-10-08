#include "GUI.h"
#include "imgui/imgui.h"
#include <iostream>

void GUI::init(GLFWwindow *window, const char* glsl_version)
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	// Setup platform and renderer
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init(glsl_version);
	ImGui::StyleColorsDark();
}

void GUI::newFrame(){

	// Feed inputs to imgui, start a new frame
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
}

// from https://github.com/conan-io/examples/blob/master/libraries/dear-imgui/basic/main.cpp
void GUI::render_conan_logo()
{
	ImDrawList* draw_list = ImGui::GetWindowDrawList();

	
	float sz = 300.0f;
	static ImVec4 col1 = ImVec4(68.0 / 255.0, 83.0 / 255.0, 89.0 / 255.0, 1.0f);
	static ImVec4 col2 = ImVec4(40.0 / 255.0, 60.0 / 255.0, 80.0 / 255.0, 1.0f);
	static ImVec4 col3 = ImVec4(50.0 / 255.0, 65.0 / 255.0, 82.0 / 255.0, 1.0f);
	static ImVec4 col4 = ImVec4(20.0 / 255.0, 40.0 / 255.0, 60.0 / 255.0, 1.0f);
	const ImVec2 p = ImGui::GetCursorScreenPos();
	float x = p.x + 4.0f, y = p.y + 4.0f;
	draw_list->AddQuadFilled(ImVec2(x, y + 0.25 * sz), ImVec2(x + 0.5 * sz, y + 0.5 * sz), ImVec2(x + sz, y + 0.25 * sz), ImVec2(x + 0.5 * sz, y), ImColor(col1));
	draw_list->AddQuadFilled(ImVec2(x, y + 0.25 * sz), ImVec2(x + 0.5 * sz, y + 0.5 * sz), ImVec2(x + 0.5 * sz, y + 1.0 * sz), ImVec2(x, y + 0.75 * sz), ImColor(col2));
	draw_list->AddQuadFilled(ImVec2(x + 0.5 * sz, y + 0.5 * sz), ImVec2(x + sz, y + 0.25 * sz), ImVec2(x + sz, y + 0.75 * sz), ImVec2(x + 0.5 * sz, y + 1.0 * sz), ImColor(col3));
	draw_list->AddLine(ImVec2(x + 0.75 * sz, y + 0.375 * sz), ImVec2(x + 0.75 * sz, y + 0.875 * sz), ImColor(col4));
	draw_list->AddBezierCubic(ImVec2(x + 0.72 * sz, y + 0.24 * sz), ImVec2(x + 0.68 * sz, y + 0.15 * sz), ImVec2(x + 0.48 * sz, y + 0.13 * sz), ImVec2(x + 0.39 * sz, y + 0.17 * sz), ImColor(col4), 10, 18);
	draw_list->AddBezierCubic(ImVec2(x + 0.39 * sz, y + 0.17 * sz), ImVec2(x + 0.2 * sz, y + 0.25 * sz), ImVec2(x + 0.3 * sz, y + 0.35 * sz), ImVec2(x + 0.49 * sz, y + 0.38 * sz), ImColor(col4), 10, 18);
}

void GUI::renderBezier(std::vector<std::vector<ImVec2>>& curves,  std::vector<std::vector<Brush>>& strokes, Brush& brushState) // Takes a 2D vector of Curves
{
	ImGui::Begin("Sumi-e Curve Editor");

	int dip = 0; // Initialize so that we do not dip
	if (ImGui::Button("New Stroke")) {
		curves.push_back(std::vector<ImVec2>());
		strokes.push_back(std::vector<Brush>());
		//dip = 1; // Dip the brush and add it to the back of stroke list
		//Brush tmp(brushState.m_brushSize, brushState.m_pressure, brushState.m_wetness, brushState.m_angle);
		//strokes.back().push_back(tmp); 
	}

	if (curves.empty() || strokes.empty())
	{
		ImGui::End();
		return; // Early return if curves is empty, skipping any drawing.
	}
	std::vector<ImVec2>& currCurve = curves.back(); // Grab the last curve
	std::vector<Brush>& currStroke = strokes.back(); // Set the current brush to the last known state

	// Determine canvas size and dimensions
	ImVec2 canvas_topLeft = ImGui::GetCursorScreenPos();
	ImVec2 canvas_size = ImGui::GetContentRegionAvail();
	if (canvas_size.x < 300.0f) canvas_size.x = 300.0f; // Set minimum size of canvas to 300 x 300 square
	if (canvas_size.y < 300.0f) canvas_size.y = 300.0f;
	ImVec2 canvas_bottomRight = ImVec2(canvas_topLeft.x + canvas_size.x, canvas_topLeft.y + canvas_size.y);

	// Draw the canvas background
	ImDrawList* draw_list = ImGui::GetWindowDrawList();
	draw_list->AddRectFilled(canvas_topLeft, canvas_bottomRight, IM_COL32(30, 30, 30, 255));// Background color
	draw_list->AddRect(canvas_topLeft, canvas_bottomRight, IM_COL32(220, 220, 220, 255));	// Canvas color

	// Mouse click listener to add control points
	ImGui::InvisibleButton("canvas", canvas_size);
	// Draw a control point
	if (ImGui::IsItemHovered() && ImGui::IsMouseClicked(ImGuiMouseButton_Left))
	{
		ImVec2 mousePos = ImVec2(ImGui::GetMousePos().x - canvas_topLeft.x, ImGui::GetMousePos().y - canvas_topLeft.y);
		currCurve.push_back(mousePos);

		// Snapshot the global brush settings and push it onto the stroke
		Brush tmpBrush(brushState.m_brushSize, brushState.m_pressure, brushState.m_colors[0], brushState.m_colors[1], brushState.m_colors[2], brushState.m_wetness, brushState.m_angle);
		currStroke.push_back(tmpBrush);
		std::cout << "position: (" << currCurve.back().x << ", " << currCurve.back().y << ")" << std::endl;
		tmpBrush.print();
	}
	// Undo last control point if not empty
	else if (ImGui::IsItemHovered() && ImGui::IsMouseClicked(ImGuiMouseButton_Right) && currCurve.size() > 0)
	{
		currCurve.pop_back();
	}

	// Draw all curves
	for (std::vector<ImVec2> curvePt : curves)
	{
		for (int i = 0; i < curvePt.size(); i++)
		{
			draw_list->AddCircleFilled(ImVec2(canvas_topLeft.x + curvePt[i].x, canvas_topLeft.y + curvePt[i].y), 5.0f, IM_COL32(255, 0, 0, 255));
			if (i > 0)
			{
				draw_list->AddLine(
					ImVec2(canvas_topLeft.x + curvePt[i - 1].x, canvas_topLeft.y + curvePt[i - 1].y),
					ImVec2(canvas_topLeft.x + curvePt[i].x, canvas_topLeft.y + curvePt[i].y),
					IM_COL32(0, 0, 255, 255), 2.0f);
			}
		}

	

		if (curvePt.size() >= 2)
		{
			int steps = 100; // Number of segments for smooth line
			ImVec2 prev = calculateBezierPoints(curvePt, 0.0f);
			for (int i = 1; i < steps + 1; i++)
			{
				float t = (float)i / (float)steps;
				ImVec2 curr = calculateBezierPoints(curvePt, t);
				draw_list->AddLine(
					ImVec2(canvas_topLeft.x + prev.x, canvas_topLeft.y + prev.y),
					ImVec2(canvas_topLeft.x + curr.x, canvas_topLeft.y + curr.y),
					IM_COL32(0, 255, 0, 255), 2.0f);
				prev = curr;
			}
		}
	}

	ImGui::End();
}

void GUI::updateBrush(Brush& brush)
{
	
	ImGui::Text("Brush Settings");               // Display some text (you can use a format strings too)
	
	// Brush setup
	ImGui::SliderFloat("pressure", &brush.m_pressure, 0.0f, 1.0f, "%.2f");            // Edit 1 float using a slider from 0.0f to 1.0f
	ImGui::SameLine(); GUI::HelpMarker(
		"Click and drag to adjust the pressure of the brush.\n"
		"Hold SHIFT/ALT for faster/slower edit.\n"
		"Double-click or CTRL+click to input value.");

	ImGui::SliderInt("brush size", &brush.m_brushSize, 1, 100, "%d");
	ImGui::SameLine(); GUI::HelpMarker(
		"Click and drag to adjust the size of the brush (# of bristles).\n"
		"Hold SHIFT/ALT for faster/slower edit.\n"
		"Double-click or CTRL+click to input value.");


	ImGui::SliderAngle("brush rotation", &brush.m_angle, -90.0f, 90.0f, "%.0f deg");
	ImGui::SameLine(); GUI::HelpMarker(
		"Click and drag to adjust the rotation of the brush.\n"
		"Hold SHIFT/ALT for faster/slower edit.\n"
		"Double-click or CTRL+click to input value.");

	// Bristles state setup
	ImGui::Text("\t  tip\t\t  mid\t\t  base");
	ImGui::SliderFloat3("ink loading", brush.m_colors, 0.0f, 1.0f, "%.2f");
	ImGui::SameLine(); GUI::HelpMarker(
		"Click and drag to set the colors of the tip, middle body, and base of the brush.\n"
		"Hold SHIFT/ALT for faster/slower edit.\n"
		"Double-click or CTRL+click to input value.");

	ImGui::SliderFloat("% wet bristles", &brush.m_wetness, 0.0f, 1.0f, "wetness: %.2f");
	ImGui::SameLine(); GUI::HelpMarker(
		"Click and drag to set percentage of wet bristles.\n"
		"Hold SHIFT/ALT for faster/slower edit.\n"
		"Double-click or CTRL+click to input value.");


}

void GUI::render()
{
	// Render imgui onto screen
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void GUI::shutdown() {
	// Cleanup
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}


void GUI::HelpMarker(const char* desc)
{
	ImGui::TextDisabled("(?)");
	if (ImGui::BeginItemTooltip())
	{
		ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
		ImGui::TextUnformatted(desc);
		ImGui::PopTextWrapPos();
		ImGui::EndTooltip();
	}
}

ImVec2 GUI::calculateBezierPoints(const std::vector<ImVec2>& control_points, float t) {
	std::vector<ImVec2> points = control_points;
	while (points.size() > 1) {
		for (int i = 0; i < points.size() - 1; ++i) {
			points[i].x = (1.0f - t) * points[i].x + t * points[i + 1].x;
			points[i].y = (1.0f - t) * points[i].y + t * points[i + 1].y;
		}
		points.pop_back();
	}
	return points[0];
}