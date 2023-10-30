#include "ModuleEditor.h"
#include "ModuleInput.h"
#include "Application.h"
#include "ModuleWindow.h"
#include "ModuleRenderer3D.h"

ModuleEditor::ModuleEditor(Application* app, bool start_enabled) : Module(app, start_enabled)
{
}

ModuleEditor::~ModuleEditor()
{
}

bool ModuleEditor::Init()
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;

    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {

        UpdateAditionalWindows = true;
    }

    ImGui::StyleColorsDark();

    ImGuiStyle& style = ImGui::GetStyle();
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        style.WindowRounding = 0.0f;
        style.Colors[ImGuiCol_WindowBg].w = 1.0f;
    }

    ImGui_ImplSDL2_InitForOpenGL(App->window->window, App->renderer3D->context);
    ImGui_ImplOpenGL3_Init();

    aFPSLog.reserve(30);
    aDtLog.reserve(30);

    return true;
}

void ModuleEditor::Draw()
{
    // Start the Dear ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    ImGui::NewFrame();

    // -----------------------------------------------------------------------------------------------------------

    aFPS = App->framesPerSecond();
    updateFPS(aFPS);

    aDt = App->deltaTime();
    updateDT(aDt);

    //ImGui::ShowDemoWindow();

    menuBar();

    if (settingsWindow) {
        settings();
    }

    if (consoleWindow) {
        console();
    }

    if (aboutWindow) {
        about();
    }

    // -----------------------------------------------------------------------------------------------------------

    ImGui::Render();
    ImGui::EndFrame();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    if (UpdateAditionalWindows)
    {
        SDL_Window* backup_current_window = SDL_GL_GetCurrentWindow();
        SDL_GLContext backup_current_context = SDL_GL_GetCurrentContext();
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
        SDL_GL_MakeCurrent(backup_current_window, backup_current_context);
    }
}

bool ModuleEditor::CleanUp()
{
    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();

    return true;
}

void ModuleEditor::menuBar() {

    if (ImGui::BeginMainMenuBar())
    {
        if (ImGui::BeginMenu("File"))
        {
            //if (ImGui::MenuItem("New scene")) { /* Ctrl + N */ }
            //if (ImGui::MenuItem("Save scene")) { /* Do stuff */ }
            //if (ImGui::MenuItem("Load scene")) { /* Do stuff */ }
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Edit"))
        {
            //if (ImGui::MenuItem("Name")) { /* Do stuff */ }
            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Window"))
        {
            if (ImGui::MenuItem("Settings"))
            {
                settingsWindow = true;
            }
            if (ImGui::MenuItem("Console"))
            {
                consoleWindow = true;
            }
            if (ImGui::MenuItem("About"))
            {
                aboutWindow = true;
            }
            ImGui::EndMenu();
        }
        if (ImGui::Button("Exit"))
        {
            App->input->quit = true;
        }
        ImGui::EndMainMenuBar();
    }
}

void ModuleEditor::settings()
{
    ImGui::Begin("Settings", &settingsWindow, ImGuiWindowFlags_MenuBar);
    {
        ImGui::PlotHistogram("FPS", &aFPSLog[0], aFPSLog.size(), 0, "", 0, 240, ImVec2(200, 30));
        ImGui::PlotHistogram("Delta Time", &aDtLog[0], aDtLog.size(), 0, "", 0, 0.01, ImVec2(200, 30));
        if (ImGui::CollapsingHeader("Screen Settings"))
        {
            ImGui::SeparatorText("Window");
            
            ImGui::SliderInt("Width", &Width, 426, 1920);
            ImGui::SliderInt("Height", &Height, 240, 1080);

            ImGui::Checkbox("FullScreen", &fullscreen);
            ImGui::Checkbox("Borderless", &borderless);

            if (ImGui::Button("Apply"))
            {
                App->window->screenSettings(Width, Height, fullscreen, borderless);
            }
        }
        if (ImGui::CollapsingHeader("Renderer"))
        {
            ImGui::SeparatorText("Open GL");

            if (ImGui::Checkbox("Lighting", &lighting))
            {
                if (lighting)
                {
                    glEnable(GL_LIGHTING);
                }
                else
                {
                    glDisable(GL_LIGHTING);
                }
            }
            if (ImGui::Checkbox("Cull Facing", &cullFace))
            {
                if (cullFace)
                {
                    glEnable(GL_CULL_FACE);
                }
                else
                {
                    glDisable(GL_CULL_FACE);
                }
            }
            if (ImGui::Checkbox("Alpha Test", &alphaTest))
            {
                if (alphaTest)
                {
                    glEnable(GL_ALPHA_TEST);
                }
                else
                {
                    glDisable(GL_ALPHA_TEST);
                }
            }
            if (ImGui::Checkbox("Depth Test", &depthTest))
            {
                if (depthTest)
                {
                    glEnable(GL_DEPTH_TEST);
                }
                else
                {
                    glDisable(GL_DEPTH_TEST);
                }
            }
            if (ImGui::Checkbox("Blend", &blend))
            {
                if (alphaTest)
                {
                    glEnable(GL_BLEND);
                }
                else
                {
                    glDisable(GL_BLEND);
                }
            }
            if (ImGui::Checkbox("Texture 2D", &texture2d))
            {
                if (texture2d)
                {
                    glEnable(GL_TEXTURE_2D);
                }
                else
                {
                    glDisable(GL_TEXTURE_2D);
                }
            }
        }
        ImGui::End();
    }
}

void ModuleEditor::console() 
{
    ImGui::Begin("Console", &consoleWindow, ImGuiWindowFlags_MenuBar);
    {
        for (int i = 0; i < logConsole.size(); i++)
        {
            ImGui::Text(logConsole[i].c_str());
        }
    }
    ImGui::End();
}

void ModuleEditor::about()
{
    ImGui::Begin("About", &aboutWindow, ImGuiWindowFlags_MenuBar);
    {
        ImGui::SeparatorText("ABOUT VALKYRIE ENGINE:");
        ImGui::Text("Valkyrie Engine v.0.1");
        ImGui::Text("By Joel Maldonado Salvador");

        /*ImGui::SeparatorText("OPEN GL:");
        ImGui::Text("Vendor: %s", glGetString(GL_VENDOR));
        ImGui::Text("Renderer: %s", glGetString(GL_RENDERER));
        ImGui::Text("OpenGL version supported %s", glGetString(GL_VERSION));
        ImGui::Text("GLSL: %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));*/

        ImGui::NewLine();
        ImGui::SeparatorText("LICENSE:");
        
        /*ImGui::Text("MIT LICENSE:");

        ImGui::NewLine();

        ImGui::Text("Copyright (c) 2012-2023 Scott Chacon and others");
        ImGui::Text("Permission is hereby granted, free of charge, to any person obtaining");
        ImGui::Text("a copy of this software and associated documentation files (the '");
        ImGui::Text("Software'), to deal in the Software without restriction, including");
        ImGui::Text("without limitation the rights to use, copy, modify, merge, publish,");
        ImGui::Text("distribute, sublicense, and/or sell copies of the Software, and to");
        ImGui::Text("permit persons to whom the Software is furnished to do so, subject to");
        ImGui::Text("the following conditions:");
        ImGui::Text("The above copyright notice and this permission notice shall be");
        ImGui::Text("included in all copies or substantial portions of the Software.");
        ImGui::Text("THE SOFTWARE IS PROVIDED 'AS IS', WITHOUT WARRANTY OF ANY KIND,");
        ImGui::Text("EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF");
        ImGui::Text("MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND");
        ImGui::Text("NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE");
        ImGui::Text("LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION");
        ImGui::Text("OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION");
        ImGui::Text("WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.");*/

        ImGui::NewLine();
        ImGui::SeparatorText("ABOUT ME:");
        if (ImGui::MenuItem("Press here to visit the repository"))
        {
            ShellExecute(NULL, "open", "https://github.com/Neffyer/Valkyrie_Engine", 0, 0, SW_SHOWNORMAL);
        }
        ImGui::End();
    }
}

void ModuleEditor::updateFPS(const float aFPS)
{
    if (aFPSLog.size() < 30)
    {
        aFPSLog.push_back(aFPS);
    }
    else
    {
        for (unsigned int i = 0; i < aFPSLog.size(); i++)
        {
            if (i + 1 < aFPSLog.size())
            {
                float copy = aFPSLog[i + 1];
                aFPSLog[i] = copy;
            }
        }
        aFPSLog[aFPSLog.capacity() - 1] = aFPS;
    }
}

void ModuleEditor::updateDT(const float aDt)
{
    if (aDtLog.size() < 30)
    {
        aDtLog.push_back(aDt);
    }
    else
    {
        for (unsigned int i = 0; i < aDtLog.size(); i++)
        {
            if (i + 1 < aDtLog.size())
            {
                float copy = aDtLog[i + 1];
                aDtLog[i] = copy;
            }
        }
        aDtLog[aDtLog.capacity() - 1] = aDt;
    }
}