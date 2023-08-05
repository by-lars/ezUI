#include "ez/Graphics/Renderer/Color.hpp"

namespace ez::gfx {
    Color::Color(uint8_t brightness) : r(brightness), g(brightness), b(brightness), a(255) {}
    Color::Color(uint8_t r, uint8_t g, uint8_t b, uint8_t a) : r(g), g(g), b(b), a(a) {}
    Color::Color(uint8_t r, uint8_t g, uint8_t b) : r(g), g(g), b(b), a(255) {}

    Color::Color(float r, float g, float b, float a) {
        this->r = static_cast<uint8_t>(r * 255.0f);
        this->g = static_cast<uint8_t>(g * 255.0f);
        this->b = static_cast<uint8_t>(b * 255.0f);
        this->a = static_cast<uint8_t>(a * 255.0f);
    }

    Color::Color(float r, float g, float b) {
        this->r = static_cast<uint8_t>(r * 255.0f);
        this->g = static_cast<uint8_t>(g * 255.0f);
        this->b = static_cast<uint8_t>(b * 255.0f);
        this->a = 255;
    }
}