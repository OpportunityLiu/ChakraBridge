#pragma once
#include "JsValue.h"
#include "JsEnum.h"

namespace Opportunity::ChakraBridge::WinRT
{
    public interface class IJsSymbol : IJsValue
    {
    };

    ref class JsSymbolImpl : JsValueImpl, IJsSymbol
    {
    internal:
        JsSymbolImpl(JsValueRef ref);
        INHERIT_INTERFACE_R_PROPERTY(Type, JsValueType, IJsValue);
        INHERIT_INTERFACE_R_PROPERTY(Context, JsContext^, IJsValue);
        INHERIT_INTERFACE_METHOD(ToInspectable, object^, IJsValue);

    public:
        virtual ~JsSymbolImpl();
        virtual Platform::String^ ToString() override;
    };

    /// <summary>
    /// Static methods of <see cref="IJsSymbol"/>.
    /// </summary>
    public ref class JsSymbol sealed
    {
    private:
        JsSymbol() {}

    };
}