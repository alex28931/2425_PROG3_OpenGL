#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <format>
#include "XWindow.h"

int main() {
	
	XWindow Win = XWindow(640, 460, "Hello OpenGL");

	float TitleUpdateMaxTime = 1.f;
	float TitleUpdateElapsed = 0.f;

	Win.SetTitle("Hello from here");

	while (Win.IsOpened())
	{
		float DeltaTime = Win.GetDeltaTime();
		
		TitleUpdateElapsed += DeltaTime;
		if (TitleUpdateElapsed >= TitleUpdateMaxTime) {
			int Fps = 1.f / DeltaTime;
			std::string formattedTitle = std::format("OpenGL App | DeltaTime: {} - FPS: {}", DeltaTime, Fps);
			Win.SetTitle(formattedTitle);
			
			TitleUpdateElapsed -= TitleUpdateMaxTime;
		}

		Win.Update();
	}
	
	return 0;	
}
