#pragma once
#include "JsObject.h"
#include "JsEnum.h"
#include "Native\ThrowHelper.h"

namespace Opportunity::ChakraBridge::WinRT
{
    public interface class IJsError
    {
    };

    ref class JsErrorImpl sealed : JsObjectImpl, IJsError
    {
    private:
    internal:
        JsErrorImpl(JsValueRef ref)
            :JsObjectImpl(ref) {}
    };
}
