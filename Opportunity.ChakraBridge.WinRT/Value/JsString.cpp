#include "pch.h"
#include "JsString.h"

using namespace Opportunity::ChakraBridge::WinRT;

JsStringImpl::~JsStringImpl()
{
    // ignore error.
    JsRelease(Reference.Ref, nullptr);
}

JsStringImpl::JsStringImpl(RawValue ref)
    :JsValueImpl(std::move(ref))
{
    Reference.AddRef();
}

uint32 JsStringImpl::Length::get()
{
    return static_cast<uint32>(Reference.StrLength());
}

string^ JsStringImpl::ToString()
{
    return Reference.ToString();
}

IJsString^ JsString::Create(string^ value)
{
    return ref new JsStringImpl(RawValue(value->Data(),value->Length()));
}
