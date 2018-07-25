#include "pch.h"
#include "JsSymbol.h"

using namespace Opportunity::ChakraBridge::WinRT;

JsSymbolImpl::~JsSymbolImpl()
{
    // ignore error.
    JsRelease(Reference.Ref, nullptr);
}

JsSymbolImpl::JsSymbolImpl(RawValue ref)
    :JsValueImpl(std::move(ref))
{
    Reference.AddRef();
}

string^ JsSymbolImpl::ToString()
{
    return Reference.ToJsObjet().ToJsString().ToString();
}

IJsSymbol^ InnerCreate(RawValue description)
{
    return ref new JsSymbolImpl(RawValue::CreateSymbol(description));
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
    return InnerCreate(RawValue(description->Data(), description->Length()));
}

IJsSymbol^ JsSymbol::Create()
{
    return InnerCreate(RawValue::Invalid());
}

template<JsType TExpacted>
RawValue GetSymbolProperty(const wchar_t* name)
{
    try
    {
        RawValue value = RawValue::GlobalObject()[L"Symbol"][name];
        auto valueType = value.Type();
        if (valueType != TExpacted)
            goto GET_FALLBACK;
        return value;
    }
    catch (...)
    {
        goto GET_FALLBACK;
    }
GET_FALLBACK:
    const auto sym = RawValue::CreateSymbol(RawValue::Invalid()).ToJsObjet();
    return sym[L"constructor"][name];
}

IJsSymbol^ JsSymbol::For(IJsValue^ key)
{
    NULL_CHECK(key);
    const auto forFunc = GetSymbolProperty<JsType::Function>(L"for");
    return safe_cast<IJsSymbol^>(JsValue::CreateTyped(forFunc.Invoke(RawValue::Invalid(), to_impl(key)->Reference)));
}

IJsSymbol^ JsSymbol::For(string^ key)
{
    const auto forFunc = GetSymbolProperty<JsType::Function>(L"for");
    return safe_cast<IJsSymbol^>(JsValue::CreateTyped(forFunc.Invoke(RawValue::Invalid(), RawValue(key->Data(), key->Length()))));
}

IJsString^ JsSymbol::KeyFor(IJsSymbol^ symbol)
{
    NULL_CHECK(symbol);
    const auto keyforFunc = GetSymbolProperty<JsType::Function>(L"keyFor");
    const auto key = keyforFunc.Invoke(RawValue::Invalid(), to_impl(symbol)->Reference);
    if (key.Type() != JsType::String)
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
