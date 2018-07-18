#pragma once
#include "JsValue.h"
#include "JsEnum.h"
#include "Native\ThrowHelper.h"

namespace Opportunity::ChakraBridge::WinRT
{
    public interface class IJsNull : IJsValue
    {
    };

    ref class JsNullImpl sealed : JsValueImpl, IJsNull
    {
    internal:
        JsNullImpl(JsValueRef ref);
    };

    static public ref class JsNull sealed
    {

    };
}
