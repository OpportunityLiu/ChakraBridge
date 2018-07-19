#include "pch.h"
#include "JsObject.h"
#include <vector>

using namespace Opportunity::ChakraBridge::WinRT;

JsObjectImpl::~JsObjectImpl()
{
    CHAKRA_CALL(JsRelease(this->Reference, nullptr));
}

JsObjectImpl::JsObjectImpl(JsValueRef ref)
    :JsValueImpl(ref)
{
    CHAKRA_CALL(JsAddRef(ref, nullptr));
}

IJsValue^ JsObjectImpl::Lookup(string^ key)
{
    if (key->IsEmpty())
        throw ref new Platform::InvalidArgumentException(L"key is null or empty.");
    return JsValue::CreateTyped(RawGetProperty(Reference, key->Data()));
}

IJsValue^ JsObjectImpl::Lookup(IJsSymbol^ key)
{
    if (key == nullptr)
        throw ref new Platform::InvalidArgumentException(L"key is null.");
    return JsValue::CreateTyped(RawGetProperty(Reference, to_impl(key)->Reference));
}

bool JsObjectImpl::HasKey(string^ key)
{
    if (key->IsEmpty())
        throw ref new Platform::InvalidArgumentException(L"key is null or empty.");
    bool r;
    CHAKRA_CALL(JsHasProperty(Reference, RawGetPropertyId(key->Data()), &r));
    return r;
}

bool JsObjectImpl::HasKey(IJsSymbol^ key)
{
    if (key == nullptr)
        throw ref new Platform::InvalidArgumentException(L"key is null.");
    bool r;
    CHAKRA_CALL(JsHasIndexedProperty(Reference, to_impl(key)->Reference, &r));
    return r;
}

bool JsObjectImpl::Insert(string^ key, IJsValue^ value)
{
    bool r = HasKey(key);
    auto vref = value != nullptr ? to_impl(value)->Reference : RawNull();
    CHAKRA_CALL(JsSetProperty(Reference, RawGetPropertyId(key->Data()), vref, true));
    return r;
}

bool JsObjectImpl::Insert(IJsSymbol^ key, IJsValue^ value)
{
    bool r = HasKey(key);
    auto vref = value != nullptr ? to_impl(value)->Reference : RawNull();
    CHAKRA_CALL(JsSetIndexedProperty(Reference, to_impl(key)->Reference, vref));
    return r;
}

void JsObjectImpl::Remove(string^ key)
{
    if (key->IsEmpty())
        throw ref new Platform::InvalidArgumentException(L"key is null or empty.");
    JsValueRef r;
    CHAKRA_CALL(JsDeleteProperty(Reference, RawGetPropertyId(key->Data()), true, &r));
}

void JsObjectImpl::Remove(IJsSymbol^ key)
{
    if (key == nullptr)
        throw ref new Platform::InvalidArgumentException(L"key is null.");
    CHAKRA_CALL(JsDeleteIndexedProperty(Reference, to_impl(key)->Reference));
}

JsObjectImpl::IStrIterator^ JsObjectImpl::StrFirst()
{
    JsValueRef r;
    CHAKRA_CALL(JsGetOwnPropertyNames(Reference, &r));
    auto n = RawNumberToInt(RawGetProperty(r, L"length"));
    auto view = ref new KVIteratorImpl<string^, IJsValue^>();
    for (int i = 0; i < n; i++)
    {
        auto kf = RawGetProperty(r, RawIntToNumber(i));
        auto k = RawStringToPointer(kf);
        view->Data.emplace_back(k, JsValue::CreateTyped(RawGetProperty(Reference, kf)));
    }
    view->Init();
    return view;
}

JsObjectImpl::ISymIterator^ JsObjectImpl::SymFirst()
{
    JsValueRef r;
    CHAKRA_CALL(JsGetOwnPropertyNames(Reference, &r));
    auto n = RawNumberToInt(RawGetProperty(r, L"length"));
    auto view = ref new KVIteratorImpl<IJsSymbol^, IJsValue^>();
    for (int i = 0; i < n; i++)
    {
        auto kf = RawGetProperty(r, RawIntToNumber(i));
        view->Data.emplace_back(ref new JsSymbolImpl(kf), JsValue::CreateTyped(RawGetProperty(Reference, kf)));
    }
    view->Init();
    return view;
}

uint32 JsObjectImpl::StrSize::get()
{
    JsValueRef r;
    CHAKRA_CALL(JsGetOwnPropertyNames(Reference, &r));
    return static_cast<uint32>(RawNumberToInt(RawGetProperty(r, L"length")));
}

uint32 JsObjectImpl::SymSize::get()
{
    JsValueRef r;
    CHAKRA_CALL(JsGetOwnPropertyNames(Reference, &r));
    return static_cast<uint32>(RawNumberToInt(RawGetProperty(r, L"length")));
}

string^ JsObjectImpl::ToString()
{
    JsValueRef strref;
    if (JsConvertValueToString(Reference, &strref) == JsNoError)
    {
        return RawStringToPointer(strref);
    }
    // no toString method.
    return "[object Object]";
}
