#pragma once
#include "PreDeclear.h"
#include "RawRef.h"
#include "JsEnum.h"

namespace Opportunity::ChakraBridge::WinRT
{
    using RawThreadServiceCallback = bool (CALLBACK *)(_In_ JsBackgroundWorkItemCallback callback, _In_opt_ void *callbackState);

    template<typename T>
    using RawBeforeCollectCallback = void(const T& callbackState);

    template<typename T>
    using RawMemoryAllocationCallback = bool(const T& callbackState, const JsMEType allocationEvent, const size_t allocationSize);

    struct[[nodiscard]] RawRuntime sealed :public RawRef<JsRuntimeHandle>
    {
        explicit RawRuntime(const JsRA attributes, const RawThreadServiceCallback threadService)
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

        void Dispose() const
        {
            CHAKRA_CALL(JsDisposeRuntime(Ref));
        }

    private: template<typename T, RawBeforeCollectCallback<T> callback>
        static void CALLBACK JsBeforeCollectCallback(_In_opt_ void *callbackState)
        {
            callback(DataFromJsrt<T>(callbackState));
        }

    public: template<typename T, RawBeforeCollectCallback<T> callback>
        void BeforeCollectCallback(const T& callbackState) const
        {
            CHAKRA_CALL(JsSetRuntimeBeforeCollectCallback(Ref, DataToJsrt(callbackState), JsBeforeCollectCallback<T, callback>));
        }

    private: template<typename T, RawMemoryAllocationCallback<T> callback>
        static bool CALLBACK JsMemoryAllocationCallback(_In_opt_ void *callbackState, _In_::JsMemoryEventType allocationEvent, _In_ size_t allocationSize)
        {
            return callback(DataFromJsrt<T>(callbackState), static_cast<JsMEType>(allocationEvent), allocationSize);
        }

    public: template<typename T, RawMemoryAllocationCallback<T> callback>
        void MemoryAllocationCallback(const T& callbackState) const
        {
            CHAKRA_CALL(JsSetRuntimeMemoryAllocationCallback(Ref, DataToJsrt(callbackState), JsMemoryAllocationCallback<T, callback>));
        }

        explicit constexpr RawRuntime(JsRuntimeHandle handle) : RawRef(std::move(handle)) {}
        constexpr RawRuntime(std::nullptr_t) : RawRef(JS_INVALID_RUNTIME_HANDLE) {}
        constexpr RawRuntime() : RawRef(JS_INVALID_REFERENCE) {}
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