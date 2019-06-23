
#include "imgui.h"
#include "BsPrerequisites.h"
#include "RenderAPI/BsBlendState.h"
#include "Renderer/BsParamBlocks.h"
#include "Debug/BsBitmapWriter.h"
// #include "Mesh/BsMesh.h"
#include "RenderAPI/BsVertexDataDesc.h"
#include "Mesh/BsMeshHeap.h"
// #include "Mesh/BsTransientMesh.h"
#include "RenderAPI/BsIndexBuffer.h"
#include "RenderAPI/BsVertexBuffer.h"

namespace bs {	

	class ImguiRenderer {
		ImguiRenderer() {

				auto vertexDeclDesc = bs_shared_ptr_new<VertexDataDesc>();
				vertexDeclDesc->addVertElem(VET_FLOAT2, VES_POSITION);
				vertexDeclDesc->addVertElem(VET_FLOAT2, VES_TEXCOORD);
				vertexDeclDesc->addVertElem(VET_COLOR, VES_COLOR);
				vertexDecl = VertexDeclaration::create(vertexDeclDesc);

				// UINT32 iniitialVertices = 1000;
				// UINT32 initialIndices = 3000;
				// meshHeap = MeshHeap::create(iniitialVertices, initialIndices, vertexDeclDesc);
		}

		public:
			// SPtr<TransientMesh> createMesh(ImDrawList* drawList) {
   //      MESH_DESC meshDesc;
   //      meshDesc.vertexDesc = vertexDesc;
   //      meshDesc.numVertices = drawList->VtxBuffer.size;
   //      meshDesc.numIndices = drawList->IdxBuffer.size;
   //      SPtr<MeshData> meshData = MeshData::create(meshDesc.numVertices, meshDesc.numIndices, vertexDesc);

   //      const ImDrawVert* vtx_buffer = cmd_list->VtxBuffer.Data;
   //      const ImDrawIdx* idx_buffer = cmd_list->IdxBuffer.Data;
   //      meshData->setVertexData(VES_POSITION, vtx_buffer + IM_OFFSETOF(ImDrawVert, pos));
   //      meshData->setVertexData(VES_TEXCOORD, vtx_buffer + IM_OFFSETOF(ImDrawVert, pos));
   //      meshData->setVertexData(VES_POSITION, vtx_buffer + IM_OFFSETOF(ImDrawVert, pos));
   //      // glVertexPointer(2, GL_FLOAT, sizeof(ImDrawVert), (const GLvoid*)((const char*)vtx_buffer + IM_OFFSETOF(ImDrawVert, pos)));
   //      // glTexCoordPointer(2, GL_FLOAT, sizeof(ImDrawVert), (const GLvoid*)((const char*)vtx_buffer + IM_OFFSETOF(ImDrawVert, uv)));
   //      // glColorPointer(4, GL_UNSIGNED_BYTE, sizeof(ImDrawVert), (const GLvoid*)((const char*)vtx_buffer + IM_OFFSETOF(ImDrawVert, col)));



			// 	meshHeap.alloc(meshData);
			// }

		private:
			// SPtr<IndexBuffer> indexBuffer;
			// SPtr<VertexBuffer> vertexBuffer;
			SPtr<VertexDeclaration> vertexDecl;
			// SPtr<MeshHeap> meshHeap;
	};
}

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


	PIPELINE_STATE_DESC pipelineDesc;
	pipelineDesc.blendState = BlendState::create(blendDesc);
	

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

void ImGui_ImplBsf_RenderDrawData(ImDrawData* draw_data) {
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

    std::cout << "CMD LISTS COUTN " << draw_data->CmdListsCount << std::endl;
    for (int n = 0; n < draw_data->CmdListsCount; n++)
    {
        const ImDrawList* cmd_list = draw_data->CmdLists[n];
        // const ImDrawVert* vtx_buffer = cmd_list->VtxBuffer.Data;
        // const ImDrawIdx* idx_buffer = cmd_list->IdxBuffer.Data;

        VERTEX_BUFFER_DESC desc;
				desc.vertexSize = sizeof(ImDrawVert); 
				desc.numVerts = cmd_list->VtxBuffer.Size; 
				desc.usage = GBU_STATIC;
        SPtr<VertexBuffer> vbuf = VertexBuffer::create(desc);
        vbuf->writeData(0, desc.numVerts, cmd_list->VtxBuffer.Data);

        // assert that imgui index type is 16 bit or 2 bytes.
        static_assert(sizeof(ImDrawIdx) == 2);
        INDEX_BUFFER_DESC indexDesc;
				indexDesc.indexType = IT_16BIT;
				indexDesc.numIndices = cmd_list->IdxBuffer.Size; 
				indexDesc.usage = GBU_STATIC; // BWT_DISCARD???
        SPtr<IndexBuffer> ibuf = IndexBuffer::create(indexDesc);
        ibuf->writeData(0, indexDesc.numIndices, cmd_list->VtxBuffer.Data);

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
									renderAPI.setIndexBuffer(ibuf);
									renderAPI.setVertexBuffers(0, &vbuf, 1);
  								renderAPI.setDrawOperation(DOT_TRIANGLE_LIST);
									renderAPI.drawIndexed(0, indexDesc.numIndices, 0, desc.numVerts);
                }
            }
        }
    }
}

} // namespace bs::ct

namespace bs {


	bool ImGui_ImplBsf_CreateFontsTexture()
	{
	  // Build texture atlas
		// for now let's just use bsf default font.
	  // Build texture atlas
	  ImGuiIO& io = ImGui::GetIO();
	  unsigned char* pixels;
	  int width, height;
	  io.Fonts->GetTexDataAsRGBA32(&pixels, &width, &height);   // Load as RGBA 32-bits (75% of the memory is wasted, but default font is so small) because it is more likely to be compatible with user's existing shaders. If your ImTextureId represent a higher-level concept than just a GL texture id, consider calling GetTexDataAsAlpha8() instead to save on GPU memory.

	  UINT32 bytesPerPixel = 32;
		UINT32 bmpDataSize = BitmapWriter::getBMPSize(width, height, bytesPerPixel);
		UINT8* bmpBuffer = bs_newN<UINT8>(bmpDataSize);

		BitmapWriter::rawPixelsToBMP(pixels, bmpBuffer, width, height, bytesPerPixel);

	  return true;
	}
}