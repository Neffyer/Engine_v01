#pragma once
#ifndef _Module_Editor_
#define _Module_Editor_

#include "Module.h"
#include "Globals.h"
#include <vector>

#include "..\Source\Externals\imGui\imgui.h"
#include "..\Source\Externals\imGui\backends\imgui_impl_sdl2.h"
#include "..\Source\Externals\imGui\backends\imgui_impl_opengl3.h"

class ModuleEditor : public Module
{
public:

	ModuleEditor(Application* app, bool start_enabled = true);
	~ModuleEditor();

	bool Init() override;
	void Draw();
	bool CleanUp() override;

	void AddFPS(float aFPS);

	std::vector<float> mFPSLog;

private:
	const char* lastValue = "";

};

#endif // !_Module_Editor_