// imgui implementation via a RendererExtension. The imgui rendering logic can
// occure on main thread while the actual frame starting/ending and rendering
// can occure on the core thread. There's no thread-safety guarantee though,
// so probably not good for user interface, but it seems relatively safe
// enough for developer use.

#pragma once

#include "Renderer/BsRendererExtension.h"

struct ImDrawData;

namespace bs {

/* 
 * generate the default imgui material with generated fontmap. Should be used as
 * part of the initialization of the ImguiRendererExtension
*/
HMaterial defaultImguiMaterial() ;

/* call demo imgui windows */
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