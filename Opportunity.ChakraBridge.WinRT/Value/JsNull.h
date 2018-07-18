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
        virtual property JsValueType TypeInterface { JsValueType get() = IJsValue::Type::get{ return Type; } };
    public:
        virtual Platform::String^ ToString() override { return "null"; }
    };

    public ref class JsNull sealed
    {
    private:
        JsNull() {}
    public:
        static property IJsNull^ Instance
        {
            IJsNull^ get();
        }
    };
}
