#include "./imgui_impl_bsf.h"
#include "BsCorePrerequisites.h"
// class MyPlugin : Module<MyPlugin>
// {
// };

namespace bs {

static SPtr<ct::ImguiRendererExtension> gRendererExt;


extern "C" BS_PLUGIN_EXPORT const char* getPluginName()
{
	return "BsImgui";
}

extern "C" BS_PLUGIN_EXPORT void* loadPlugin()
{


  IMGUI_CHECKVERSION();
  ImGui::CreateContext();

  ImGui::StyleColorsDark();


	HMaterial imguiMaterial = defaultImguiMaterial();
	gRendererExt = RendererExtension::create<ct::ImguiRendererExtension>(imguiMaterial);
  
  initImgui();

	return nullptr; // Not used
}

extern "C" BS_PLUGIN_EXPORT void updatePlugin()
{
	// Do something every frame
	gRendererExt->syncImDrawDataToCore();
}

extern "C" BS_PLUGIN_EXPORT void unloadPlugin()
{
	// MyPlugin::shutDown();
	gRendererExt->destroy();
}

};  // namespace bs