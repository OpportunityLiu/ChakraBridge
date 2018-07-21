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

    inline bool operator ==(IJsSymbol^ sym1, IJsSymbol^ sym2)
    {
        return JsValue::ReferenceEquals(sym1, sym2);
    }

    inline bool operator !=(IJsSymbol^ sym1, IJsSymbol^ sym2)
    {
        return !(sym1 == sym2);
    }

    /// <summary>
    /// Static methods of <see cref="IJsSymbol"/>.
    /// </summary>
    public ref class JsSymbol sealed
    {
    private:
        JsSymbol() {}

    };
};

template <> struct ::std::hash<Opportunity::ChakraBridge::WinRT::IJsSymbol^>
{
    typedef Opportunity::ChakraBridge::WinRT::IJsSymbol^ argument_type;
    typedef size_t result_type;
    size_t operator()(Opportunity::ChakraBridge::WinRT::IJsSymbol^ sym) const
    {
        if (sym == nullptr)
            return 0;
        return reinterpret_cast<size_t>(Opportunity::ChakraBridge::WinRT::to_impl(sym)->Reference);
    };
};