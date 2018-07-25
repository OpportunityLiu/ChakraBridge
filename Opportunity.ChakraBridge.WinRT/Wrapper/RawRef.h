#pragma once
#include "PreDeclear.h"

namespace Opportunity::ChakraBridge::WinRT
{
    template<typename TRef>
    struct RawRef abstract
    {
        using My_type = RawRef<TRef>;

        TRef Ref;
        constexpr RawRef() = default;
        explicit constexpr RawRef(TRef ref) : Ref(std::move(ref)) {};

        constexpr bool IsValid() const { return Ref != nullptr; };

        template<typename T>
        friend constexpr std::enable_if_t<std::is_base_of_v<My_type, T>, bool> operator ==(const T& a, const T& b) { return a.Ref == b.Ref; };
        template<typename T>
        friend constexpr std::enable_if_t<std::is_base_of_v<My_type, T>, bool> operator !=(const T& a, const T& b) { return a.Ref != b.Ref; };
    };

    template<typename TRef>
    struct RawRcRef abstract :public RawRef<TRef>
    {
        using ref_count_t = unsigned int;

        constexpr RawRcRef() :RawRef() {};
        constexpr RawRcRef(TRef ref) : RawRef(std::move(ref)) {};

        ref_count_t AddRef() const { ref_count_t count; CHAKRA_CALL(JsAddRef(Ref, &count)); return count; }
        ref_count_t Release() const { ref_count_t count; CHAKRA_CALL(JsRelease(Ref, &count)); return count; }
    };

    static_assert(sizeof(RawRcRef<JsRef>) == sizeof(JsRef));
};