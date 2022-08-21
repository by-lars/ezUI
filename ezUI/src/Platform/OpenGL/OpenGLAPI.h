#pragma once
#include "Graphics/RenderAPI.h"
#include <vector>

namespace ez {
	struct GLShader {
		uint32_t Id;
	};

	struct GLBuffer {
		uint32_t Id;
		uint32_t Type;
		uint32_t SizeInBytes;
	};

	class OpenGLAPI : public RenderAPI {
	public:
		OpenGLAPI();
		~OpenGLAPI();
	
		HShader CreateShader(const std::string& vertex, const std::string& fragment) override;
		void SetShaderUniform(HShader hShader, const std::string& location, const glm::mat4& matrix) override;
		void SetShaderUniform(HShader hShader, const std::string& location, float value) override;
		void BindShader(HShader hShader) override;
		
		HBuffer CreateBuffer(uint32_t size, BufferType type, void* data) override;
		void* GetBufferPointer(HBuffer hBuffer) override;

		void Clear() override;
		void SetClearColor(float r, float g, float b, float a) override;
		void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) override;

		void DrawArrays(DrawMode mode, uint32_t count, uint32_t offset) override;
	private:
		std::vector<GLBuffer> m_Buffers;
		std::vector<GLShader> m_Shaders;

		uint32_t m_VAO;
	};
}