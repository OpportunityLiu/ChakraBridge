#pragma once
#include "JsValue.h"
#include "JsEnum.h"

namespace Opportunity::ChakraBridge::WinRT
{
    /// <summary>
    /// A JavaScript string value.
    /// </summary>
    public interface class IJsString : IJsValue
    {
        /// <summary>
        /// Gets the length of a <see cref="IJsString"/> value.
        /// </summary>
        /// <remarks>Requires an active script context.</remarks>
        /// <returns>The length of the string.</returns>
        DECL_R_PROPERTY(uint32, Length);
    };

    ref class JsStringImpl sealed : JsValueImpl, [Default] IJsString
    {
    private:
        ~JsStringImpl();
    internal:
        JsStringImpl(RawValue ref);

        INHERIT_INTERFACE_R_PROPERTY(Type, JsType, IJsValue);
        INHERIT_INTERFACE_R_PROPERTY(Context, JsContext^, IJsValue);
        INHERIT_INTERFACE_METHOD(ToInspectable, object^, IJsValue);

    public:
        virtual string^ ToString() override;
        virtual DECL_R_PROPERTY(uint32, Length);
    };

    /// <summary>
    /// Static methods of <see cref="IJsString"/>.
    /// </summary>
    public ref class JsString sealed
    {
    private:
        JsString() {}
    public:
        /// <summary>
        /// Creates a <see cref="IJsString"/> value from a <see langword="string"/>.
        /// </summary>
        /// <param name="stringValue">The <see langword="string"/> to convert to a <see cref="IJsString"/> value.</param>
        /// <returns>The new <see cref="IJsString"/> value.</returns>
        /// <remarks>Requires an active script context.</remarks>
        static IJsString^ Create(string^ value);
    };
}
