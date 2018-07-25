#pragma once
#include "PreDeclear.h"
#include "RawRef.h"

namespace Opportunity::ChakraBridge::WinRT
{
    using RawThreadServiceCallback = bool (CALLBACK *)(_In_ JsBackgroundWorkItemCallback callback, _In_opt_ void *callbackState);

    struct[[nodiscard]] RawRuntime sealed :public RawRef<JsRuntimeHandle>
    {
        using JsRtAttr = ::Opportunity::ChakraBridge::WinRT::JsRuntimeAttributes;

        explicit RawRuntime(const JsRtAttr attributes, const RawThreadServiceCallback threadService)
        {
            CHAKRA_CALL(JsCreateRuntime(static_cast<::JsRuntimeAttributes>(attributes), threadService, &Ref));
        }

        void CollectGarbage() const
        {
            CHAKRA_CALL(JsCollectGarbage(Ref));
        }

        size_t MemoryUsage() const
        {
            size_t memoryUsage;
            CHAKRA_CALL(JsGetRuntimeMemoryUsage(Ref, &memoryUsage));
            return memoryUsage;
        }

        size_t MemoryLimit() const
        {
            size_t memoryLimit;
            CHAKRA_CALL(JsGetRuntimeMemoryLimit(Ref, &memoryLimit));
            return memoryLimit;
        }

        void MemoryLimit(size_t value) const
        {
            CHAKRA_CALL(JsSetRuntimeMemoryLimit(Ref, value));
        }

        bool Enabled() const
        {
            bool isDisabled;
            CHAKRA_CALL(JsIsRuntimeExecutionDisabled(Ref, &isDisabled));
            return !isDisabled;
        }

        void Enabled(bool value) const
        {
            if (value)
                CHAKRA_CALL(JsEnableRuntimeExecution(Ref));
            else
                CHAKRA_CALL(JsDisableRuntimeExecution(Ref));
        }

        explicit constexpr RawRuntime(JsRuntimeHandle handle) : RawRef(std::move(handle)) {}
        constexpr RawRuntime(std::nullptr_t) : RawRef(JS_INVALID_RUNTIME_HANDLE) {}
        constexpr RawRuntime() :RawRef(JS_INVALID_REFERENCE) {}
    };

    static_assert(sizeof(RawRuntime) == sizeof(JsRuntimeHandle));
};

template<> struct ::std::hash<Opportunity::ChakraBridge::WinRT::RawRuntime>
{
    constexpr size_t operator()(const Opportunity::ChakraBridge::WinRT::RawRuntime& v) const
    {
        return reinterpret_cast<size_t>(v.Ref);
    }
};