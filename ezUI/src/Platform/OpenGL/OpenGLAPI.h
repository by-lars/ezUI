#pragma once
#include "Graphics/RenderAPI.h"
#include <vector>

namespace ez::gfx {
	struct GLShader {
		uint32_t Id;
	};

	struct GLBuffer {
		uint32_t Id;
		uint32_t Type;
		uint32_t SizeInBytes;
	};

	struct GLTexture {
		uint32_t Id;
		uint32_t Format;
		uint32_t Width;
		uint32_t Height;
	};

	struct GLTextureArray {
		uint32_t Id;
		uint32_t Format;
		uint32_t Width;
		uint32_t Height;
		uint32_t Layers;
	};

	class OpenGLAPI : public RenderAPI {
	public:
		OpenGLAPI();
		~OpenGLAPI();

		int QueryInfo(DeviceInfo info) override;

		HShader CreateShader(const std::string& vertex, const std::string& fragment) override;
		void SetShaderUniform(HShader hShader, const std::string& location, const glm::mat4& matrix) override;
		void SetShaderUniform(HShader hShader, const std::string& location, float value) override;
		void SetShaderUniform(HShader hShader, const std::string& location, int value) override;
		void BindShader(HShader hShader) override;

		HBuffer CreateBuffer(uint32_t size, BufferType type, void* data) override;
		void* GetBufferPointer(HBuffer hBuffer) override;

		HTexture CreateTexture(uint32_t width, uint32_t height, TextureFormat format, TextureFilter filter) override;
		HTextureArray CreateTextureArray(uint32_t width, uint32_t height, uint32_t layers, TextureFormat format, TextureFilter filter) override;
		void UploadTexture(HTexture hTexture, void* data) override;
		void UploadTexture(HTextureArray hTexture, uint32_t layer, void* data) override;
		void BindTextureSlot(HTexture hTexture, uint32_t slot) override;
		void BindTextureSlot(HTextureArray hTexture, uint32_t slot) override;

		void Clear() override;
		void SetClearColor(float r, float g, float b, float a) override;
		void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) override;

		void DrawArrays(DrawMode mode, uint32_t count, uint32_t offset) override;
	private:
		std::vector<GLBuffer> m_Buffers;
		std::vector<GLShader> m_Shaders;
		std::vector<GLTexture> m_Textures;
		std::vector<GLTextureArray> m_TextureArrays;

		uint32_t m_VAO;
	};
}