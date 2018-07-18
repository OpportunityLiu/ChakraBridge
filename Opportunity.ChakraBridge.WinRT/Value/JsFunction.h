#pragma once
#include "JsObject.h"
#include "JsEnum.h"
#include "Native\ThrowHelper.h"

namespace Opportunity::ChakraBridge::WinRT
{
    public interface class IJsFunction
    {
    };

    ref class JsFunctionImpl sealed : JsObjectImpl, IJsFunction
    {
    private:
    internal:
        JsFunctionImpl(JsValueRef ref)
            :JsObjectImpl(ref) {}
    };
}
