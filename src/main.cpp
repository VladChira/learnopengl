#include <iostream>
#include <string>
#include <cmath>

#define STB_IMAGE_IMPLEMENTATION

#include "Application.hpp"

int main()
{
	Application app;
	app.Start("Bad rasterizer", 1280, 720, true);
	return 0;
}