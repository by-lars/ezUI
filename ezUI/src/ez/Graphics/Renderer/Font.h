#pragma once
#include <memory>
#include <string>

namespace ez::gfx {
	class Font {
	public:
		static std::shared_ptr<Font> FromFile(const std::string& file);



	private:
		Font(const std::string& file);
		int m_Ascent;
		int m_Descent;
		int m_LineGap;
	};
}