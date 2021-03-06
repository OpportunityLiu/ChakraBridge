#pragma once
#include "PreDeclare.h"
#include "JsEnum.h"
#include <type_traits>
#include "Wrapper\Declear.h"

namespace Opportunity::ChakraBridge::WinRT
{
    ref class JsContext;

    /// <summary>
    /// A JavaScript value.
    /// </summary>
    /// <remarks>
    /// A JavaScript value is one of the following types of values: Undefined, Null, Boolean, 
    /// String, Number, or Object.
    /// </remarks>
    public interface class IJsValue : Windows::Foundation::IStringable
    {
        /// <summary>
        /// Gets the JavaScript type of the value.
        /// </summary>
        /// <remarks>Requires an active script context.</remarks>
        /// <returns>The type of the value.</returns>
        DECL_R_PROPERTY(JsType, Type);

        /// <summary>
        /// Unwraps a JavaScript object to an <c>IInspectable</c> pointer 
        /// </summary>
        /// <returns>An <c>IInspectable</c> value of the object. </returns>
        /// <remarks><para>Hosts are responsible for enforcing COM threading rules. </para>
        /// <para>Requires an active script context. </para></remarks>
        object^ ToInspectable();

        /// <summary>
        /// Gets the script context that the object belongs to. 
        /// </summary>
        DECL_R_PROPERTY(JsContext^, Context);
    };

    ref class JsValueImpl abstract : [Default] IJsValue
    {
    private:
    internal:
        RawValue Reference;
        JsValueImpl(RawValue ref);
    public:
        virtual Platform::String^ ToString() override = 0;
        virtual DECL_R_PROPERTY(JsType, Type);
        virtual object^ ToInspectable();
        virtual DECL_R_PROPERTY(JsContext^, Context);
    };

    /// <summary>
    /// Static methods of <see cref="IJsValue"/>.
    /// </summary>
    public ref class JsValue sealed
    {
    private:
        JsValue() {}
    internal:
        static JsValueImpl^ CreateTyped(RawValue ref);
    public:
        /// <summary>
        /// Gets the global object in the current script context.
        /// </summary>
        /// <remarks>Requires an active script context.</remarks>
        static DECL_R_PROPERTY(IJsObject^, GlobalObject);

        /// <summary>
        /// Compare two JavaScript values for reference equality.
        /// </summary>
        /// <remarks>
        /// <para>
        /// This function compares inner reference of two values.
        /// </para>
        /// <para>
        /// Requires an active script context.
        /// </para>
        /// </remarks>
        /// <returns>Whether the values are equal.</returns>
        static bool ReferenceEquals(IJsValue^ v1, IJsValue^ v2);

        /// <summary>
        /// Compare two JavaScript values for equality.
        /// </summary>
        /// <remarks>
        /// <para>
        /// This function is equivalent to the "==" operator in JavaScript.
        /// </para>
        /// <para>
        /// Requires an active script context.
        /// </para>
        /// </remarks>
        /// <returns>Whether the values are equal.</returns>
        static bool Equals(IJsValue^ v1, IJsValue^ v2);

        /// <summary>
        /// Compare two JavaScript values for strict equality.
        /// </summary>
        /// <remarks>
        /// <para>
        /// This function is equivalent to the "===" operator in JavaScript.
        /// </para>
        /// <para>
        /// Requires an active script context.
        /// </para>
        /// </remarks>
        /// <returns>Whether the values are strictly equal.</returns>
        static bool StrictEquals(IJsValue^ v1, IJsValue^ v2);

        /// <summary>
        /// Creates a JavaScript value that is a projection of the passed in <c>IInspectable</c> pointer. 
        /// </summary>
        /// <param name="inspectable">An <c>IInspectable</c> to be projected. </param>
        /// <returns>A JavaScript value that is a projection of <paramref name="inspectable"/>. </returns>
        /// <remarks>
        /// <para>The projected value can be used by script to call a WinRT object. Hosts are responsible for enforcing COM threading rules. </para>
        /// <para>Requires an active script context. </para>
        /// </remarks>
        static IJsValue^ Create(object^ inspectable);

        /// <summary>
        /// Converts the value to <see cref="IJsBoolean"/> using regular JavaScript semantics.
        /// </summary>
        /// <param name="value">Value to convert.</param>
        /// <remarks>Requires an active script context.</remarks>
        /// <returns>The converted value.</returns>
        static IJsBoolean^ ToJsBoolean(IJsValue^ value);

        /// <summary>
        /// Converts the value to <see cref="IJsNumber"/> using regular JavaScript semantics.
        /// </summary>
        /// <param name="value">Value to convert.</param>
        /// <remarks>Requires an active script context.</remarks>
        /// <returns>The converted value.</returns>
        static IJsNumber^ ToJsNumber(IJsValue^ value);

        /// <summary>
        /// Converts the value to <see cref="IJsString"/> using regular JavaScript semantics.
        /// </summary>
        /// <param name="value">Value to convert.</param>
        /// <remarks>Requires an active script context.</remarks>
        /// <returns>The converted value.</returns>
        static IJsString^ ToJsString(IJsValue^ value);

        /// <summary>
        /// Converts the value to <see cref="IJsObject"/> using regular JavaScript semantics.
        /// </summary>
        /// <param name="value">Value to convert.</param>
        /// <remarks>Requires an active script context.</remarks>
        /// <returns>The converted value.</returns>
        static IJsObject^ ToJsObject(IJsValue^ value);
    };
}
