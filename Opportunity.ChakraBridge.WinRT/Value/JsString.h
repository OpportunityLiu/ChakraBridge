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
        property int32 Length { int32 get(); }
    };

    ref class JsStringImpl sealed : JsValueImpl, IJsString
    {
    internal:
        JsStringImpl(JsValueRef ref);
        INHERIT_INTERFACE_R_PROPERTY(Type, JsValueType, IJsValue);
        INHERIT_INTERFACE_R_PROPERTY(Context, JsContext^, IJsValue);
        INHERIT_INTERFACE_METHOD(ToInspectable, object^, IJsValue);

    public:
        virtual ~JsStringImpl();
        virtual string^ ToString() override;
        virtual property int32 Length {int32 get(); }
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
        static IJsString^ Of(string^ value);
    };
}
