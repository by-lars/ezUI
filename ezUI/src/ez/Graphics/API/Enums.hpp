#pragma once

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
}