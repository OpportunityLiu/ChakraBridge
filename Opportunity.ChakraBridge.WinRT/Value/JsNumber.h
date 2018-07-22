#pragma once
#include "alias.h"
#include "JsValue.h"
#include "JsEnum.h"

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
        INHERIT_INTERFACE_R_PROPERTY(Context, JsContext^, IJsValue);
        INHERIT_INTERFACE_METHOD(ToInspectable, object^, IJsValue);

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
        [Overload("CreateFromInt32")]
        static IJsNumber^ Create(int32 value);
        [DefaultOverload]
        [Overload("CreateFromDouble")]
        static IJsNumber^ Create(float64 value);
    };
}
