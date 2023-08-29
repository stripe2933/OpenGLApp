//
// Created by gomkyung2 on 2023/08/29.
//

/*
 * Show ImGui demo window.
 * You can distinguish the ImGui input and GLFW input using ImGuiIO::WantCaptureMouse. In this example, message printed
 * only if user click the window region but not ImGui window region.
 */

#include <iostream>
#include <OpenGLApp/Window.hpp>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

class App : public OpenGL::Window{
private:
    void onMouseButtonChanged(int button, int action, int mods) override {
        ImGuiIO &io = ImGui::GetIO();
        io.AddMouseButtonEvent(button, action);
        if (io.WantCaptureMouse){
            return;
        }

        if(button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS){
            std::cout << "Mouse pressed\n";
        }
    }

    void update(float time_delta) override {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        ImGui::ShowDemoWindow();

        ImGui::Render();
    }

    void draw() const override {
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }

    void initImGui(){
        // Setup Dear ImGui context
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO(); (void)io;
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls

        // Setup Dear ImGui style
        ImGui::StyleColorsDark();

        // Setup Platform/Renderer backends
        ImGui_ImplGlfw_InitForOpenGL(window, true);
        ImGui_ImplOpenGL3_Init("#version 330");
    }

public:
    App() : OpenGL::Window { 800, 480, "ImGui Example" } {
        initImGui();
    }

    ~App() noexcept override{
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
    }
};

int main(){
    App{}.run();
}