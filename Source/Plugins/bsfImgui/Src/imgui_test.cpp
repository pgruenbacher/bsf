#include "Testing/BsTestSuite.h"

#include "imgui.h"
#include "imgui_impl_bsf.h"
// #include "imgui_impl_glew.h"
#include "BsCoreApplication.h"
#include "CoreThread/BsCoreThread.h"

namespace bs {

class ImguiTestSuite : public bs::Test {};

void asyncRender() {
	bs::ct::ImGui_ImplBsf_RenderDrawData(ImGui::GetDrawData());	
}

TEST_F(ImguiTestSuite, TestImgui) {  // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    // Setup Dear ImGui style
    ImGui::StyleColorsDark();

    SPtr<RenderWindow> window = gCoreApplication().getPrimaryWindow();
    // SPtr<GLContext> ctx =

    ImGui_ImplBsf_Init(window.get());
    // ImGui_ImplOpenGL2_Init();


    // 2. Show a simple window that we create ourselves. We use a Begin/End pair to created a named window.

    bool done = false;	
    while(!done) {
	    // ImGui_ImplBsf_NewFrame();
	    ImGui_ImplBsf_NewFrame();
	    ImGui::NewFrame();

	    {
	    	bool show_demo_window = true;
		    bool show_another_window = false;
		    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
	        static float f = 0.0f;
	        static int counter = 0;

	        ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

	        ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
	        ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
	        ImGui::Checkbox("Another Window", &show_another_window);

	        ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
	        ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

	        if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
	            counter++;
	        ImGui::SameLine();
	        ImGui::Text("counter = %d", counter);

	        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	        ImGui::End();
	    }

        ImGui::Render();

        gCoreThread().queueCommand(&asyncRender);
        bool waitToComplete = true;
        gCoreThread().submit(waitToComplete);
    }

    // cleanup.
    ImGui_ImplBsf_Shutdown();
    ImGui::DestroyContext();

}

}