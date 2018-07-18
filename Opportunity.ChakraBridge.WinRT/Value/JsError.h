#pragma once
#include "JsObject.h"
#include "JsEnum.h"
#include "Native\ThrowHelper.h"

namespace Opportunity::ChakraBridge::WinRT
{
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
    };

    ref class JsErrorImpl sealed : JsObjectImpl, IJsError
    {
    internal:
        JsErrorImpl(JsValueRef ref) :JsObjectImpl(ref) {}
        INHERIT_INTERFACE_R_PROPERTY(Type, JsValueType, IJsValue);
        INHERIT_INTERFACE_R_PROPERTY(Context, JsContext^, IJsValue);
        INHERIT_INTERFACE_METHOD(ToInspectable, object^, IJsValue);

    public:
        virtual Platform::String^ ToString() override;
        virtual property Platform::String^ Message;
        virtual property Platform::String^ Name;
    };

    /// <summary>
    /// Static methods of <see cref="IJsError"/>.
    /// </summary>
    public ref class JsError sealed
    {
    private:
        JsError() {}
    };
}
