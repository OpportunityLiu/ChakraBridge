#pragma once
#include "JsValue.h"
#include "JsEnum.h"

namespace Opportunity::ChakraBridge::WinRT
{
    /// <summary>
    /// A JavaScript boolean value.
    /// </summary>
    public interface class IJsBoolean : IJsValue
    {
        /// <summary>
        /// Retrieves the <see langword="bool"/> value of a <see cref="IJsBoolean"/> value. 
        /// </summary>
        /// <remarks>Requires an active script context.</remarks>
        bool ToBoolean();
    };

    ref class JsBooleanImpl sealed : JsValueImpl, [Default] IJsBoolean
    {
    internal:
        JsBooleanImpl(RawValue ref);
        INHERIT_INTERFACE_R_PROPERTY(Type, JsType, IJsValue);
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

        /// <summary>
        /// Gets the value of <see langword="true"/> in the current script context.
        /// </summary>
        /// <remarks>Requires an active script context.</remarks>
        static DECL_R_PROPERTY(IJsBoolean^, True);

        /// <summary>
        /// Gets the value of <see langword="false"/> in the current script context.
        /// </summary>
        /// <remarks>Requires an active script context.</remarks>
        static DECL_R_PROPERTY(IJsBoolean^, False);

        /// <summary>
        /// Creates a <see cref="IJsBoolean"/> value from a <see langword="bool"/> value.
        /// </summary>
        /// <param name="booleanValue">The value to be converted.</param>
        /// <returns>The converted value.</returns>
        /// <remarks>Requires an active script context.</remarks>
        static IJsBoolean^ Create(bool value);
    };
}
