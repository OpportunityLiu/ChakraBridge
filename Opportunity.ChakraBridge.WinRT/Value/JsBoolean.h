#pragma once
#include "JsValue.h"
#include "JsEnum.h"
#include "Native\ThrowHelper.h"

namespace Opportunity::ChakraBridge::WinRT
{
    public interface class IJsBoolean : IJsValue
    {
        bool ToBoolean();
    };

    ref class JsBooleanImpl sealed : JsValueImpl, IJsBoolean
    {
    internal:
        JsBooleanImpl(JsValueRef ref);
        virtual property JsValueType TypeInterface { JsValueType get() = IJsValue::Type::get{ return Type; } };

    public:
        virtual Platform::String^ ToString() override;
        virtual bool ToBoolean();
    };

    public ref class JsBoolean sealed
    {
    private:
        JsBoolean() {}
    public:

        static property IJsBoolean^ True
        {
            IJsBoolean^ get();
        }
        static property IJsBoolean^ False
        {
            IJsBoolean^ get();
        }

        static IJsBoolean^ OfBoolean(bool value);
    };
}
