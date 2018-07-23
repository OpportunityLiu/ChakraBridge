#pragma once
#include "JsValue.h"
#include "JsEnum.h"

namespace Opportunity::ChakraBridge::WinRT
{
    /// <summary>
    /// A JavaScript symbol value.
    /// </summary>
    public interface class IJsSymbol : IJsValue
    {
    };

    ref class JsSymbolImpl : JsValueImpl, IJsSymbol
    {
    internal:
        JsSymbolImpl(JsValueRef ref);
        INHERIT_INTERFACE_R_PROPERTY(Type, JsType, IJsValue);
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

    public:
        /// <summary>
        /// Creates a JavaScript symbol. 
        /// </summary>
        /// <param name="description">The description of the symbol. Can be <see langword="null"/>.</param>
        [Overload("CreateWithJsValueDescription")]
        static IJsSymbol^ Create(IJsValue^ description);

        /// <summary>
        /// Creates a JavaScript symbol. 
        /// </summary>
        /// <param name="description">The string description of the symbol.</param>
        [Overload("CreateWithStringDescription")]
        static IJsSymbol^ Create(string^ description);

        /// <summary>
        /// Creates a JavaScript symbol. 
        /// </summary>
        [DefaultOverload]
        [Overload("Create")]
        static IJsSymbol^ Create();

        /// <summary>
        /// The <c>Symbol.for(key)</c> method searches for existing symbols in a global symbol registry with the given key and returns it if found. Otherwise a new symbol gets created in the global symbol registry with this key.
        /// </summary>
        /// <param name="description">The key for the symbol (and also used for the description of the symbol).</param>
        [Overload("ForJsValue")]
        [DefaultOverload]
        static IJsSymbol^ For(IJsValue^ key);

        /// <summary>
        /// The <c>Symbol.for(key)</c> method searches for existing symbols in a global symbol registry with the given key and returns it if found. Otherwise a new symbol gets created in the global symbol registry with this key.
        /// </summary>
        /// <param name="description">The key for the symbol (and also used for the description of the symbol).</param>
        [Overload("ForString")]
        static IJsSymbol^ For(string^ key);

        /// <summary>
        /// The <c>Symbol.keyFor(sym)</c> method retrieves a shared symbol key from the global symbol registry for the given symbol.
        /// </summary>
        /// <param name="symbol">The symbol to find a key for.</param>
        static IJsString^ KeyFor(IJsSymbol^ symbol);

        /// <summary><c>hasInstance</c> property of Javascript <c>Symbol</c></summary>
        static property IJsSymbol^ HasInstance { IJsSymbol^ get(); };

        /// <summary><c>isConcatSpreadable</c> property of Javascript <c>Symbol</c></summary>
        static property IJsSymbol^ IsConcatSpreadable { IJsSymbol^ get(); };

        /// <summary><c>iterator</c> property of Javascript <c>Symbol</c></summary>
        static property IJsSymbol^ Iterator { IJsSymbol^ get(); };

        /// <summary><c>species</c> property of Javascript <c>Symbol</c></summary>
        static property IJsSymbol^ Species { IJsSymbol^ get(); };

        /// <summary><c>toPrimitive</c> property of Javascript <c>Symbol</c></summary>
        static property IJsSymbol^ ToPrimitive { IJsSymbol^ get(); };

        /// <summary><c>toStringTag</c> property of Javascript <c>Symbol</c></summary>
        static property IJsSymbol^ ToStringTag { IJsSymbol^ get(); };

        /// <summary><c>unscopables</c> property of Javascript <c>Symbol</c></summary>
        static property IJsSymbol^ Unscopables { IJsSymbol^ get(); };
    };

    inline bool operator ==(IJsSymbol^ sym1, IJsSymbol^ sym2)
    {
        return JsValue::ReferenceEquals(sym1, sym2);
    };

    inline bool operator !=(IJsSymbol^ sym1, IJsSymbol^ sym2)
    {
        return !(sym1 == sym2);
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