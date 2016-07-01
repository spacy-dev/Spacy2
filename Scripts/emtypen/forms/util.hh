#pragma once

#include <cassert>
#include <cstddef>
#include <memory>

namespace TypeErasure
{
    template <class BufferHandle, class Buffer>
    void* get_buffer_ptr(Buffer& buffer)
    {
        void* buffer_ptr = &buffer;
        auto buffer_size = sizeof(buffer);
        return std::align( alignof(BufferHandle),
                           sizeof(BufferHandle),
                           buffer_ptr,
                           buffer_size);

    }

    template <typename T>
    inline unsigned char* char_ptr (T* ptr)
    {
        return static_cast<unsigned char*>(static_cast<void*>(ptr));
    }

    template <class HandleBase>
    inline HandleBase* handle_ptr (unsigned char* ptr)
    {
        return static_cast<HandleBase*>(static_cast<void*>(ptr));
    }

    template <class HandleBase, class Buffer>
    inline std::ptrdiff_t handle_offset (HandleBase* handle, Buffer& buffer)
    {
        assert(handle);
        return char_ptr(handle) - char_ptr(&buffer);
    }
}
