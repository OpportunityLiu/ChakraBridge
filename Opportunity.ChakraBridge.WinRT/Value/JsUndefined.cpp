#include "pch.h"
#include "JsUndefined.h"

using namespace Opportunity::ChakraBridge::WinRT;

JsUndefinedImpl::JsUndefinedImpl(RawValue ref)
    :JsValueImpl(std::move(ref)) {}

string^ JsUndefinedImpl::ToString() { return L"undefined"; }

IJsUndefined^ JsUndefined::Instance::get()
{
    return ref new JsUndefinedImpl(RawValue::Undefined());
}

