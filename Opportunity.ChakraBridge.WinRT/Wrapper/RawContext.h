#pragma once
#include "RawRef.h"
#include "RawRuntime.h"

namespace Opportunity::ChakraBridge::WinRT
{
    struct RawValue;

    struct[[nodiscard]] RawContext sealed : public RawRcRef
    {
        static constexpr RawContext Invalid() { return RawContext(); }

        static RawContext Current()
        {
            RawContext ref;
            CHAKRA_CALL(JsGetCurrentContext(&ref.Ref));
            return ref;
        }

        static void Current(const RawContext& ref)
        {
            CHAKRA_CALL(JsSetCurrentContext(ref.Ref));
        }

        static bool HasException()
        {
            bool hasException;
            CHAKRA_CALL(JsHasException(&hasException));
            return hasException;
        }

        static void SetException(const RawValue& exception);

        static void StartDebugging()
        {
            CHAKRA_CALL(JsStartDebugging());
        }

        static unsigned int Idle()
        {
            unsigned int ticks;
            CHAKRA_CALL(JsIdle(&ticks));
            return ticks;
        }

        static void ProjectWinRTNamespace(const wchar_t*const namespaceName)
        {
            CHAKRA_CALL(JsProjectWinRTNamespace(namespaceName));
        }

        constexpr RawContext() :RawRcRef() {}
        constexpr RawContext(JsContextRef ref) : RawRcRef(std::move(ref)) {}

        RawRuntime Runtime() const
        {
            RawRuntime r;
            CHAKRA_CALL(JsGetRuntime(Ref, &r.Ref));
            return r;
        }

    };

    static_assert(sizeof(RawContext) == sizeof(JsContextRef));

};

template<> struct ::std::hash<Opportunity::ChakraBridge::WinRT::RawContext>
{
    constexpr size_t operator()(const Opportunity::ChakraBridge::WinRT::RawContext& v) const
    {
        return reinterpret_cast<size_t>(v.Ref);
    }
};