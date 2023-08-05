#pragma once

#include <cstdint>

namespace ez::gfx {
    struct Color {
        Color(uint8_t r, uint8_t g, uint8_t b, uint8_t a);
        Color(uint8_t r, uint8_t g, uint8_t b);
        explicit Color(uint8_t brightness);

        Color(float r, float g, float b, float a);
        Color(float r, float g, float b);

        uint8_t r;
        uint8_t g;
        uint8_t b;
        uint8_t a;
    };
}
