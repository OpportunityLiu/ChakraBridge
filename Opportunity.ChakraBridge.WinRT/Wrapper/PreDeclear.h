#pragma once
#include "alias.h"
#include <type_traits>
#include "Native\Helper.h"

namespace Opportunity::ChakraBridge::WinRT
{
    using void_ptr = void*;

    template<typename T>
    T DataFromJsrt(const void_ptr& data)
    {
        static_assert(sizeof(T) == sizeof(void_ptr));
        static_assert(std::is_trivially_copyable_v<T>);
        return *reinterpret_cast<const T*>(&data);
    }

    template<typename T>
    void_ptr DataToJsrt(const T& data)
    {
        static_assert(sizeof(T) == sizeof(void_ptr));
        static_assert(std::is_trivially_copyable_v<T>);
        return *reinterpret_cast<const void_ptr*>(&data);
    }

    template<typename T>
    void_ptr* DataToJsrtPtr(T& data)
    {
        static_assert(sizeof(T) == sizeof(void_ptr));
        static_assert(std::is_trivially_copyable_v<T>);
        return reinterpret_cast<void_ptr*>(&data);
    }

    struct RawRuntime;
    struct RawContext;
    struct RawValue;
    struct RawPropertyId;
}
