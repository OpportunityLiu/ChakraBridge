#pragma once
#include "RawRuntime.h"
#include "RawContext.h"
#include "RawPropertyId.h"
#include "RawValue.h"
#include <sstream>

namespace Opportunity::ChakraBridge::WinRT
{
    inline RawPropertyId::RawPropertyId(const RawValue& symbol)
    {
        CHAKRA_CALL(JsGetPropertyIdFromSymbol(symbol.Ref, &Ref));
    }

    inline RawValue RawPropertyId::Symbol() const
    {
        RawValue r;
        CHAKRA_CALL(JsGetSymbolFromPropertyId(Ref, &r.Ref));
        return r;
    }

    inline RawContext RawValue::Context() const
    {
        RawContext ref;
        CHAKRA_CALL(JsGetContextOfObject(Ref, &ref.Ref));
        return ref;
    }

    template<typename T, RawNativeFunction<T> function>
    static JsValueRef CALLBACK RawValue::RawNativeFunctionCall(_In_ JsValueRef callee, _In_ bool isConstructCall, _In_ JsValueRef *arguments, _In_ unsigned short argumentCount, _In_opt_ void *callbackState)
    {
        try
        {
            const auto caller = (argumentCount == 0 || arguments[0] == JS_INVALID_REFERENCE)
                ? GlobalObject()
                : RawValue(arguments[0]);
            const auto tCallee = RawValue(callee);
            const auto data = DataFromJsrt<T>(callbackState);
            if (argumentCount <= 1)
                return function(tCallee, caller, isConstructCall, nullptr, 0, data).Ref;
            return function(tCallee, caller, isConstructCall, reinterpret_cast<const RawValue*>(arguments + 1), argumentCount - 1, data).Ref;
        }
        catch (const std::exception& ex)
        {
            std::wstringstream str;
            str << L"Exception in RawValue::RawNativeFunctionCall." << std::endl;
            str << ex.what();
            const auto error = RawValue::CreateError(RawValue(str.str().c_str()));
            RawContext::SetException(error);
            return JS_INVALID_REFERENCE;
        }
        catch (...)
        {
            const auto error = RawValue::CreateError(RawValue(L"Unknown exception in RawValue::RawNativeFunctionCall."));
            RawContext::SetException(error);
            return JS_INVALID_REFERENCE;
        }
    }
}