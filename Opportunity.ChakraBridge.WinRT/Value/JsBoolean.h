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
        INHERIT_INTERFACE_R_PROPERTY(Type, JsValueType, IJsValue);
        INHERIT_INTERFACE_R_PROPERTY(Context, JsContext^, IJsValue);
        INHERIT_INTERFACE_METHOD(ToInspectable, object^, IJsValue);

    public:
        virtual string^ ToString() override;
        virtual bool ToBoolean();
    };

    /// <summary>
    /// Static methods of <see cref="IJsBoolean"/>.
    /// </summary>
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

        static IJsBoolean^ Of(bool value);
    };
}
