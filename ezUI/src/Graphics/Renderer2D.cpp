#include "Renderer2D.h"
#include "Core/Base.h"
#include "Graphics/RenderAPI.h"
#include "Graphics/BuiltinShader.h"
#include "Graphics/ManagedBuffer.h"
#include "Utility/Profiling.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include <initializer_list>

namespace ez {
	static std::shared_ptr<RenderAPI> s_Device = nullptr;

	static int MAX_QUADS = 20000;

	struct QuadData {
		float Data[8];
	};

	struct RenderData {
		uint32_t RenderWidth;
		uint32_t RenderHeight;

		HShader QuadShader;
		HBuffer QuadBuffer;

		std::unique_ptr<ManagedBuffer<QuadData>> QuadDataBuffer;
	};

	static RenderData s_Data;

	void Renderer2D::Init(uint32_t width, uint32_t height) {
		EZ_PROFILE_FUNCTION();
		EZ_CORE_DEBUG_ALLOC("Initializing Renderer2D");
		s_Device = RenderAPI::Create();
		
		//Load Shader
		s_Data.QuadShader = s_Device->CreateShader(ez::DEFAULT_VERTEX_SHADER, ez::DEFAULT_FRAGMENT_SHADER);
		s_Device->BindShader(s_Data.QuadShader);

		s_Data.QuadDataBuffer = ManagedBuffer<QuadData>::Create(s_Device, MAX_QUADS);

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

		s_Device->SetShaderUniform(s_Data.QuadShader, "uProj", proj);
		s_Device->SetShaderUniform(s_Data.QuadShader, "uOffset", depthZ);

		s_Device->SetViewport(0, 0, width, height);
	}

	void Renderer2D::DrawRect(const glm::vec3& position, const glm::vec2& size, const glm::vec3& rotation) {
		EZ_PROFILE_FUNCTION();

		s_Data.QuadDataBuffer->PushBack(QuadData {
			position.x, position.y, position.z, size.x, size.y, rotation.x, rotation.y, rotation.z
		});
	}

	void Renderer2D::SetViewMatrix(const glm::mat4& matrix) {
		EZ_PROFILE_FUNCTION();
		glm::mat4 view(1.0f);
		view = glm::translate(view, glm::vec3(-0.5f * s_Data.RenderWidth, 0.5f * s_Data.RenderHeight, 0.0f));
		view = glm::scale(view, glm::vec3(1.0f, -1.0f, 1.0f));
		s_Device->SetShaderUniform(s_Data.QuadShader, "uView", view);
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
}