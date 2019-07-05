#include "Testing/BsTestSuite.h"

#include "imgui.h"
#include "imgui_impl_bsf.h"
// #include "imgui_impl_glew.h"
#include "BsCoreApplication.h"
#include "CoreThread/BsCoreThread.h"
#include "BsApplication.h"
#include "Resources/BsBuiltinResources.h"
#include "Material/BsMaterial.h"
#include "Components/BsCRenderable.h"
#include "Scene/BsSceneObject.h"

namespace bs {

class ImguiTestSuite : public bs::Test {
};

// void makeInterfaceFrame() {
//   ImGui_ImplBsf_NewFrame();
//   ImGui::NewFrame();

//   {
//   	bool show_demo_window = true;
//     bool show_another_window = false;
//     ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
//       static float f = 0.0f;
//       static int counter = 0;

//       ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

//       ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
//       ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
//       ImGui::Checkbox("Another Window", &show_another_window);

//       ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
//       ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

//       if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
//           counter++;
//       ImGui::SameLine();
//       ImGui::Text("counter = %d", counter);

//       ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
//       ImGui::End();
//   }

//   ImGui::Render();
// 	// bs::ct::ImGui_ImplBsf_RenderDrawData(ImGui::GetDrawData());	
// }

class ApplicationWithImgui : public Application {
	SPtr<ct::ImguiRendererExtension> mImguiRenderExt;

	void onStartUp() override {
		Application::onStartUp();
  	HMaterial imguiMaterial = defaultImguiMaterial();
		mImguiRenderExt = RendererExtension::create<ct::ImguiRendererExtension>(imguiMaterial);
	}

	void preUpdate() override {
		Application::preUpdate();
		mImguiRenderExt->syncImDrawDataToCore();
	}
};

class DemoUI : public Component {
public:
	DemoUI(const HSceneObject& parent)
		: Component(parent)
	{
		setFlag(ComponentFlag::AlwaysRun, true);
		setName("DemoUI");
	}

	void update() override {
		std::cout << "DRAW DATA? " << std::endl;
		Component::update();
		demoImguiUI();
	}
};

HSceneObject addBox() {
	HShader shader = gBuiltinResources().getBuiltinShader(BuiltinShader::Standard);
	HMaterial material = Material::create(shader);
	HMesh boxMesh = gBuiltinResources().getMesh(BuiltinMesh::Box);
	HSceneObject boxSO = SceneObject::create("Box");
	HRenderable boxRenderable = boxSO->addComponent<CRenderable>();
	boxRenderable->setMesh(boxMesh);
	boxRenderable->setMaterial(material);
	boxSO->setPosition(Vector3(0.0f, 0.5f, 0.5f));
	return boxSO;
}

TEST_F(ImguiTestSuite, TestImgui) {  // Setup Dear ImGui context

	addFlyableCamera();

  // ImGuiIO& io = ImGui::GetIO(); (void)io;
  // Setup Dear ImGui style

  // SPtr<RenderWindow> window = gCoreApplication().getPrimaryWindow();
  // SPtr<GLContext> ctx =

 //  HMaterial imguiMaterial = defaultImguiMaterial();
	// SPtr<ct::ImguiRendererExtension> renderExt = RendererExtension::create<ct::ImguiRendererExtension>(imguiMaterial);

	DynLib* gRendererPlugin = nullptr;

	Application::instance().loadPlugin("bsfImgui", &gRendererPlugin);

  // makeInterfaceFrame();
  addBox();
  // auto obj = SceneObject::create("UI");
  // obj->addComponent<DemoUI>();
  // DemoUI::create();
	// Application::instance().runMainSteps(4);
	// while(true) {
	// 	renderExt->syncImDrawDataToCore();
	// 	Application::instance().runMainSteps(1);
	// }
	Application::instance().runMainLoop();

	Application::instance().unloadPlugin(gRendererPlugin);
  // cleanup.
  // ImGui_ImplBsf_Shutdown();
  // ImGui::DestroyContext();

}

}