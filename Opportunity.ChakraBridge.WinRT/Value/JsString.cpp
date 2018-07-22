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
    return RawStringToPointer(Reference);
}

IJsString^ JsString::Create(string^ value)
{
    JsValueRef ref;
    CHAKRA_CALL(JsPointerToString(value->Data(), value->Length(), &ref));
    return ref new JsStringImpl(ref);
}
