#pragma once
#include "JsValue.h"
#include "JsEnum.h"
#include "Native\ThrowHelper.h"

namespace Opportunity::ChakraBridge::WinRT
{
    public interface class IJsUndefined : IJsValue
    {
    };

    ref class JsUndefinedImpl sealed : JsValueImpl, IJsUndefined
    {
    internal:
        JsUndefinedImpl(JsValueRef ref);
    };

    static public ref class JsUndefined sealed
    {
    public:
        property IJsUndefined^ Instance
        {
            IJsUndefined^ get();
        }
    };
}
