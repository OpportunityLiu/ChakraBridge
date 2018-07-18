#pragma once
#include "JsValue.h"
#include "JsEnum.h"
#include "Native\ThrowHelper.h"

namespace Opportunity::ChakraBridge::WinRT
{
    public interface class IJsObject : IJsValue
    {
    };

    ref class JsObjectImpl : JsValueImpl, IJsObject
    {
    public:
        virtual ~JsObjectImpl();
    internal:
        JsObjectImpl(JsValueRef ref);
    };

    static public ref class JsObject sealed
    {

    };
}
