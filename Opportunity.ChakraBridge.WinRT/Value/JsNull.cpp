#include "pch.h"
#include "JsNull.h"

using namespace Opportunity::ChakraBridge::WinRT;

JsNullImpl::JsNullImpl(RawValue ref)
    :JsValueImpl(std::move(ref)) {}

string^ JsNullImpl::ToString() { return L"null"; }

IJsNull^ JsNull::Instance::get()
{
    return ref new JsNullImpl(RawValue::Null());
}
