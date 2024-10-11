/*
* Japanese watercolor drawing app.
* 
* @Boilerplate GLFW / GLUI code provided by https://github.com/ocornut/imgui/tree/master
* @author Benji Northrop
*/

// Dependent libraries
#include <iostream>
#include <glad/glad.h>
#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>
#include <stdio.h>
#define GL_SILENCE_DEPRECATION
#include <GLFW/glfw3.h> // Will drag system OpenGL headers
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <vector>

// local program header files
#include "ShaderProgram.h"
#include "Brush.h"
#include "GUI.h"


// Shader data
const char* VERTEX_SHADER_PATH = "./shaders/basic.vert";
const char* FRAGMENT_SHADER_PATH = "./shaders/basic.frag";

int g_windowWidth = 1280; 
int g_windowHeight = 768;
const char* APP_TITLE = "Sumi-e";

// Max vertices to draw
const int MAX_VERTICES = 10000;
int numPoints[MAX_VERTICES] = { 0 };
int lineNumber = 0;

GLfloat vertices[MAX_VERTICES][MAX_VERTICES];
GLuint VBO[MAX_VERTICES], VAO[MAX_VERTICES];

bool moving = false;
bool enableUI = true; // true to show the UI, false to hide UI menu

// Callback functions
void glfw_error_callback(int error, const char* description);
void glfw_onKey(GLFWwindow* window, int key, int scancode, int action, int mode);
void glfw_onFrameBufferSize(GLFWwindow* window, int width, int height);
void glfw_onClick(GLFWwindow* window, int button, int action, int mods);
void mouse_button_callback(GLFWwindow* g_window, int button, int action, int mods);
void add_point(float xpos, float ypos);





// Main code
int main(int, char**)
{
    //-----------------------------------------
    //Initialize GLFW and ImGui
    //-----------------------------------------
    glfwSetErrorCallback(glfw_error_callback);
    if (!glfwInit())
        return -1;


    // Decide GL+GLSL versions
    #if defined(IMGUI_IMPL_OPENGL_ES2)
        // GL ES 2.0 + GLSL 100
    const char* glsl_version = "#version 100";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
    #elif defined(__APPLE__)
        // GL 3.2 + GLSL 150
    const char* glsl_version = "#version 150";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // Required on Mac
    #else
        // GL 3.0 + GLSL 130
    const char* glsl_version = "#version 130";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    //glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
    //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // 3.0+ only
    #endif

        // Create window with graphics context
    GLFWwindow* window = glfwCreateWindow(g_windowWidth, g_windowHeight, APP_TITLE, nullptr, nullptr);
    if (window == nullptr)
        return -1;
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); // Enable vsync

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    glfwGetFramebufferSize(window, &g_windowWidth, &g_windowHeight);
    glViewport(0, 0, g_windowWidth, g_windowHeight);

    GUI myGUI;
    myGUI.init(window, glsl_version);

    Brush brushState;
    bool showControlPoints;
    std::vector<std::vector<ImVec2>> curves;
    std::vector<std::vector<Brush>> strokes;

    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();
        myGUI.newFrame();

      
        // Rendering
        glClearColor(0.3f, 0.3f, 0.3f, 0.3f);
        glClear(GL_COLOR_BUFFER_BIT);
        // Create the main window to hold the application
        ImGui::Begin("Brush Settings");
        myGUI.mainMenu(brushState, showControlPoints);

        ImGui::End();
        myGUI.renderBezier(curves, strokes, brushState, showControlPoints);
        myGUI.render();
        glfwSwapBuffers(window);
    }

    myGUI.shutdown();

    return 0;
}

void glfw_error_callback(int error, const char* description)
{
    fprintf(stderr, "GLFW Error %d: %s\n", error, description);
}

void glfw_onKey(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }
   
}

void glfw_onFrameBufferSize(GLFWwindow* window, int width, int height)
{
    g_windowWidth = width;
    g_windowHeight = height;
    glViewport(0, 0, g_windowWidth, g_windowHeight);
}

void glfw_onClick(GLFWwindow* window, int button, int action, int mods)
{
    ImGuiIO& io = ImGui::GetIO();
    io.AddMouseButtonEvent(GLFW_MOUSE_BUTTON_LEFT, GLFW_PRESS);

    if (!io.WantCaptureMouse)
    {
        if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
        {
            double xpos, ypos, press;
            glfwGetCursorPos(window, &xpos, &ypos);
            add_point(xpos, ypos);
            moving = true;
            std::cout << "clicked at " << xpos << " " << ypos << std::endl;
        }

        if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE)
        {
            moving = false;
            lineNumber++;
        }
    }

    

}


void mouse_button_callback(GLFWwindow* g_window, int button, int action, int mods)
{
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
    {
        double xpos, ypos;
        glfwGetCursorPos(g_window, &xpos, &ypos);
        add_point(xpos, ypos);
        moving = true;
    }

    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE)
    {
        moving = false;
        lineNumber++;
    }
}

void add_point(float xpos, float ypos)
{
    if (numPoints[lineNumber] >= MAX_VERTICES / 2) return;

    float dotX = 2 * (float)xpos / g_windowWidth - 1.0f;
    float dotY = 1 - 2 * (float)(ypos / g_windowHeight);

    vertices[lineNumber][2 * numPoints[lineNumber]] = dotX;
    vertices[lineNumber][2 * numPoints[lineNumber] + 1] = dotY;

    numPoints[lineNumber]++;
}