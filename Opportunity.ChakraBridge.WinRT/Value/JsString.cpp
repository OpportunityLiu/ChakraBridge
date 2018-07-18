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

int32 JsStringImpl::Length::get()
{
    int l;
    CHAKRA_CALL(JsGetStringLength(Reference, &l));
    return static_cast<int32>(l);
}

string^ JsStringImpl::ToString()
{
    const wchar_t* str;
    size_t len;
    CHAKRA_CALL(JsStringToPointer(Reference, &str, &len));
    return ref new string(str, static_cast<unsigned int>(len));
}

IJsString^ JsString::Of(string^ value)
{
    JsValueRef ref;
    CHAKRA_CALL(JsPointerToString(value->Data(), value->Length(), &ref));
    return ref new JsStringImpl(ref);
}
