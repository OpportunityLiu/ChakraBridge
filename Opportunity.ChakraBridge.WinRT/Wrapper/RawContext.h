#pragma once
#include "RawRef.h"
#include "RawRuntime.h"
#include "RawValue.h"

namespace Opportunity::ChakraBridge::WinRT
{
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

        static void SetException(const RawValue& exception)
        {
            CHAKRA_CALL(JsSetException(exception.Ref));
        }

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

        static RawValue RunScript(const wchar_t*const script, const JsSourceContext sourceContext, const wchar_t *sourceUrl)
        {
            RawValue r;
            CHAKRA_CALL(JsRunScript(script, sourceContext, sourceUrl, &r.Ref));
            return r;
        }

        static RawValue RunScript(
            const wchar_t *const script,
            BYTE *const buffer,
            const JsSourceContext sourceContext,
            const wchar_t *const sourceUrl)
        {
            RawValue r;
            CHAKRA_CALL(JsRunSerializedScript(script, buffer, sourceContext, sourceUrl, &r.Ref));
            return r;
        }

        static RawValue RunScript(
            const ::JsSerializedScriptLoadSourceCallback scriptLoadCallback,
            const ::JsSerializedScriptUnloadCallback scriptUnloadCallback,
            BYTE *const buffer,
            const JsSourceContext sourceContext,
            const wchar_t *const sourceUrl)
        {
            RawValue r;
            CHAKRA_CALL(JsRunSerializedScriptWithCallback(scriptLoadCallback, scriptUnloadCallback, buffer, sourceContext, sourceUrl, &r.Ref));
            return r;
        }

        static unsigned long SerializeScript(const wchar_t *const script, BYTE *const buffer = nullptr)
        {
            unsigned long r;
            CHAKRA_CALL(JsSerializeScript(script, buffer, &r));
            return r;
        }

        static RawValue ParseScript(const wchar_t*const script, const JsSourceContext sourceContext,  const wchar_t *sourceUrl)
        {
            RawValue r;
            CHAKRA_CALL(JsParseScript(script, sourceContext, sourceUrl, &r.Ref));
            return r;
        }

        static RawValue ParseScript(
            const wchar_t *const script,
            BYTE *const buffer,
            const JsSourceContext sourceContext,
            const wchar_t *const sourceUrl)
        {
            RawValue r;
            CHAKRA_CALL(JsParseSerializedScript(script, buffer, sourceContext, sourceUrl, &r.Ref));
            return r;
        }

        static RawValue ParseScript(
            const ::JsSerializedScriptLoadSourceCallback scriptLoadCallback,
            const ::JsSerializedScriptUnloadCallback scriptUnloadCallback,
            BYTE *const buffer,
            const JsSourceContext sourceContext,
            const wchar_t *const sourceUrl)
        {
            RawValue r;
            CHAKRA_CALL(JsParseSerializedScriptWithCallback(scriptLoadCallback, scriptUnloadCallback, buffer, sourceContext, sourceUrl, &r.Ref));
            return r;
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