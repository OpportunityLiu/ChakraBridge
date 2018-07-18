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
        INHERIT_INTERFACE_R_PROPERTY(Type, JsValueType, IJsValue);
        INHERIT_INTERFACE_R_PROPERTY(Context, JsContext^, IJsValue);
        INHERIT_INTERFACE_METHOD(ToInspectable, object^, IJsValue);

    public:
        virtual Platform::String^ ToString() override { return "null"; }
    };

    /// <summary>
    /// Static methods of <see cref="IJsNull"/>.
    /// </summary>
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
