#include "imgui.h"
#include "./imgui_impl_bsf.h"

#include "BsPrerequisites.h"
#include "Input/BsInputFwd.h"
#include "Platform/BsPlatform.h"
#include "Platform/BsCursor.h"

namespace bs {


enum BsfClientApi
{
    GlfwClientApi_Unknown,
    GlfwClientApi_OpenGL,
    GlfwClientApi_Vulkan
};

static const char* ImGui_ImplGlfw_GetClipboardText(void* user_data)
{
    return Platform::copyFromClipboard().c_str();
}

static void ImGui_ImplGlfw_SetClipboardText(void* user_data, const char* text)
{
    Platform::copyToClipboard(text);
}


static CursorType g_MouseCursors[ImGuiMouseCursor_COUNT] = {
};

static void initCursorMap() {
 	g_MouseCursors[ImGuiMouseCursor_Arrow] = CursorType::Arrow;
    g_MouseCursors[ImGuiMouseCursor_TextInput] = CursorType::IBeam;
    g_MouseCursors[ImGuiMouseCursor_ResizeAll] = CursorType::ArrowDrag;
    g_MouseCursors[ImGuiMouseCursor_ResizeNS] = CursorType::SizeNS;
    g_MouseCursors[ImGuiMouseCursor_ResizeEW] = CursorType::SizeWE;
    g_MouseCursors[ImGuiMouseCursor_ResizeNESW] =  CursorType::SizeNESW;
    g_MouseCursors[ImGuiMouseCursor_ResizeNWSE] =  CursorType::SizeNWSE;
    g_MouseCursors[ImGuiMouseCursor_Hand] = CursorType::Deny;
}

static bool ImGui_ImplGlfw_Init(RenderWindow* window, bool install_callbacks, BsfClientApi client_api)
{
    // g_Window = window;
    // g_Time = 0.0;

    // Setup back-end capabilities flags
    ImGuiIO& io = ImGui::GetIO();
    io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors;         // We can honor GetMouseCursor() values (optional)
    io.BackendFlags |= ImGuiBackendFlags_HasSetMousePos;          // We can honor io.WantSetMousePos requests (optional, rarely used)
    io.BackendPlatformName = "imgui_impl_bsf";

    // Keyboard mapping. ImGui will use those indices to peek into the io.KeysDown[] array.
    io.KeyMap[ImGuiKey_Tab] = BC_TAB;
    io.KeyMap[ImGuiKey_LeftArrow] = BC_LEFT;
    io.KeyMap[ImGuiKey_RightArrow] = BC_RIGHT;
    io.KeyMap[ImGuiKey_UpArrow] = BC_UP;
    io.KeyMap[ImGuiKey_DownArrow] = BC_DOWN;
    io.KeyMap[ImGuiKey_PageUp] = BC_PGUP;
    io.KeyMap[ImGuiKey_PageDown] = BC_PGDOWN;
    io.KeyMap[ImGuiKey_Home] = BC_HOME;
    io.KeyMap[ImGuiKey_End] = BC_END;
    io.KeyMap[ImGuiKey_Insert] = BC_INSERT;
    io.KeyMap[ImGuiKey_Delete] = BC_DELETE;
    io.KeyMap[ImGuiKey_Backspace] = BC_BACK;
    io.KeyMap[ImGuiKey_Space] = BC_SPACE;
    io.KeyMap[ImGuiKey_Enter] = BC_RETURN;
    io.KeyMap[ImGuiKey_Escape] = BC_ESCAPE;
    io.KeyMap[ImGuiKey_A] = BC_A;
    io.KeyMap[ImGuiKey_C] = BC_C;
    io.KeyMap[ImGuiKey_V] = BC_V;
    io.KeyMap[ImGuiKey_X] = BC_X;
    io.KeyMap[ImGuiKey_Y] = BC_Y;
    io.KeyMap[ImGuiKey_Z] = BC_Z;

    io.SetClipboardTextFn = ImGui_ImplGlfw_SetClipboardText;
    io.GetClipboardTextFn = ImGui_ImplGlfw_GetClipboardText;
//     io.ClipboardUserData = g_Window;
// #if defined(_WIN32)
//     io.ImeWindowHandle = (void*)glfwGetWin32Window(g_Window);
// #endif

//     // Chain GLFW callbacks: our callbacks will call the user's previously installed callbacks, if any.
//     g_PrevUserCallbackMousebutton = NULL;
//     g_PrevUserCallbackScroll = NULL;
//     g_PrevUserCallbackKey = NULL;
//     g_PrevUserCallbackChar = NULL;
//     if (install_callbacks)
//     {
//         g_PrevUserCallbackMousebutton = glfwSetMouseButtonCallback(window, ImGui_ImplGlfw_MouseButtonCallback);
//         g_PrevUserCallbackScroll = glfwSetScrollCallback(window, ImGui_ImplGlfw_ScrollCallback);
//         g_PrevUserCallbackKey = glfwSetKeyCallback(window, ImGui_ImplGlfw_KeyCallback);
//         g_PrevUserCallbackChar = glfwSetCharCallback(window, ImGui_ImplGlfw_CharCallback);
//     }

//     g_ClientApi = client_api;
    initCursorMap();
    return true;
}



static void ImGui_ImplGlfw_UpdateMouseCursor()
{
    ImGuiIO& io = ImGui::GetIO();
    if ((io.ConfigFlags & ImGuiConfigFlags_NoMouseCursorChange))
        return;

    ImGuiMouseCursor imgui_cursor = ImGui::GetMouseCursor();
    if (imgui_cursor == ImGuiMouseCursor_None || io.MouseDrawCursor)
    {
        // Hide OS mouse cursor if imgui is drawing it or if it wants no cursor
        gCursor().hide();
    }
    else
    {
        // Show OS mouse cursor
        gCursor().setCursor(g_MouseCursors[imgui_cursor]);
        gCursor().show();
    }
}

// static void ImGui_ImplGlfw_UpdateMousePosAndButtons()
// {
//     // Update buttons
//     ImGuiIO& io = ImGui::GetIO();
//     for (int i = 0; i < IM_ARRAYSIZE(io.MouseDown); i++)
//     {
//         // If a mouse press event came, always pass it as "mouse held this frame", so we don't miss click-release events that are shorter than 1 frame.
//         io.MouseDown[i] = g_MouseJustPressed[i] || glfwGetMouseButton(g_Window, i) != 0;
//         g_MouseJustPressed[i] = false;
//     }

//     // Update mouse position
//     const ImVec2 mouse_pos_backup = io.MousePos;
//     io.MousePos = ImVec2(-FLT_MAX, -FLT_MAX);
// #ifdef __EMSCRIPTEN__
//     const bool focused = true; // Emscripten
// #else
//     const bool focused = glfwGetWindowAttrib(g_Window, GLFW_FOCUSED) != 0;
// #endif
//     if (focused)
//     {
//         if (io.WantSetMousePos)
//         {
//             glfwSetCursorPos(g_Window, (double)mouse_pos_backup.x, (double)mouse_pos_backup.y);
//         }
//         else
//         {
//             double mouse_x, mouse_y;
//             glfwGetCursorPos(g_Window, &mouse_x, &mouse_y);
//             io.MousePos = ImVec2((float)mouse_x, (float)mouse_y);
//         }
//     }
// }

}  // namespace bs