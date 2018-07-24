#pragma once
#include "RawRef.h"

namespace Opportunity::ChakraBridge::WinRT
{
    struct RawRuntime sealed :public RawRef<JsRuntimeHandle>
    {
        static constexpr RawRuntime Invalid() { return RawRuntime(); }

        constexpr RawRuntime() :RawRef(JS_INVALID_RUNTIME_HANDLE) {}
        constexpr RawRuntime(JsRuntimeHandle handle) : RawRef(std::move(handle)) {}
        constexpr bool IsValid() const { return Ref != JS_INVALID_RUNTIME_HANDLE; };

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