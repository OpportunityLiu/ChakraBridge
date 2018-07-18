#pragma once
#include "JsValue.h"
#include "JsEnum.h"
#include "Native\ThrowHelper.h"

namespace Opportunity::ChakraBridge::WinRT
{
    public interface class IJsNumber : IJsValue
    {
        float64 ToDouble();
        int32 ToInt32();
    };

    ref class JsNumberImpl sealed : JsValueImpl, IJsNumber
    {
    internal:
        JsNumberImpl(JsValueRef ref);
        virtual property JsValueType TypeInterface { JsValueType get() = IJsValue::Type::get{ return Type; } };
    public:
        virtual Platform::String^ ToString() override;
        virtual float64 ToDouble();
        virtual int32 ToInt32();
    };

    public ref class JsNumber sealed
    {
    private:
        JsNumber() {}
    public:
        static IJsNumber^ OfInt32(int32 value);
        static IJsNumber^ OfDouble(float64 value);
    };
}
