#pragma once
#include "JsValue.h"
#include "JsEnum.h"
#include "Native\ThrowHelper.h"

namespace Opportunity::ChakraBridge::WinRT
{
    /// <summary>
    /// A JavaScript undefined value.
    /// </summary>
    public interface class IJsUndefined : IJsValue
    {
    };

    ref class JsUndefinedImpl sealed : JsValueImpl, IJsUndefined
    {
    internal:
        JsUndefinedImpl(JsValueRef ref);
        INHERIT_INTERFACE_R_PROPERTY(Type, JsValueType, IJsValue);
        INHERIT_INTERFACE_R_PROPERTY(Context, JsContext^, IJsValue);
        INHERIT_INTERFACE_METHOD(ToInspectable, object^, IJsValue);

    public:
        virtual string^ ToString() override { return "undefined"; }
    };

    /// <summary>
    /// Static methods of <see cref="IJsUndefined"/>.
    /// </summary>
    public ref class JsUndefined sealed
    {
    private:
        JsUndefined() {}
    public:
        /// <summary>
        /// Gets the value of <see href="IJsUndefined"/> in the current script context.
        /// </summary>
        /// <remarks>Requires an active script context.</remarks>
        static property IJsUndefined^ Instance
        {
            IJsUndefined^ get();
        }
    };
}
