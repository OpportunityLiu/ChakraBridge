#pragma once
#include "alias.h"
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
        INHERIT_INTERFACE_R_PROPERTY(Type, JsValueType, IJsValue);

    public:
        virtual string^ ToString() override;
        virtual float64 ToDouble();
        virtual int32 ToInt32();
    };

    /// <summary>
    /// Static methods of <see cref="IJsNumber"/>.
    /// </summary>
    public ref class JsNumber sealed
    {
    private:
        JsNumber() {}
    public:
        [Overload("OfInt32")]
        static IJsNumber^ Of(int32 value);
        [DefaultOverload]
        [Overload("OfDouble")]
        static IJsNumber^ Of(float64 value);
    };
}
