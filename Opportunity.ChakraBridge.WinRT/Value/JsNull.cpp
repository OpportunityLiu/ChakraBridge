#include "pch.h"
#include "JsNull.h"

using namespace Opportunity::ChakraBridge::WinRT;

JsNullImpl::JsNullImpl(JsValueRef ref)
    :JsValueImpl(ref) {}

IJsNull^ JsNull::Instance::get()
{
    return ref new JsNullImpl(RawNull());
}