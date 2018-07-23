#include "pch.h"
#include "JsString.h"

using namespace Opportunity::ChakraBridge::WinRT;

JsStringImpl::~JsStringImpl()
{
    JsRelease(this->Reference, nullptr);
}

JsStringImpl::JsStringImpl(JsValueRef ref)
    :JsValueImpl(ref)
{
    CHAKRA_CALL(JsAddRef(ref, nullptr));
}

uint32 JsStringImpl::Length::get()
{
    int l;
    CHAKRA_CALL(JsGetStringLength(Reference, &l));
    return static_cast<uint32>(l);
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
