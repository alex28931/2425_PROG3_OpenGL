#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <format>
#include "XWindow.h"
#include "Ex01TriangleDraw.h"
#include "Ex02QuadDraw.h"
#include "Ex03QuadIndexDraw.h"
#include "Ex04QuadColorDraw.h"
#include "Ex05QuadTextureDraw.h"
#include "Ex06QuadPerspDraw.h"
#include "Ex07CubePerspDraw.h"
#include "Ex08PhongDraw.h"
#include "Ex09PostFxDraw.h"

int main() {
	
	XWindow Win = XWindow(800, 600, "Hello OpenGL");

	float TitleUpdateMaxTime = 1.f;
	float TitleUpdateElapsed = 0.f;

	Win.SetTitle("Hello from here");

	//Ex01TriangleDraw Scene;
	//Ex02QuadDraw Scene;
	//Ex03QuadIndexDraw Scene;
	//Ex04QuadColorDraw Scene;
	//Ex05QuadTextureDraw Scene;
	//Ex06QuadPerspDraw Scene;
	//Ex07CubePerspDraw Scene;
	//Ex08PhongDraw Scene;
	Ex09PostFxDraw Scene;

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

		Scene.Update(DeltaTime);

		Win.Update();
	}	
	return 0;	
}
