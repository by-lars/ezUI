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
	constexpr int MAX_QUADS = 1000000;//20000;

	struct QuadData {
		float Data[8];
		uint32_t Data2[4];
	};

	static Ref<RenderAPI> s_Device;

	struct RendererData {
		Ref<TextureArray> Brushes;
		Ref<StreamStorage> QuadStorage;
		Ref<Shader> QuadShader;

		uint32_t RenderWidth;
		uint32_t RenderHeight;
	};

	static RendererData s_Data;

	void Renderer2D::Init(uint32_t width, uint32_t height) {
		EZ_PROFILE_FUNCTION();
		EZ_CORE_DEBUG_ALLOC("Initializing Renderer2D");
		s_Device = RenderAPI::Create(ez::gfx::API::OPENGL);

		//Load Shader
		s_Data.QuadShader = s_Device->CreateShader({ 
			{Shader::Type::VERTEX,		ez::gfx::GL_QUAD_VERTEX_SHADER}, 
			{Shader::Type::FRAGMENT,	ez::gfx::GL_QUAD_FRAGMENT_SHADER} 
		});
		s_Data.QuadShader->Bind();

		s_Data.QuadStorage = s_Device->CreateStreamStorage(MAX_QUADS, sizeof(QuadData));

		s_Data.Brushes = s_Device->CreateTextureArray(2, 2, Format::RGBA, Filter::LINEAR);
		s_Data.Brushes->BindToSlot(0);

		s_Data.QuadShader->Set("uBrushTex", 0);

		EZ_CORE_DEBUG("Max Texture Array Layers: ", s_Data.Brushes->GetMaxLayers());

		//Setup default state
		s_Device->SetClearColor(0.15f, 0.15f, 0.15f, 1.0f);
		SetRenderSize(width, height);
	}

	void Renderer2D::Shutdown() {
		EZ_PROFILE_FUNCTION();
		EZ_CORE_DEBUG_DEALLOC("Shutting down Renderer2D");
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

	void Renderer2D::BeginFrame() {
		EZ_PROFILE_FUNCTION();
		s_Data.QuadStorage->BeginFrame();
	}

	void Renderer2D::EndFrame() {
		EZ_PROFILE_FUNCTION();
		s_Device->Clear();
		s_Device->DrawArrays(DrawMode::TRIANGLES, s_Data.QuadStorage->Count() * 6, s_Data.QuadStorage->Offset() * 6);
		s_Data.QuadStorage->EndFrame();
	}

	Brush Renderer2D::CreateSolidColorBrush(glm::vec4 color) {
		color *= 255.0f;

		uint8_t colorData[]{
			color.r, color.g, color.b, color.a,
			color.r, color.g, color.b, color.a,
			color.r, color.g, color.b, color.a,
			color.r, color.g, color.b, color.a,
		};

		return Brush((int32_t)s_Data.Brushes->PushBack(colorData));
	}

	Brush Renderer2D::CreateGradientBrush(glm::vec4 color1, glm::vec4 color2) {
		color1 *= 255.0f;
		color2 *= 255.0f;

		uint8_t colorData[]{
			color1.r, color1.g, color1.b, color1.a,
			color1.r, color1.g, color1.b, color1.a,
			color2.r, color2.g, color2.b, color2.a,
			color2.r, color2.g, color2.b, color2.a,
		};

		return Brush((int32_t)s_Data.Brushes->PushBack(colorData));
	}


	void Renderer2D::DrawRect(Brush brush, const glm::vec3& position, const glm::vec2& size, const glm::vec3& rotation) {
		EZ_PROFILE_FUNCTION();

		QuadData data{
			position.x, position.y, position.z, size.x, size.y, rotation.x, rotation.y, rotation.z, (int32_t)brush, 0, 0, 0
		};

		s_Data.QuadStorage->PushBack(&data);
	}

	void Renderer2D::DrawRect(Brush brush, const glm::vec3& position, const glm::vec2& size) {
		EZ_PROFILE_FUNCTION();

		QuadData data {
			position.x, position.y, position.z, size.x, size.y, 0, 0, 0, (int32_t)brush, 0, 0, 0
		};

		s_Data.QuadStorage->PushBack(&data);
	}
}