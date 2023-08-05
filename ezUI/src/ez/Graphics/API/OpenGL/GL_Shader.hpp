#pragma once
#include "ez/Graphics/API/DeviceResources.hpp"

namespace ez::gfx {
	class GL_Shader : public Shader {
	public:
		GL_Shader(const std::initializer_list<std::pair<Shader::Type, const std::string&>>& shaders);
		~GL_Shader() override;

		void bind() override;

		void set(const std::string& location, const glm::mat4& matrix) override;
		void set(const std::string& location, float value) override;
		void set(const std::string& location, int value) override;
	private:
		uint32_t m_program;
	};
}