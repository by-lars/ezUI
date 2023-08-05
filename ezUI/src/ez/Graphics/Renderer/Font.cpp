#include "ez/Core/Base.hpp"
#include "ez/Graphics/Renderer/Font.hpp"

#include <stb/stb_truetype.h>

#include <fstream>
#include <vector>

namespace ez::gfx {
	Font::Font(const std::string& file, const Ref<RenderAPI>& device) {
        m_texture = device->create_texture_array(32, 32, Format::RGBA, Filter::LINEAR);

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
		unsigned char* data = stbtt_GetCodepointSDF(&info, scale, 'A', 3, 128, 64.0, &width, &height, &x, &y);
        m_texture->push_back(data);
	}

	const Ref<TextureArray>& Font::get_texture() {
		return m_texture;
	}

}