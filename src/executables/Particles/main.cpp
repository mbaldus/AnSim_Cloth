#include <cstdio>
#include <algorithm> //std::sort
#include <cstdlib>
#include <cmath>

#define NUM_PARTICLES 30000

#include <imgui-glfw/imgui.h>
#include "imgui-glfw/imgui_impl_glfw_gl3.h"

#include <ParticleSystem/ParticleSystem.h>
#include <Util/util.h>
#include <ownGL/GLTools.h>
#include <ownGL/CVK_Trackball.h>
#include <ownGL/ShaderProgram.h>
#include <ownGL/CVK_Sphere.h>
#include <ownGL/Texture.h>

void checkGLError(const std::string &when) {
    GLenum err;
    while((err = glGetError()) != GL_NO_ERROR) {
        std::cout << "gl Error in " << when << ": " << err << std::endl;
    }
}

static void error_callback(int error, const char* description)
{
    fprintf(stderr, "Error %d: %s\n", error, description);
}

int main() {
	printf("Particles\n");

    GLFWwindow* window = GLTools::generateWindow(1280,720,100,100,"Particles");
    glfwSwapInterval(0); //vsync off
    glClearColor(0.84, 0.85, 0.85, 0.0);
    Trackball trackball(GLTools::getWidth(window),GLTools::getHeight(window));


    // Setup ImGui binding
    ImGui_ImplGlfwGL3_Init(window, true);

    // Load Fonts
    // (there is a default font, this is only if you want to change it. see extra_fonts/README.txt for more details)
    //ImGuiIO& io = ImGui::GetIO();
    //io.Fonts->AddFontDefault();
    //io.Fonts->AddFontFromFileTTF("../../extra_fonts/Cousine-Regular.ttf", 15.0f);
    //io.Fonts->AddFontFromFileTTF("../../extra_fonts/DroidSans.ttf", 16.0f);
    //io.Fonts->AddFontFromFileTTF("../../extra_fonts/ProggyClean.ttf", 13.0f);
    //io.Fonts->AddFontFromFileTTF("../../extra_fonts/ProggyTiny.ttf", 10.0f);
    //io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\ArialUni.ttf", 18.0f, NULL, io.Fonts->GetGlyphRangesJapanese());

    bool show_test_window = true;
    bool show_another_window = false;
    ImVec4 clear_color = ImColor(114, 144, 154);


    auto * particleSystem = new ParticleSystem(0.005f, NUM_PARTICLES, RenderingType::instancing);

    //###################################################################
	//				ownGL ShaderProgram and Camera Settings
        ShaderProgram* shaderprogram;
    if (particleSystem->getRenderingType() == RenderingType::glpoints)
	    shaderprogram = new ShaderProgram("/particlesVS.vert", "/particlesFS.frag");
    if (particleSystem->getRenderingType() == RenderingType::instancing)
        shaderprogram = new ShaderProgram("/particlesInstancedVS.vert", "/particlesInstancedFS.frag");

	glm::mat4 model = glm::mat4(1.0f);
	glm::mat4 view = glm::lookAt(glm::vec3(0.0f,-0.1f,1.0f),glm::vec3(0.0f,0.0f,0.0f), glm::vec3(0.0f,1.0f,0.0f));
	glm::mat4 projection = glm::perspective(45.f, GLTools::getRatio(window), 0.1f, 100.f);
	shaderprogram->update("model",model);
	shaderprogram->update("view",view);
	shaderprogram->update("projection",projection);

	//###################################################################


    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);

    int framecount=0;

	std::function<void(double)> loop =
			[&particleSystem,
					&shaderprogram,
					&trackball,
					&view, &framecount,
					&window,
                    &show_test_window,
                    &show_another_window,
                    &clear_color](double deltatime)
			{
                glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


                shaderprogram->use();
                shaderprogram->update("view", view);
                particleSystem->execute();


                ImGui_ImplGlfwGL3_NewFrame();

                // 1. Show a simple window
                // Tip: if we don't call ImGui::Begin()/ImGui::End() the widgets appears in a window automatically called "Debug"
                {
                    static float f = 0.0f;
                    ImGui::Text("Hello, world!");
                    ImGui::SliderFloat("float", &f, 0.0f, 1.0f);
                    ImGui::ColorEdit3("clear color", (float*)&clear_color);
                    if (ImGui::Button("Test Window")) show_test_window ^= 1;
                    if (ImGui::Button("Another Window")) show_another_window ^= 1;
                    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
                }

                // 2. Show another simple window, this time using an explicit Begin/End pair
                if (show_another_window)
                {
                    ImGui::SetNextWindowSize(ImVec2(200,100), ImGuiSetCond_FirstUseEver);
                    ImGui::Begin("Another Window", &show_another_window);
                    ImGui::Text("Hello");
                    ImGui::End();
                }

                // 3. Show the ImGui test window. Most of the sample code is in ImGui::ShowTestWindow()
                if (show_test_window)
                {
                    ImGui::SetNextWindowPos(ImVec2(650, 20), ImGuiSetCond_FirstUseEver);
                    ImGui::ShowTestWindow(&show_test_window);
                }

                // Rendering
                int display_w, display_h;
                glfwGetFramebufferSize(window, &display_w, &display_h);
                glViewport(0, 0, display_w, display_h);
                ImGui::Render();

                trackball.update(window,view);

                framecount++;
            };

	GLTools::render(window, loop);

	//cleanup
	GLTools::destroyWindow(window);
	delete shaderprogram;
	delete particleSystem;
	return 0;
}