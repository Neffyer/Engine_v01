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

    //-----------------------------------------------------------------------------------------------------------

    aFPS = App->framesPerSecond();
    updateFPS(aFPS);

    aDt = App->deltaTime();
    updateDT(aDt);

    //ImGui::ShowDemoWindow();

    menuBar();

    if (settingsWindow) {
        settings();
    }

    //-----------------------------------------------------------------------------------------------------------

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
            if (ImGui::MenuItem("Settings"))
            {
                settingsWindow = true;
            }
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("About"))
        {
            if (ImGui::MenuItem("Github"))
            {
                ShellExecute(NULL, "open", "https://github.com/Neffyer/Valkyrie_Engine", 0, 0, SW_SHOWNORMAL);
            }
            ImGui::EndMenu();   
        }
        ImGui::EndMainMenuBar();
    }
}

void ModuleEditor::settings() {
    
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
        ImGui::End();
    }
}

void ModuleEditor::console() {
    
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