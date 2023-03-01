#include "Renderer2D.h"
#include "ez/Core/Base.h"
#include "ez/Core/Profiling.h"
#include "ez/Core/StrongHandle.h"

#include "ez/Graphics/API/RenderAPI.h"
#include "ez/Graphics/API/OpenGL/GL_BuiltinShader.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include <initializer_list>
#include <queue>

namespace ez::gfx {
	static std::shared_ptr<RenderAPI> s_Device = nullptr;

	constexpr int MAX_QUADS = 20000;

	struct QuadData {
		float Data[8];
		uint32_t Data2[4];
	};

	struct RenderData {
		const int MAX_BRUSHES = 256;

		uint32_t RenderWidth;
		uint32_t RenderHeight;

		std::shared_ptr<Shader> QuadShader;

		//HShader QuadShader;

		HTextureArray BrushTex;
		std::queue<uint32_t> BrushTexLayers;
		HandleStore<HBrush, uint32_t> Brushes;

		std::unique_ptr<ManagedBuffer<QuadData>> QuadDataBuffer;
	};

	static RenderData s_Data;

	void Renderer2D::Init(uint32_t width, uint32_t height) {
		EZ_PROFILE_FUNCTION();
		EZ_CORE_DEBUG_ALLOC("Initializing Renderer2D");
		s_Device = RenderAPI::Create(ez::gfx::API::OPENGL);

		//Load Shader
		s_Data.QuadShader = s_Device->CreateShader({ 
			{Shader::Type::VERTEX,		ez::gfx::GL_QUAD_VERTEX_SHADER}, 
			{Shader::Type::FRAGMENT,	ez::gfx::GL_QUAD_VERTEX_SHADER} 
		});
		s_Data.QuadShader->Bind();

		
		s_Data.QuadDataBuffer = ManagedBuffer<QuadData>::Create(s_Device, MAX_QUADS);

		s_Data.BrushTex = s_Device->CreateTextureArray(2, 2, s_Data.MAX_BRUSHES, TextureFormat::RGBA, TextureFilter::LINEAR);
		s_Device->BindTextureSlot(s_Data.BrushTex, 0);
		s_Device->SetShaderUniform(s_Data.QuadShader, "uBrushTex", 0);

		for (int i = 0; i < s_Data.MAX_BRUSHES; i++) {
			s_Data.BrushTexLayers.push(i);
		}

		//Setup default state
		s_Device->SetClearColor(0.15f, 0.15f, 0.15f, 1.0f);
		SetRenderSize(width, height);
	}

	void Renderer2D::SetRenderSize(uint32_t width, uint32_t height) {
		EZ_PROFILE_FUNCTION();

		s_Data.RenderWidth = width;
		s_Data.RenderHeight = height;

		float fovy = glm::radians(45.0f);
		float aspect = (float)s_Data.RenderWidth / (float)s_Data.RenderHeight;
		float depthZ = (-1.0f * s_Data.RenderHeight) / (2.0f * tanf(fovy / 2.0f));
		glm::mat4 proj = glm::perspective(fovy, aspect, 1.0f, s_Data.RenderHeight * 2.0f);

		s_Data.QuadShader->Set("uProj", proj);
		s_Data.QuadShader->Set("uOffset", depthZ);

		s_Device->SetViewport(0, 0, width, height);
	}

	void Renderer2D::SetViewMatrix(const glm::mat4& matrix) {
		EZ_PROFILE_FUNCTION();
		glm::mat4 view(1.0f);
		view = glm::translate(view, glm::vec3(-0.5f * s_Data.RenderWidth, 0.5f * s_Data.RenderHeight, 0.0f));
		view = glm::scale(view, glm::vec3(1.0f, -1.0f, 1.0f));
		s_Data.QuadShader->Set("uView", view);
	}

	void Renderer2D::BeginScene() {
		EZ_PROFILE_FUNCTION();
		s_Data.QuadDataBuffer->BeginScene();
	}

	void Renderer2D::EndScene() {
		EZ_PROFILE_FUNCTION();
		s_Device->Clear();
		s_Device->DrawArrays(DrawMode::TRIANGLES, s_Data.QuadDataBuffer->GetCount() * 6, s_Data.QuadDataBuffer->GetOffset() * 6);
		s_Data.QuadDataBuffer->EndScene();
	}

	void Renderer2D::Shutdown() {
		EZ_PROFILE_FUNCTION();
		EZ_CORE_DEBUG_DEALLOC("Shutting down Renderer2D");
	}

	HBrush Renderer2D::CreateSolidColorBrush(const glm::vec4& color) {
		uint8_t r = color.r * 255;
		uint8_t g = color.g * 255;
		uint8_t b = color.b * 255;
		uint8_t a = color.a * 255;


		uint8_t colorData[]{
			r, g, b, a,
			r, g, b, a,
			r, g, b, a,
			r, g, b, a
		};

		uint32_t layer = s_Data.BrushTexLayers.front();
		s_Data.BrushTexLayers.pop();
		s_Device->UploadTexture(s_Data.BrushTex, layer, colorData);

		return s_Data.Brushes.PushBack(layer);
	}

	void Renderer2D::DrawRect(HBrush brush, const glm::vec3& position, const glm::vec2& size, const glm::vec3& rotation) {
		EZ_PROFILE_FUNCTION();

		s_Data.QuadDataBuffer->PushBack(QuadData{
			position.x, position.y, position.z, size.x, size.y, rotation.x, rotation.y, rotation.z, s_Data.Brushes.Get(brush), 0, 0, 0
		});
	}

	void Renderer2D::DrawRect(HBrush brush, const glm::vec3& position, const glm::vec2& size) {
		s_Data.QuadDataBuffer->PushBack(QuadData{
			position.x, position.y, position.z, size.x, size.y, 0, 0, 0, s_Data.Brushes.Get(brush), 0, 0, 0
		});
	}
}