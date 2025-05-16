#include "XWindow.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stdexcept>

XWindow::XWindow(int InWidth, int InHeight, std::string InTitle)
    : Width(InWidth), Height(InHeight), Title(InTitle), 
	RawWindow(nullptr), DeltaTime(0.f)
{ 
    if (glfwInit() == GLFW_FALSE) {
		throw std::runtime_error("Error glfw init");
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);

	GLFWmonitor* monitor = NULL;
	RawWindow = glfwCreateWindow(Width, Height, Title.c_str(), monitor, NULL);

	if (RawWindow == NULL) {
		throw std::runtime_error("Error creating window");
	}

	glfwMakeContextCurrent(RawWindow);

    if (gladLoadGL() == 0) {
		throw std::runtime_error("Error loading GL Functions");
	}

}

XWindow::~XWindow() 
{
    glfwDestroyWindow(RawWindow);
}

bool XWindow::IsOpened() const
{
    return !glfwWindowShouldClose(RawWindow);
}

void XWindow::SetTitle(const std::string InTitle)
{
    glfwSetWindowTitle(RawWindow, InTitle.c_str());
}

void XWindow::Update()
{
    static float LastTime = glfwGetTime();
    float CurrTime = glfwGetTime();
    DeltaTime = CurrTime - LastTime;
    LastTime = CurrTime;

	glfwSwapBuffers(RawWindow);
	glfwPollEvents();
}

float XWindow::GetDeltaTime() const
{
    return DeltaTime;
}

void XWindow::SetVSync(bool InEnabled) {
    if (!InEnabled) {
        glfwSwapInterval(0);
    } else {
        glfwSwapInterval(1); //swap for each frame
    }
}