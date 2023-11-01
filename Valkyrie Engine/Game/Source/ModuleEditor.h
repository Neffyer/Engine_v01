#pragma once
#ifndef _Module_Editor_
#define _Module_Editor_

#include "Module.h"
#include "Globals.h"
#include <vector>
#include <string>

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

	std::vector <float> aDtLog;
	float aDt;
	void updateDT(const float aDt);

	int cpu;
	int cacheCpu;
	int ram;

	bool UpdateAditionalWindows = false;
	
	void menuBar();

	void settings();

	void window();

	void renderer();

	std::vector <std::string> logConsole;
	void console();

	void about();

private:

	bool settingsWindow = false;
	bool consoleWindow = true;
	bool aboutWindow = false;
	
	bool lighting = true;
	bool cullFace = false;
	bool alphaTest = false;
	bool depthTest = false;
	bool blend = false;
	bool texture2d = false;

	int Width = 1080;
	int Height = 720;
	bool fullscreen = false;
	bool borderless = false;
};

#endif // !_Module_Editor_