#include "pch.h"
#include "JsBoolean.h"

using namespace Opportunity::ChakraBridge::WinRT;

JsBooleanImpl::JsBooleanImpl(RawValue ref)
    :JsValueImpl(ref) {}

bool JsBooleanImpl::ToBoolean()
{
    return Reference.ToBoolean();
}

string^ JsBooleanImpl::ToString()
{
    return Reference.ToBoolean() ? L"true" : L"false";
}

IJsBoolean^ JsBoolean::True::get()
{
    return ref new JsBooleanImpl(RawValue::True());
}

IJsBoolean^ JsBoolean::False::get()
{
    return ref new JsBooleanImpl(RawValue::False());
}

IJsBoolean^ JsBoolean::Create(const bool value)
{
    return ref new JsBooleanImpl(RawValue(value));
}