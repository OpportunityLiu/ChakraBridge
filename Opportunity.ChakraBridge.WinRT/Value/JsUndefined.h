#pragma once
#include "JsValue.h"
#include "JsEnum.h"

namespace Opportunity::ChakraBridge::WinRT
{
    /// <summary>
    /// A JavaScript undefined value.
    /// </summary>
    public interface class IJsUndefined : IJsValue
    {
    };

    ref class JsUndefinedImpl sealed : JsValueImpl, [Default] IJsUndefined
    {
    internal:
        JsUndefinedImpl(RawValue ref);
        INHERIT_INTERFACE_R_PROPERTY(Type, JsType, IJsValue);
        INHERIT_INTERFACE_R_PROPERTY(Context, JsContext^, IJsValue);
        INHERIT_INTERFACE_METHOD(ToInspectable, object^, IJsValue);

    public:
        virtual string^ ToString() override;
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
        /// Gets the value of <see langword="undefined"/> in the current script context.
        /// </summary>
        /// <remarks>Requires an active script context.</remarks>
        static DECL_R_PROPERTY(IJsUndefined^, Instance);
    };
}
