#pragma once
#include "Core/StrongHandle.h"
#include <stdint.h>

namespace ez::gfx {
	EZ_MAKE_STRONG_HANDLE(HShader);
	EZ_MAKE_STRONG_HANDLE(HBuffer);
	EZ_MAKE_STRONG_HANDLE(HTexture);
	EZ_MAKE_STRONG_HANDLE(HTextureArray);

	enum class DeviceInfo {
		MAX_TEXTURE_ARRAY_LAYERS = 0
	};

	enum class DrawMode {
		TRIANGLES = 0,
		TRIANGLE_STRIP
	};

	enum class BufferType {
		ARRAY = 0,
		SHADER_STORAGE
	};

	enum class TextureFormat {
		RGB = 0,
		RGBA
	};

	enum class TextureFilter {
		LINEAR = 0,
		NEAREST
	};
}