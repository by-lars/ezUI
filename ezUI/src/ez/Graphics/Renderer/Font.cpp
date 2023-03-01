#include "ez/Core/Base.h"
#include "ez/Graphics/Renderer/Font.h"

#include <stb/stb_truetype.h>

#include <fstream>
#include <vector>

namespace ez::gfx {
	std::shared_ptr<Font> Font::FromFile(const std::string& file) {
		return std::make_shared<Font>(file);
	}

	Font::Font(const std::string& file) {
		std::ifstream infile(file, std::ios::binary);

		if (infile.is_open() == false) {
			EZ_CORE_ERROR("Could not open font file: ", file)
			return;
		}

		std::vector<unsigned char> buffer(std::istreambuf_iterator<char>(infile), {});

		stbtt_fontinfo info;
		if (!stbtt_InitFont(&info, buffer.data(), 0)) {
			EZ_CORE_ERROR("Could not init font");
			return;
		}

		int x, y, width, height;
		float scale = stbtt_ScaleForPixelHeight(&info, 64);
		unsigned char* data = stbtt_GetCodepointSDF(&info, scale, 32, 3, 128, 64.0, &width, &height, &x, &y);
		//m_GlyphBuffer.PushBack(width, height, GL_RGBA, data);
	}
}