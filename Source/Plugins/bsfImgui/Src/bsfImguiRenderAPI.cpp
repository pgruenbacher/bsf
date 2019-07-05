
#include "imgui.h"
#include "BsPrerequisites.h"
#include "RenderAPI/BsBlendState.h"
#include "Renderer/BsParamBlocks.h"
#include "Debug/BsBitmapWriter.h"
// #include "Mesh/BsMesh.h"
#include "RenderAPI/BsVertexDataDesc.h"
#include "Mesh/BsMeshHeap.h"
// #include "Mesh/BsTransientMesh.h"
#include "Material/BsMaterial.h"
#include "RenderAPI/BsIndexBuffer.h"
#include "RenderAPI/BsVertexBuffer.h"
#include "Importer/BsImporter.h"
#include "Material/BsShader.h"
#include "Resources/BsBuiltinResources.h"
#include "FileSystem/BsFileSystem.h"
#include "FileSystem/BsDataStream.h"
#include "Renderer/BsRendererUtility.h"

namespace bs {	

	HTexture ImGui_ImplBsf_CreateFontsTexture()
	{
	  // Build texture atlas
		// for now let's just use bsf default font.
	  // Build texture atlas
	  ImGuiIO& io = ImGui::GetIO();
	  unsigned char* pixels;
	  int width, height;
	  // io.Fonts->AddFontDefault();
	  io.Fonts->GetTexDataAsRGBA32(&pixels, &width, &height);   // Load as RGBA 32-bits (75% of the memory is wasted, but default font is so small) because it is more likely to be compatible with user's existing shaders. If your ImTextureId represent a higher-level concept than just a GL texture id, consider calling GetTexDataAsAlpha8() instead to save on GPU memory.


	  if (false) {
	  	// debug bmp output of the generated texture.
		  UINT32 bytesPerPixel = 4;
			UINT32 bmpDataSize = BitmapWriter::getBMPSize(width, height, bytesPerPixel);
			UINT8* bmpBuffer = bs_newN<UINT8>(bmpDataSize);
			for (int i = 0; i < width * height; ++i) {
				// convert alph to black for debugging.
				// since otherwise it won't be as easy to see the alpha.
				pixels[i*4] = pixels[i*4 + 3];
				pixels[i*4 + 1] = pixels[i*4 + 3];
				pixels[i*4 + 2] = pixels[i*4 + 3];
			}
			BitmapWriter::rawPixelsToBMP(pixels, bmpBuffer, width, height, bytesPerPixel);
			SPtr<DataStream> ds = FileSystem::createAndOpenFile("test.bmp");
			ds->write(bmpBuffer, bmpDataSize);
			ds->close();	  	
	  }

		TEXTURE_DESC textureDesc;
		textureDesc.format = PF_RGBA8;
		textureDesc.width = width;
		textureDesc.height = height;
		HTexture texture = Texture::create(textureDesc);
		SPtr<PixelData> pixelData = PixelData::create(width, height, 1, PF_RGBA8);
		Vector<Color> colors;
		for (int i = 0; i < width * height; ++i) {
			colors.push_back(Color(1,1,1, pixels[i*4 + 3] / 255.f));
		}
		pixelData->setColors(colors);
		texture->writeData(pixelData);

	  return texture;
	}

	static HMaterial gMaterial;

	void ImGui_ImplBsf_SetupPipeline() {

		// HShader shader = BuiltinResources::instance().getShader("Imgui.bsl");
		Path imguiPath = BuiltinResources::instance().getRawShaderFolder().append("Imgui.bsl");
		HShader shader = gImporter().import<Shader>(imguiPath);
		gMaterial = Material::create(shader);
		HTexture texture = ImGui_ImplBsf_CreateFontsTexture();
		gMaterial->setTexture("gMainTexture", texture);
	}

}

namespace bs::ct {

	static void ImGui_ImplBsf_SetupRenderState(ImDrawData* draw_data, int width, int height)
	{
    auto& renderAPI = RenderAPI::instance();
		SPtr<RenderWindow> renderWindow = gCoreApplication().getPrimaryWindow()->getCore();
		renderAPI.setRenderTarget(renderWindow);

		UINT32 passIdx = 0;
		UINT32 techniqueIdx = gMaterial->getDefaultTechnique();
		gRendererUtility().setPass(gMaterial->getCore(), passIdx, techniqueIdx);
		auto params = gMaterial->getCore()->createParamsSet(techniqueIdx);
		gRendererUtility().setPassParams(params, passIdx);


		// opengl2 notes...
    // // Setup viewport, orthographic projection matrix
    // // Our visible imgui space lies from draw_data->DisplayPos (top left) to draw_data->DisplayPos+data_data->DisplaySize (bottom right). DisplayPos is (0,0) for single viewport apps.
    // glViewport(0, 0, (GLsizei)fb_width, (GLsizei)fb_height);
    // glMatrixMode(GL_PROJECTION);
    // glPushMatrix();
    // glLoadIdentity();
    // glOrtho(draw_data->DisplayPos.x, draw_data->DisplayPos.x + draw_data->DisplaySize.x, draw_data->DisplayPos.y + draw_data->DisplaySize.y, draw_data->DisplayPos.y, -1.0f, +1.0f);
    // glMatrixMode(GL_MODELVIEW);
    // glPushMatrix();
    // glLoadIdentity();
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
				indexDesc.usage = GBU_STATIC;
        SPtr<IndexBuffer> ibuf = IndexBuffer::create(indexDesc);
        ibuf->writeData(0, indexDesc.numIndices, cmd_list->IdxBuffer.Data);

				renderAPI.setIndexBuffer(ibuf);
				renderAPI.setVertexBuffers(0, &vbuf, 1);
				renderAPI.setDrawOperation(DOT_TRIANGLE_LIST);

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
                	renderAPI.setScissorRect((int)clip_rect.x, (int)(fb_height - clip_rect.w), (int)(clip_rect.z - clip_rect.x), (int)(clip_rect.w - clip_rect.y));
									renderAPI.drawIndexed(0, indexDesc.numIndices, 0, desc.numVerts);
									std::cout << "DRAW? indices: " << indexDesc.numIndices << " verts: " << desc.numVerts << std::endl;
                }
            }
        }
    }
}

} // namespace bs::ct