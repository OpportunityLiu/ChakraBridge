#pragma once
#include "JsValue.h"
#include "JsEnum.h"
#include "Native\ThrowHelper.h"

namespace Opportunity::ChakraBridge::WinRT
{
    public interface class IJsString : IJsValue
    {
    };

    ref class JsStringImpl sealed : JsValueImpl, IJsString
    {
    internal:
        JsStringImpl(JsValueRef ref);
        virtual property JsValueType TypeInterface { JsValueType get() = IJsValue::Type::get{ return Type; } };

    public:
        virtual ~JsStringImpl();
        virtual Platform::String^ ToString() override;
    };

    public ref class JsString sealed
    {
    private:
        JsString() {}
    public:
        static IJsString^ OfString(Platform::String^ value);
    };
}
