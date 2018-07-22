#pragma once
#include "JsObject.h"
#include "JsEnum.h"

namespace Opportunity::ChakraBridge::WinRT
{
    /// <summary>
    /// A JavaScript error object.
    /// </summary>
    public interface class IJsError : IJsObject
    {

        /// <summary>
        /// <c>message</c> property of Error.
        /// </summary>
        property Platform::String^ Message;

        /// <summary>
        /// <c>name</c> property of Error.
        /// </summary>
        property Platform::String^ Name;

        /// <summary>
        /// <c>stack</c> property of Error.
        /// </summary>
        property Platform::String^ Stack;
    };

    ref class JsErrorImpl sealed : JsObjectImpl, IJsError
    {
    internal:
        JsErrorImpl(JsValueRef ref) :JsObjectImpl(ref) {}
        INHERIT_INTERFACE_R_PROPERTY(Type, JsValueType, IJsValue);
        INHERIT_INTERFACE_R_PROPERTY(Context, JsContext^, IJsValue);
        INHERIT_INTERFACE_METHOD(ToInspectable, object^, IJsValue);
        INHERIT_INTERFACE_METHOD(ToString, string^, Windows::Foundation::IStringable);

        INHERIT_INTERFACE_RW_PROPERTY(Proto, IJsObject^, IJsObject);
        INHERIT_INTERFACE_METHOD(PreventExtension, void, IJsObject);
        INHERIT_INTERFACE_R_PROPERTY(IsExtensionAllowed, bool, IJsObject);
        INHERIT_INTERFACE_RW_PROPERTY(ObjectCollectingCallback, JsObjectBeforeCollectCallback^, IJsObject);

        INHERIT_INTERFACE_METHOD_PARAM1(Lookup, IJsValue^, IStrMap, string^);
        INHERIT_INTERFACE_METHOD_PARAM1(Lookup, IJsValue^, ISymMap, IJsSymbol^);
        INHERIT_INTERFACE_METHOD_EXPLICT(Clear, StrClear, void, IStrMap);
        INHERIT_INTERFACE_METHOD_EXPLICT(Clear, SymClear, void, ISymMap);
        INHERIT_INTERFACE_METHOD_PARAM1(HasKey, bool, IStrMap, string^);
        INHERIT_INTERFACE_METHOD_PARAM1(HasKey, bool, ISymMap, IJsSymbol^);
        INHERIT_INTERFACE_METHOD_PARAM1(Remove, void, IStrMap, string^);
        INHERIT_INTERFACE_METHOD_PARAM1(Remove, void, ISymMap, IJsSymbol^);
        INHERIT_INTERFACE_METHOD_PARAM2(Insert, bool, IStrMap, string^, IJsValue^);
        INHERIT_INTERFACE_METHOD_PARAM2(Insert, bool, ISymMap, IJsSymbol^, IJsValue^);
        INHERIT_INTERFACE_METHOD_EXPLICT(GetView, GetStrView, IStrMapView^, IStrMap);
        INHERIT_INTERFACE_METHOD_EXPLICT(GetView, GetSymView, ISymMapView^, ISymMap);
        INHERIT_INTERFACE_R_PROPERTY_EXPLICT(Size, StrSize, uint32, IStrMap);
        INHERIT_INTERFACE_R_PROPERTY_EXPLICT(Size, SymSize, uint32, ISymMap);
        INHERIT_INTERFACE_METHOD_EXPLICT(First, StrFirst, IStrIterator^, IStrIterable);
        INHERIT_INTERFACE_METHOD_EXPLICT(First, SymFirst, ISymIterator^, ISymIterable);

    public:
        virtual property string^ Message { string^ get(); void set(string^ value); };
        virtual property string^ Name { string^ get(); void set(string^ value); }
        virtual property string^ Stack { string^ get(); void set(string^ value); }
    };

    /// <summary>
    /// Static methods of <see cref="IJsError"/>.
    /// </summary>
    public ref class JsError sealed
    {
    private:
        JsError() {}
    public:

        /// <summary>
        /// Creates a new JavaScript error object
        /// </summary>
        /// <remarks>Requires an active script context.</remarks>
        /// <param name="message">Message for the error object.</param>
        /// <returns>The new error object.</returns>
        [Overload("CreateErrorWithJsString")]
        static IJsError^ CreateError(IJsString^ message);

        /// <summary>
        /// Creates a new JavaScript error object
        /// </summary>
        /// <remarks>Requires an active script context.</remarks>
        /// <param name="message">Message for the error object.</param>
        /// <returns>The new error object.</returns>
        [DefaultOverload]
        [Overload("CreateErrorWithString")]
        static IJsError^ CreateError(string^ message);

        /// <summary>
        /// Creates a new JavaScript RangeError error object
        /// </summary>
        /// <remarks>Requires an active script context.</remarks>
        /// <param name="message">Message for the error object.</param>
        /// <returns>The new error object.</returns>
        [Overload("CreateRangeErrorWithJsString")]
        static IJsError^ CreateRangeError(IJsString^ message);

        /// <summary>
        /// Creates a new JavaScript RangeError error object
        /// </summary>
        /// <remarks>Requires an active script context.</remarks>
        /// <param name="message">Message for the error object.</param>
        /// <returns>The new error object.</returns>
        [DefaultOverload]
        [Overload("CreateRangeErrorWithString")]
        static IJsError^ CreateRangeError(string^ message);

        /// <summary>
        /// Creates a new JavaScript ReferenceError error object
        /// </summary>
        /// <remarks>Requires an active script context.</remarks>
        /// <param name="message">Message for the error object.</param>
        /// <returns>The new error object.</returns>
        [Overload("CreateReferenceErrorWithJsString")]
        static IJsError^ CreateReferenceError(IJsString^ message);

        /// <summary>
        /// Creates a new JavaScript ReferenceError error object
        /// </summary>
        /// <remarks>Requires an active script context.</remarks>
        /// <param name="message">Message for the error object.</param>
        /// <returns>The new error object.</returns>
        [DefaultOverload]
        [Overload("CreateReferenceErrorWithString")]
        static IJsError^ CreateReferenceError(string^ message);

        /// <summary>
        /// Creates a new JavaScript SyntaxError error object
        /// </summary>
        /// <remarks>Requires an active script context.</remarks>
        /// <param name="message">Message for the error object.</param>
        /// <returns>The new error object.</returns>
        [Overload("CreateSyntaxErrorWithJsString")]
        static IJsError^ CreateSyntaxError(IJsString^ message);

        /// <summary>
        /// Creates a new JavaScript SyntaxError error object
        /// </summary>
        /// <remarks>Requires an active script context.</remarks>
        /// <param name="message">Message for the error object.</param>
        /// <returns>The new error object.</returns>
        [DefaultOverload]
        [Overload("CreateSyntaxErrorWithString")]
        static IJsError^ CreateSyntaxError(string^ message);

        /// <summary>
        /// Creates a new JavaScript TypeError error object
        /// </summary>
        /// <remarks>Requires an active script context.</remarks>
        /// <param name="message">Message for the error object.</param>
        /// <returns>The new error object.</returns>
        [Overload("CreateTypeErrorWithJsString")]
        static IJsError^ CreateTypeError(IJsString^ message);

        /// <summary>
        /// Creates a new JavaScript TypeError error object
        /// </summary>
        /// <remarks>Requires an active script context.</remarks>
        /// <param name="message">Message for the error object.</param>
        /// <returns>The new error object.</returns>
        [DefaultOverload]
        [Overload("CreateTypeErrorWithString")]
        static IJsError^ CreateTypeError(string^ message);

        /// <summary>
        /// Creates a new JavaScript URIError error object
        /// </summary>
        /// <remarks>Requires an active script context.</remarks>
        /// <param name="message">Message for the error object.</param>
        /// <returns>The new error object.</returns>
        [Overload("CreateUriErrorWithJsString")]
        static IJsError^ CreateUriError(IJsString^ message);

        /// <summary>
        /// Creates a new JavaScript URIError error object
        /// </summary>
        /// <remarks>Requires an active script context.</remarks>
        /// <param name="message">Message for the error object.</param>
        /// <returns>The new error object.</returns>
        [DefaultOverload]
        [Overload("CreateUriErrorWithString")]
        static IJsError^ CreateUriError(string^ message);
    };
}
