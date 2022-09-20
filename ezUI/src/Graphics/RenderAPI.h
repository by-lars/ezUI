#pragma once
#include "Graphics/DeviceResources.h"
#include <string>
#include <glm/glm.hpp>
#include <memory>

namespace ez::gfx {
	class RenderAPI {
	public:
		enum class API {
			NONE = 0,
			OPENGL
		};

		virtual ~RenderAPI() = default;

		static std::shared_ptr<RenderAPI> Create();
		static API GetAPI();

		virtual int QueryInfo(DeviceInfo info) = 0;

		virtual HShader CreateShader(const std::string& vertexCode, const std::string& fragmentCode) = 0;
		virtual void SetShaderUniform(HShader hShader, const std::string& location, const glm::mat4& matrix) = 0;
		virtual void SetShaderUniform(HShader hShader, const std::string& location, float value) = 0;
		virtual void SetShaderUniform(HShader hShader, const std::string& location, int value) = 0;
		virtual void BindShader(HShader hShader) = 0;

		virtual HBuffer CreateBuffer(uint32_t size, BufferType type, void* data) = 0;
		virtual void* GetBufferPointer(HBuffer hBuffer) = 0;

		virtual HTexture CreateTexture(uint32_t width, uint32_t height, TextureFormat format, TextureFilter filter) = 0;
		virtual HTextureArray CreateTextureArray(uint32_t width, uint32_t height, uint32_t layers, TextureFormat format, TextureFilter filter) = 0;
		virtual void UploadTexture(HTexture hTexture, void* data) = 0;
		virtual void UploadTexture(HTextureArray hTexture, uint32_t layer, void* data) = 0;
		virtual void BindTextureSlot(HTexture hTexture, uint32_t slot) = 0;
		virtual void BindTextureSlot(HTextureArray hTexture, uint32_t slot) = 0;

		virtual void Clear() = 0;
		virtual void SetClearColor(float r, float g, float b, float a) = 0;
		virtual void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) = 0;

		virtual void DrawArrays(DrawMode mode, uint32_t count, uint32_t offset) = 0;
	private:
		static API s_API;
	};
}