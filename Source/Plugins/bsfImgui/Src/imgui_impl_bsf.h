
// dear imgui: Platform Binding for Bsf
// This needs to be used along with a Renderer (e.g. OpenGL3, Vulkan..)

#pragma once

namespace bs {
class RenderWindow;
IMGUI_IMPL_API bool     ImGui_ImplBsf_Init(bs::RenderWindow* window, bool install_callbacks = true);
// IMGUI_IMPL_API bool     ImGui_ImplBsf_InitForVulkan(bs::RenderWindow* window, bool install_callbacks = true);
IMGUI_IMPL_API void     ImGui_ImplBsf_Shutdown();
IMGUI_IMPL_API void     ImGui_ImplBsf_NewFrame();

bool ImGui_ImplBsf_CreateFontsTexture();
}

namespace bs::ct {
	void ImGui_ImplBsf_RenderDrawData(ImDrawData* draw_data);
}


// InitXXX function with 'install_callbacks=true': install GLFW callbacks. They will call user's previously installed callbacks, if any.
// InitXXX function with 'install_callbacks=false': do not install GLFW callbacks. You will need to call them yourself from your own GLFW callbacks.
// IMGUI_IMPL_API void     ImGui_ImplBsf_MouseButtonCallback(bs::RenderWindow* window, int button, int action, int mods);
// IMGUI_IMPL_API void     ImGui_ImplBsf_ScrollCallback(bs::RenderWindow* window, double xoffset, double yoffset);
// IMGUI_IMPL_API void     ImGui_ImplBsf_KeyCallback(bs::RenderWindow* window, int key, int scancode, int action, int mods);
// IMGUI_IMPL_API void     ImGui_ImplBsf_CharCallback(bs::RenderWindow* window, unsigned int c);