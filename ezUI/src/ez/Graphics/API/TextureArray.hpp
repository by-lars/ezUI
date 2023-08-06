#pragma once

#include "ez/Core/StrongHandle.hpp"

namespace ez::gfx {
    EZ_MAKE_STRONG_HANDLE(Layer);

    class TextureArray {
    public:
        virtual ~TextureArray() = default;
        virtual void bind() = 0;
        virtual void bind_to_slot(uint32_t slot) = 0;
        virtual Layer push_back(void* data) = 0;
        virtual void erase(Layer layer) = 0;
        virtual uint32_t get_max_layers() = 0;
    };
}
