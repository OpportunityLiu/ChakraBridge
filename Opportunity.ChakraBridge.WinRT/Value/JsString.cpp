#include "pch.h"
#include "JsString.h"

using namespace Opportunity::ChakraBridge::WinRT;

JsStringImpl::~JsStringImpl()
{
    CHAKRA_CALL(JsRelease(Reference, nullptr));
}

JsStringImpl::JsStringImpl(JsValueRef ref)
    :JsValueImpl(ref)
{
    CHAKRA_CALL(JsAddRef(ref, nullptr));
}

Platform::String^ JsStringImpl::ToString()
{
    const wchar_t* str;
    size_t len;
    CHAKRA_CALL(JsStringToPointer(Reference, &str, &len));
    return ref new Platform::String(str, static_cast<unsigned int>(len));
}

IJsString^ JsString::OfString(Platform::String^ value)
{
    JsValueRef ref;
    CHAKRA_CALL(JsPointerToString(value->Data(), value->Length(), &ref));
    return ref new JsStringImpl(ref);
}
