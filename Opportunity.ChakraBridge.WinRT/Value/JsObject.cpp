#include "pch.h"
#include "JsObject.h"

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
    CHAKRA_CALL(JsHasProperty(Reference, RawGetPropertyId(to_impl(key)->Reference), &r));
    return r;
}

bool JsObjectImpl::Insert(string^ key, IJsValue^ value)
{
    bool r = HasKey(key);
    JsValueRef = value != nullptr ? to_impl(value)->Reference : RawGetNull();
    CHAKRA_CALL(JsSetProperty(Reference, RawGetPropertyId(key->Data()), to_impl(value)->Reference, true));
    return r;
}

bool JsObjectImpl::Insert(IJsSymbol^ key, IJsValue^ value)
{
    bool r = HasKey(key);
    CHAKRA_CALL(JsSetProperty(Reference, RawGetPropertyId(to_impl(key)->Reference), to_impl(value)->Reference, true));
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
    JsValueRef r;
    CHAKRA_CALL(JsDeleteProperty(Reference, RawGetPropertyId(to_impl(key)->Reference), true, &r));
}

string^ JsObjectImpl::ToString()
{
    JsValueRef strref;
    if (JsConvertValueToString(Reference, &strref) == JsNoError)
    {
        const wchar_t* str;
        size_t len;
        CHAKRA_CALL(JsStringToPointer(strref, &str, &len));
        return ref new string(str, static_cast<unsigned int>(len));
    }
    // no toString method.
    return "[object Object]";
}
