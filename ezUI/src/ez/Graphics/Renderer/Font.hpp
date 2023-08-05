#pragma once
#include "ez/Core/Base.hpp"
#include "ez/Graphics/API/DeviceResources.hpp"
#include "ez/Graphics/API/RenderAPI.hpp"

#include <memory>
#include <string>

namespace ez::gfx {
	class Font {
	public:
		Font(const std::string& file, const Ref<RenderAPI>& texture);
		const Ref<TextureArray>& get_texture();

	private:
		Ref<TextureArray> m_texture;

		int m_ascent;
		int m_descent;
		int m_line_gap;
	};
}