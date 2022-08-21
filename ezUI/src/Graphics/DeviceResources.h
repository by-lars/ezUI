#pragma once
#include "Utility/StrongHandle.h"
#include <stdint.h>

namespace ez {
EZ_MAKE_STRONG_HANDLE(HShader);
EZ_MAKE_STRONG_HANDLE(HBuffer);
EZ_MAKE_STRONG_HANDLE(HTexture);

enum class DrawMode {
	TRIANGLES = 0,
	TRIANGLE_STRIP
};

enum class BufferType {
	ARRAY = 0,
	SHADER_STORAGE
};

enum class TextureFormat {
	RGB_8 = 0,
	RGBA_8
};

}