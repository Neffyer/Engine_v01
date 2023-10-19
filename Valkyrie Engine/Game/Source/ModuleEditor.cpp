#include "ModuleEditor.h"
#include "Externals/ImGui/imgui.h"
#include "Externals/ImGui/backends/imgui_impl_opengl3.h"
#include "Externals/ImGui/backends/imgui_impl_sdl2.h"
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

    ImGui::StyleColorsDark();

    ImGui_ImplSDL2_InitForOpenGL(App->window->window, App->renderer3D->context);
    ImGui_ImplOpenGL3_Init();

    //mFPSLog.capacity(30.0f);

    return true;
}

void ModuleEditor::Draw()
{
    // Start the Dear ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    ImGui::NewFrame();

    if (ImGui::BeginMainMenuBar())
    {
        if (ImGui::Button("Exit"))
        {
            App->renderer3D->TurnOff = true;
        }

        if (ImGui::BeginMenu("Menu"))
        {
            ImGui::Text("Texto");
            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Help"))
        {
            if (ImGui::CollapsingHeader("About"))
            {
                ImGui::SeparatorText("ABOUT ODYSSEY ENGINE:");
                ImGui::Text("BLA BLA BLA BLA BLA EN FIN COSAS ");
            }

            if (ImGui::CollapsingHeader("GitHub"))
            {
                ImGui::Text("https://github.com/flishflash/OdysseyEngine");
            }

            ImGui::EndMenu();
        }

        ImGui::EndMainMenuBar();
    }

    if (ImGui::Begin("Settings"))
    {
        ImGui::PlotHistogram("FPS", mFPSLog.data(), mFPSLog.size(), 2, lastValue);
        ImGui::End();
    }

    ImGui::ShowDemoWindow();

    ImGui::Render();
    ImGui::EndFrame();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

bool ModuleEditor::CleanUp()
{
    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();

    return true;
}

void ModuleEditor::AddFPS(float aFPS)
{
    if (mFPSLog.size() < 30)
    {
        mFPSLog.push_back(aFPS);
    }
    else
    {
        mFPSLog.erase(mFPSLog.begin());
        mFPSLog.push_back(aFPS);
    }
}