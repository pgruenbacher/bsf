
#include "imgui.h"
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




namespace bs {

static SPtr<RendererExtension> renderExt;
static SPtr<VertexDeclaration> gVertexDecl;
// static SPtr<GpuParamsSetType> gParamSet;
static HMaterial gMaterial;

void makeInterfaceFrame2() {
  ImGui_ImplBsf_NewFrame();
  ImGui::NewFrame();

  {
  	bool show_demo_window = true;
    bool show_another_window = false;
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
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

  ImGui::Render();
	// bs::ct::ImGui_ImplBsf_RenderDrawData(ImGui::GetDrawData());	
}
}


namespace bs::ct {


BS_PARAM_BLOCK_BEGIN(ImguiParamBlockDef)
	BS_PARAM_BLOCK_ENTRY(float, gInvViewportWidth)
	BS_PARAM_BLOCK_ENTRY(float, gInvViewportHeight)
	BS_PARAM_BLOCK_ENTRY(float, gViewportYFlip)
BS_PARAM_BLOCK_END

ImguiParamBlockDef gImguiParamBlockDef;

void ImGui_ImplBsf_RenderDrawData(ImDrawData* draw_data, const ct::Camera& camera);
class ImguiRendererExtension : public RendererExtension
{
public:
	ImguiRendererExtension() : 
		RendererExtension(RenderLocation::Overlay, 10)
	{}
	// ... other extension code

	bool check(const ct::Camera& camera) override {
		return true;
	}
	
	void render(const ct::Camera& camera) override {
		// std::cout << "RENDERER EXTENSION " << std::endl;
		makeInterfaceFrame2();
		ImGui_ImplBsf_RenderDrawData(ImGui::GetDrawData(), camera);
	}

};

	void ImGui_ImplBsf_SetupRenderState(const ct::Camera& camera, ImDrawData* draw_data, int width, int height)
	{
  //   auto& renderAPI = RenderAPI::instance();
		// SPtr<RenderWindow> renderWindow = gCoreApplication().getPrimaryWindow()->getCore();
		// renderAPI.setRenderTarget(renderWindow);

		float invViewportWidth = 1.0f / (camera.getViewport()->getPixelArea().width * 0.5f);
		float invViewportHeight = 1.0f / (camera.getViewport()->getPixelArea().height * 0.5f);
		float viewflipYFlip = (gCaps().conventions.ndcYAxis == Conventions::Axis::Down) ? -1.0f : 1.0f;

		// gMaterial->getCore()->setFloat("gInvViewportWidth", invViewportWidth);
		// gMaterial->getCore()->setFloat("gInvViewportHeight", invViewportHeight);
		// gMaterial->getCore()->setFloat("gViewportYFlip", viewflipYFlip);

		auto buffer = gImguiParamBlockDef.createBuffer();
		gImguiParamBlockDef.gInvViewportWidth.set(buffer, invViewportWidth);
		gImguiParamBlockDef.gInvViewportHeight.set(buffer, invViewportHeight);
		gImguiParamBlockDef.gViewportYFlip.set(buffer, viewflipYFlip);
		buffer->flushToGPU();


		UINT32 passIdx = 0;
		UINT32 techniqueIdx = gMaterial->getDefaultTechnique();

		SPtr<GpuParamsSet> paramSet = gMaterial->getCore()->createParamsSet(techniqueIdx);
		auto mParamBufferIdx = paramSet->getParamBlockBufferIndex("GUIParams");
		assert( paramSet->getParamBlockBufferIndex("NoParamsTest") == (UINT32)-1);
		assert(mParamBufferIdx != (UINT32)-1);
		paramSet->setParamBlockBuffer(mParamBufferIdx, buffer, true);
		// gMaterial->getCore()->updateParamsSet(paramSet);
		gRendererUtility().setPass(gMaterial->getCore(), passIdx, techniqueIdx);
		gRendererUtility().setPassParams(paramSet);


		// auto params = gMaterial->getCore()->createParamsSet(techniqueIdx);
		// gRendererUtility().setPassParams(params, passIdx);

		// std::cout << gMaterial->getCore()->getFloat("gInvViewportWidth") << std::endl;
		// std::cout << gMaterial->getCore()->getFloat("gInvViewportHeight") << std::endl;
		// std::cout << gMaterial->getCore()->getFloat("gViewportYFlip") << std::endl;

		// gGUISpriteParamBlockDef.gInvViewportWidth.set(buffer, invViewportWidth);
		// gGUISpriteParamBlockDef.gInvViewportHeight.set(buffer, invViewportHeight);
		// gGUISpriteParamBlockDef.gViewportYFlip.set(buffer, viewflipYFlip);
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

void ImGui_ImplBsf_RenderDrawData(ImDrawData* draw_data, const ct::Camera& camera) {
    // Avoid rendering when minimized, scale coordinates for retina displays (screen coordinates != framebuffer coordinates)
    int fb_width = (int)(draw_data->DisplaySize.x * draw_data->FramebufferScale.x);
    int fb_height = (int)(draw_data->DisplaySize.y * draw_data->FramebufferScale.y);
    if (fb_width == 0 || fb_height == 0)
        return;


    ImGui_ImplBsf_SetupRenderState(camera, draw_data, fb_width, fb_height);

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


				UINT32 numBuffers = 1;
				renderAPI.setVertexDeclaration(gVertexDecl->getCore());
				renderAPI.setVertexBuffers(0, &vbuf, numBuffers);
				renderAPI.setIndexBuffer(ibuf);
				renderAPI.setDrawOperation(DOT_TRIANGLE_LIST);

        for (int cmd_i = 0; cmd_i < cmd_list->CmdBuffer.Size; cmd_i++)
        {
            const ImDrawCmd* pcmd = &cmd_list->CmdBuffer[cmd_i];
            if (pcmd->UserCallback)
            {
                // User callback, registered via ImDrawList::AddCallback()
                // (ImDrawCallback_ResetRenderState is a special callback value used by the user to request the renderer to reset render state.)
                if (pcmd->UserCallback == ImDrawCallback_ResetRenderState)
                    ImGui_ImplBsf_SetupRenderState(camera, draw_data, fb_width, fb_height);
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
                	std::cout << "clip? " << clip_rect.x << " " << clip_rect.y << " " << fb_width << " " << fb_height << std::endl;
                    // // Apply scissor/clipping rectangle
                	renderAPI.setScissorRect(
                		(clip_rect.x), 
                		(fb_height - clip_rect.w), 
                		(clip_rect.z - clip_rect.x), 
                		(clip_rect.w - clip_rect.y)
                	);
									assert(pcmd->ElemCount % 3 == 0); // should always be triangle indices.
									assert(pcmd->VtxOffset == 0); // should always be zero
									assert(pcmd->IdxOffset < indexDesc.numIndices);
									renderAPI.drawIndexed(
										pcmd->IdxOffset, 
										pcmd->ElemCount, 
										pcmd->VtxOffset, 
										desc.numVerts);

									// if (cmd_i == 1) {
						   //      for (uint i = pcmd->IdxOffset; i < pcmd->IdxOffset + pcmd->ElemCount; ++i) {
						   //      	std::cout << "i ? " << std::to_string((cmd_list->IdxBuffer.Data)[i]) << std::endl;
						   //      	auto vert = (cmd_list->VtxBuffer.Data[(cmd_list->IdxBuffer.Data)[i]]);
						   //      	std::cout << "v ? " << std::to_string(vert.pos.x) << " " << std::to_string(vert.pos.y)  << std::endl;
						   //      }										
									// }

									std::cout << "CMD " << cmd_i << " DRAW? indices: " << pcmd->ElemCount 
										<< " texture: " << pcmd->TextureId
										<< " vtx offset " << pcmd->VtxOffset 
										<< " idx offset " << pcmd->IdxOffset
										<< " verts: " << desc.numVerts << std::endl;
                }
            }
        }
    }
}

} // namespace bs::ct


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


	void ImGui_ImplBsf_SetupPipeline() {

		// HShader shader = BuiltinResources::instance().getShader("Imgui.bsl");
		Path imguiPath = BuiltinResources::instance().getRawShaderFolder().append("Imgui.bsl");
		HShader shader = gImporter().import<Shader>(imguiPath);
		gMaterial = Material::create(shader);
		HTexture texture = ImGui_ImplBsf_CreateFontsTexture();
		gMaterial->setTexture("gMainTexture", texture);
		// no initial data necessary... so just pass in nullptr.
		renderExt = RendererExtension::create<ct::ImguiRendererExtension>(nullptr);


			auto vertexDeclDesc = bs_shared_ptr_new<VertexDataDesc>();
			vertexDeclDesc->addVertElem(VET_FLOAT2, VES_POSITION);
			vertexDeclDesc->addVertElem(VET_FLOAT2, VES_TEXCOORD);
			vertexDeclDesc->addVertElem(VET_COLOR, VES_COLOR);
			gVertexDecl = VertexDeclaration::create(vertexDeclDesc);
	}

}