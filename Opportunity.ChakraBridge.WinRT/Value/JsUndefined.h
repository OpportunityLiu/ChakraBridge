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
        virtual property JsValueType TypeInterface { JsValueType get() = IJsValue::Type::get{ return Type; } };

    public:
        virtual Platform::String^ ToString() override { return "undefined"; }
    };

    /// <summary>
    /// Static methods of <see href="IJsUndefined"/>.
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
