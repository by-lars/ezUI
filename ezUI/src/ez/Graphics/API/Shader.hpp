#pragma once

#include <string>
#include <glm/glm.hpp>

namespace ez::gfx {
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
}