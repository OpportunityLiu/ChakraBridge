#pragma once
#include <utility> 
#include <type_traits>
#include "alias.h"
#include "Native\Helper.h"

namespace Opportunity::ChakraBridge::WinRT
{
    template<typename TRef>
    struct RawRef abstract
    {
        using My_type = RawRef<TRef>;

        TRef Ref;
        constexpr RawRef(TRef ref) : Ref(std::move(ref)) {};

        template<typename T>
        constexpr std::enable_if_t<std::is_base_of_v<My_type, T>, bool> operator ==(const T& other) const { return Ref == other.Ref; };
        template<typename T>
        constexpr std::enable_if_t<std::is_base_of_v<My_type, T>, bool> operator !=(const T& other) const { return Ref != other.Ref; };
    };

    struct RawRcRef abstract :public RawRef<JsRef>
    {
        using ref_count_t = unsigned int;

        constexpr RawRcRef() : RawRef(JS_INVALID_REFERENCE) {};
        constexpr RawRcRef(JsRef ref) : RawRef(std::move(ref)) {};
        constexpr bool IsValid() const { return Ref != JS_INVALID_REFERENCE; };

        ref_count_t AddRef() const { ref_count_t count; CHAKRA_CALL(JsAddRef(Ref, &count)); return count; }
        ref_count_t Release() const { ref_count_t count; CHAKRA_CALL(JsRelease(Ref, &count)); return count; }
    };

    static_assert(sizeof(RawRcRef) == sizeof(JsRef));
};