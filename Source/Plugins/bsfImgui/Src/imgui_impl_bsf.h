
// dear imgui: Platform Binding for Bsf
// This needs to be used along with a Renderer (e.g. OpenGL3, Vulkan..)

#pragma once

#include "imgui.h"
#include "BsCorePrerequisites.h"
#include "Renderer/BsRendererExtension.h"

namespace bs {

class RenderWindow;
bool  initImgui();
void disconnectImgui();
void  updateImguiInputs();
HMaterial defaultImguiMaterial() ;
void demoImguiUI();

}   // namespace bs

namespace bs::ct {

class ImguiRendererExtension : public RendererExtension {
  Mutex mImguiRenderMutex;
  SPtr<GpuParamBlockBuffer> gBuffer;
  SPtr<VertexDeclaration> gVertexDecl;
  HMaterial gMaterial;

 public:
  ImguiRendererExtension();
  // ... other extension code

  void initialize(const Any& data) override;
  void destroy() override;

  bool check(const ct::Camera& camera) override;

  void render(const ct::Camera& camera) override;

  // must be called in the main thread.
  void syncImDrawDataToCore();

 private:
  void setupRenderState(const ct::Camera& camera, ImDrawData* draw_data,
                        int width, int height);
  void renderDrawData(ImDrawData* draw_data, const ct::Camera& camera);
};

}  // namespace bs::ct