#pragma once
#include "ez/Core/StrongHandle.hpp"

#include <cstdint>
#include <string>
#include <glm/glm.hpp>

namespace ez::gfx {
	enum class DrawMode {
		TRIANGLES = 0,
		TRIANGLE_STRIP
	};

	enum class Format {
		RGB = 0,
		RGBA
	};

	enum class Filter {
		NEAREST = 0,
		LINEAR
	};

	EZ_MAKE_STRONG_HANDLE(Layer);
	class TextureArray {
	public:
		virtual ~TextureArray() = default;
		virtual void bind() = 0;
		virtual void bind_to_slot(uint32_t slot) = 0;
		virtual Layer push_back(void* data) = 0;
		virtual void erase(Layer layer) = 0;
		virtual uint32_t get_max_layers() = 0;
	};

	class Shader {
	public:
		enum class Type {
			VERTEX = 0,
			FRAGMENT
		};

		virtual ~Shader() = default;
		virtual void bind() = 0;
		virtual void set(const std::string& location, const glm::mat4& matrix) = 0;
		virtual void set(const std::string& location, float value) = 0;
		virtual void set(const std::string& location, int value) = 0;
	};

	class StreamStorage {
	public:
		virtual ~StreamStorage() = default;
		virtual void push_back(void* data) = 0;

		virtual uint32_t count() = 0;
		virtual uint32_t offset() = 0;

		virtual void begin_frame() = 0;
		virtual void end_frame() = 0;
	};
}