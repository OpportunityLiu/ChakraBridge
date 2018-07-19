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
    if (key == nullptr)
        throw ref new Platform::InvalidArgumentException("key is null.");
    JsPropertyIdRef k;
    CHAKRA_CALL(JsGetPropertyIdFromName(key->Data(), &k));
    JsValueRef v;
    CHAKRA_CALL(JsGetProperty(Reference, k, &v));
    return JsValue::CreateTyped(v);
}

IJsValue^ JsObjectImpl::Lookup(IJsSymbol^ key)
{
    if (key == nullptr)
        throw ref new Platform::InvalidArgumentException("key is null.");
    JsPropertyIdRef k;
    CHAKRA_CALL(JsGetPropertyIdFromSymbol(to_impl(key)->Reference, &k));
    JsValueRef v;
    CHAKRA_CALL(JsGetProperty(Reference, k, &v));
    return JsValue::CreateTyped(v);
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
