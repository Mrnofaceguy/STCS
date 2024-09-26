#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <GLFW/glfw3.h>
#include <iostream>

// Simulated device temperatures
float therm_01 = 20.0f, therm_02 = 25.0f, therm_03 = 18.0f, therm_04 = 22.0f;
float minTemp[4] = {10.0f, 10.0f, 10.0f, 10.0f};
float maxTemp[4] = {30.0f, 30.0f, 30.0f, 30.0f};
bool controlEnabled = false;

// Function to render the GUI
void render_gui() {
    ImGui::Begin("Temperature Control System");

    for (int i = 0; i < 4; ++i) {
        ImGui::Text("Device %d", i + 1);
        ImGui::Text("Current Temp: %.1f", i == 0 ? therm_01 : i == 1 ? therm_02 : i == 2 ? therm_03 : therm_04);
        ImGui::SliderFloat(("Min Temp " + std::to_string(i + 1)).c_str(), &minTemp[i], -50.0f, 50.0f);
        ImGui::SliderFloat(("Max Temp " + std::to_string(i + 1)).c_str(), &maxTemp[i], -50.0f, 50.0f);
        ImGui::Separator();
    }

    if (ImGui::Button(controlEnabled ? "Stop Control" : "Start Control")) {
        controlEnabled = !controlEnabled;
    }

    ImGui::End();
}

int main() {
    // Initialize GLFW
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return -1;
    }

    // Set up OpenGL context and window
    const char* glsl_version = "#version 130";
    GLFWwindow* window = glfwCreateWindow(1280, 720, "Temperature Control GUI", NULL, NULL);
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    // Initialize ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();

    // Set up Dear ImGui style
    ImGui::StyleColorsDark();

    // Initialize ImGui for GLFW and OpenGL
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);

    // Main loop
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        // Start the ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // Render the GUI
        render_gui();

        // Rendering
        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(0.45f, 0.55f, 0.60f, 1.00f);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
    }

    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
