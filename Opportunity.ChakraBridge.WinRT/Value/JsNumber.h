#pragma once
#include "alias.h"
#include "JsValue.h"
#include "JsEnum.h"

namespace Opportunity::ChakraBridge::WinRT
{
    /// <summary>
    /// A JavaScript number value.
    /// </summary>
    public interface class IJsNumber : IJsValue
    {
        /// <summary>
        /// Retrieves the <see langword="float64"/> value of a number value.
        /// </summary>
        /// <returns>The <see langword="float64"/> value.</returns>
        float64 ToDouble();

        /// <summary>
        /// Retrieves the <see langword="int32"/> value of a number value.
        /// </summary>
        /// <returns>The <see langword="int32"/> value.</returns>
        int32 ToInt32();
    };

    ref class JsNumberImpl sealed : JsValueImpl, [Default] IJsNumber
    {
    internal:
        JsNumberImpl(RawValue ref);
        INHERIT_INTERFACE_R_PROPERTY(Type, JsType, IJsValue);
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
        /// <summary>
        /// Creates a <see cref="IJsNumber"/> value from a <see langword="int32"/> value.
        /// </summary>
        /// <param name="int32Value">The value to be converted.</param>
        /// <returns>The new <see cref="IJsNumber"/> value.</returns>
        /// <remarks>Requires an active script context.</remarks>
        [Overload("CreateFromInt32")]
        static IJsNumber^ Create(int32 value);
        /// <summary>
        /// Creates a <see cref="IJsNumber"/> value from a <see langword="float64"/> value.
        /// </summary>
        /// <param name="doubleValue">The value to be converted.</param>
        /// <returns>The new <see cref="IJsNumber"/> value.</returns>
        /// <remarks>Requires an active script context.</remarks>
        [DefaultOverload]
        [Overload("CreateFromDouble")]
        static IJsNumber^ Create(float64 value);
    };
}
