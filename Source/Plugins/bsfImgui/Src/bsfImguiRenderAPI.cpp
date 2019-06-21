
#include "imgui.h"
#include "BsPrerequisites.h"
#include "RenderAPI/BsBlendState.h"
#include "Renderer/BsParamBlocks.h"

namespace bs::ct {

	BS_PARAM_BLOCK_BEGIN(GUISpriteParamBlockDef)
		BS_PARAM_BLOCK_ENTRY(Matrix4, gWorldTransform)
		BS_PARAM_BLOCK_ENTRY(float, gInvViewportWidth)
		BS_PARAM_BLOCK_ENTRY(float, gInvViewportHeight)
		BS_PARAM_BLOCK_ENTRY(float, gViewportYFlip)
		BS_PARAM_BLOCK_ENTRY(Color, gTint)
		BS_PARAM_BLOCK_ENTRY(Vector4, gUVSizeOffset)
	BS_PARAM_BLOCK_END

	GUISpriteParamBlockDef gGUISpriteParamBlockDef;


static void ImGui_ImplBsf_SetupRenderState(ImDrawData* draw_data, int width, int height)
{
    // Setup blending
    // al_set_blender(ALLEGRO_ADD, ALLEGRO_ALPHA, ALLEGRO_INVERSE_ALPHA);
	BLEND_STATE_DESC desc;
	desc.renderTargetDesc[0].blendEnable = true; // Enable blending
	desc.renderTargetDesc[0].srcBlend = BF_SOURCE_ALPHA; // Use the current alpha value to blend the source (new value)
	desc.renderTargetDesc[0].dstBlend = BF_INV_SOURCE_ALPHA; // Use the inverse of the current alpha value to blend the destination (stored value)
	desc.renderTargetDesc[0].blendOp = BO_ADD; // Add the source and destination together

    // Setup orthographic projection matrix
    // Our visible imgui space lies from draw_data->DisplayPos (top left) to draw_data->DisplayPos+data_data->DisplaySize (bottom right).
    {
        // float L = draw_data->DisplayPos.x;
        // float R = draw_data->DisplayPos.x + draw_data->DisplaySize.x;
        // float T = draw_data->DisplayPos.y;
        // float B = draw_data->DisplayPos.y + draw_data->DisplaySize.y;
        // Transform transform;
        // al_identity_transform(&transform);
        // al_use_transform(&transform);
        // al_orthographic_transform(&transform, L, T, 1.0f, R, B, -1.0f);
        // al_use_projection_transform(&transform);
    }
}

void render(ImDrawData* draw_data) {
    // Avoid rendering when minimized, scale coordinates for retina displays (screen coordinates != framebuffer coordinates)
    int fb_width = (int)(draw_data->DisplaySize.x * draw_data->FramebufferScale.x);
    int fb_height = (int)(draw_data->DisplaySize.y * draw_data->FramebufferScale.y);
    if (fb_width == 0 || fb_height == 0)
        return;

    ImGui_ImplBsf_SetupRenderState(draw_data, fb_width, fb_height);

    auto& renderAPI = RenderAPI::instance();

    // Will project scissor/clipping rectangles into framebuffer space
    ImVec2 clip_off = draw_data->DisplayPos;         // (0,0) unless using multi-viewports
    ImVec2 clip_scale = draw_data->FramebufferScale; // (1,1) unless using retina display which are often (2,2)

    for (int n = 0; n < draw_data->CmdListsCount; n++)
    {
        const ImDrawList* cmd_list = draw_data->CmdLists[n];
        const ImDrawVert* vtx_buffer = cmd_list->VtxBuffer.Data;
        const ImDrawIdx* idx_buffer = cmd_list->IdxBuffer.Data;

        for (int cmd_i = 0; cmd_i < cmd_list->CmdBuffer.Size; cmd_i++)
        {
            const ImDrawCmd* pcmd = &cmd_list->CmdBuffer[cmd_i];
            if (pcmd->UserCallback)
            {
                // User callback, registered via ImDrawList::AddCallback()
                // (ImDrawCallback_ResetRenderState is a special callback value used by the user to request the renderer to reset render state.)
                if (pcmd->UserCallback == ImDrawCallback_ResetRenderState)
                    ImGui_ImplBsf_SetupRenderState(draw_data, fb_width, fb_height);
                else
                    pcmd->UserCallback(cmd_list, pcmd);
            } else {
                // Project scissor/clipping rectangles into framebuffer space
                ImVec4 clip_rect;
                clip_rect.x = (pcmd->ClipRect.x - clip_off.x) * clip_scale.x;
                clip_rect.y = (pcmd->ClipRect.y - clip_off.y) * clip_scale.y;
                clip_rect.z = (pcmd->ClipRect.z - clip_off.x) * clip_scale.x;
                clip_rect.w = (pcmd->ClipRect.w - clip_off.y) * clip_scale.y;

                if (clip_rect.x < fb_width && clip_rect.y < fb_height && clip_rect.z >= 0.0f && clip_rect.w >= 0.0f)
                {

                    // // Apply scissor/clipping rectangle
                    // glScissor((int)clip_rect.x, (int)(fb_height - clip_rect.w), (int)(clip_rect.z - clip_rect.x), (int)(clip_rect.w - clip_rect.y));
                	renderAPI.setScissorRect((int)clip_rect.x, (int)(fb_height - clip_rect.w), (int)(clip_rect.z - clip_rect.x), (int)(clip_rect.w - clip_rect.y));
                    // // Bind texture, Draw
                    // glBindTexture(GL_TEXTURE_2D, (GLuint)(intptr_t)pcmd->TextureId);
                    // glDrawElements(GL_TRIANGLES, (GLsizei)pcmd->ElemCount, sizeof(ImDrawIdx) == 2 ? GL_UNSIGNED_SHORT : GL_UNSIGNED_INT, idx_buffer);
                }
            }
        }
    }
}


// void render(ImDrawData* _drawData) {
//   const ImGuiIO& io = ImGui::GetIO();
//   const float width = io.DisplaySize.x;
//   const float height = io.DisplaySize.y;

//   bgfx::setViewName(m_viewId, "ImGui");
//   bgfx::setViewMode(m_viewId, bgfx::ViewMode::Sequential);

//   const bgfx::Caps* caps = bgfx::getCaps();
//   {
//     float ortho[16];
//     bx::mtxOrtho(ortho, 0.0f, width, height, 0.0f, 0.0f, 1000.0f, 0.0f,
//                  caps->homogeneousDepth);
//     bgfx::setViewTransform(m_viewId, NULL, ortho);
//     bgfx::setViewRect(m_viewId, 0, 0, uint16_t(width), uint16_t(height));
//   }

//   // Render command lists
//   for (int32_t ii = 0, num = _drawData->CmdListsCount; ii < num; ++ii) {
//     bgfx::TransientVertexBuffer tvb;
//     bgfx::TransientIndexBuffer tib;

//     const ImDrawList* drawList = _drawData->CmdLists[ii];
//     uint32_t numVertices = (uint32_t)drawList->VtxBuffer.size();
//     uint32_t numIndices = (uint32_t)drawList->IdxBuffer.size();

//     if (!checkAvailTransientBuffers(numVertices, m_decl, numIndices)) {
//       // not enough space in transient buffer just quit drawing the rest...
//       break;
//     }

//     bgfx::allocTransientVertexBuffer(&tvb, numVertices, m_decl);
//     bgfx::allocTransientIndexBuffer(&tib, numIndices);

//     ImDrawVert* verts = (ImDrawVert*)tvb.data;
//     bx::memCopy(verts, drawList->VtxBuffer.begin(),
//                 numVertices * sizeof(ImDrawVert));

//     ImDrawIdx* indices = (ImDrawIdx*)tib.data;
//     bx::memCopy(indices, drawList->IdxBuffer.begin(),
//                 numIndices * sizeof(ImDrawIdx));

//     uint32_t offset = 0;
//     for (const ImDrawCmd *cmd = drawList->CmdBuffer.begin(),
//                          *cmdEnd = drawList->CmdBuffer.end();
//          cmd != cmdEnd; ++cmd) {
//       if (cmd->UserCallback) {
//         cmd->UserCallback(drawList, cmd);
//       } else if (0 != cmd->ElemCount) {
//         uint64_t state =
//             0 | BGFX_STATE_WRITE_RGB | BGFX_STATE_WRITE_A | BGFX_STATE_MSAA;

//         bgfx::TextureHandle th = m_texture;
//         bgfx::ProgramHandle program = m_program;

//         if (NULL != cmd->TextureId) {
//           union {
//             ImTextureID ptr;
//             struct {
//               bgfx::TextureHandle handle;
//               uint8_t flags;
//               uint8_t mip;
//             } s;
//           } texture = {cmd->TextureId};
//           state |= 0 != (IMGUI_FLAGS_ALPHA_BLEND & texture.s.flags)
//                        ? BGFX_STATE_BLEND_FUNC(BGFX_STATE_BLEND_SRC_ALPHA,
//                                                BGFX_STATE_BLEND_INV_SRC_ALPHA)
//                        : BGFX_STATE_NONE;
//           th = texture.s.handle;
//           if (0 != texture.s.mip) {
//             const float lodEnabled[4] = {float(texture.s.mip), 1.0f, 0.0f,
//                                          0.0f};
//             bgfx::setUniform(u_imageLodEnabled, lodEnabled);
//             program = m_imageProgram;
//           }
//         } else {
//           state |= BGFX_STATE_BLEND_FUNC(BGFX_STATE_BLEND_SRC_ALPHA,
//                                          BGFX_STATE_BLEND_INV_SRC_ALPHA);
//         }

//         const uint16_t xx = uint16_t(bx::max(cmd->ClipRect.x, 0.0f));
//         const uint16_t yy = uint16_t(bx::max(cmd->ClipRect.y, 0.0f));
//         bgfx::setScissor(xx, yy,
//                          uint16_t(bx::min(cmd->ClipRect.z, 65535.0f) - xx),
//                          uint16_t(bx::min(cmd->ClipRect.w, 65535.0f) - yy));

//         bgfx::setState(state);
//         bgfx::setTexture(0, s_tex, th);
//         bgfx::setVertexBuffer(0, &tvb, 0, numVertices);
//         bgfx::setIndexBuffer(&tib, offset, cmd->ElemCount);
//         bgfx::submit(m_viewId, program);
//       }

//       offset += cmd->ElemCount;
//     }
//   }
// }

} // namespace bs::ct