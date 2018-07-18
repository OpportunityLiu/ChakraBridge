#pragma once
#include "JsValue.h"
#include "JsEnum.h"
#include "Native\ThrowHelper.h"

namespace Opportunity::ChakraBridge::WinRT
{
    public interface class IJsObject : IJsValue
    {
    };

    ref class JsObjectImpl : JsValueImpl, IJsObject
    {
    internal:
        JsObjectImpl(JsValueRef ref);
        INHERIT_INTERFACE_R_PROPERTY(Type, JsValueType, IJsValue);

    public:
        virtual ~JsObjectImpl();
        virtual Platform::String^ ToString() override;
    };

    /// <summary>
    /// Static methods of <see cref="IJsObject"/>.
    /// </summary>
    public ref class JsObject sealed
    {
    private:
        JsObject() {}

    };
}
