#include "GUI.h"
#include "imgui/imgui.h"
#include <iostream>
#include <cmath>

#define DEBUG 0; // Change to 1 for debugging print statements

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

void GUI::renderBezier(std::vector<std::vector<ImVec2>>& curves,  std::vector<std::vector<Brush>>& strokes, Brush& brushState, bool& showControlPoints) // Takes a 2D vector of Curves
{
	ImGui::Begin("Sumi-e Curve Editor");


	if (ImGui::Button("New Stroke (n)") || ImGui::IsKeyPressed(ImGuiKey_N))
	{
		curves.emplace_back(std::vector<ImVec2>());
		strokes.emplace_back(std::vector<Brush>());
		//Brush tmp(brushState.m_brushSize, brushState.m_pressure, brushState.m_wetness, brushState.m_angle);
		//strokes.back().push_back(tmp); 
	}


	if (curves.empty() || strokes.empty())
	{
		ImGui::End();
		return; // Early return if curves is empty, skipping any drawing.
	}
	ImGui::SameLine();
	if (ImGui::Button("Undo Stroke (z)") || ImGui::IsKeyPressed(ImGuiKey_Z))
	{
		if (curves.size() > 1)
			curves.pop_back();
		if (strokes.size() > 1)
			strokes.pop_back();
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
	draw_list->AddRectFilled(canvas_topLeft, canvas_bottomRight, IM_COL32(250, 250, 250, 255));// Background color
	draw_list->AddRect(canvas_topLeft, canvas_bottomRight, IM_COL32(30, 30, 30, 255));	// Canvas color

	// Mouse click listener to add control points
	ImGui::InvisibleButton("canvas", canvas_size);
	// Draw a control point
	if (ImGui::IsItemHovered() && ImGui::IsMouseClicked(ImGuiMouseButton_Left))
	{
		ImVec2 mousePos = ImVec2(ImGui::GetMousePos().x - canvas_topLeft.x, ImGui::GetMousePos().y - canvas_topLeft.y);
		currCurve.push_back(mousePos);

		// Snapshot the global brush settings and copy it into the stroke vector
		Brush tmpBrush(brushState.m_brushSize, brushState.m_pressure, brushState.m_colors[0], brushState.m_colors[1], brushState.m_colors[2], brushState.m_wetness, brushState.m_angle);
		currStroke.push_back(tmpBrush);

		#if DEBUG
			std::cout << "position: (" << currCurve.back().x << ", " << currCurve.back().y << ")" << std::endl;
			tmpBrush.print();
			std::cout << "Total brushes: " << std::endl;
			for (int i = 0; i < currStroke.size(); i++) {
				std::cout << "Brush " << i << std::endl;
				currStroke[i].print();
				std::cout << std::endl;
			}
		#endif
	}

	// Undo last control point and brush if not empty
	else if (ImGui::IsItemHovered() && ImGui::IsMouseClicked(ImGuiMouseButton_Right) && currCurve.size() > 0)
	{
		currCurve.pop_back();
		currStroke.pop_back();
	}

	
	// ToDo: Add a toggle button to show or hide these dots
	// Draw the control points on the screen and point to point lines
	for (int i = 0; i < curves.size(); i++)
	{
		
		drawControlPointsAndLines(curves[i], canvas_topLeft, draw_list, showControlPoints);

		drawBezierCurve(curves[i], canvas_topLeft, strokes[i], draw_list);
	
		
	}

	ImGui::End();
}

void GUI::mainMenu(Brush& brush, bool& showControlPoints)
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

	ImGui::Checkbox("Show control points", &showControlPoints);
	ImGui::SameLine(); GUI::HelpMarker(
		"Select to show your control points"
	);

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

ImVec2 GUI::calculateBezierPoints(const std::vector<ImVec2>& controlPoints, float t) 
{
	std::vector<ImVec2> points = controlPoints;
	while (points.size() > 1) 
	{
		for (int i = 0; i < points.size() - 1; ++i) {
			points[i].x = (1.0f - t) * points[i].x + t * points[i + 1].x;
			points[i].y = (1.0f - t) * points[i].y + t * points[i + 1].y;
		}
		points.pop_back();
	}
	return points[0];
}

void GUI::drawControlPointsAndLines(std::vector<ImVec2>& curvePt, ImVec2& canvas_topLeft, ImDrawList* draw_list, bool& show)
{
	if (show)
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
	}
}

void GUI::drawBezierCurve(std::vector<ImVec2> &curve, ImVec2& canvas_topLeft, std::vector<Brush> &stroke, ImDrawList* draw_list)
{
	// Draw the link segments
	if (curve.size() >= 2)
	{
		ImDrawListFlags_AntiAliasedLines;
		int steps = 100 + 100 * curve.size() / 5; // 100 segments per 5 points
		// Initialize the brush tracking indices
		// We linearly interpolate the brush parameters from the beginning to end of each stroke.
		int brushIndex = 1;
		float brushRatio = 1.0f / ((float)stroke.size() - 1.0f);
		float prevThreshold = 0.0f;
		float currThreshold = brushRatio;

		ImVec2 prev = calculateBezierPoints(curve, 0.0f);
		Brush tmpBrush;
		Brush prevBrush = stroke[brushIndex - 1];
		int BRUSH_SIZE = prevBrush.m_brushSize; // Brush size cannot change in the middle of a stroke
		#if DEBUG
			std::cout << std::endl << "drawBezierCurve" << std::endl;
			std::cout << "prev: " << std::endl;
			prevBrush.print();
		#endif
			
		for (int i = 1; i < steps + 1; i++)
		{
			float t = (float)i / (float)steps;
			float strokeFlowCoeff = -(std::powf((2.0f * t - 1.0f), 4.0f)) + 1.0f; // coeff = -(t-1)^2 + 1, approximating gaussian curve from [0, 1]  
			ImVec2 curr = calculateBezierPoints(curve, t);
			Brush currBrush = stroke[brushIndex];
			float a = ((float)t - prevThreshold) / brushRatio; // Determine the alpha blend ratio

			// alpha blend the brush values
			tmpBrush.lerpBrush(a, prevBrush, currBrush);

			// Determine quad points and put into temp variable
			ImVec2 q[4];
		
			tmpBrush.calculateQuad(prev, curr, BRUSH_BASE, q[0], q[1], q[2], q[3]);
			for (int i = 0; i < 4; i++)
			{
				q[i] += canvas_topLeft;
			}
			draw_list->AddQuad(q[0], q[1], q[2], q[3], IM_COL32(tmpBrush.m_colors[2] * 255, tmpBrush.m_colors[2] * 255, tmpBrush.m_colors[2] * 255, 255), tmpBrush.m_pressure * (float)tmpBrush.m_brushSize * strokeFlowCoeff);
			
			tmpBrush.calculateQuad(prev, curr, BRUSH_MID, q[0], q[1], q[2], q[3]);
			for (int i = 0; i < 4; i++)
			{
				q[i] += canvas_topLeft;
			}
			draw_list->AddQuad(q[0], q[1], q[2], q[3], IM_COL32(tmpBrush.m_colors[1] * 255, tmpBrush.m_colors[1] * 255, tmpBrush.m_colors[1] * 255, 255), tmpBrush.m_pressure * (float)tmpBrush.m_brushSize * strokeFlowCoeff);

			tmpBrush.calculateQuad(prev, curr, BRUSH_TIP, q[0], q[1], q[2], q[3]);
			for (int i = 0; i < 4; i++)
			{
				q[i] += canvas_topLeft;
			}
			draw_list->AddQuad(q[0], q[1], q[2], q[3], IM_COL32(tmpBrush.m_colors[0] * 255, tmpBrush.m_colors[0] * 255, tmpBrush.m_colors[0] * 255, 255), tmpBrush.m_pressure * (float)tmpBrush.m_brushSize * strokeFlowCoeff);
			




			#if DEBUG
			std::cout << "pressure: " << tmpBrush.m_pressure << std::endl;
			std::cout << "brush Size: " << tmpBrush.m_brushSize << std::endl;
			std::cout << "stroke coeff: " << strokeFlowCoeff << std::endl;
			std::cout << "thickness: " << tmpBrush.m_pressure * (float)tmpBrush.m_brushSize * strokeFlowCoeff << std::endl;

			std::cout << "Quad points after adding: " << std::endl;
			std::cout << "(" << q1.x << ", " << q1.y << ")" << std::endl;
			std::cout << "(" << q2.x << ", " << q2.y << ")" << std::endl;
			std::cout << "(" << q3.x << ", " << q3.y << ")" << std::endl;
			std::cout << "(" << q4.x << ", " << q4.y << ")" << std::endl;
			#endif

			// Draw the quad for the base, middle, and tip in order so they stack on each other.
			

			/*draw_list->AddLine(
				ImVec2(canvas_topLeft.x + prev.x, canvas_topLeft.y + prev.y),
				ImVec2(canvas_topLeft.x + curr.x, canvas_topLeft.y + curr.y),
				IM_COL32(tmpBrush.m_colors[0] * 255, tmpBrush.m_colors[1] * 255, tmpBrush.m_colors[2] * 255, 255), tmpBrush.m_pressure * (float)tmpBrush.m_brushSize * strokeFlowCoeff);*/
			prev = curr;
			if (t > currThreshold)
			{
				prevBrush = currBrush;
				currBrush = stroke[++brushIndex];
				prevThreshold = currThreshold;
				currThreshold += brushRatio;
				#if DEBUG
				std::cout << "Set next brush: " << brushIndex << std::endl;
				std::cout << "t: " << t << std::endl;
				std::cout << "currThreshold: " << currThreshold << std::endl;
				std::cout << "Brush: " << std::endl;
				tmpBrush.print();
				#endif
			}
		}
	}
}

