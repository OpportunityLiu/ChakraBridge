#pragma once
#include "JsObject.h"
#include "JsEnum.h"
#include "Native\ThrowHelper.h"

namespace Opportunity::ChakraBridge::WinRT
{
    public interface class IJsFunction :IJsObject
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
        using IJsValueVectorView = Windows::Foundation::Collections::IVectorView<IJsValue^>;
    internal:
        JsFunctionImpl(JsValueRef ref) :JsObjectImpl(ref) {}
        INHERIT_INTERFACE_R_PROPERTY(Type, JsValueType, IJsValue);

    public:
        virtual Platform::String^ ToString() override;
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
    };
}
