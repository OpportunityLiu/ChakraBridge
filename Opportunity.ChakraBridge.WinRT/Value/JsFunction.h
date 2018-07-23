#pragma once
#include "JsObject.h"
#include "JsEnum.h"
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
        /// Read-only <c>name</c> property of the <see cref="IJsFunction"/>.
        /// </summary>
        /// <remarks>Requires an active script context.</remarks>
        property string^ Name { string^ get(); }

        /// <summary>
        /// Read-only <c>length</c> property of the <see cref="IJsFunction"/>.
        /// </summary>
        /// <remarks>Requires an active script context.</remarks>
        property int32 Length { int32 get(); }

        /// <summary>
        /// <c>prototype</c> property of the <see cref="IJsFunction"/>.
        /// </summary>
        /// <remarks>Requires an active script context.</remarks>
        property IJsObject^ Prototype { IJsObject^ get(); void set(IJsObject^ value); }

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
        using JsFunctionDelegate = Opportunity::ChakraBridge::WinRT::JsNativeFunction;
        using IJsValueVectorView = Windows::Foundation::Collections::IVectorView<IJsValue^>;

        JsFunctionImpl(JsValueRef ref) :JsObjectImpl(ref) {}

        void InitForNativeFunc(JsFunctionDelegate^ function);

        INHERIT_INTERFACE_R_PROPERTY(Type, JsType, IJsValue);
        INHERIT_INTERFACE_R_PROPERTY(Context, JsContext^, IJsValue);
        INHERIT_INTERFACE_METHOD(ToInspectable, object^, IJsValue);

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

        static std::unordered_map<JsValueRef, JsFunctionDelegate^> FunctionTable;
        static void CALLBACK JsFunctionBeforeCollectCallbackImpl(_In_ JsRef ref, _In_opt_ void *callbackState);

    public:
        virtual IJsValue^ Invoke(IJsValue^ caller, IJsValueVectorView^ arguments);
        virtual IJsObject^ New(IJsValueVectorView^ arguments);
        virtual property string^ Name { string^ get(); }
        virtual property int32 Length { int32 get(); }
        virtual property IJsObject^ Prototype { IJsObject^ get(); void set(IJsObject^ value); }
        virtual string^ ToString() override;
        virtual property JsOBCC^ ObjectCollectingCallback { void set(JsOBCC^ value) override; }
    };

    /// <summary>
    /// Static methods of <see cref="IJsFunction"/>.
    /// </summary>
    public ref class JsFunction sealed
    {
    private:
        JsFunction() {}

    public:
        /// <summary>
        /// Creates a new JavaScript function.
        /// </summary>
        /// <param name="function">The method to call when the function is invoked.</param>
        /// <returns>A new JavaScript function.</returns>
        /// <remarks>Requires an active script context.</remarks>
        [Overload("OfNativeFunction")]
        static IJsFunction^ Create(JsNativeFunction^ function);

        /// <summary>
        /// Creates a new JavaScript function.
        /// </summary>
        /// <param name="function">The method to call when the function is invoked.</param>
        /// <param name="name">The name of this function that will be used for diagnostics and stringification purposes. </param>
        /// <returns>A new JavaScript function.</returns>
        /// <remarks>Requires an active script context.</remarks>
        [Overload("OfNativeFunctionWithJsName")]
        static IJsFunction^ Create(JsNativeFunction^ function, IJsString^ name);

        /// <summary>
        /// Creates a new JavaScript function.
        /// </summary>
        /// <param name="function">The method to call when the function is invoked.</param>
        /// <param name="name">The name of this function that will be used for diagnostics and stringification purposes. </param>
        /// <returns>A new JavaScript function.</returns>
        /// <remarks>Requires an active script context.</remarks>
        [DefaultOverload]
        [Overload("OfNativeFunctionWithName")]
        static IJsFunction^ Create(JsNativeFunction^ function, string^ name);
    };
}
