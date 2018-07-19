#pragma once
#include "JsObject.h"
#include "JsEnum.h"
#include "Native\ThrowHelper.h"
#include <unordered_map>

namespace Opportunity::ChakraBridge::WinRT
{
    /// <summary>
    /// A function callback.
    /// </summary>
    /// <param name="callee">
    /// A <see cref="IJsFunction"/> object that represents the function being invoked.
    /// </param>
    /// <param name="caller">The object that the thisArg is.</param>
    /// <param name="isConstructCall">Indicates whether this is a regular call or a 'new' call.</param>
    /// <param name="arguments">The arguments to the call.</param>
    /// <returns>The result of the call, if any.</returns>
    public delegate IJsValue^ JsNativeFunction(IJsFunction^ callee, IJsObject^ caller, bool isConstructCall, Windows::Foundation::Collections::IVectorView<IJsValue^>^ arguments);

    public interface class IJsFunction : IJsObject
    {
        /// <summary>
        /// Invokes a function.
        /// </summary>
        /// <remarks>Requires an active script context.</remarks>
        /// <param name="caller">The object that the thisArg is.</param>
        /// <param name="arguments">The arguments to the call.</param>
        /// <returns>The <c>Value</c> returned from the function invocation, if any.</returns>
        IJsValue^ Invoke(IJsValue^ caller, Windows::Foundation::Collections::IVectorView<IJsValue^>^ arguments);

        /// <summary>
        /// Invokes a function as a constructor.
        /// </summary>
        /// <remarks>Requires an active script context.</remarks>
        /// <param name="arguments">The arguments to the call.</param>
        /// <returns>The <c>Value</c> returned from the function invocation.</returns>
        IJsObject^ New(Windows::Foundation::Collections::IVectorView<IJsValue^>^ arguments);
    };

    ref class JsFunctionImpl sealed : JsObjectImpl, IJsFunction
    {
    internal:
        JsFunctionImpl(JsValueRef ref) :JsObjectImpl(ref) {}
        using IJsValueVectorView = Windows::Foundation::Collections::IVectorView<IJsValue^>;
        INHERIT_INTERFACE_R_PROPERTY(Type, JsValueType, IJsValue);
        INHERIT_INTERFACE_R_PROPERTY(Context, JsContext^, IJsValue);
        INHERIT_INTERFACE_METHOD(ToInspectable, object^, IJsValue);
        INHERIT_INTERFACE_METHOD(ToString, string^, Windows::Foundation::IStringable);

        INHERIT_INTERFACE_METHOD_PARAM1(Lookup, IJsValue^, IStrMap, string^);
        INHERIT_INTERFACE_METHOD_PARAM1(Lookup, IJsValue^, IStrMapView, string^);
        INHERIT_INTERFACE_METHOD_PARAM1(Lookup, IJsValue^, ISymMap, IJsSymbol^);
        INHERIT_INTERFACE_METHOD_PARAM1(Lookup, IJsValue^, ISymMapView, IJsSymbol^);
        INHERIT_INTERFACE_METHOD(Clear, void, IStrMap);
        INHERIT_INTERFACE_METHOD(Clear, void, ISymMap);
        INHERIT_INTERFACE_METHOD_PARAM1(HasKey, bool, IStrMap, string^);
        INHERIT_INTERFACE_METHOD_PARAM1(HasKey, bool, IStrMapView, string^);
        INHERIT_INTERFACE_METHOD_PARAM1(HasKey, bool, ISymMap, IJsSymbol^);
        INHERIT_INTERFACE_METHOD_PARAM1(HasKey, bool, ISymMapView, IJsSymbol^);
        INHERIT_INTERFACE_METHOD_PARAM1(Remove, void, IStrMap, string^);
        INHERIT_INTERFACE_METHOD_PARAM1(Remove, void, ISymMap, IJsSymbol^);
        INHERIT_INTERFACE_METHOD_PARAM2(Insert, bool, IStrMap, string^, IJsValue^);
        INHERIT_INTERFACE_METHOD_PARAM2(Insert, bool, ISymMap, IJsSymbol^, IJsValue^);
        INHERIT_INTERFACE_METHOD_EXPLICT(GetView, GetStrView, IStrMapView^, IStrMap);
        INHERIT_INTERFACE_METHOD_EXPLICT(GetView, GetSymView, ISymMapView^, ISymMap);
        INHERIT_INTERFACE_METHOD_PARAM2(Split, void, IStrMapView, IStrMapView^*, IStrMapView^*);
        INHERIT_INTERFACE_METHOD_PARAM2(Split, void, ISymMapView, ISymMapView^*, ISymMapView^*);
        INHERIT_INTERFACE_R_PROPERTY_EXPLICT(Size, StrSize, uint32, IStrMap);
        INHERIT_INTERFACE_R_PROPERTY_EXPLICT(Size, StrSize, uint32, IStrMapView);
        INHERIT_INTERFACE_R_PROPERTY_EXPLICT(Size, SymSize, uint32, ISymMap);
        INHERIT_INTERFACE_R_PROPERTY_EXPLICT(Size, SymSize, uint32, ISymMapView);
        INHERIT_INTERFACE_METHOD_EXPLICT(First, StrFirst, IStrIterator^, IStrIterable);
        INHERIT_INTERFACE_METHOD_EXPLICT(First, SymFirst, ISymIterator^, ISymIterable);

    public:
        virtual IJsValue^ Invoke(IJsValue^ caller, IJsValueVectorView^ arguments);
        virtual IJsObject^ New(IJsValueVectorView^ arguments);
    };

    /// <summary>
    /// Static methods of <see cref="IJsFunction"/>.
    /// </summary>
    public ref class JsFunction sealed
    {
    private:
        JsFunction() {}
    internal:
        using JsFunctionDelegate = Opportunity::ChakraBridge::WinRT::JsNativeFunction;
    public:
        /// <summary>
        /// Creates a new JavaScript function.
        /// </summary>
        /// <param name="function">The method to call when the function is invoked.</param>
        /// <returns>A new JavaScript function.</returns>
        /// <remarks>Requires an active script context.</remarks>
        [Overload("OfNativeFunction")]
        static IJsFunction^ Of(JsNativeFunction^ function);

        /// <summary>
        /// Creates a new JavaScript function.
        /// </summary>
        /// <param name="function">The method to call when the function is invoked.</param>
        /// <param name="name">The name of this function that will be used for diagnostics and stringification purposes. </param>
        /// <returns>A new JavaScript function.</returns>
        /// <remarks>Requires an active script context.</remarks>
        [Overload("OfNativeFunctionWithJsName")]
        static IJsFunction^ Of(JsNativeFunction^ function, IJsString^ name);

        /// <summary>
        /// Creates a new JavaScript function.
        /// </summary>
        /// <param name="function">The method to call when the function is invoked.</param>
        /// <param name="name">The name of this function that will be used for diagnostics and stringification purposes. </param>
        /// <returns>A new JavaScript function.</returns>
        /// <remarks>Requires an active script context.</remarks>
        [DefaultOverload]
        [Overload("OfNativeFunctionWithName")]
        static IJsFunction^ Of(JsNativeFunction^ function, string^ name);
    };
}
