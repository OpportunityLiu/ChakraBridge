#include "pch.h"
#include "JsSymbol.h"

using namespace Opportunity::ChakraBridge::WinRT;

JsSymbolImpl::~JsSymbolImpl()
{
    CHAKRA_CALL(JsRelease(this->Reference, nullptr));
}

JsSymbolImpl::JsSymbolImpl(JsValueRef ref)
    :JsValueImpl(ref)
{
    CHAKRA_CALL(JsAddRef(ref, nullptr));
}

string^ JsSymbolImpl::ToString()
{
    JsValueRef symObj;
    CHAKRA_CALL(JsConvertValueToObject(Reference, &symObj));
    auto tostrFunc = RawGetProperty(symObj, L"constructor", L"prototype", L"toString");
    JsValueRef strref = RawCallFunction(tostrFunc, this->Reference);
    return RawStringToPointer(strref);
}

IJsSymbol^ InnerCreate(JsValueRef description)
{
    JsValueRef r;
    CHAKRA_CALL(JsCreateSymbol(description, &r));
    return ref new JsSymbolImpl(r);
}

IJsSymbol^ JsSymbol::Create(IJsValue^ description)
{
    if (description == nullptr)
        return Create();
    return InnerCreate(to_impl(description)->Reference);
}

IJsSymbol^ JsSymbol::Create(string^ description)
{
    if (description == nullptr)
        return Create();
    return InnerCreate(RawPointerToString(description));
}

IJsSymbol^ JsSymbol::Create()
{
    return InnerCreate(JS_INVALID_REFERENCE);
}

using JsType = Opportunity::ChakraBridge::WinRT::JsValueType;

template<JsType TExpacted>
JsValueRef GetSymbolProperty(const wchar_t* name)
{
    try
    {
        auto value = RawGetProperty(RawGlobalObject(), L"Symbol", name);
        auto valueType = RawGetValueType(value);
        if (valueType != TExpacted)
            goto GET_FALLBACK;
        return value;
    }
    catch (...)
    {
        goto GET_FALLBACK;
    }
GET_FALLBACK:
    JsValueRef csym;
    CHAKRA_CALL(JsCreateSymbol(JS_INVALID_REFERENCE, &csym));
    JsValueRef symObj;
    CHAKRA_CALL(JsConvertValueToObject(csym, &symObj));
    return RawGetProperty(symObj, L"constructor", name);
}

IJsSymbol^ JsSymbol::For(IJsValue^ key)
{
    NULL_CHECK(key);
    auto forFunc = GetSymbolProperty<JsType::Function>(L"for");
    return safe_cast<IJsSymbol^>(JsValue::CreateTyped(RawCallFunction(forFunc, RawGlobalObject(), to_impl(key)->Reference)));
}

IJsSymbol^ JsSymbol::For(string^ key)
{
    auto forFunc = GetSymbolProperty<JsType::Function>(L"for");
    return safe_cast<IJsSymbol^>(JsValue::CreateTyped(RawCallFunction(forFunc, RawGlobalObject(), RawPointerToString(key))));
}

IJsString^ JsSymbol::KeyFor(IJsSymbol^ symbol)
{
    NULL_CHECK(symbol);
    auto keyforFunc = GetSymbolProperty<JsType::Function>(L"keyFor");
    auto key = RawCallFunction(keyforFunc, to_impl(symbol)->Reference, to_impl(symbol)->Reference);
    if (RawGetValueType(key) != JsType::String)
        return nullptr;
    return ref new JsStringImpl(key);
}

IJsSymbol^_WELL_KNOWN_SYMBOL_GETTER(const wchar_t* name)
{
    return ref new JsSymbolImpl(GetSymbolProperty<JsType::Symbol>(name));
}

#define WELL_KNOWN_SYMBOL_GETTER(pname, name) \
IJsSymbol^ JsSymbol::pname::get() { return _WELL_KNOWN_SYMBOL_GETTER(_CRT_WIDE(_CRT_STRINGIZE(name))); }

WELL_KNOWN_SYMBOL_GETTER(HasInstance, hasInstance);
WELL_KNOWN_SYMBOL_GETTER(IsConcatSpreadable, isConcatSpreadable);
WELL_KNOWN_SYMBOL_GETTER(Iterator, iterator);
WELL_KNOWN_SYMBOL_GETTER(Species, species);
WELL_KNOWN_SYMBOL_GETTER(ToPrimitive, toPrimitive);
WELL_KNOWN_SYMBOL_GETTER(ToStringTag, toStringTag);
WELL_KNOWN_SYMBOL_GETTER(Unscopables, unscopables);
