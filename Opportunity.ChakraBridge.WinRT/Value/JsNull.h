#pragma once
#include "JsValue.h"
#include "JsEnum.h"

namespace Opportunity::ChakraBridge::WinRT
{
    /// <summary>
    /// A JavaScript <see langword="null"/> value.
    /// </summary>
    public interface class IJsNull : IJsValue
    {
    };

    ref class JsNullImpl sealed : JsValueImpl, [Default] IJsNull
    {
    internal:
        JsNullImpl(RawValue ref);
        INHERIT_INTERFACE_R_PROPERTY(Type, JsType, IJsValue);
        INHERIT_INTERFACE_R_PROPERTY(Context, JsContext^, IJsValue);
        INHERIT_INTERFACE_METHOD(ToInspectable, object^, IJsValue);

    public:
        virtual string^ ToString() override;
    };

    /// <summary>
    /// Static methods of <see cref="IJsNull"/>.
    /// </summary>
    public ref class JsNull sealed
    {
    private:
        JsNull() {}
    public:
        /// <summary>
        /// Gets the value of <see langword="null"/> in the current script context.
        /// </summary>
        /// <remarks>Requires an active script context.</remarks>
        static DECL_R_PROPERTY(IJsNull^, Instance);
    };
}
