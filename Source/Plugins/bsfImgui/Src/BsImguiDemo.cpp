

#include "imgui.h"
#include "ImGuizmo.h"
#include "BsPrerequisites.h"
#include "RenderAPI/BsBlendState.h"
#include "Renderer/BsParamBlocks.h"
#include "Debug/BsBitmapWriter.h"
// #include "Mesh/BsMesh.h"
#include "Renderer/BsCamera.h"
#include "RenderAPI/BsVertexDataDesc.h"
#include "Mesh/BsMeshHeap.h"
// #include "Mesh/BsTransientMesh.h"
#include "Material/BsMaterial.h"
#include "RenderAPI/BsIndexBuffer.h"
#include "RenderAPI/BsVertexBuffer.h"
#include "Importer/BsImporter.h"
#include "Material/BsGpuParamsSet.h"
#include "Material/BsShader.h"
#include "Resources/BsBuiltinResources.h"
#include "FileSystem/BsFileSystem.h"
#include "FileSystem/BsDataStream.h"
#include "Renderer/BsRendererUtility.h"
#include "Renderer/BsRendererExtension.h"
#include "./imgui_impl_bsf.h"
#include "./BsImGuizmo.h"

namespace bs {

void BsDemoImguiUI() {

  static Transform transform;
	static bool show_demo_window = true;
	static bool show_another_window = false;
	static ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

  {
  	if (show_demo_window) {
  		ImGui::ShowDemoWindow(&show_demo_window);
  	}

    {
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

	  if (show_another_window)
	  {
	      ImGui::Begin("Another Window", &show_another_window);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
	      ImGui::Text("Hello from another window!");
	      if (ImGui::Button("Close Me"))
	          show_another_window = false;
	      ImGui::End();
	  }
  }
}

}  // namespace bs