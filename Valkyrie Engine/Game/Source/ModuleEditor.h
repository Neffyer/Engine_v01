#pragma once
#ifndef _Module_Editor_
#define _Module_Editor_

#include "Module.h"
#include "Globals.h"
#include <vector>

#include "Externals\imGui\imgui.h"
#include "Externals\imGui\backends\imgui_impl_sdl2.h"
#include "Externals\imGui\backends\imgui_impl_opengl3.h"
#include "Externals/Glew/include/glew.h"
#include "Externals/SDL/include/SDL_opengl.h"

class ModuleEditor : public Module
{
public:

	ModuleEditor(Application* app, bool start_enabled = true);
	~ModuleEditor();

	bool Init() override;
	void Draw();
	bool CleanUp() override;

	std::vector<float> aFPSLog;
	float aFPS;

	void updateFPS(const float aFPS);

	std::vector<float> aDtLog;
	float aDt;
	void updateDT(const float aDt);

	bool UpdateAditionalWindows = false;
	
	void menuBar();

	void settings();

	void console();

private:

	bool settingsWindow = false;

	int Height = 720, Width = 1080;
	bool fullscreen = false;
	bool borderless = false;
};

#endif // !_Module_Editor_